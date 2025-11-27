#include "obstaculografico.h"
#include <QDebug>

ObstaculoGrafico::ObstaculoGrafico(Obstaculo* obstaculo)
{
    o = obstaculo;
    ancho = (int)o->getAncho();
    alto = (int)o->getAlto();

    setPos(o->getX(), altoEscena - o->getY() - o->getAlto());
}

QRectF ObstaculoGrafico::boundingRect() const
{
    // Siempre la posicion se empieza en el 0 0, que es como la parte izquierda de la pantralla//
    return QRectF(0, 0, ancho, alto);
}

void ObstaculoGrafico::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Aqui se dibuja el obstaculo
    if (o->estaDestruido()) {
        painter->setBrush(QBrush(Qt::transparent));
    } else {
        painter->setBrush(QBrush(QColor(150, 0, 0)));
        painter->drawRect(boundingRect());
    }

    // Se diuja una barra de vida//
    if (!o->estaDestruido()) {
        double resistenciaActual = o->getResistencia();
        double resistenciaMaxima = o->getResistenciaInicial();
        double porcentajeVida = resistenciaActual / resistenciaMaxima;

        //Een este aprtado se define el espacio de la barra de vida//
        const int altoBarra = 5;
        const int margenSuperior = 10;

        //El color del fonde de la barra de vida//
        painter->setBrush(QBrush(Qt::gray));
        painter->drawRect(0, -margenSuperior, ancho, altoBarra);

        // Y el color de la vida actual//
        painter->setBrush(QBrush(Qt::green));
        painter->drawRect(0, -margenSuperior, ancho * porcentajeVida, altoBarra);
    }
}

void ObstaculoGrafico::actualizarPosicion()
{
    // Este solo se llama si es necesario hacer algun cambio, ya sea en el color de l abarra de vida, etc//
    update();
}
