#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QMessageBox>
#include <cmath>

//Se colocan o incluyen todos los archivos que hicimos de fisica y parte graficca//
#include "simulador.h"
#include "particula.h"
#include "particulagrafica.h"
#include "obstaculografico.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //El botond e disparo//
    void on_btnIngresar_clicked();

    //Es el que constantemente esta actualizando posicion y lleva el timer de Qtimer//
    void actualizarSimulacion();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Simulador *simulador;
    QTimer *timer;

    ParticulaGrafica *proyectilGrafico;

    //La dimension de la escena o como prefiero llamarlo yo, la resolucion//
    const double anchoEscena = 800.0;
    const double altoEscena = 600.0;

    //Los metyodos con los que se gestiona el juego//
    void inicializarJuego();
    void resetearProyectil();
    void verificarColisiones();
};
#endif // MAINWINDOW_H
