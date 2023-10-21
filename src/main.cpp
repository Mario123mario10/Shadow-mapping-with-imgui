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
#include <random>
#include <iostream>

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime);

template<typename T>
std::shared_ptr<VertexBuffer> createVertexBuffer(std::vector<T> data, bool instanced = false) {
    std::shared_ptr<VertexBuffer> vbo(new VertexBuffer);
    vbo->use();
    vbo->parseData(data);
    VertexBufferLayout& layout = vbo->getLayout();
    layout.addLayoutElement<T>(1, instanced);
    return vbo;
}

template<typename T>
std::shared_ptr<IndexBuffer> createIndexBuffer(std::vector<T> data) {
    std::shared_ptr<IndexBuffer> ibo(new IndexBuffer);
    ibo->use();
    ibo->parseElementData(data);
    return ibo;
}

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

    std::vector<std::string_view> texture_filenames = {
        CUBEMAP_PATH "right.jpg",
        CUBEMAP_PATH "left.jpg",
        CUBEMAP_PATH "top.jpg",
        CUBEMAP_PATH "bottom.jpg",
        CUBEMAP_PATH "front.jpg",
        CUBEMAP_PATH "back.jpg"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-100.0f, 100.0f);
    std::uniform_int_distribution<int> binDis(0, 1);

    const int instances = { 10000 };
    std::vector<glm::mat4> models(instances);

    for (auto& model : models) {
        model = glm::translate(glm::mat4(1.0f), glm::vec3(dis(gen), dis(gen), dis(gen)));
        model = glm::rotate(model, glm::radians(static_cast<float>(dis(gen))), glm::vec3(binDis(gen), binDis(gen), binDis(gen)));
    }

    FPSCamera camera(0.1f, 150.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), glm::vec3(0.0f, 0.0f, 5.0f), glm::radians(60.0f));
    Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");
    Shader lightCubeShader(SHADERS_PATH "lightcube.vert.glsl", SHADERS_PATH "lightcube.frag.glsl"); // light shader setup here because other shaders are here as well
    Shader shaderMSAA(SHADERS_PATH "multisample_to_texture_2d.vert.glsl", SHADERS_PATH "multisample_to_texture_2d.frag.glsl");
    Shader shaderCubeMap(SHADERS_PATH "cubemap.vert.glsl", SHADERS_PATH "cubemap.frag.glsl");
    ObjectLoader<uint8_t> obj(MODELS_PATH "cube.obj");  // only loads mesh from .obj file

    std::shared_ptr<Texture2D> textureImage(new Texture2D(TEXTURES_PATH "drakan.jpg"));

    const Mesh<Vertex3D, uint8_t>& cubeMesh = obj.getMesh();    // we can get mesh from object
    const Mesh<Vertex2D, uint8_t>& screenMesh = { screenVertices, screenIndices };  // also we can create mesh from our own vectors

    auto cubeVbo = createVertexBuffer(cubeMesh.vertices);
    auto cubeIbo = createIndexBuffer(cubeMesh.indices);

    ObjectInstanced cubes(instances);
    cubes.addVertexBuffer(cubeVbo); // first vertex buffer which stores mesh of the cube
    cubes.addVertexBuffer(createVertexBuffer(models, true));    // second vertex buffer which stores model matrices of our cubes held in "cubes" variable
    cubes.attachIndexBuffer(cubeIbo);
    cubes.addTexture(textureImage);

    Object cube;    // regular cube object
    cube.addVertexBuffer(cubeVbo);
    cube.attachIndexBuffer(cubeIbo);

    Object screen;  // screen plane for postprocessing
    screen.addVertexBuffer(createVertexBuffer(screenMesh.vertices));
    screen.attachIndexBuffer(createIndexBuffer(screenMesh.indices));

    Object cubemap;
    cubemap.addVertexBuffer(cubeVbo);
    cubemap.attachIndexBuffer(cubeIbo);


    std::shared_ptr<TextureCubeMap> textureCubeMap(new TextureCubeMap(texture_filenames));
    int samples = { 4 };
    std::shared_ptr<Texture2DMultisample> hdrTexture = std::make_shared<Texture2DMultisample>(screenWidth, screenHeight, GL_R11F_G11F_B10F, samples);   // here we store colours of each fragment/pixel
    RenderbufferMultisample hdrRenderbuffer(screenWidth, screenHeight, GL_DEPTH_COMPONENT, samples);     // here we store depths of each fragment/pixel
    Framebuffer hdrFramebuffer({ GL_COLOR_ATTACHMENT0 }, GL_NONE); // framebuffer requires buffers to store colour and depth...
    hdrFramebuffer.use();
    hdrFramebuffer.attach(*hdrTexture, GL_COLOR_ATTACHMENT0);   // we pass colour storage
    hdrFramebuffer.attach(hdrRenderbuffer, GL_DEPTH_ATTACHMENT); // we pass depth storage
    hdrFramebuffer.isComplete();    // check if everything is fine

    shaderMSAA.use();
    shaderMSAA.modifyUniform<int>("planeTexture", 0);
    shaderMSAA.modifyUniform<int>("numSamples", samples);
    shaderMSAA.modifyUniform<float>("gamma", 2.2f);     // gamma correction - originally 2.2 but adjust this value so that it looks good
    shaderMSAA.modifyUniform<float>("exposure", 0.3f);     // High Dynamic Range - adjust this value so that it looks good

    shaderCubeMap.use();
    shaderCubeMap.modifyUniform<int>("cubemap", 0);
    shaderCubeMap.modifyUniform<glm::mat4>("projection", camera.getProjectionMatrix());
    cubemap.addTexture(textureCubeMap);

    screen.addTexture(hdrTexture);

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));


    camera.setMovementSpeed(7.0f);
    camera.setMouseSpeed(0.05f);

    shader.use();
    shader.modifyUniform<glm::vec3>("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.modifyUniform<glm::vec3>("lightPos", lightPos);
    shader.modifyUniform<int>("diffuseTexture", 0);

    float last = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - last;
        last = glfwGetTime();

        glfwPollEvents();
        processInput(window, camera, deltaTime);

        hdrFramebuffer.use(); // from now on we render to our own framebuffer

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw instanced cubes
        shader.use();
        shader.modifyUniform<glm::mat4>("PV", camera.getProjectionMatrix() * camera.getViewMatrix());
        shader.modifyUniform<glm::vec3>("viewPos", camera.getPosition());
        cubes.render();

        // draw light cube
        lightCubeShader.use();
        lightCubeShader.modifyUniform<glm::mat4>("PVM", camera.getProjectionMatrix() * camera.getViewMatrix() * lightModel);
        cube.render();

        shaderCubeMap.use();
        shaderCubeMap.modifyUniform<glm::mat4>("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        cubemap.render();
        glCullFace(GL_BACK);

        // postprocessing
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // from now on we render to default framebuffer
        shaderMSAA.use();
        screen.render();
        glDepthFunc(GL_LESS);

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