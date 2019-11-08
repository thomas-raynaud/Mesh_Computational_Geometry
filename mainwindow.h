#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonDisplayType_released();

    void on_comboBoxMeshswitch_currentIndexChanged(int index);

    void on_comboBoxColorswitch_currentIndexChanged(int index);

    void on_comboBoxTPswitch_currentIndexChanged(int index);

    void on_pushButtonInsertNPoints_released();

    void on_pushButtonInsertPoint_released();

    void on_pushButtonxxyy_released();

    void on_pushButtonRosenbrock_released();

    void on_pushButtonSimplifyMesh_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
