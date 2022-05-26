#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


class OrbitCamera {

public:
    OrbitCamera();
    void rotate(float dx, float dy);
    void zoom(float dz);
    void initialize_position(glm::vec3 pivot_point, float distance);

    glm::mat4 get_view_matrix();

private:
    glm::mat4 m_view;
    glm::vec3 m_pivot_point;    // Center of the orbit-camera
    glm::vec3 m_camera_pos;
    glm::vec3 m_camera_front;
    glm::vec3 m_camera_up;
    float m_yaw;
    float m_pitch;
    float m_distance;

    void compute_view_matrix();
};

#endif  // CAMERA_H