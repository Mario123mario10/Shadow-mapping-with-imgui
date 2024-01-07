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
#include <light.h>
#include <actor.h>
// standard template libraries
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime);

GLuint FBO; // frame buffer object
GLuint texture_id; // the texture id we'll need later to create a texture 
                // out of our framebuffer
GLuint RBO; // rendering buffer object

// our window dimensions
const GLuint WIDTH = 1000;
const GLint HEIGHT = 700;



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

template<typename T>
std::shared_ptr<ObjectLoader<T>> create(const std::string path) {
    return std::shared_ptr<ObjectLoader<T>>(new ObjectLoader<T>(path));
}

void create_framebuffer()
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenRenderbuffers(1, &RBO);
    //glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
    glEnable(GL_POLYGON_OFFSET_FILL);

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); });

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

    const int instances = { 100000 };
    std::vector<glm::mat4> models(instances);

    for (auto& model : models) {
        model = glm::translate(glm::mat4(1.0f), glm::vec3(dis(gen), dis(gen), dis(gen)));
        model = glm::rotate(model, glm::radians(static_cast<float>(dis(gen))), glm::vec3(binDis(gen), binDis(gen), binDis(gen)));
    }

    FPSCamera camera(0.1f, 150.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), glm::vec3(0.0f, 0.0f, 5.0f), glm::radians(60.0f));
    Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");
    Shader lightCubeShader(SHADERS_PATH "lightcube.vert.glsl", SHADERS_PATH "lightcube.frag.glsl"); // light shader setup here because other shaders are here as well
    Shader shaderMSAA(SHADERS_PATH "multisample_to_texture_2d.vert.glsl", SHADERS_PATH "multisample_to_texture_2d.frag.glsl");
    Shader shaderCubeMap(SHADERS_PATH "cubemap.vert.glsl", SHADERS_PATH "cubemap.frag.glsl");
    Shader shaderShadow(SHADERS_PATH "shadowmap.vert.glsl", SHADERS_PATH "shadowmap.frag.glsl");
    ObjectLoader<uint8_t> cubeObj(MODELS_PATH "cube.obj");  // only loads mesh from .obj file
    ObjectLoader<uint16_t> bulbObj(MODELS_PATH "bulb.obj");
    
    const int samples = { 8 };
    std::shared_ptr<RenderbufferInterface> hdrRenderbuffer(new RenderbufferMultisample(screenWidth, screenHeight, GL_DEPTH_COMPONENT, samples));     // here we store depths of each fragment/pixel
    std::shared_ptr<Texture> hdrTexture(new Texture2DMultisample(screenWidth, screenHeight, GL_R11F_G11F_B10F, samples));   // here we store colours of each fragment/pixel
    std::shared_ptr<Texture> textureCubeMap(new TextureCubeMap(texture_filenames));
    std::shared_ptr<Texture> textureImage(new Texture2D(TEXTURES_PATH "drakan.jpg"));
    const int shadowMapWidth = 1024, shadowMapHeight = 1024;
    std::shared_ptr<Texture> shadowMap(new ShadowMap(shadowMapWidth, shadowMapHeight, GL_DEPTH_COMPONENT32F));

    const Mesh<Vertex2D, uint8_t>& screenMesh = { DefaultGeometry::screenVertices, DefaultGeometry::screenIndices };  // also we can create mesh from our own vectors

    auto cubeVbo = createVertexBuffer(cubeObj.getMesh().vertices);
    auto cubeIbo = createIndexBuffer(cubeObj.getMesh().indices);

    auto bulbVbo = createVertexBuffer(bulbObj.getMesh().vertices);
    auto bulbIbo = createIndexBuffer(bulbObj.getMesh().indices);

    auto screenVbo = createVertexBuffer(screenMesh.vertices);
    auto screenIbo = createIndexBuffer(screenMesh.indices);

    PerspectiveLight light(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    light.setPosition(0.0f, 0.0f, 0.0f);
    light.setViewDirection(0.0f, 0.0f, -1.0f);
    light.setColor(1.0f, 1.0f, 1.0f);
    light.setAttenuation(1.0f, 0.09f, 0.032f);

    ObjectInstanced cubes(instances);
    cubes.addVertexBuffer(cubeVbo); // first vertex buffer which stores mesh of the cube
    cubes.attachIndexBuffer(cubeIbo);
    cubes.addVertexBuffer(createVertexBuffer(models, true));    // second vertex buffer which stores model matrices of our cubes held in "cubes" variable
    cubes.addTexture(textureImage);
    cubes.addTexture(shadowMap);

    Object bulb;    // regular cube object
    bulb.addVertexBuffer(bulbVbo);
    bulb.attachIndexBuffer(bulbIbo);

    Object screen;  // screen plane for postprocessing
    screen.addVertexBuffer(screenVbo);
    screen.attachIndexBuffer(screenIbo);
    screen.addTexture(hdrTexture);

    Object cubemap;
    cubemap.addVertexBuffer(cubeVbo);
    cubemap.attachIndexBuffer(cubeIbo);
    cubemap.addTexture(textureCubeMap);

    Framebuffer hdrFramebuffer({ GL_COLOR_ATTACHMENT0 }, GL_NONE); // framebuffer requires buffers to store colour and depth...
    hdrFramebuffer.use();
    hdrFramebuffer.attach(*hdrTexture, GL_COLOR_ATTACHMENT0);   // we pass colour storage
    hdrFramebuffer.attach(*hdrRenderbuffer, GL_DEPTH_ATTACHMENT); // we pass depth storage
    hdrFramebuffer.isComplete();    // check if everything is fine

    Framebuffer shadowFramebuffer({ GL_NONE }, GL_NONE);
    shadowFramebuffer.use();
    shadowFramebuffer.attach(*shadowMap, GL_DEPTH_ATTACHMENT);
    shadowFramebuffer.isComplete();

    shaderMSAA.use();
    shaderMSAA.modifyUniform<int>("planeTexture", 0);
    shaderMSAA.modifyUniform<int>("numSamples", samples);
    shaderMSAA.modifyUniform<float>("gamma", 2.2f);     // gamma correction - originally 2.2 but adjust this value so that it looks good
    shaderMSAA.modifyUniform<float>("exposure", 0.6f);     // High Dynamic Range - adjust this value so that it looks good

    shaderCubeMap.use();
    shaderCubeMap.modifyUniform<int>("cubemap", 0);
    shaderCubeMap.modifyUniform<glm::mat4>("projection", camera.getProjectionMatrix());


    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), light.getPosition()) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

    camera.setMovementSpeed(7.0f);
    camera.setMouseSpeed(0.05f);

    shader.use();
    shader.modifyUniform<glm::vec3>("lightColor", light.getColor());
    shader.modifyUniform<glm::vec3>("light.position", light.getPosition());
    shader.modifyUniform<float>("light.constant", light.getAttenuationConstantFactor());
    shader.modifyUniform<float>("light.linear", light.getAttenuationLinearFactor());
    shader.modifyUniform<float>("light.quadratic", light.getAttenuationQuadraticFactor());
    shader.modifyUniform<int>("diffuseTexture", 0);
    shader.modifyUniform<int>("shadowMap", 1);

    float last = 0.0f;
    glPolygonOffset(1.7f, 3.5f);

    
    create_framebuffer();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) 
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    });

    while (!glfwWindowShouldClose(window)) {
        //glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        float deltaTime = glfwGetTime() - last;
        last = glfwGetTime();
        // std::cout << 1.0f / deltaTime << std::endl;
        glfwPollEvents();
        processInput(window, camera, deltaTime);

        const glm::mat4 cameraPV = camera.getProjectionMatrix() * camera.getViewMatrix();
        const glm::mat4 lightPV = light.getProjectionMatrix() * light.getViewMatrix();
        // shadow depth map
        shadowFramebuffer.use();
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        shaderShadow.use();
        //shaderShadow.modifyUniform<glm::mat4>("model", glm::mat4(1.0f));
        shaderShadow.modifyUniform<glm::mat4>("PV", lightPV);
        shaderShadow.modifyUniform<bool>("instanced", true);
        glEnable(GL_POLYGON_OFFSET_FILL);
        cubes.renderGeometry();
        glDisable(GL_POLYGON_OFFSET_FILL);

        //shaderShadow.modifyUniform<bool>("instanced", false);
        //bulb.renderDepth();

        hdrFramebuffer.use(); // from now on we render to our own framebuffer
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // draw instanced cubes
        shader.use();
        shader.modifyUniform<glm::mat4>("ProjViewMat", cameraPV);
        shader.modifyUniform<glm::mat4>("LightProjViewMat", lightPV);
        shader.modifyUniform<glm::vec3>("viewPos", camera.getPosition());
        cubes.render();

        // draw light cube
        lightCubeShader.use();
        lightCubeShader.modifyUniform<glm::mat4>("PVM", cameraPV * lightModel);
        bulb.render();

        shaderCubeMap.use(); 
        shaderCubeMap.modifyUniform<glm::mat4>("view", glm::mat4(glm::mat3(camera.getViewMatrix())));

        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        cubemap.render();
        glCullFace(GL_BACK);

        
        // postprocessing
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
       
        shaderMSAA.use();
        screen.render();
        glDepthFunc(GL_LESS);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // from now on we render to default framebuffer

        // Renderowanie ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
        ImGui::Begin("OpenGL Texture Window");
        ImGui::Image((void*)(intptr_t)texture_id, ImVec2(WIDTH, HEIGHT));
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // and cleanup, when we're done 
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &texture_id);
    glDeleteRenderbuffers(1, &RBO);

    glfwDestroyWindow(window);
    
    glfwTerminate();


    return 0;
}

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (ImGui::IsWindowHovered())
    {
        std::cout << "yes" << std::endl;
    }
    else std::cout << "no" << std::endl;

    //ImVec2 mousePos = ImGui::GetMousePos();
    //xpos = mousePos.x;
    //ypos = mousePos.y;
    //std::cout << xpos << " " << ypos << std::endl;

    camera.updateEuler(xpos, -ypos);
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