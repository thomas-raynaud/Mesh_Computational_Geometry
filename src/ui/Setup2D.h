#ifndef SETUP_2D_H
#define SETUP_2D_H

#include <QWidget>

#include "ui_setup_2d.h"


enum AlgorithmType {
    Delaunay,
    Crust,
    Ruppert,
    Parabola
}

class Setup2D : public QWidget
{
    Q_OBJECT

public:
public slots:
    void slot_change_mesh2d_type();
    void slot_toggle_voronoi_display();
    void slot_insert_points(int nb_pts);
signals:
    void algorithm_type_changed(AlgorithmType algo_type);

private:
    Ui::m_setup_2d ui;
};

#endif  // SETUP_2D_H