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
	void resizeEvent(QResizeEvent *event) override final;
	void timerEvent(QTimerEvent *event) override final;

private:
	Params _params;
	QProcess *_pListWindows {nullptr};
	QProcess *_pResizeWindow {nullptr};
	quint32 _wId {0};
	bool _resizeNeeded {false};
	QSize _size;
};

#endif // CHILDPROCESSWIDGET_H
