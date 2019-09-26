#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalWidgetTP2->hide();
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
        ui->verticalWidgetTP1->show();
        ui->verticalWidgetTP2->hide();
    } else {
        ui->verticalWidgetTP1->hide();
        ui->verticalWidgetTP2->show();
    }
   ui->widget->switchTP(index);
}

void MainWindow::on_pushButtonFlipEdge_released()
{
    ui->widget->flipRandomEdge();
}
