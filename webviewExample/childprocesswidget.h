#ifndef CHILDPROCESSWIDGET_H
#define CHILDPROCESSWIDGET_H

#include <QWidget>
#include <QProcess>


class ChildProcessWidget : public QWidget
{
	Q_OBJECT
public:
	struct Params
	{
		QString app; // сейчас это путь от текущей директории, так что не из любой папочки можно такую программу запускать
		QStringList args {};
		const char *windowTitle {nullptr};
	};
	ChildProcessWidget(Params p_params, QWidget *p_parent = nullptr);

private slots:
	void onListWindowReadyRead();
	void onErrorOccurred(QProcess::ProcessError error);
	void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void onStateChanged(QProcess::ProcessState newState);

private:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void moveEvent(QMoveEvent *event) override;
	void timerEvent(QTimerEvent *event) override;
	void closeEvent(QCloseEvent *) override;
	void focusOutEvent(QFocusEvent *event) override;
	//void

private:
	Params _params;
	QProcess *process {nullptr};
	QProcess *_pListWindows {nullptr};
	QProcess *_pPreResizeWindow {nullptr};
	QProcess *_pResizeWindow {nullptr};
	QProcess *_pResizeWindow2 {nullptr};
	quint32 _wId {0};
	bool _resizeNeeded {true};
	QSize _size;
	QPixmap _screenshot;
	uint _timer {0};
};

#endif // CHILDPROCESSWIDGET_H
