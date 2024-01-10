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
// standard template libraries
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <iostream>
#include <chrono>
#include <numbers>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void renderImgui(int screenWidth, int screenHeight, std::shared_ptr<Texture2D> windowTexture, const Shader& shader, PerspectiveLight& pointLight, PerspectiveLight& standingLight, SpotLight& spotLight);


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

class BoxStack {
	private:
		double floorLevel;
		double boxHeight;
		double angle;
    public:
        BoxStack(double floorLevel, double boxHeight, double angle): 
            floorLevel(floorLevel), boxHeight(boxHeight), angle(angle) {}
        void modelBoxStack(std::vector<glm::mat4>& models, double xCoordinate, double yCoordinate, std::initializer_list<std::size_t> indices) {
            int height = 0;
            for (std::size_t index : indices) {
                auto& model = models[index];
				model = glm::translate(glm::mat4(1.0f), glm::vec3(xCoordinate, floorLevel + index*height, yCoordinate));
				model = glm::rotate(model, glm::radians(static_cast<float>(angle)), glm::vec3(0, 1, 0));
                height += 1;
            }
        }

        void modelRandomlyRotatedBoxStack(std::vector<glm::mat4>& models, double xCoordinate, double yCoordinate, std::initializer_list<std::size_t> indices) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(-100.0f, 100.0f);
			std::uniform_int_distribution<int> binDis(0, 1);
            int height = 0;
            for (std::size_t index : indices) {
                auto& model = models[index];
				model = glm::translate(glm::mat4(1.0f), glm::vec3(xCoordinate, floorLevel + boxHeight*height, yCoordinate));
				model = glm::rotate(model, glm::radians(static_cast<float>(dis(gen))), glm::vec3(0, 1, 0));
                height += 1;
            }
        }
};

struct CallbackData {
    bool mouseLocked = false;
    double xpos = -1.0, ypos = -1.0;
    bool flashLightOn = false;
};





int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //int screenWidth = 3840, screenHeight = 2400;
    int screenWidth = 1920, screenHeight = 1080;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Interpolated Triangle with Shaders", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
        TEXTURES_PATH "nightright.jpg",
        TEXTURES_PATH "nightleft.jpg",
        TEXTURES_PATH "nighttop.jpg",
        TEXTURES_PATH "drakan.jpg",
        TEXTURES_PATH "nightfront.jpg",
        TEXTURES_PATH "nightback.jpg"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-100.0f, 100.0f);
    std::uniform_int_distribution<int> binDis(0, 1);

    const double floorLevel = 0;
    const double boxLength = 2;
    const int numberOfCubes = { 10 };

    std::vector<glm::mat4> cubeModels(numberOfCubes);
    BoxStack boxStack(floorLevel, boxLength, 0);
    // Two box stack
    boxStack.modelRandomlyRotatedBoxStack(cubeModels, 10, 5, { 0, 1 });
    boxStack.modelRandomlyRotatedBoxStack(cubeModels, -6, -5, { 2, 3, 4 });
    boxStack.modelRandomlyRotatedBoxStack(cubeModels, -2, 4, { 5 });
    boxStack.modelRandomlyRotatedBoxStack(cubeModels, 6, 4, { 6 });
	boxStack.modelRandomlyRotatedBoxStack(cubeModels, 3, -8, { 7, 8, 9 });


    const int numberOfFloors = { 1 };
    const int floorLevelCorrection = { - 1 };
    std::vector<glm::mat4> floorModels(numberOfFloors);
    floorModels[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0, floorLevel+floorLevelCorrection, 0));
    floorModels[0] = glm::scale(floorModels[0], glm::vec3(10, 10, 10));


    const int numberOfCeilings = { 1 };
    const float ceilingLevel = { 10.5 };
    std::vector<glm::mat4> ceilingModels(numberOfCeilings);
    ceilingModels[0] = glm::translate(glm::mat4(1.0f), glm::vec3(1, ceilingLevel, -2.5));
    ceilingModels[0] = glm::scale(ceilingModels[0], glm::vec3(0.7, 0.7, 1.));


    const int numberOfSticks = { 1 };
    const float stickLevel = { 0 };
    const float stickBulb = { 11 };
    std::vector<glm::mat4> stickModels(numberOfSticks);
    stickModels[0] = glm::translate(glm::mat4(1.0f), glm::vec3(stickBulb, stickLevel, 0));
    stickModels[0] = glm::scale(stickModels[0], glm::vec3(2, 2, 2));

    const int numberOfGizmos = { 1 };
    std::vector<glm::mat4> gizmoModels(numberOfGizmos);
    gizmoModels[0] = glm::translate(glm::mat4(1.0f), glm::vec3(5, floorLevel+0.95 , -1));
    gizmoModels[0] = glm::scale(gizmoModels[0], glm::vec3(2, 2, 2));

    const int numberOfWalls = { 3 };
    const int wallLevelCorrection = { 4 };
    const float wallscaling = 2;
    std::vector<glm::mat4> wallModels(numberOfWalls);
    wallModels[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-10.7, floorLevel + wallLevelCorrection, -2.8));
    wallModels[0] = glm::rotate(wallModels[0], glm::radians(static_cast<float>(90)), glm::vec3(1, 0, 0));
    wallModels[0] = glm::rotate(wallModels[0], glm::radians(static_cast<float>(90)), glm::vec3(0, 0, 1));
    wallModels[0] = glm::scale(wallModels[0], glm::vec3(wallscaling, wallscaling, wallscaling));
    wallModels[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0, floorLevel + wallLevelCorrection, -10));
    wallModels[1] = glm::rotate(wallModels[1], glm::radians(static_cast<float>(90)), glm::vec3(1, 0, 0));
    wallModels[1] = glm::scale(wallModels[1], glm::vec3(wallscaling, wallscaling, wallscaling));
    wallModels[2] = glm::translate(glm::mat4(1.0f), glm::vec3(0, floorLevel + wallLevelCorrection, 8.3));
    wallModels[2] = glm::rotate(wallModels[2], glm::radians(static_cast<float>(90)), glm::vec3(1, 0, 0));
    wallModels[2] = glm::scale(wallModels[2], glm::vec3(wallscaling, wallscaling, wallscaling));

    FPSCamera camera(0.1f, 150.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), glm::vec3(0.0f, 0.0f, 5.0f), glm::radians(60.0f));
    Shader shader(SHADERS_PATH "vertex.vert.glsl", SHADERS_PATH "fragment.frag.glsl");
    Shader lightCubeShader(SHADERS_PATH "lightcube.vert.glsl", SHADERS_PATH "lightcube.frag.glsl"); // light shader setup here because other shaders are here as well
    Shader shaderMSAA(SHADERS_PATH "multisample_to_texture_2d.vert.glsl", SHADERS_PATH "multisample_to_texture_2d.frag.glsl");
    Shader shaderCubeMap(SHADERS_PATH "cubemap.vert.glsl", SHADERS_PATH "cubemap.frag.glsl");
    Shader shaderShadow(SHADERS_PATH "shadowmap.vert.glsl", SHADERS_PATH "shadowmap.frag.glsl");
    ObjectLoader<uint8_t> cubeObj(MODELS_PATH "cube.obj");  // only loads mesh from .obj file
    ObjectLoader<uint16_t> bulbObj(MODELS_PATH "bulb.obj");
    ObjectLoader<uint16_t> floorObj(MODELS_PATH "floor.obj");
    ObjectLoader<uint16_t> ceilingObj(MODELS_PATH "wall.obj");
    ObjectLoader<uint16_t> wallObj(MODELS_PATH "newall.obj");
    ObjectLoader<uint16_t> stickObj(MODELS_PATH "badyl.obj");
    ObjectLoader<uint16_t> gizmoObj(MODELS_PATH "gizmo.obj");

    const int samples = { 4 };
    RenderbufferMultisample hdrRenderbuffer(screenWidth, screenHeight, GL_DEPTH_COMPONENT, samples);     // here we store depths of each fragment/pixel
    std::shared_ptr<Texture2DMultisample> hdrTexture = std::make_shared<Texture2DMultisample>(screenWidth, screenHeight, GL_R11F_G11F_B10F, samples);   // here we store colours of each fragment/pixel
    std::shared_ptr<TextureCubeMap> textureCubeMap(new TextureCubeMap(texture_filenames));
    std::shared_ptr<Texture2D> crateImage(new Texture2D(TEXTURES_PATH "crate.jpg"));
    std::shared_ptr<Texture2D> windowTexture(new Texture2D(screenWidth, screenHeight));

    std::shared_ptr<Texture2D> drakanImage(new Texture2D(TEXTURES_PATH "drakan.jpg"));
    std::shared_ptr<Texture2D> ceilingImage(new Texture2D(TEXTURES_PATH "ceiling.jpg"));
    std::shared_ptr<Texture2D> floorImage(new Texture2D(TEXTURES_PATH "floor.jpg"));
    std::shared_ptr<Texture2D> warehouseWallImage(new Texture2D(TEXTURES_PATH "warehousewall2.jpg"));
    std::shared_ptr<Texture2D> stickImage(new Texture2D(TEXTURES_PATH "wood.jpg"));
    std::shared_ptr<Texture2D> gizmoImage(new Texture2D(TEXTURES_PATH "marble.jpg"));

    const int shadowMapWidth = 2048, shadowMapHeight = 2048;
    std::shared_ptr<ShadowMap> ceilingShadowMap(new ShadowMap(shadowMapWidth, shadowMapHeight, GL_DEPTH_COMPONENT32F));
    std::shared_ptr<ShadowMap> standingShadowMap(new ShadowMap(shadowMapWidth, shadowMapHeight, GL_DEPTH_COMPONENT32F));
    
    //std::shared_ptr<Texture2D> textureImage(new Texture2D(TEXTURES_PATH "crate.jpg"));

    const Mesh<Vertex2D, uint8_t>& screenMesh = { screenVertices, screenIndices };  // also we can create mesh from our own vectors

    auto cubeVbo = createVertexBuffer(cubeObj.getMesh().vertices);
    auto cubeIbo = createIndexBuffer(cubeObj.getMesh().indices);

    auto bulbVbo = createVertexBuffer(bulbObj.getMesh().vertices);
    auto bulbIbo = createIndexBuffer(bulbObj.getMesh().indices);

    auto screenVbo = createVertexBuffer(screenMesh.vertices);
    auto screenIbo = createIndexBuffer(screenMesh.indices);

    auto floorVbo = createVertexBuffer(floorObj.getMesh().vertices);
    auto floorIbo = createIndexBuffer(floorObj.getMesh().indices);
    
    auto ceilingVbo = createVertexBuffer(ceilingObj.getMesh().vertices);
    auto ceilingIbo = createIndexBuffer(ceilingObj.getMesh().indices);

    auto wallVbo = createVertexBuffer(wallObj.getMesh().vertices);
    auto wallIbo = createIndexBuffer(wallObj.getMesh().indices);

    auto stickVbo = createVertexBuffer(stickObj.getMesh().vertices);
    auto stickIbo = createIndexBuffer(stickObj.getMesh().indices);

    auto gizmoVbo = createVertexBuffer(gizmoObj.getMesh().vertices);
    auto gizmoIbo = createIndexBuffer(gizmoObj.getMesh().indices);

    PerspectiveLight pointLight(glm::radians(120.0f), 1.0f, 0.1f, 15.0f);
    pointLight.setPosition(0.0f, 9.6f, 0.0f);
    pointLight.setViewDirection(0.0f, -5.0f, -0.5f);
    pointLight.setColor(1.f, 0.35f, 0.35f);
    pointLight.setAttenuation(0.5f, 0.09f, 0.04f);
    pointLight.setAmbient(0.0f, 0.0f, 0.0f);
    pointLight.setDiffuse(0.5f, 0.5f, 0.5f);
    pointLight.setSpecular(1.0f, 1.0f, 1.0f);
    
    PerspectiveLight standingLight(glm::radians(120.0f), 1.0f, 0.1f, 30.0f);
    standingLight.setPosition(stickBulb, 6.4f, 0.0f);
    standingLight.setViewDirection(-3.0f, 0.0f, -0.5f);
    standingLight.setColor(0.35f, 0.35f, 0.35f);
    standingLight.setAttenuation(1.0f, 0.045f, 0.05f);
    standingLight.setAmbient(0.0f, 0.0f, 0.0f);
    standingLight.setDiffuse(1.0f, 1.0f, 1.0f);
    standingLight.setSpecular(1.0f, 1.0f, 1.0f);
    
    SpotLight spotLight(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    spotLight.setPosition(0.0f, 0.0f, 0.0f);
    spotLight.setViewDirection(0.0f, 0.0f, -1.0f);
    spotLight.setColor(1.f, 1.f, 1.f);
    spotLight.setAttenuation(0.5f, 0.045f, 0.01f);
    spotLight.setAmbient(0.0f, 0.0f, 0.0f);
    spotLight.setDiffuse(1.0f, 1.0f, 1.0f);
    spotLight.setSpecular(1.0f, 1.0f, 1.0f);
    spotLight.setInnerCutOff(glm::cos(glm::radians(12.5)));
    spotLight.setOuterCutOff(glm::cos(glm::radians(17.5)));

    ObjectInstanced cubes(numberOfCubes);
    cubes.addVertexBuffer(cubeVbo); // first vertex buffer which stores mesh of the cube
    cubes.attachIndexBuffer(cubeIbo);
    cubes.addVertexBuffer(createVertexBuffer(cubeModels, true));    // second vertex buffer which stores model matrices of our cubes held in "cubes" variable
    cubes.addTexture(crateImage); // index 0
    cubes.addTexture(ceilingShadowMap);    // index 1
    cubes.addTexture(standingShadowMap);    // index 2
    
    ObjectInstanced floors(numberOfFloors);
    floors.addVertexBuffer(floorVbo); 
    floors.attachIndexBuffer(floorIbo);
    floors.addVertexBuffer(createVertexBuffer(floorModels, true));
    floors.addTexture(floorImage); // index 0
    floors.addTexture(ceilingShadowMap);  // index 1
    floors.addTexture(standingShadowMap);   // index 2

    ObjectInstanced ceilings(numberOfCeilings);
    ceilings.addVertexBuffer(ceilingVbo); 
    ceilings.attachIndexBuffer(ceilingIbo);
    ceilings.addVertexBuffer(createVertexBuffer(ceilingModels, true));
    ceilings.addTexture(ceilingImage); // index 0
    ceilings.addTexture(ceilingShadowMap);  // index 1
    ceilings.addTexture(standingShadowMap);  // index 2

    ObjectInstanced walls(numberOfWalls);
    walls.addVertexBuffer(wallVbo); 
    walls.attachIndexBuffer(wallIbo);
    walls.addVertexBuffer(createVertexBuffer(wallModels, true));
    walls.addTexture(warehouseWallImage); // index 0
    walls.addTexture(ceilingShadowMap);  // index 1
    walls.addTexture(standingShadowMap);  // index 2

    ObjectInstanced sticks(numberOfSticks);
    sticks.addVertexBuffer(stickVbo); 
    sticks.attachIndexBuffer(stickIbo);
    sticks.addVertexBuffer(createVertexBuffer(stickModels, true));
    sticks.addTexture(stickImage); // index 0
    sticks.addTexture(ceilingShadowMap);  // index 1
    sticks.addTexture(standingShadowMap);  // index 2

    ObjectInstanced gizmos(numberOfGizmos);
    gizmos.addVertexBuffer(gizmoVbo); 
    gizmos.attachIndexBuffer(gizmoIbo);
    gizmos.addVertexBuffer(createVertexBuffer(gizmoModels, true));
    gizmos.addTexture(gizmoImage); // index 0
    gizmos.addTexture(ceilingShadowMap);  // index 1
    gizmos.addTexture(standingShadowMap);  // index 2

    Object bulb;    // regular cube object
    bulb.addVertexBuffer(bulbVbo);
    bulb.attachIndexBuffer(bulbIbo);

    Object screen;  // screen plane for postprocessing
    screen.addVertexBuffer(screenVbo);
    screen.attachIndexBuffer(screenIbo);

    Object cubemap;
    cubemap.addVertexBuffer(cubeVbo);
    cubemap.attachIndexBuffer(cubeIbo);
    cubemap.addTexture(textureCubeMap);
    
    Framebuffer hdrFramebuffer({ GL_COLOR_ATTACHMENT0 }, GL_NONE); // framebuffer requires buffers to store colour and depth...
    hdrFramebuffer.use();
    hdrFramebuffer.attach(*hdrTexture, GL_COLOR_ATTACHMENT0);   // we pass colour storage
    hdrFramebuffer.attach(hdrRenderbuffer, GL_DEPTH_ATTACHMENT); // we pass depth storage
    hdrFramebuffer.isComplete();    // check if everything is fine

    Framebuffer shadowFramebuffer({ GL_NONE }, GL_NONE);
    shadowFramebuffer.use();
    shadowFramebuffer.attach(*ceilingShadowMap, GL_DEPTH_ATTACHMENT);
    shadowFramebuffer.isComplete();

    Framebuffer standingShadowFramebuffer({ GL_NONE }, GL_NONE);
    standingShadowFramebuffer.use();
    standingShadowFramebuffer.attach(*standingShadowMap, GL_DEPTH_ATTACHMENT);
    shadowFramebuffer.isComplete();

    Framebuffer windowFramebuffer({ GL_COLOR_ATTACHMENT0 }, GL_NONE);
    windowFramebuffer.use();
    windowFramebuffer.attach(*windowTexture, GL_COLOR_ATTACHMENT0);   // we pass colour storage

    shaderMSAA.use();
    shaderMSAA.modifyUniform<int>("planeTexture", 0);
    shaderMSAA.modifyUniform<int>("numSamples", samples);
    shaderMSAA.modifyUniform<float>("gamma", 1.6f);        // gamma correction - originally 2.2 but adjust this value so that it looks good
    shaderMSAA.modifyUniform<float>("exposure", 0.6f);     // High Dynamic Range - adjust this value so that it looks good

    shaderCubeMap.use();
    shaderCubeMap.modifyUniform<int>("cubemap", 0);
    shaderCubeMap.modifyUniform<glm::mat4>("projection", camera.getProjectionMatrix());

    screen.addTexture(hdrTexture);


    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), pointLight.getPosition()) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    const glm::mat4 lightPV = pointLight.getProjectionMatrix() * pointLight.getViewMatrix();

    glm::mat4 standingLampModel = glm::translate(glm::mat4(1.0f), standingLight.getPosition()) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    const glm::mat4 standingLightPV = standingLight.getProjectionMatrix() * standingLight.getViewMatrix();

    camera.setMovementSpeed(7.0f);
    camera.setMouseSpeed(0.05f);

    shader.use();
    shader.modifyUniform<int>("numPointLights", 2);
    shader.modifyUniform<int>("numDirLights", 0);
    // point light (ceil)
    shader.modifyUniform<glm::mat4>("LightProjViewMat[0]", lightPV);    // the same matrix as in here ***
    shader.modifyUniform<glm::vec3>("pointLight[0].position", pointLight.getPosition());
    shader.modifyUniform<glm::vec3>("pointLight[0].color", pointLight.getColor());
    shader.modifyUniform<glm::vec3>("pointLight[0].ambient", pointLight.getAmbient());
    shader.modifyUniform<glm::vec3>("pointLight[0].diffuse", pointLight.getDiffuse());
    shader.modifyUniform<glm::vec3>("pointLight[0].specular", pointLight.getSpecular());
    shader.modifyUniform<float>("pointLight[0].constant", pointLight.getAttenuationConstantFactor());
    shader.modifyUniform<float>("pointLight[0].linear", pointLight.getAttenuationLinearFactor());
    shader.modifyUniform<float>("pointLight[0].quadratic", pointLight.getAttenuationQuadraticFactor());
    shader.modifyUniform<int>("pointLight[0].shadowIndex", 0);   // *** index of LightProjViewMat

    // point light (BADYL)
    shader.modifyUniform<glm::mat4>("LightProjViewMat[1]", standingLightPV);
    shader.modifyUniform<glm::vec3>("pointLight[1].position", standingLight.getPosition());
    shader.modifyUniform<glm::vec3>("pointLight[1].color", standingLight.getColor());
    shader.modifyUniform<glm::vec3>("pointLight[1].ambient", standingLight.getAmbient());
    shader.modifyUniform<glm::vec3>("pointLight[1].diffuse", standingLight.getDiffuse());
    shader.modifyUniform<glm::vec3>("pointLight[1].specular", standingLight.getSpecular());
    shader.modifyUniform<float>("pointLight[1].constant", standingLight.getAttenuationConstantFactor());
    shader.modifyUniform<float>("pointLight[1].linear", standingLight.getAttenuationLinearFactor());
    shader.modifyUniform<float>("pointLight[1].quadratic", standingLight.getAttenuationQuadraticFactor());
    shader.modifyUniform<int>("pointLight[1].shadowIndex", 1);   // *** index of LightProjViewMat

    // spotlight
    shader.modifyUniform<glm::vec3>("spotlight.ambient", spotLight.getAmbient());
    shader.modifyUniform<glm::vec3>("spotlight.diffuse", spotLight.getDiffuse());
    shader.modifyUniform<glm::vec3>("spotlight.specular", spotLight.getSpecular());
    shader.modifyUniform<glm::vec3>("spotlight.color", spotLight.getColor());
    shader.modifyUniform<float>("spotlight.constant", spotLight.getAttenuationConstantFactor());
    shader.modifyUniform<float>("spotlight.linear", spotLight.getAttenuationLinearFactor());
    shader.modifyUniform<float>("spotlight.quadratic", spotLight.getAttenuationQuadraticFactor());
    shader.modifyUniform<float>("spotlight.innerCutOff", spotLight.getInnerCutOff());
    shader.modifyUniform<float>("spotlight.outerCutOff", spotLight.getOuterCutOff());

    shader.modifyUniform<int>("diffuseTexture", 0);
    shader.modifyUniform<int>("shadowMap[0]", 1);
    shader.modifyUniform<int>("shadowMap[1]", 2);

    // ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    CallbackData callbackData;

    glfwSetWindowUserPointer(window, &callbackData);

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
        {
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        });


    glfwSetKeyCallback(window, keyCallback);

    glfwGetCursorPos(window, &callbackData.xpos, &callbackData.ypos);

    float last = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = glfwGetTime() - last;
        last = glfwGetTime();

        glfwPollEvents();
        processInput(window, camera, deltaTime);

        const glm::mat4 cameraPV = camera.getProjectionMatrix() * camera.getViewMatrix();
        // Update flashlight position to follow camera.
        spotLight.setPosition(camera.getPosition());
        spotLight.setViewDirection(camera.getDirectionVector());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // shadow depth map
        // 1. ceiling lamp.
        shadowFramebuffer.use();
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.7f, 3.0f);
        shaderShadow.use();
        shaderShadow.modifyUniform<bool>("instanced", true);
        shaderShadow.modifyUniform<glm::mat4>("PV", lightPV);
        cubes.renderGeometry();
        floors.renderGeometry();
        walls.renderGeometry();
        ceilings.renderGeometry();
        sticks.renderGeometry();
        gizmos.renderGeometry();

        // 2. standing lamp.
        standingShadowFramebuffer.use();
        glClear(GL_DEPTH_BUFFER_BIT);
        shaderShadow.modifyUniform<glm::mat4>("PV", standingLightPV);
        cubes.renderGeometry();
        floors.renderGeometry();
        walls.renderGeometry();
        ceilings.renderGeometry();
        sticks.renderGeometry();
        gizmos.renderGeometry();

        glDisable(GL_POLYGON_OFFSET_FILL);
        
        // From now on we render to our own framebuffer.
        hdrFramebuffer.use(); 
        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Draw instanced cubes.
        shader.use();
        shader.modifyUniform<bool>("flashlightOn", callbackData.flashLightOn);
        shader.modifyUniform<glm::mat4>("ProjViewMat", cameraPV);
        shader.modifyUniform<glm::vec3>("viewPos", camera.getPosition());
        cubes.render();
        walls.render();
        floors.render();
        ceilings.render();
        sticks.render();
        gizmos.render();

        // flashlight
        shader.modifyUniform<glm::vec3>("spotlight.position", spotLight.getPosition());
        shader.modifyUniform<glm::vec3>("spotlight.direction", spotLight.getViewDirection());
        // Draw light cube.
        lightCubeShader.use();
        float multiplier = 16;
        lightCubeShader.modifyUniform<glm::vec3>("color", multiplier * pointLight.getColor() );
        lightCubeShader.modifyUniform<glm::mat4>("PVM", cameraPV * lightModel);
        bulb.render();
        lightCubeShader.modifyUniform<glm::vec3>("color", multiplier * standingLight.getColor());
        lightCubeShader.modifyUniform<glm::mat4>("PVM", cameraPV * standingLampModel);
        bulb.render();

        // Render sky box at the end in terms of scene geometry.
        shaderCubeMap.use(); 
        shaderCubeMap.modifyUniform<glm::mat4>("view", glm::mat4(glm::mat3(camera.getViewMatrix())));

        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        cubemap.render();
        glCullFace(GL_BACK);

        // Postprocessing.
        glBindFramebuffer(GL_FRAMEBUFFER, windowFramebuffer.getId());
        shaderMSAA.use();
        screen.render();
        glDepthFunc(GL_LESS);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // from now on we render to default framebuffer

        // Rendering ImGui
        renderImgui(screenWidth, screenHeight, windowTexture, shader, pointLight, standingLight, spotLight);

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime)
{
    
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

    if (!callbackData)
        return;
    
    static float flashLightDelay = 0.0f;
    static float flashLightThreshhold = 1.0f;
    flashLightDelay += deltaTime;

    if (callbackData->mouseLocked)
        glfwGetCursorPos(window, &callbackData->xpos, &callbackData->ypos);

    camera.updateEuler(callbackData->xpos, callbackData->ypos);
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
    if (flashLightDelay > flashLightThreshhold && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        flashLightDelay = 0.0f;
        callbackData->flashLightOn = !callbackData->flashLightOn;
    }
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    CallbackData* callbackData = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

    if (!callbackData)
        return;

    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
        callbackData->mouseLocked = !callbackData->mouseLocked;
        if (callbackData->mouseLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, callbackData->xpos, callbackData->ypos);
        }

        else {
            glfwGetCursorPos(window, &callbackData->xpos, &callbackData->ypos);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }


    }
}