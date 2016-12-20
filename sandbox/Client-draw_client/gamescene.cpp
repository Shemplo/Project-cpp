#include "gamescene.h"

GameScene::GameScene (QObject* parent) : QGraphicsScene () {
	Q_UNUSED (parent);
}

void GameScene::mouseMoveEvent (QGraphicsSceneMouseEvent* event) {
	emit signalMove (event->scenePos ());
}

void GameScene::mousePressEvent (QGraphicsSceneMouseEvent* event) {
	emit signalClick (event->button ());
}

void GameScene::mouseReleaseEvent (QGraphicsSceneMouseEvent* event) {
	Q_UNUSED (event);
	emit signalClick (-1);
}
