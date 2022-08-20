#ifndef ARCBALL_CAMERA_H
#define ARCBALL_CAMERA_H

#include <glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

// Arcball algorithm:
// http://courses.cms.caltech.edu/cs171/assignments/hw3/hw3-notes/notes-hw3.html
// Paper: https://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf
// Add zoom: https://github.com/Twinklebear/arcball-cpp

class ArcballCamera {

public:
    ArcballCamera();
    void mouse_click(glm::vec2 p);
    void mouse_motion(glm::vec2 p);
    void mouse_release();
    void zoom(float dz);
    glm::mat4 get_current_rotation();
    glm::mat4 get_transform();
    void set_screen_dimensions(int width, int height);
    void set_center(glm::vec3 center);

private:
    glm::quat m_last_rotation;
    glm::quat m_current_rotation;
    glm::mat4 m_center_translation;
    glm::mat4 m_translation;
    glm::vec3 m_p_start;
    glm::vec3 m_p_current;
    int m_screen_width;
    int m_screen_height;
};

#endif  // ARCBALL_CAMERA_H