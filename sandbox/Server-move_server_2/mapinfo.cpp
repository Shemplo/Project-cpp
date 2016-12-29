#include "mapinfo.h"

MapInfo::MapInfo () {
	Map first;
	
	first.spawns.append (QPoint (-200, -200));
	first.spawns.append (QPoint (200, 200));
	first.spawns.append (QPoint (200, -200));
	first.spawns.append (QPoint (-200, 200));
	
	QPolygonF fObs1;
	fObs1 << QPointF (-200, -150);
	fObs1 << QPointF (-190, -110);
	fObs1 << QPointF (-130, -110);
	fObs1 << QPointF (-40, -180);
	fObs1 << QPointF (-110, -190);
	fObs1 << QPointF (-170, -170);
	first.obstacles.append (fObs1);
	
	maps.append (first);
}

MapInfo::Map MapInfo::getMap (qint32 index) {
	if (index >= 0 && index < maps.size ()) {
		return maps.at (index);
	}
	
	Map map;
	return map;
}

qint32 MapInfo::getNumber () {
	return maps.size ();
}
