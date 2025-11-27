#ifndef OBSTACULOGRAFICO_H
#define OBSTACULOGRAFICO_H

#include <QGraphicsItem>
#include <QPainter>
#include "obstaculo.h"

class ObstaculoGrafico : public QGraphicsItem
{
private:
    Obstaculo* o;
    int ancho, alto;
    const int altoEscena = 600;

public:
    ObstaculoGrafico(Obstaculo* obstaculo);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void actualizarPosicion();
    Obstaculo* getObstaculo() const { return o; }
};

#endif // OBSTACULOGRAFICO_H
