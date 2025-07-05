#include "sphere.h"
#include <glad/glad.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

Sphere::Sphere() {
    std::vector<float> vertices = GenerateSphereVertices(0.5f, 36, 18);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vertexCount = vertices.size() / 3;
}

void Sphere::Draw(Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);

    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
    glBindVertexArray(0);
}

void Sphere::SetPosition(const glm::vec3& position) {
    this->position = position;

    model = glm::translate(glm::mat4(1.0f), position);
}

std::vector<float> Sphere::GenerateSphereVertices(float radius, int sectors, int stacks) {
    std::vector<float> vertices;

    const float sectorStep = 2 * M_PI / sectors;
    const float stackStep = M_PI / stacks;

    for (int i = 0; i <= stacks; ++i) 
    {

        const float stackAngle = M_PI / 2 - i * stackStep;
        
        const float xy = radius * cosf(stackAngle);
        const float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        {
            const float sectorAngle = j * sectorStep;

            vertices.push_back(xy * cosf(sectorAngle));
            vertices.push_back(xy * sinf(sectorAngle));
            vertices.push_back(z);
        }
    }

    return vertices;
}