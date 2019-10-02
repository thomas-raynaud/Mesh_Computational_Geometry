#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonFlipEdge->hide();
    ui->pushButtonSplitTriangle->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonDisplayType_released() {
    if (ui->pushButtonDisplayType->text() == "Show wireframe")
        ui->pushButtonDisplayType->setText("Show plain faces");
    else
        ui->pushButtonDisplayType->setText("Show wireframe");
    ui->widget->toggleDisplayType();
}

void MainWindow::on_comboBoxMeshswitch_currentIndexChanged(int index) {
    ui->widget->switchMesh(index);
}

void MainWindow::on_comboBoxColorswitch_currentIndexChanged(int index) {
    ui->widget->switchCurveAxis(index);
}

void MainWindow::on_comboBoxTPswitch_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->pushButtonFlipEdge->hide();
        ui->pushButtonSplitTriangle->hide();
        ui->comboBoxColorswitch->show();
        ui->comboBoxMeshswitch->show();
    } else {
        ui->pushButtonFlipEdge->show();
        ui->pushButtonSplitTriangle->show();
        ui->comboBoxColorswitch->hide();
        ui->comboBoxMeshswitch->hide();
    }
   ui->widget->switchTP(index);
}

void MainWindow::on_pushButtonFlipEdge_released()
{
    ui->widget->flipRandomEdge();
}

void MainWindow::on_pushButtonSplitTriangle_released()
{
    ui->widget->splitRandomTriangle();
}
