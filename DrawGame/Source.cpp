#include <iostream>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objects/base_object.h"
#include "objects/game_object.h"
#include "objects/cube.h"
#include "objects/sphere.h"
#include "objects/ground.h"

#include "lighting/sunlight.h"

#include "physics/collision/collision_system.h"
#include "physics/physics_engine.h"
#include "physics/rigidbody.h"
#include "physics/collision/box_collider.h"
#include "physics/collision/sphere_collider.h"

#include "src/camera.h"
#include "src/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

glm::vec3 ScreenToWorld(GLFWwindow* window, Camera& camera, float x, float y);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::vector<std::unique_ptr<BaseObject>> objects; // objects for rendering
int currentObjectType = 1;

std::vector<std::unique_ptr<GameObject>> gameObjects; // objects for collision

SunLight sun;
//PhysicsEngine physics;
CollisionSystem collisionSystem;

void HandleCollision(GameObject* one, GameObject* two) {
    std::cout << "Collision detected!" << std::endl;

    const float restitution = 0.7f;

    if (one->physicsBody && two->physicsBody)
    {
        glm::vec3 relativeVelocity = two->physicsBody->velocity - one->physicsBody->velocity;
        glm::vec3 collisionNormal = glm::normalize(two->GetCollider()->GetCenter() - one->GetCollider()->GetCenter());

        float impulse = glm::dot(relativeVelocity, collisionNormal) * (1 + restitution);

        one->physicsBody->velocity += impulse * collisionNormal;
        two->physicsBody->velocity -= impulse * collisionNormal;
    }
    else if (one->physicsBody)
    {
        glm::vec3 collisionNormal = glm::normalize(one->GetCollider()->GetCenter() - two->GetCollider()->GetCenter());
        one->physicsBody->velocity = restitution * glm::reflect(one->physicsBody->velocity, collisionNormal);
    }
    else if (two->physicsBody)
    {
        glm::vec3 collisionNormal = glm::normalize(two->GetCollider()->GetCenter() - one->GetCollider()->GetCenter());
        two->physicsBody->velocity = restitution * glm::reflect(two->physicsBody->velocity, collisionNormal);
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");
    if (!ourShader.ID) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return -1;
    }

    Shader wireframeShader("shaders/wireframe.vert", "shaders/wireframe.frag");
    if (!wireframeShader.ID) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return -1;
    }

    ourShader.use();
    //wireframeShader.use();

    Ground ground;

    physics.AddRigidBody(ground.GetPhysicsBody());
    collisionSystem.AddCollider(ground.GetCollider());

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sun.ApplyToShader(ourShader);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        /*wireframeShader.setMat4("projection", projection);
        wireframeShader.setMat4("view", view);*/

        /*if (ground.GetCollider()) 
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            if (auto box = dynamic_cast<BoxCollider*>(ground.GetCollider())) 
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), box->GetCenter()) * glm::scale(glm::mat4(1.0f), box->GetSize());
                
                wireframeShader.setMat4("model", model);
                wireframeShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
                
                collisionSystem.DrawWireframeCube(wireframeShader);
            }
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }*/

        /*for (auto& obj : objects)
        {
            if (auto collider = obj->GetCollider()) 
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                
                if (auto box = dynamic_cast<BoxCollider*>(collider)) 
                {
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), box->GetCenter()) * glm::scale(glm::mat4(1.0f), box->GetSize());
                    
                    wireframeShader.setMat4("model", model);
                    wireframeShader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
                    
                    collisionSystem.DrawWireframeCube(wireframeShader);
                }
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }*/

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        ground.Draw(ourShader);

        physics.Update(deltaTime, collisionSystem);

        for (auto& obj : objects) 
        {
            if (obj->physicsBody) 
            {
                obj->SetPosition(obj->physicsBody->position);
                if (auto collider = obj->GetCollider()) 
                {
                    collider->UpdatePosition(obj->physicsBody->position);
                }
            }
        }

        /*for (auto& obj : objects) 
        {
            if (obj->GetCollider()) 
            {
                obj->GetCollider()->UpdatePosition(obj->position);
            }
        }

        for (auto& obj : objects) 
        {
            if (obj->physicsBody) 
            {
                obj->SetPosition(obj->physicsBody->position);
            }
        }*/

        collisionSystem.CheckAllCollisions(physics);

        for (auto& obj : objects) 
        {
            obj->Draw(ourShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(3, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) 
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glm::vec3 spawnPos = camera.Position + camera.Front * 5.0f;
        spawnPos.y = 2.0f;

        auto obj = std::unique_ptr<BaseObject>(BaseObject::Create(currentObjectType));
        if (!obj) return;

        if (currentObjectType == 1) 
        {
            obj->SetCollider(new BoxCollider(glm::vec3(0.5f)));
        }
        else 
        {
            obj->SetCollider(new SphereCollider(0.5f));
        }

        obj->physicsBody = new RigidBody();
        obj->physicsBody->position = spawnPos;
        obj->physicsBody->velocity = glm::vec3(0.0f);
        obj->physicsBody->collider = obj->GetCollider();

        if (obj->GetCollider())
        {
            obj->GetCollider()->SetRigidBody(obj->physicsBody);
            obj->GetCollider()->UpdatePosition(spawnPos);
        }

        physics.AddRigidBody(obj->physicsBody);
        
        if (obj->GetCollider()) 
        {
            collisionSystem.AddCollider(obj->GetCollider());
        }

        obj->SetPosition(spawnPos);
        objects.push_back(std::move(obj));
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) 
    {
        if (key == GLFW_KEY_1) currentObjectType = 1;
        if (key == GLFW_KEY_2) currentObjectType = 2;
    }
}