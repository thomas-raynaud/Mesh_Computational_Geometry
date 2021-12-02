#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonInsertNPoints->hide();
    ui->pushButtonInsertPoint->hide();
    ui->pushButtonxxyy->hide();
    ui->pushButtonRosenbrock->hide();
    ui->pushButtonSimplifyMesh->hide();
    ui->pushButtonRaffine->hide();
}


MainWindow::~MainWindow() {
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


void MainWindow::on_comboBoxTPswitch_currentIndexChanged(int index) {
    ui->pushButtonxxyy->setEnabled(true);
    ui->pushButtonRosenbrock->setEnabled(true);
    ui->pushButtonDisplayType->setEnabled(true);
    switch (index) {
        case 0:
            ui->pushButtonInsertNPoints->hide();
            ui->pushButtonInsertPoint->hide();
            ui->pushButtonxxyy->hide();
            ui->pushButtonRosenbrock->hide();
            ui->comboBoxColorswitch->show();
            ui->comboBoxMeshswitch->show();
            ui->pushButtonSimplifyMesh->hide();
            ui->pushButtonRaffine->hide();
            break;
        case 1:
            ui->pushButtonInsertNPoints->show();
            ui->pushButtonInsertPoint->show();
            ui->pushButtonxxyy->hide();
            ui->pushButtonRosenbrock->hide();
            ui->comboBoxColorswitch->hide();
            ui->comboBoxMeshswitch->hide();
            ui->pushButtonSimplifyMesh->hide();
            ui->pushButtonRaffine->hide();
            break;
        case 2:
            ui->pushButtonInsertNPoints->hide();
            ui->pushButtonInsertPoint->hide();
            ui->pushButtonxxyy->show();
            ui->pushButtonRosenbrock->show();
            ui->comboBoxColorswitch->hide();
            ui->comboBoxMeshswitch->hide();
            ui->pushButtonSimplifyMesh->hide();
            ui->pushButtonRaffine->hide();
            break;
        case 3:
            ui->pushButtonInsertNPoints->hide();
            ui->pushButtonInsertPoint->hide();
            ui->pushButtonxxyy->show();
            ui->pushButtonRosenbrock->show();
            ui->comboBoxColorswitch->hide();
            ui->comboBoxMeshswitch->hide();
            ui->pushButtonSimplifyMesh->hide();
            ui->pushButtonxxyy->setEnabled(false);
            ui->pushButtonRosenbrock->setEnabled(false);
            ui->pushButtonRaffine->hide();
            break;
        case 4:
            ui->pushButtonInsertNPoints->hide();
            ui->pushButtonInsertPoint->hide();
            ui->pushButtonxxyy->hide();
            ui->pushButtonRosenbrock->hide();
            ui->comboBoxColorswitch->hide();
            ui->comboBoxMeshswitch->show();
            ui->pushButtonSimplifyMesh->show();
            ui->pushButtonDisplayType->setEnabled(false);
            ui->pushButtonSimplifyMesh->setEnabled(true);
            //ui->comboBoxMeshswitch->setEnabled(false);
            ui->pushButtonRaffine->hide();
            break;
        case 5:
            ui->pushButtonInsertNPoints->hide();
            ui->pushButtonInsertPoint->hide();
            ui->pushButtonxxyy->hide();
            ui->pushButtonRosenbrock->hide();
            ui->comboBoxColorswitch->hide();
            ui->comboBoxMeshswitch->hide();
            ui->pushButtonSimplifyMesh->show();
            ui->pushButtonDisplayType->setEnabled(false);
            ui->pushButtonSimplifyMesh->setEnabled(false);
            ui->pushButtonRaffine->show();
            break;
    }

    ui->widget->switchTP(index);
}


void MainWindow::on_pushButtonInsertNPoints_released() {
    ui->widget->insertNPoints(100);
}


void MainWindow::on_pushButtonInsertPoint_released() {
    ui->widget->insertNPoints(1);
}


void MainWindow::on_pushButtonxxyy_released() {
    ui->widget->switchParabolaType(0);
}


void MainWindow::on_pushButtonRosenbrock_released() {
    ui->widget->switchParabolaType(1);
}


void MainWindow::on_pushButtonSimplifyMesh_clicked() {
   ui->widget->simplifyMesh();
}


void MainWindow::on_pushButtonRaffine_clicked() {
    ui->widget->raffine(0.85);
}