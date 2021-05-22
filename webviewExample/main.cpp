#include <QApplication>
#include <string.h>
#include <stdio.h>

#include "mainwindow.h"

#include "childprocesswidget.h"

int main(int argc, char **argv)
{
	for (int iArg = 0 ; iArg < argc ; ++iArg)
	{
		char *arg = argv[iArg];
		if (!strcmp(arg, "--help"))
		{
			puts(R"(Example of Qt5-application (widgets-based) with thirdside webview (under BSD license 2.0))");
			return 0;
		}
		else if (arg[0] == '-')
		{
			printf("Unknown key \"%s\". See help.\n", arg);
			return 1;
		}
	}

	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.resize(500,500);
	mainWindow.show();

//	ChildProcessWidget *webview = new ChildProcessWidget(
//		ChildProcessWidget::Params{
//			"webview/1/webview-example",
//			{},
//			"Minimal example"
//		}
//	);
//	webview->resize(500,500);
//	webview->show();

	return app.exec();
}
