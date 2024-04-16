#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QTimer"
#include "NetFace.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    netFace = new NetFace(7654);
    connect(netFace, &NetFace::dataReceived, this, &MainWindow::onDataReceived);
}

MainWindow::~MainWindow() {
    delete ui;
    delete netFace;
}

void MainWindow::onDataReceived(const QVector<ShapeTime> &mouthShapes) {
    for (auto shapeTime: mouthShapes) {
        QTimer::singleShot(shapeTime.delay, Qt::PreciseTimer, this, [shapeTime, this]() {
            changeMouthShape(shapeTime.shape);
        });
    }
}

void MainWindow::changeMouthShape(MouthShape mouthShape) {
    QString file = "";
    switch (mouthShape) {
        case MouthShape::B:
            file = "b.png";
            break;
        case MouthShape::C:
            file = "c.png";
            break;
        case MouthShape::D:
            file = "d.png";
            break;
        case MouthShape::E:
            file = "e.png";
            break;
        case MouthShape::F:
            file = "f.png";
            break;
            // A and X route to the same image
        default:
            file = "a.png";
            break;
    }
    QString fullPath = terraDir + file;

    QPixmap image(fullPath);
    ui->Face->setPixmap(image);
}
