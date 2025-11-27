#include "particulagrafica.h"

ParticulaGrafica::ParticulaGrafica(Particula* particula)
{
    p = particula;
    radioGrafico = (int)p->getRadio();

    // Carga la imagen para representar el objeto, o lo que se mueve//
    pixmap.load(":/ave/pajaro.png");
    pixmap = pixmap.scaled(2 * radioGrafico, 2 * radioGrafico, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPos(p->getX() - radioGrafico, 600 - p->getY() - radioGrafico);
}

QRectF ParticulaGrafica::boundingRect() const
{
    return QRectF(0, 0, 2 * radioGrafico, 2 * radioGrafico);
}

void ParticulaGrafica::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(boundingRect(), pixmap, pixmap.rect());
}

void ParticulaGrafica::actualizarPosicion()
{
    setPos(p->getX() - radioGrafico, 600 - p->getY() - radioGrafico);
}
