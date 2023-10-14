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
#include <framebuffer.h>
#include <renderbuffer.h>
#include <primitives.h>
#include <object.h>
// standard template libraries
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int screenWidth = 1920, screenHeight = 1080;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Interpolated Triangle with Shaders", NULL, NULL);
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

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); });
    
    const std::vector<Vertex2D> screenVertices = {
        { {-1.0f, -1.0f},   {0.0f, 0.0f} },
        { { 1.0f, -1.0f},   {1.0f, 0.0f} },
        { { 1.0f,  1.0f},   {1.0f, 1.0f} },
        { {-1.0f,  1.0f},   {0.0f, 1.0f} },
    };

    const std::vector<uint8_t> screenIndices = {
        0, 1, 2,
        2, 3, 0
    };

    FPSCamera camera(0.1f, 100.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), glm::vec3(0.0f, 0.0f, 5.0f), glm::radians(60.0f));
    Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");
    Shader shaderMSAA(SHADERS_PATH "multisample_to_texture_2d.vert.glsl", SHADERS_PATH "multisample_to_texture_2d.frag.glsl");
    Shader lightCubeShader(SHADERS_PATH "lightcube.vert.glsl", SHADERS_PATH "lightcube.frag.glsl"); // light shader setup here because other shaders are here as well
    ObjectLoader<uint8_t> obj(MODELS_PATH "cube.obj");

    const Mesh<Vertex3D, uint8_t>& cubeMesh = obj.getMesh();
    const Mesh<Vertex2D, uint8_t>& screenMesh = { screenVertices, screenIndices };
    Object cube(cubeMesh);
    Object screen(screenMesh);

    int samples = { 8 };
    Texture2DMultisample hdrTexture(screenWidth, screenHeight, GL_R11F_G11F_B10F, samples);
    RenderbufferMultisample hdrRenderbuffer(screenWidth, screenHeight, GL_DEPTH_COMPONENT32F, samples);
    Framebuffer hdrFramebuffer({ GL_COLOR_ATTACHMENT0 }, GL_NONE);
    hdrFramebuffer.use();
    hdrFramebuffer.attach(hdrTexture, GL_COLOR_ATTACHMENT0);
    hdrFramebuffer.attach(static_cast<RenderbufferInterface>(hdrRenderbuffer), GL_DEPTH_ATTACHMENT);
    hdrFramebuffer.isComplete();

    shaderMSAA.use();
    shaderMSAA.modifyUniform<int>("planeTexture", 0);
    shaderMSAA.modifyUniform<int>("numSamples", samples);
    
    screen.addTexture(hdrTexture);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


    // set up light cube
    ObjectLoader<uint8_t> lightObj = obj;
    const Mesh<Vertex3D, uint8_t>& lightMesh = obj.getMesh();
    Object lightCube(lightMesh);
    // position and scaling
    glm::vec3 lightPos = glm::vec3(3.5f, 0.7f, 3.0f);
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));


    camera.setMovementSpeed(7.0f);
    camera.setMouseSpeed(50.0f);
    // Main loop
    float t = 0.0f;
    float last = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - last;
        last = glfwGetTime();
        std::cout << 1.0f / deltaTime << std::endl;

        glfwPollEvents();
        processInput(window, camera, deltaTime);

        hdrFramebuffer.use(); // from now on we render to our own framebuffer

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //t += deltaTime;

        shader.use();
        shader.modifyUniform<glm::mat4>("PVM", camera.getProjectionMatrix() * camera.getViewMatrix() * model);
        // send some shit
        shader.modifyUniform<glm::mat4>("model", model);
        shader.modifyUniform<glm::mat4>("invModel", glm::inverse(model));

        shader.modifyUniform<glm::vec3>("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.modifyUniform<glm::vec3>("lightPos", lightPos);
        shader.modifyUniform<glm::vec3>("viewPos", camera.getPosition());
        cube.renderObject();
        
        // for the light cube
        lightCubeShader.use();
        lightCubeShader.modifyUniform<glm::mat4>("PVM", camera.getProjectionMatrix() * camera.getViewMatrix() * lightModel);
        lightCube.renderObject();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // from now on we render to default framebuffer
        glClear(GL_DEPTH_BUFFER_BIT);

        shaderMSAA.use();
        screen.renderObject();

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
    camera.updateEuler(xpos, ypos, deltaTime);
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
