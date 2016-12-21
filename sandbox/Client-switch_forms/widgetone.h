#ifndef WIDGETONE_H
#define WIDGETONE_H

#include <QWidget>

namespace Ui {
	class WidgetOne;
}

class WidgetOne : public QWidget {
		
	Q_OBJECT
		
	public:
		explicit WidgetOne (QWidget* parent = 0);
		~WidgetOne ();
		
	signals:
		void signalSwitchForm (int form);
		
	public slots:
		void slotToTwoForm   ();
		void slotToThreeForm ();
		
	private:
		Ui::WidgetOne* ui;
		
};

#endif // WIDGETONE_H
