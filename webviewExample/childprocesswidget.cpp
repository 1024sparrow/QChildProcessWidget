#include "childprocesswidget.h"

#include <QGuiApplication>
#include <QScreen>
#include <QWindow>
#include <QMoveEvent>

#include <QProcess>
#include <QTextStream>
#include <QPainter>
#include <QDebug> // boris debug

ChildProcessWidget::ChildProcessWidget(Params p_params, QWidget *p_parent)
	: QWidget(p_parent)
	, _params(p_params)
{
	process = new QProcess(this);
	process->start(p_params.app, p_params.args);

	if (p_params.windowTitle)
	{
		_pListWindows = new QProcess();
		connect(_pListWindows, &QProcess::errorOccurred,
				this, &ChildProcessWidget::onErrorOccurred);
		connect(_pListWindows, &QProcess::readyRead,
				this, &ChildProcessWidget::onListWindowReadyRead);
		connect(_pListWindows, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
				this, &ChildProcessWidget::onFinished);
		connect(_pListWindows, &QProcess::stateChanged,
				this, &ChildProcessWidget::onStateChanged);
	}

	_pPreResizeWindow = new QProcess();
	connect(_pPreResizeWindow, &QProcess::errorOccurred,
			this, &ChildProcessWidget::onErrorOccurred);
//		connect(_pPreResizeWindow, &QProcess::readyRead,
//				this, &ChildProcessWidget::onListWindowReadyRead);
	connect(_pPreResizeWindow, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			this, &ChildProcessWidget::onFinished);
	connect(_pPreResizeWindow, &QProcess::stateChanged,
			this, &ChildProcessWidget::onStateChanged);

	_pResizeWindow = new QProcess();
	connect(_pResizeWindow, &QProcess::errorOccurred,
			this, &ChildProcessWidget::onErrorOccurred);
//		connect(_pResizeWindow, &QProcess::readyRead,
//				this, &ChildProcessWidget::onListWindowReadyRead);
	connect(_pResizeWindow, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			this, &ChildProcessWidget::onFinished);
	connect(_pResizeWindow, &QProcess::stateChanged,
			this, &ChildProcessWidget::onStateChanged);

	_pResizeWindow2 = new QProcess();
	connect(_pResizeWindow2, &QProcess::errorOccurred,
			this, &ChildProcessWidget::onErrorOccurred);
//		connect(_pResizeWindow2, &QProcess::readyRead,
//				this, &ChildProcessWidget::onListWindowReadyRead);
	connect(_pResizeWindow2, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
			this, &ChildProcessWidget::onFinished);
	connect(_pResizeWindow2, &QProcess::stateChanged,
			this, &ChildProcessWidget::onStateChanged);

	startTimer(50);
}

void ChildProcessWidget::onListWindowReadyRead()
{
//	qDebug() << "--------" << _pListWindows->readAllStandardOutput();
	QByteArray ba = _pListWindows->readAllStandardOutput();
	QTextStream ts(ba);
	ts.setCodec("utf-8");
	quint32 targetWindowId = 0;
	for (QString line = ts.readLine() ; !line.isEmpty() ; line = ts.readLine())
	{
		int state = 0;
		QString strWid;
		QString strDesktop;
		QString strPcName;
		QString strWindowName;
		for (QChar ch : line)
		{
			if (state == 0)
			{
				if (ch.isSpace())
				{
					state = 1;
				}
				else
				{
					strWid.append(ch);
				}
			}
			else if (state == 1)
			{
				if (ch.isSpace())
				{
					if (strDesktop.isEmpty())
					{
						continue;
					}
					else
					{
						state = 2;
					}
				}
				else
				{
					strDesktop.append(ch);
				}
			}
			else if (state == 2)
			{
				if (ch.isSpace())
				{
					if (strPcName.isEmpty())
					{
						continue;
					}
					else
					{
						state = 3;
					}
				}
				else
				{
					strPcName.append(ch);
				}
			}
			else if (state == 3)
			{
				strWindowName.append(ch);
			}
		}
		//qDebug() << strWindowName << _params.windowTitle;
		if (!QString::compare(strWindowName, _params.windowTitle))
		{
			bool ok = false;
			targetWindowId = strWid.toUInt(&ok, 16);
			if (!ok)
			{
				targetWindowId = 0;
			}
			break;
		}
	}
	_wId = targetWindowId;
	qDebug() << _wId;
}

void ChildProcessWidget::onErrorOccurred(QProcess::ProcessError error)
{
	qDebug()<< "error: " << static_cast<int>(error);
}

void ChildProcessWidget::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
//	qDebug() << "finished";
}

void ChildProcessWidget::onStateChanged(QProcess::ProcessState newState)
{
//	qDebug() << "onStateChanged";
}

void ChildProcessWidget::paintEvent(QPaintEvent *event)
{
}

void ChildProcessWidget::resizeEvent(QResizeEvent *event)
{
}

void ChildProcessWidget::moveEvent(QMoveEvent *event)
{
}

void ChildProcessWidget::timerEvent(QTimerEvent *event)
{
	//qDebug() << mapToGlobal(pos());
	if (_pListWindows)
	{
		if (++_timer % 20 == 0)
		{
			_pListWindows->start("wmctrl", {"-l"});
		}
	}

	if (_wId)
	{
		QString strWid = QString("0x") + QString::number(_wId, 16);
		QString strWidth = QString::number(_size.width());
		QString strHeigt = QString::number(_size.height());

		QWindow *wind = QWindow::fromWinId(_wId);
		if (isVisible() && !window()->isMinimized())
		{
			if (_timer % 60 == 0)
			{
	//			if (auto windowState = window->windowState())
				{
					_pResizeWindow2->start("wmctrl", {"-ir", strWid, "-b", "remove,hidden"});
					_pPreResizeWindow->start("wmctrl", {"-ir", strWid, "-b", "remove,maximized_vert,maximized_horz"});
					_pResizeWindow->start("wmctrl", {"-ir", strWid, "-b", "add,above"});
				}
			}
			wind->resize(size().width() - 1, size().height() - 1);
			QPoint posCand = pos();
			if (isWindow())
			{
				posCand.rx() += 1;
				posCand.setY(posCand.y() + frameSize().height() - height());
			}
			wind->setPosition(posCand);
		}
		else
		{
			wind->hide();
			_pPreResizeWindow->start("wmctrl", {"-ir", strWid, "-b", "add,hidden"});
		}
	}
}

void ChildProcessWidget::closeEvent(QCloseEvent *)
{
	if (_wId)
	{
		process->close();
	}
}

void ChildProcessWidget::focusOutEvent(QFocusEvent *event)
{
}
