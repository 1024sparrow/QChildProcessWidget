#include "mainwindow.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

const QString BN_NAME_SHOW_WEBVIEW = "[ SHOW ]";
const QString BN_NAME_HIDE_WEBVIEW = "[ HIDE ]";

MainWindow::MainWindow()
	: QWidget(nullptr)
{
	QBoxLayout *lay = new QVBoxLayout(this);
	{
		QPushButton *bnToggleWebview = new QPushButton(BN_NAME_SHOW_WEBVIEW, this);
		{
			bnToggleWebview->setCheckable(true);
			bnToggleWebview->setChecked(false);
			connect(bnToggleWebview, &QAbstractButton::clicked, this, &MainWindow::onBnShowWebviewToggled);
		}
		bnToggleWebview->setFixedHeight(64);
		lay->addWidget(bnToggleWebview);

		QLabel *contentWidget = new QLabel("Some content ...", this);
		{
			contentWidget->setAutoFillBackground(true);
			auto pal = contentWidget->palette();
			pal.setBrush(QPalette::Background, Qt::BDiagPattern);
			contentWidget->setPalette(pal);
		}
		lay->addWidget(contentWidget, 1);
	}

}

void MainWindow::onBnShowWebviewToggled(bool b)
{
	QPushButton *bnSender = qobject_cast<QPushButton *>(sender());
	if (b)
	{
		bnSender->setText(BN_NAME_HIDE_WEBVIEW);
	}
	else
	{
		bnSender->setText(BN_NAME_SHOW_WEBVIEW);
	}
}
