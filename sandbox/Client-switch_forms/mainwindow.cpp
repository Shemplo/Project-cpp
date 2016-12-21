#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget* parent) : QMainWindow (parent),
											ui(new Ui::MainWindow) {
	ui->setupUi (this);
	slotSwitchForm (1);
}

void MainWindow::slotSwitchForm (int form) {
	ui->centralWidget->deleteLater ();
	
	if (form == 1) {
		WidgetOne* widget = new WidgetOne (this);
		
		ui->centralWidget->setStyle (widget->style ());
		this->setFixedSize (widget->size ());
		ui->centralWidget = widget;
		ui->centralWidget->show ();
		
		connect (widget, &WidgetOne::signalSwitchForm,
				 this, &MainWindow::slotSwitchForm);
	} else if (form == 2) {
		WidgetTwo* widget = new WidgetTwo (this);
		
		ui->centralWidget->setStyle (widget->style ());
		this->setFixedSize (widget->size ());
		ui->centralWidget = widget;
		ui->centralWidget->show ();
		
		connect (widget, &WidgetTwo::signalSwitchForm,
				 this, &MainWindow::slotSwitchForm);
	} else if (form == 3) {
		WidgetThree* widget = new WidgetThree (this);
		
		ui->centralWidget->setStyle (widget->style ());
		this->setFixedSize (widget->size ());
		ui->centralWidget = widget;
		ui->centralWidget->show ();
		
		connect (widget, &WidgetThree::signalSwitchForm,
				 this, &MainWindow::slotSwitchForm);
	}
}

MainWindow::~MainWindow () {
	delete ui;
}
