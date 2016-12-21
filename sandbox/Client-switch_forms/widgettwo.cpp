#include "widgettwo.h"
#include "ui_widgettwo.h"

WidgetTwo::WidgetTwo (QWidget* parent) : QWidget (parent),
											ui (new Ui::WidgetTwo) {
	ui->setupUi (this);
	
	connect (ui->buttonW1, &QPushButton::clicked,
			 this, &WidgetTwo::slotToOneForm);
	connect (ui->buttonW3, &QPushButton::clicked,
			 this, &WidgetTwo::slotToThreeForm);
}

void WidgetTwo::slotToOneForm () {
	emit signalSwitchForm (1);
}

void WidgetTwo::slotToThreeForm () {
	emit signalSwitchForm (3);
}

WidgetTwo::~WidgetTwo () {
	delete ui;
}
