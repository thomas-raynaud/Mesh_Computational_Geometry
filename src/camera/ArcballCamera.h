#ifndef ARCBALL_CAMERA_H
#define ARCBALL_CAMERA_H

#include <glm/glm.hpp>

// Arcball algorithm:
// http://courses.cms.caltech.edu/cs171/assignments/hw3/hw3-notes/notes-hw3.html

class ArcballCamera {

public:
    ArcballCamera();
    void mouse_click(glm::vec2 p);
    void mouse_motion(glm::vec2 p);
    void mouse_release();
    glm::mat4 get_current_rotation();
    void set_screen_dimensions(int width, int height);

private:
    glm::mat4 m_last_rotation;
    glm::mat4 m_current_rotation;
    glm::vec3 m_p_start;
    glm::vec3 m_p_current;
    int m_screen_width;
    int m_screen_height;
};

#endif  // ARCBALL_CAMERA_H