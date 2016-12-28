#ifndef MAPINFO_H
#define MAPINFO_H

#include <QVector>
#include <QPolygonF>

class MapInfo {
		
	public:
		explicit MapInfo ();
		
		struct Map {
			QVector <QPointF>   spawns;
			QVector <QPolygonF> bases;
			QVector <QPolygonF> obstacles;
		};
		
		Map    getMap    (qint32);
		qint32 getNumber ();
		
	private:
		QVector <Map> maps;
		
};

#endif // MAPINFO_H
