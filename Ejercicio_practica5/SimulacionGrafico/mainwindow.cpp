#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(0, 0, anchoEscena, altoEscena, this);
    scene->setSceneRect(0, 0, anchoEscena, altoEscena);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->scale(1, -1);
    ui->graphicsView->translate(0, -altoEscena);


    simulador = new Simulador(anchoEscena, altoEscena, 0.9);

    inicializarJuego();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarSimulacion);

    proyectilGrafico = nullptr;
}

MainWindow::~MainWindow()
{
    delete simulador;
    delete timer;
    delete ui;
}

void MainWindow::inicializarJuego()
{
    double x_obs = 650;
    double y_obs = 400;
    double ancho_obs = 100;
    double alto_obs = 100;
    double resistencia = 500;

    Obstaculo *rivalFisico = new Obstaculo(x_obs, y_obs, ancho_obs, alto_obs, resistencia);
    simulador->agregarObstaculo(rivalFisico);

    ObstaculoGrafico *rivalGrafico = new ObstaculoGrafico(rivalFisico);
    scene->addItem(rivalGrafico);
}

void MainWindow::resetearProyectil()
{
    if (proyectilGrafico) {
        if (timer->isActive()) timer->stop();

        Particula* p_fisica = proyectilGrafico->getParticula();

        scene->removeItem(proyectilGrafico);
        simulador->eliminarParticula(p_fisica);

        delete proyectilGrafico;
        proyectilGrafico = nullptr;
    }
}

void MainWindow::verificarColisiones()
{
    if (!proyectilGrafico) return;

    Particula* p = proyectilGrafico->getParticula();

    for (int i = 0; i < simulador->getNumObstaculos(); ++i) {
        Obstaculo* obsFisico = simulador->getObstaculo(i);

        if (!obsFisico->estaDestruido() && obsFisico->verificaColision(*p)) {
            double velocidad_cuadrada = std::pow(p->getVX(), 2) + std::pow(p->getVY(), 2);
            double dano = 0.5 * p->getMasa() * velocidad_cuadrada / 100.0;

            obsFisico->recibirDano(dano);

            for (QGraphicsItem *item : scene->items()) {
                ObstaculoGrafico *obsGrafico = dynamic_cast<ObstaculoGrafico*>(item);
                if (obsGrafico && obsGrafico->getObstaculo() == obsFisico) {
                    obsGrafico->actualizarPosicion();
                    break;
                }
            }
        }
    }
}

void MainWindow::actualizarSimulacion()
{
    if (!proyectilGrafico) {
        timer->stop();
        return;
    }

    double dt = 0.05;

    Particula* p = proyectilGrafico->getParticula();

    simulador->pasoDeSimulacion(dt);
    scene->update();

    proyectilGrafico->actualizarPosicion();


    if ((std::fabs(p->getVX()) < 0.01 && std::fabs(p->getVY()) < 0.01) || p->getY() < -50) {
        resetearProyectil();
    }
    else if (p->getX() > anchoEscena + 50 || p->getY() > altoEscena + 50 || p->getX() < -50) {
        resetearProyectil();
    }
}

void MainWindow::on_btnIngresar_clicked()
{
    if (timer->isActive() || proyectilGrafico) return;

    bool ok_v, ok_ang, ok_m;
    double v0 = ui->le_V->text().toDouble(&ok_v);
    double ang_deg = ui->le_Angulo->text().toDouble(&ok_ang);
    double masa = ui->le_Masa->text().toDouble(&ok_m);

    if (!ok_v || !ok_ang || !ok_m || v0 <= 0 || masa <= 0 || ang_deg < 0 || ang_deg > 180) {
        QMessageBox::warning(this, "Error de Entrada", "Por favor coloque los valores positivos y que sean validos obvio (Velocidad, Masa) y un Ángulo entre 0 y 180.");
        return;
    }

    double ang_rad = (ang_deg * M_PI) / 180.0;
    double vx0 = v0 * std::cos(ang_rad);
    double vy0 = -v0 * std::sin(ang_rad);

    Particula *nuevaParticula = new Particula(0, 500, vx0, vy0, masa, 15);
    nuevaParticula->setG(9.8);
    simulador->agregarParticula(nuevaParticula);

    proyectilGrafico = new ParticulaGrafica(nuevaParticula);
    scene->addItem(proyectilGrafico);

    timer->start(50);
}
