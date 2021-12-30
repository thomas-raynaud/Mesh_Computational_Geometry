#include "MainWindow.h"

#include "meshes/Mesh2D.h"
#include "meshes/Mesh3D.h"
#include "techniques/simplify.h"
#include "techniques/2d/MeshCrust.h"
#include "techniques/2d/MeshRuppert.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui()
{
    ui.setupUi(this);

    MeshConfig mesh_config;
    mesh_config.dimension = Dimension::D2;
    mesh_config.mesh_display_type = MeshDisplayType::PlainFaces;
    mesh_config.algorithm_2d_type = Algorithm2DType::Delaunay;
    mesh_config.show_voronoi_display = false;
    mesh_config.parabola_type = ParabolaType::EllipticParaboloid;
    mesh_config.color_display_type = ColorDisplayType::MeanCurvature;
    mesh_config.mesh_3d_type = Mesh3DType::Tetrahedron;
    m_mesh_config = std::make_shared<MeshConfig>(mesh_config);    

    ui.d3_groupbox->hide();
    ui.parabola_layout->hide();
    ui.ruppert_layout->hide();

    m_mesh = std::make_shared<Mesh>(Mesh2D());
}


void MainWindow::switch_dimension() {
    Dimension new_dim = (
        ui.radio_button_2d.checked() ? Dimension::D2 : Dimension::D3
    );
    if (m_mesh_config.dimension != new_dim) {
        m_mesh_config.m_dimension = new_dim;
        if (m_mesh_config.dimension == Dimension::D2) {
            ui.d3_groupbox->hide();
            ui.parabola_layout->hide();
            ui.ruppert_layout->hide();
            m_mesh_config.algorithm_2d_type = Algorithm2DType::Delaunay;
            update_voronoi_vertices();
            m_mesh_config.show_voronoi_display = false;
            m_mesh = std::make_shared<Mesh>(Mesh2D());
        }
        else {
            ui.d2_groupbox->hide();
            m_mesh_config.show_voronoi_display = false;
            m_mesh_config.mesh_3d_type = Mesh3DType::Tetrahedron;
            set_mesh(Tetrahedron());
            compute_laplacians(m_mesh);
            set_curvature_colors(m_mesh, m_mesh_config.color_display_type);
        }
    }
}

void MainWindow::update_voronoi_vertices() {
    m_voronoi_vertices = build_voronoi((Mesh2D*)m_mesh.get());
    ui.scene->set_voronoi_vertices(m_voronoi_vertices);
}

void MainWindow::set_mesh(const Mesh &mesh) {
    m_mesh = std::make_shared<Mesh>(mesh);
    ui.scene->set_mesh(m_mesh);
}

void MainWindow::on_radio_button_2d_released() {
    switch_dimension();
}

void MainWindow::on_radio_button_3d_released() {
    switch_dimension();
}

void MainWindow::on_display_type_push_button_released() {
    m_mesh_config.mesh_display_type = (
        m_mesh_config.mesh_display_type == MeshDisplayType::PlainFaces ?
            MeshDisplayType::Wireframe : MeshDisplayType::PlainFaces
    );
    if (m_mesh_config.mesh_display_type == MeshDisplayType::PlainFaces) {
        ui.display_type_push_button->setText("Show wireframe");
    }
    else {
        ui.display_type_push_button->setText("Show plain faces");
    }
}

void MainWindow::on_algorithm_type_combobox_currentIndexChanged(int index) {
    m_mesh_config.algorithm_2d_type = index;
    if (m_mesh_config.algorithm_2d_type == Algorithm2DType::Delaunay) {
        ui.parabola_layout->hide();
        ui.ruppert_layout->hide();
        set_mesh(Mesh2D());
    }
    else if (m_mesh_config.algorithm_2d_type == Algorithm2DType::Crust) {
        ui.parabola_layout->hide();
        ui.ruppert_layout->hide();
        set_mesh(MeshCrust());
    }
    else if (m_mesh_config.algorithm_2d_type == Algorithm2DType::Ruppert) {
        ui.parabola_layout->hide();
        ui.ruppert_layout->show();
        set_mesh(MeshRuppert());
    }
    else if (m_mesh_config.algorithm_2d_type == Algorithm2DType::Parabola) {
        ui.parabola_layout->show();
        ui.ruppert_layout->hide();
        set_mesh(MeshParabola());
    }
    update_voronoi_vertices();
}

void MainWindow::on_voronoi_display_push_button_released() {
    m_mesh_config.show_voronoi_display = !m_show_voronoi_display;
    if (m_mesh_config.show_voronoi_display) 
        ui.voronoi_display_push_button->setText("Hide Voronoi wireframe");
    else
        ui.voronoi_display_push_button->setText("Show Voronoi wireframe");
}

void MainWindow::on_insert_points_push_button_released() {
    size_t nb_pts = ui.insert_points_line_edit->text().toInt();
    if (nb_pts == 0) return;
    for (size_t i = 0; i < nb_pts; ++i) {
        float rand_x = -2.f + (((float) rand()) / (float) RAND_MAX) * 4.f;
        float rand_y = -2.f + (((float) rand()) / (float) RAND_MAX) * 4.f;
        insert_delaunay_vertex(m_mesh, glm::vec3(rand_x, rand_y, 0));
    }
    compute_laplacians(m_mesh);
    set_curvature_colors(m_mesh, m_mesh_config.color_display_type);
}

void MainWindow::on_parabola_type_combobox_currentIndexChanged(int index) {
    m_mesh_config.parabola_type = index;
    m_mesh = std::make_shared<Mesh>(Parabola(m_parabola_type));
    update_voronoi_vertices();
}

void MainWindow::on_refine_push_button_released() {
    ((MeshCrust*)m_mesh.get())->refine(0.85f);
    update_voronoi_vertices();
}

void MainWindow::on_color_display_combobox_currentIndexChanged(int index) {
    m_mesh_config.color_display_type = index;
    set_curvature_colors(m_mesh, m_mesh_config.color_display_type);
}

void MainWindow::on_mesh_3d_type_combobox_currentIndexChanged(int index) {
    m_mesh_config.mesh_3d_type = index;
    if (m_mesh_config.mesh_3d_type == Mesh3DType::Tetrahedron)
        m_mesh = std::make_shared<Mesh>(Tetrahedron());
    else if (m_mesh_config.mesh_3d_type == Mesh3DType::Pyramid)
        m_mesh = std::make_shared<Mesh>(Pyramid());
    else
        m_mesh = std::make_shared<Mesh>(QueenStatue());
    compute_laplacians(m_mesh);
    set_curvature_colors(m_mesh, m_mesh_config.color_display_type);
}

void MainWindow::on_simplify_push_button_released() {
    simplify(m_mesh, m_mesh->get_nb_vertices() / 2);
}