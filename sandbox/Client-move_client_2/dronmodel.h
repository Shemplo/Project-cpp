#ifndef DRONMODEL_H
#define DRONMODEL_H

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>

class DronModel : public QObject, public QGraphicsItem {
		
	Q_OBJECT
		
	public:
		explicit DronModel ();
		
	protected:
		QRectF boundingRect () const;
		void paint (QPainter* painter,
					const QStyleOptionGraphicsItem* option,
					QWidget* widget);
		
	private:
		qint32 width, height;
		QRectF bounds;
		
};

#endif // DRONMODEL_H
