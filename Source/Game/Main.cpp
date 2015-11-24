#include "StaticMesh.h"
#include "MeshTools.h"
#include "Material.h"
#include "MeshInstance.h"
#include "Renderer.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/*
* Gets called by GLFW3 when an error occurs.
*/
void errorCallback(int error, const char* description) {
    (void)error;

    std::cerr << description;
}

/*
* Gets called by GLFW3 when a keyboard key is pressed or released.
*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

/*
* Gets called everytime the framebuffer resizes.
*/
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;

    glViewport(0, 0, width, height);
}

int main() {
    glfwSetErrorCallback(&errorCallback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW3.\n";
    }

    // We need a OpenGL 3.3 Core context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA

    // Window settings
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "The Game", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create a window and context.\n";
    }

    glfwSetKeyCallback(window, &keyCallback);
    glfwSetFramebufferSizeCallback(window, &framebufferSizeCallback);
    glfwMakeContextCurrent(window);

    if (gl3wInit()) {
        std::cerr << "Failed to initialize GL3W.\n";
    }

    if (!gl3wIsSupported(3, 3)) {
        std::cerr << "OpenGL 3.3 Core is not supported on this device.\n";
    }

    // Output some system information to the standard output
    std::cout << "OpenGL Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << "\n";
    std::cout << "GLSL Version:   " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) << "\n";
    std::cout << "Renderer:       " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << "\n";

    // Create the scene
    auto boxMesh = gp::createBoxMesh(1, 1, 1);
    auto planeMesh = gp::createPlane(10, 10);

    gp::Material redMaterial;
    redMaterial.diffuse = { 1, 0, 0 };

    gp::Material greenMaterial;
    greenMaterial.diffuse = { 0, 1, 0 };

    gp::Material blueMaterial;
    blueMaterial.diffuse = { 0, 0, 1 };

    gp::MeshInstance box1(&boxMesh, &redMaterial);
    gp::MeshInstance box2(&boxMesh, &blueMaterial);
    gp::MeshInstance plane(&planeMesh, &greenMaterial);

    gp::Renderer renderer;
    renderer.setAmbientColor({ 0.2f, 0.2f, 0.2f });
    renderer.setLight({ 1, 1, 1 }, { 1, -1, -1 });
    renderer.addSceneObject(box1);
    renderer.addSceneObject(box2);
    renderer.addSceneObject(plane);

    glfwSwapInterval(1); // Turn on VSync

    float angle = 0.0f;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        auto currTime = glfwGetTime();
        auto dt = static_cast<float>(currTime - lastTime);
        lastTime = currTime;
        
        angle += glm::half_pi<float>() * dt;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        auto projMat = glm::perspective(glm::quarter_pi<float>(), static_cast<float>(width) / height , 0.1f, 100.0f);
        auto viewMat = glm::lookAt(glm::vec3(1, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        auto viewProjMat = projMat * viewMat;

        glm::mat4 modelMat(1);
        modelMat = glm::translate(glm::mat4(1), glm::vec3(2, 1, 0));
        modelMat = glm::rotate(modelMat, angle, glm::vec3(0, 0, 1));
        modelMat = glm::rotate(modelMat, angle, glm::vec3(0, 1, 0));
        modelMat = glm::rotate(modelMat, angle, glm::vec3(1, 0, 0));
        box1.setModelMatrix(modelMat);

        modelMat = glm::translate(glm::mat4(1), glm::vec3(-2, 1, 0));
        modelMat = glm::rotate(modelMat, -angle, glm::vec3(0, 0, 1));
        modelMat = glm::rotate(modelMat, angle, glm::vec3(0, 1, 0));
        modelMat = glm::rotate(modelMat, -angle, glm::vec3(1, 0, 0));
        box2.setModelMatrix(modelMat);

        renderer.renderScene(viewProjMat);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}