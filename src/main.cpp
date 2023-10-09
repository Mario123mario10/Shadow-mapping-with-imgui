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
// standard template libraries
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Set GLFW to use OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Interpolated Triangle with Shaders", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    // Initialize GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //ObjectLoader<unsigned char> obj(MODELS_PATH "cube.obj");
    // Set the viewport size and register framebuffer size callback
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");
    ObjectLoader<unsigned char> obj(MODELS_PATH "cube.obj");

    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    {
        vao.use();
        vbo.use();
        ibo.use();
        ibo.parseElementData(obj.getIndices());
        VertexBufferLayout layout = vbo.parseVertexData(obj.getVertices());
        vao.addLayout(layout);
    }
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 cameraPos = glm::vec3(-1.0f, 3.0f, -3.0f);
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, point, worldUp);
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
    glm::mat4 PVM = projection * view * model;
    // Main loop
    float t = 0.0f;
    float stop = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - stop;
        stop = glfwGetTime();
        // Poll for and process events
        glfwPollEvents();

        // Rendering commands here
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        t += deltaTime;
        // Use the shader program and draw the triangle
        shader.use();
        shader.modifyUniform<glm::mat4>("PVM", projection * view * model);
        vao.use();
        glDrawElements(GL_TRIANGLES, obj.getIndices().size(), obj.getType(), 0);
        glBindVertexArray(0);

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
