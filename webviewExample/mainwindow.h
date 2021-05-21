#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>


class MainWindow : public QWidget
{
public:
	MainWindow();

private slots:
	void onBnShowWebviewToggled(bool);
};

#endif // MAINWINDOW_H
