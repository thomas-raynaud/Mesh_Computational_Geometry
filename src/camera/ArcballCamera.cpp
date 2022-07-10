#include "ArcballCamera.h"

#include <QGLWidget>
#include <glm/gtc/matrix_transform.hpp>

#include "utils/math.h"


ArcballCamera::ArcballCamera() {
	m_last_rotation = glm::identity<glm::mat4>();
	m_current_rotation = glm::identity<glm::mat4>();
}

void ArcballCamera::mouse_click(glm::vec2 p) {
	map_point_to_ndc_coordinates(p, m_screen_width, m_screen_height, m_p_start);
}

void ArcballCamera::mouse_motion(glm::vec2 p) {
	map_point_to_ndc_coordinates(p, m_screen_width, m_screen_height, m_p_current);
	compute_rotation_matrix(m_p_start, m_p_current, m_current_rotation);
}

void ArcballCamera::mouse_release() {
	m_last_rotation = m_current_rotation * m_last_rotation;
	m_current_rotation = glm::identity<glm::mat4>();
}

glm::mat4 ArcballCamera::get_current_rotation() {
	return m_current_rotation * m_last_rotation;
}

void ArcballCamera::set_screen_dimensions(int width, int height) {
	m_screen_width = width;
	m_screen_height = height;
}