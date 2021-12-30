#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "ui_main_window.h"
#include "MeshConfig.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Main setup
    void on_radio_button_2d_released();
    void on_radio_button_3d_released();
    void on_display_type_push_button_released();
    // 2D
    void on_algorithm_type_combobox_currentIndexChanged(int index);
    void on_voronoi_display_push_button_released();
    void on_insert_points_push_button_released();
    void on_parabola_type_combobox_currentIndexChanged(int index);
    void on_refine_push_button_released();
    // 3D
    void on_color_display_combobox_currentIndexChanged(int index);
    void on_mesh_3d_type_combobox_currentIndexChanged(int index);
    void on_simplify_push_button_released();

private:
    // UI
    Ui::MainWindow ui;
    std::shared_ptr<MeshConfig> m_mesh_config;

    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<std::unordered_map<Face_Hash, Vertex>> m_voronoi_vertices;

    void switch_dimension();
    void update_voronoi_vertices();
    void set_mesh();
};

#endif  // MAINWINDOW_H
