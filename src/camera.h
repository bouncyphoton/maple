#ifndef MAPLE_CAMERA_H
#define MAPLE_CAMERA_H

#include "types.h"
#include <glm/glm.hpp>

struct Camera {
    glm::vec3 position;
    glm::vec3 lookDir;
    f32 fovRadians;
    f32 nearPlane;
    f32 farPlane;
};

#endif //MAPLE_CAMERA_H
