#ifndef WIDGETTWO_H
#define WIDGETTWO_H

#include <QWidget>

namespace Ui {
	class WidgetTwo;
}

class WidgetTwo : public QWidget {
		
	Q_OBJECT
		
	public:
		explicit WidgetTwo (QWidget* parent = 0);
		~WidgetTwo ();
		
	signals:
		void signalSwitchForm (int form);
		
	public slots:
		void slotToOneForm   ();
		void slotToThreeForm ();
		
	private:
		Ui::WidgetTwo* ui;
		
};

#endif // WIDGETTWO_H
