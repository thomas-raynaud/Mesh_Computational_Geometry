#include "curvature.h"

#include "meshes/Mesh3D.h"
#include "utils/color.h"


void compute_laplacians(Mesh3D *mesh) {
    std::unordered_map<Vertex_Hash, glm::vec3>& laplacians = mesh->get_laplacians();
    laplacians.reserve(mesh->get_nb_vertices());
    float A;                // Area
    glm::vec3 a, b, c, d;   // Adjacent points to the processed vertex
    VertexIterator vtx_it;
    FaceCirculator fc, fc_begin;
    VertexCirculator vc, vc_begin, vc_tmp;
    std::array<Vertex*, 3> face_vts;
    glm::vec3 lap;
    float cot_alpha, cot_beta, angles;
    float cot_alpha_denom, cot_beta_denom;

    for (vtx_it = mesh->vertices_begin(); vtx_it != mesh->vertices_end(); ++vtx_it) {
        // Compute the area around the vertex
        // Go through all the faces that have the vertex vertex_it
        fc_begin = mesh->incident_faces(*vtx_it);
        fc = fc_begin;
        A = 0;
        do {
            face_vts = fc->get_vertices();
            a = face_vts[0]->get_position();
            b = face_vts[1]->get_position();
            c = face_vts[2]->get_position();
            A += (1.f / 3.f) * ((1.f / 2.f) * glm::length(glm::cross(b - a, c - a)));
            ++fc;
        } while (fc != fc_begin);

        // Compute the laplacian
        // We go through all the edges of vtx_it
        lap = glm::vec3(0, 0, 0);
        vc_begin = mesh->neighbour_vertices(*vtx_it);
        vc = vc_begin;
        a = vtx_it->get_position();
        do {
            vc_tmp = vc;
            --vc_tmp;
            b = vc_tmp->get_position();
            vc_tmp++;
            c = vc_tmp->get_position();
            vc_tmp++;
            d = vc_tmp->get_position();
            cot_alpha_denom = glm::dot(glm::cross(c - b, a - b), glm::vec3(0, 0, 1));
            if (cot_alpha_denom == 0.f)
                cot_alpha = 0.f;
            else
                cot_alpha = glm::dot(a - b, c - b) / cot_alpha_denom;
            cot_beta_denom = glm::dot(glm::cross(c - d, a - d), glm::vec3(0, 0, 1));
            if (cot_beta_denom == 0.f)
                cot_beta = 0.f;
            else
                cot_beta = glm::dot(a - d, c - d) / cot_beta_denom;
            angles = cot_alpha + cot_beta;
            lap += angles * (c - a);
            ++vc;
        } while (vc != vc_begin);

        // Compute the laplacian for vtx_it
        A = 1.f / (2.f * A);
        lap = A * lap;

        laplacians[vtx_it->get_hash()] = lap;
    }
}


void set_curvature_colors(Mesh3D *mesh, ColorDisplayType color_display_type) {
    float min, max, mean_curvature;
    float length_lap;
    int hue = 0;
    std::unordered_map<Vertex_Hash, glm::vec3>& laplacians = mesh->get_laplacians();
    std::unordered_map<Vertex_Hash, float> curvatures;
    VertexIterator vtx_it;
    glm::vec3 lap;
    curvatures.reserve(mesh->get_nb_vertices());
    // Compute the mean curvature
    int curvature_axis = (int)color_display_type - 1;
    lap = laplacians[mesh->vertices_begin()->get_hash()];
    length_lap = glm::length(lap);
    if (length_lap == 0)
        mean_curvature = 0;
    else if (color_display_type == ColorDisplayType::MeanCurvature)
        mean_curvature = std::abs(std::log(glm::length(lap)) / -2.f);
    else
        mean_curvature = std::abs((lap[curvature_axis] / glm::length(lap)) / -2.f);
    min = mean_curvature;
    max = mean_curvature;
    for (vtx_it = mesh->vertices_begin(); vtx_it != mesh->vertices_end(); ++vtx_it) {
        lap = laplacians[vtx_it->get_hash()];
        length_lap = glm::length(lap);
        if (length_lap == 0)
            mean_curvature = 0;
        else if (color_display_type == ColorDisplayType::MeanCurvature)
            mean_curvature = std::abs(std::log(length_lap) / -2.f);
        else
            mean_curvature = std::abs((lap[curvature_axis] / length_lap) / -2.f);
        min = std::min(min, mean_curvature);
        max = std::max(max, mean_curvature);
        curvatures[vtx_it->get_hash()] = mean_curvature;
    }

    // Get a color based on the vertex's curvature
    for (vtx_it = mesh->vertices_begin(); vtx_it != mesh->vertices_end(); ++vtx_it) {
        // Weakest curvature -> green, to strongest curvature -> red
        if (max == 0)
            hue = (curvatures[vtx_it->get_hash()] - min) * 270.0 + 90.0;
        else
            hue = ((curvatures[vtx_it->get_hash()] - min) / max) * 270.0 + 90.0;
        vtx_it->set_color(hsv2rgb(hue, 1.0, 1.0));
    }
}
