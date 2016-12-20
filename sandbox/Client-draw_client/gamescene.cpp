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

//A - 65
//S - 83
//D - 68
//W - 87
//  - 32
//E - 69
//R - 82

void GameScene::keyPressEvent (QKeyEvent* event) {
	emit signalButton (event->key (), 1);
}

void GameScene::keyReleaseEvent (QKeyEvent* event) {
	emit signalButton (event->key (), 0);
}
