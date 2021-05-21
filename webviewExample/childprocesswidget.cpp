#include "childprocesswidget.h"

#include <QProcess>
#include <QTextStream>
#include <QDebug> // boris debug

ChildProcessWidget::ChildProcessWidget(Params p_params, QWidget *p_parent)
	: QWidget(p_parent)
	, _params(p_params)
{
	QProcess *process = new QProcess(this);
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

		_pResizeWindow = new QProcess();
		connect(_pResizeWindow, &QProcess::errorOccurred,
				this, &ChildProcessWidget::onErrorOccurred);
//		connect(_pResizeWindow, &QProcess::readyRead,
//				this, &ChildProcessWidget::onListWindowReadyRead);
		connect(_pResizeWindow, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
				this, &ChildProcessWidget::onFinished);
		connect(_pResizeWindow, &QProcess::stateChanged,
				this, &ChildProcessWidget::onStateChanged);
	}

	startTimer(1000);
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

void ChildProcessWidget::resizeEvent(QResizeEvent *event)
{
	//QProcess
	_size = size();
	_resizeNeeded = true;
}

void ChildProcessWidget::timerEvent(QTimerEvent *event)
{
	if (_pListWindows)
	{
		_pListWindows->start("wmctrl", {"-l"});
	}
	if (_pResizeWindow)
	{
		if (_wId)
		{
			QString strWid = QString("0x") + QString::number(_wId, 16);
			QString strWidth = QString::number(_size.width());
			QString strHeigt = QString::number(_size.height());
			_pResizeWindow->start("xdotool", {"windowsize", strWid, strWidth, strHeigt});
			//qDebug() << _pResizeWindow->program() << _pResizeWindow->arguments();
		}
	}
}
