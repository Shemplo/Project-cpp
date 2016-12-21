#ifndef WIDGETTHREE_H
#define WIDGETTHREE_H

#include <QWidget>

namespace Ui {
	class WidgetThree;
}

class WidgetThree : public QWidget {
		
	Q_OBJECT
		
	public:
		explicit WidgetThree (QWidget* parent = 0);
		~WidgetThree ();
		
	signals:
		void signalSwitchForm (int form);
		
	public slots:
		void slotToOneForm ();
		void slotToTwoForm ();
		
	private:
		Ui::WidgetThree* ui;
		
};

#endif // WIDGETTHREE_H
