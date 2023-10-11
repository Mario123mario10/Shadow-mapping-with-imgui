// thirdparty libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// our libraries
#include <shader.h>
#include <vertex_buffer.h>
#include <vertex_array.h>
#include <object_loader.h>
#include <index_buffer.h>
#include <primitives.h>
#include <camera.h>
#include <texture.h>
// standard template libraries
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Interpolated Triangle with Shaders", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    FPSCamera camera(0.1f, 100.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), glm::vec3(0.0f, 0.0f, 5.0f), glm::radians(60.0f));
    Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");
    ObjectLoader<unsigned char> obj(MODELS_PATH "cube.obj");

    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;
    vao.use();
    vbo.use();
    ibo.use();
    ibo.parseElementData(obj.getIndices());
    VertexBufferLayout layout = vbo.parseVertexData(obj.getVertices());
    vao.addLayout(layout);
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera.setMovementSpeed(3.0f);
    // Main loop
    float t = 0.0f;
    float last = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - last;
        last = glfwGetTime();

        processInput(window, camera, deltaTime);
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        t += deltaTime;

        shader.use();
        shader.modifyUniform<glm::mat4>("PVM", camera.getProjectionMatrix() * camera.getViewMatrix() * model);

        vao.use();
        glDrawElements(GL_TRIANGLES, obj.getIndices().size(), obj.getType(), 0);

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    camera.updateEuler(xpos, ypos);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Bindings::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Bindings::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Bindings::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Bindings::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(Bindings::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(Bindings::DOWN, deltaTime);
}
