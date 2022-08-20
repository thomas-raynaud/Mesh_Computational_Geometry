#include "ArcballCamera.h"

#include <QGLWidget>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "utils/math.h"


ArcballCamera::ArcballCamera() {
	m_last_rotation = glm::identity<glm::mat4>();
	m_current_rotation = glm::identity<glm::mat4>();
	set_center(glm::vec3(0, 0, 0));
}

void ArcballCamera::mouse_click(glm::vec2 p) {
	map_point_to_ndc_coordinates(p, m_screen_width, m_screen_height, m_p_start);
}

void ArcballCamera::mouse_motion(glm::vec2 p) {
	map_point_to_ndc_coordinates(p, m_screen_width, m_screen_height, m_p_current);
	compute_quaternion_rotation(m_p_start, m_p_current, m_current_rotation);
}

void ArcballCamera::mouse_release() {
	m_last_rotation = m_current_rotation * m_last_rotation;
	m_current_rotation = glm::identity<glm::mat4>();
}

void ArcballCamera::zoom(float dz) {
	const glm::vec3 motion(0.f, 0.f, dz);
    m_translation = glm::translate(motion) * m_translation;
}

glm::mat4 ArcballCamera::get_current_rotation() {
	return glm::mat4_cast(m_current_rotation * m_last_rotation);
}

glm::mat4 ArcballCamera::get_transform() {
	return m_translation * get_current_rotation() * m_center_translation;
}

void ArcballCamera::set_screen_dimensions(const int width, const int height) {
	m_screen_width = width;
	m_screen_height = height;
}

void ArcballCamera::set_center(glm::vec3 center) {
	glm::vec3 view_dir(0, 0, 1);
	glm::vec3 z_axis = view_dir;
    glm::vec3 x_axis = glm::normalize(glm::cross(z_axis, glm::vec3(0, 1, 0)));
    glm::vec3 y_axis = glm::normalize(glm::cross(x_axis, z_axis));
    x_axis = glm::normalize(glm::cross(z_axis, y_axis));

    m_center_translation = glm::inverse(glm::translate(center));
    m_translation = glm::translate(glm::vec3(0.f, 0.f, -glm::length(view_dir)));
    m_last_rotation = glm::normalize(glm::quat_cast(glm::transpose(glm::mat3(x_axis, y_axis, -z_axis))));
}