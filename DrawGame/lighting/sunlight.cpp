#include "sunlight.h"
#include "../src/shader.h"

void SunLight::ApplyToShader(Shader& shader) const {
    shader.setVec3("sunLight.direction", this->direction);
    shader.setVec3("sunLight.color", this->color * this->intensity);
    shader.setFloat("sunLight.intensity", this->intensity);
}