#include "OrbitCamera.h"

#include <glm/gtc/matrix_transform.hpp>


OrbitCamera::OrbitCamera() {
	m_yaw = -90.f;
	m_pitch = 0.f;
	m_distance = 1.f;
	m_view = glm::mat4(1.f);
	m_pivot_point = glm::vec3(0.f, 0.f, 0.f);
	m_camera_pos = glm::vec3();
	m_camera_front = glm::vec3();
	m_camera_up = glm::vec3(0, 1, 0);
}

void OrbitCamera::rotate(float dx, float dy) {
	glm::vec3 direction;
	m_yaw += dx;
	m_pitch += dy;
	if(m_pitch > 89.0f)
		m_pitch =  89.0f;
	if(m_pitch < -89.0f)
		m_pitch = -89.0f;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_camera_front = glm::normalize(direction);
	compute_view_matrix();
}

void OrbitCamera::zoom(float dz) {
	m_distance += dz;
	compute_view_matrix();
}

glm::mat4 OrbitCamera::get_view_matrix() {
	return m_view;
}

void OrbitCamera::initialize_position(glm::vec3 pivot_point, float distance) {
	m_pivot_point = pivot_point;
}

void OrbitCamera::compute_view_matrix() {
	glm::vec3 vec = m_camera_front * m_distance;
	m_view = glm::lookAt(
		m_camera_pos,
		m_camera_pos + m_camera_front,
		m_camera_up
	);
}