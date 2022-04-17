#include "MainWindow.h"

#include "meshes/Mesh2D.h"
#include "meshes/Mesh3D.h"
#include "techniques/simplify.h"
#include "techniques/2d/MeshCrust.h"
#include "techniques/2d/MeshRuppert.h"
#include "techniques/2d/MeshParabola.h"
#include "techniques/2d/voronoi.h"
#include "techniques/2d/delaunay.h"
#include "techniques/3d/curvature.h"


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
    ui.parabola_widget->hide();
    ui.ruppert_widget->hide();

    set_mesh(std::make_shared<Mesh2D>());
    build_convex_hull();
    update_voronoi_vertices();
    ui.scene->set_mesh_config(m_mesh_config);
}

MainWindow::~MainWindow() {}


void MainWindow::switch_dimension() {
    Dimension new_dim = (
        ui.radio_button_2d->isChecked() ? Dimension::D2 : Dimension::D3
    );
    if (m_mesh_config->dimension != new_dim) {
        m_mesh_config->dimension = new_dim;
        if (m_mesh_config->dimension == Dimension::D2) {
            ui.d3_groupbox->hide();
            ui.parabola_widget->hide();
            ui.ruppert_widget->hide();
            m_mesh_config->algorithm_2d_type = Algorithm2DType::Delaunay;
            m_mesh_config->show_voronoi_display = false;
            set_mesh(std::make_shared<Mesh2D>());
            build_convex_hull();
            update_voronoi_vertices();
        }
        else {
            ui.d2_groupbox->hide();
            m_mesh_config->show_voronoi_display = false;
            m_mesh_config->mesh_3d_type = Mesh3DType::Tetrahedron;
            set_mesh(std::make_shared<Tetrahedron>());
            compute_laplacians((Mesh3D*)m_mesh.get());
            set_curvature_colors(
                (Mesh3D*)m_mesh.get(), m_mesh_config->color_display_type
            );
        }
    }
}

void MainWindow::update_voronoi_vertices() {
    std::unordered_map<Face_Hash, glm::vec3> voronoi_pts
        = build_voronoi((Mesh2D*)m_mesh.get());
    m_voronoi_pts
        = std::make_shared<std::unordered_map<Face_Hash, glm::vec3>>(voronoi_pts);
    ui.scene->set_voronoi_points(m_voronoi_pts);
}

void MainWindow::set_mesh(const std::shared_ptr<Mesh>& mesh) {
    m_mesh = mesh;
    ui.scene->set_mesh(m_mesh);
}

void MainWindow::build_convex_hull() {
    std::shared_ptr<Mesh2D> sp_mesh = std::dynamic_pointer_cast<Mesh2D>(m_mesh);
    Mesh2D *mesh = sp_mesh.get();
    delaunay::insert_vertex(*sp_mesh, glm::vec3(-1, -1, 0));
    delaunay::insert_vertex(*mesh, glm::vec3( 1, -1, 0));
    delaunay::insert_vertex(*mesh, glm::vec3( 0,  1, 0));
}

void MainWindow::on_radio_button_2d_released() {
    switch_dimension();
}

void MainWindow::on_radio_button_3d_released() {
    switch_dimension();
}

void MainWindow::on_display_type_push_button_released() {
    m_mesh_config->mesh_display_type = (
        m_mesh_config->mesh_display_type == MeshDisplayType::PlainFaces ?
            MeshDisplayType::Wireframe : MeshDisplayType::PlainFaces
    );
    if (m_mesh_config->mesh_display_type == MeshDisplayType::PlainFaces) {
        ui.display_type_push_button->setText("Show wireframe");
    }
    else {
        ui.display_type_push_button->setText("Show plain faces");
    }
}

void MainWindow::on_algorithm_type_combobox_currentIndexChanged(int index) {
    m_mesh_config->algorithm_2d_type = (Algorithm2DType)index;
    if (m_mesh_config->algorithm_2d_type == Algorithm2DType::Delaunay) {
        ui.parabola_widget->hide();
        ui.ruppert_widget->hide();
        set_mesh(std::make_shared<Mesh2D>());
        build_convex_hull();
    }
    else if (m_mesh_config->algorithm_2d_type == Algorithm2DType::Crust) {
        ui.parabola_widget->hide();
        ui.ruppert_widget->hide();
        set_mesh(std::make_shared<MeshCrust>());
    }
    else if (m_mesh_config->algorithm_2d_type == Algorithm2DType::Ruppert) {
        ui.parabola_widget->hide();
        ui.ruppert_widget->show();
        set_mesh(std::make_shared<MeshRuppert>());
    }
    else if (m_mesh_config->algorithm_2d_type == Algorithm2DType::Parabola) {
        ui.parabola_widget->show();
        ui.ruppert_widget->hide();
        set_mesh(std::make_shared<MeshParabola>());
    }
    update_voronoi_vertices();
}

void MainWindow::on_voronoi_display_push_button_released() {
    m_mesh_config->show_voronoi_display = !m_mesh_config->show_voronoi_display;
    if (m_mesh_config->show_voronoi_display) 
        ui.voronoi_display_push_button->setText("Hide Voronoi wireframe");
    else
        ui.voronoi_display_push_button->setText("Show Voronoi wireframe");
}

void MainWindow::on_insert_points_push_button_released() {
    size_t nb_pts = ui.insert_points_line_edit->text().toInt();
    if (nb_pts == 0) return;
    std::shared_ptr<Mesh2D> sp_mesh = std::dynamic_pointer_cast<Mesh2D>(m_mesh);
    for (size_t i = 0; i < nb_pts; ++i) {
        float rand_x = -2.f + (((float) rand()) / (float) RAND_MAX) * 4.f;
        float rand_y = -2.f + (((float) rand()) / (float) RAND_MAX) * 4.f;
        glm::vec3 p(rand_x, rand_y, 0);
        delaunay::insert_vertex(*sp_mesh, p);
    }
    compute_laplacians((Mesh3D*)m_mesh.get());
    set_curvature_colors((Mesh3D*)m_mesh.get(), m_mesh_config->color_display_type);
}

void MainWindow::on_parabola_type_combobox_currentIndexChanged(int index) {
    m_mesh_config->parabola_type = (ParabolaType)index;
    m_mesh = std::make_shared<Mesh>(MeshParabola(m_mesh_config->parabola_type));
    update_voronoi_vertices();
}

void MainWindow::on_refine_push_button_released() {
    ((MeshRuppert*)m_mesh.get())->refine(0.85f);
    update_voronoi_vertices();
}

void MainWindow::on_color_display_combobox_currentIndexChanged(int index) {
    m_mesh_config->color_display_type = (ColorDisplayType)index;
    set_curvature_colors((Mesh3D*)m_mesh.get(), m_mesh_config->color_display_type);
}

void MainWindow::on_mesh_3d_type_combobox_currentIndexChanged(int index) {
    m_mesh_config->mesh_3d_type = (Mesh3DType)index;
    if (m_mesh_config->mesh_3d_type == Mesh3DType::Tetrahedron)
        m_mesh = std::make_shared<Mesh>(Tetrahedron());
    else if (m_mesh_config->mesh_3d_type == Mesh3DType::Pyramid)
        m_mesh = std::make_shared<Mesh>(Pyramid());
    else
        m_mesh = std::make_shared<Mesh>(MeshQueen());
    compute_laplacians((Mesh3D*)m_mesh.get());
    set_curvature_colors((Mesh3D*)m_mesh.get(), m_mesh_config->color_display_type);
}

void MainWindow::on_simplify_push_button_released() {
    simplify(*m_mesh, m_mesh->get_nb_vertices() / 2);
}