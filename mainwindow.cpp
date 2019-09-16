#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonDisplayType_released() {
    if (ui->pushButtonDisplayType->text() == "Show wireframe")  ui->pushButtonDisplayType->setText("Show plain faces");
    else ui->pushButtonDisplayType->setText("Show wireframe");
    ui->widget->toggleDisplayType();
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
    ui->widget->switchMesh(index);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index) {
    ui->widget->switchCurveAxis(index);
}
