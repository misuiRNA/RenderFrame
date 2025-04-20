#include "base/Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float MathUtils::AngleCos(float angle) {
    return glm::cos(glm::radians(angle));
}
