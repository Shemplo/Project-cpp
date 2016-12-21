#include "widgetone.h"
#include "ui_widgetone.h"

WidgetOne::WidgetOne (QWidget* parent) : QWidget (parent),
											ui (new Ui::WidgetOne) {
	ui->setupUi (this);
	
	connect (ui->buttonW2, &QPushButton::clicked,
			 this, &WidgetOne::slotToTwoForm);
	connect (ui->buttonW3, &QPushButton::clicked,
			 this, &WidgetOne::slotToThreeForm);
}

void WidgetOne::slotToTwoForm () {
	emit signalSwitchForm (2);
}

void WidgetOne::slotToThreeForm () {
	emit signalSwitchForm (3);
}

WidgetOne::~WidgetOne () {
	delete ui;
}
