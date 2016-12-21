#include "widgetthree.h"
#include "ui_widgetthree.h"

WidgetThree::WidgetThree (QWidget* parent) : QWidget (parent),
												ui (new Ui::WidgetThree) {
	ui->setupUi (this);
	
	connect (ui->buttonW1, &QPushButton::clicked,
			 this, &WidgetThree::slotToOneForm);
	connect (ui->buttonW2, &QPushButton::clicked,
			 this, &WidgetThree::slotToTwoForm);
}

void WidgetThree::slotToOneForm () {
	emit signalSwitchForm (1);
}

void WidgetThree::slotToTwoForm () {
	emit signalSwitchForm (2);
}

WidgetThree::~WidgetThree () {
	delete ui;
}
