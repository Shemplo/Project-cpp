#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <widgetone.h>
#include <widgettwo.h>
#include <widgetthree.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
		
	Q_OBJECT
		
	public:
		explicit MainWindow (QWidget* parent = 0);
		~MainWindow ();
		
	private slots:
		void slotSwitchForm (int form);
		
	private:
		Ui::MainWindow* ui;
		
};

#endif // MAINWINDOW_H
