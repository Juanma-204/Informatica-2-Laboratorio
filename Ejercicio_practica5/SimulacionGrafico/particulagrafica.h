#ifndef PARTICULAGRAFICA_H
#define PARTICULAGRAFICA_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include "particula.h"

class ParticulaGrafica : public QGraphicsItem
{
private:
    Particula* p;
    QPixmap pixmap;
    int radioGrafico;

public:
    ParticulaGrafica(Particula* particula);

    //Esstos son metodo obligatorios de qt para el bounding, etc//
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //Este nos actualiza o sincroniza por decirlo asi la posicion con el objeto que se vaya a entcontrar o donde este//
    void actualizarPosicion();

    // Getter para acceder a la partícula fisica si se necesita//
    Particula* getParticula() const { return p; }
};

#endif // PARTICULAGRAFICA_H
