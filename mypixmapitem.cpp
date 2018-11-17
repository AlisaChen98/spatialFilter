#include "mypixmapitem.h"
#include <QDebug>

MyPixmapItem::MyPixmapItem(QGraphicsPixmapItem *parent):QGraphicsPixmapItem(parent),
    RATE(0.2)
{

}

void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
    qDebug()<<"press";
}
void MyPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent*event)
{
   qDebug()<<"move";
   setPos(pos()
          +mapToParent(event->pos())
          -mapToParent(event->lastPos()));
}
void MyPixmapItem::wheelEvent(QGraphicsSceneWheelEvent*event)
{
    qDebug()<<"wheel";
    int delta=event->delta();
    double factor=scale();//返回当前缩放位置,factor是最新的缩放位置
    if(delta>0)
    {
        factor *= (1+RATE);
    }
    if(delta<0)
    {
        factor *= (1-RATE);
    }
    setTransformOriginPoint(boundingRect().width()/2,boundingRect().height()/2);
    setScale(factor);
}
void MyPixmapItem::reset()
{
    setPos(0,0);
    setScale(1.0);
}
