#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>


class MyPixmapItem:public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MyPixmapItem(QGraphicsPixmapItem *parent=0);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void wheelEvent(QGraphicsSceneWheelEvent*);
    void reset();
private:
    double RATE;
};

#endif // MYPIXMAPITEM_H
