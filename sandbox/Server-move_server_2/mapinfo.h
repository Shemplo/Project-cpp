#ifndef MAPINFO_H
#define MAPINFO_H

#include <QVector>
#include <QPolygonF>

class MapInfo {
		
	public:
		MapInfo ();
		
		// Structure of keeping
		// data about map (...)
		struct Map {
			QVector <QPointF>   spawns;
			QVector <QPolygonF> bases;
			QVector <QPolygonF> obstacles;
		};
		
		// Get informaton about map
		// with given number (id)
		Map    getMap    (qint32);
		
		// Get number of maps in array
		qint32 getNumber ();
		
	private:
		QVector <Map> maps;
		
};

#endif // MAPINFO_H
