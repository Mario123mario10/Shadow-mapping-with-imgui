#pragma once
#include <glm/glm.hpp>

enum class Bindings : int {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
protected:
    glm::mat4 projectionMatrix;
    mutable glm::mat4 viewMatrix;
    mutable glm::mat4 projViewMatrix;
    glm::vec3 worldUp, up, front, right, position;
    float near, far;
public:
    Camera(float near, float far, const glm::vec3& position);
    virtual ~Camera() {}
    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjectionMatrix() const;
    float getNear() const;
    float getFar() const;
    const glm::vec3& getPosition() const;
    const glm::vec3& getDirectionVector() const;
    const glm::vec3& getUpVector() const;
    void setDirection(const glm::vec3& direction);
    void setDirectionLookAt(const glm::vec3& lookAtPosition);
    void setPosition(const glm::vec3& newPosition);
};

class OrthographicCamera : public Camera {
    float width, height;
public:
    OrthographicCamera(float near, float far, float width, float height, const glm::vec3& position);
    virtual ~OrthographicCamera() = default;
};

class PerspectiveCamera : public Camera {
protected:
    float aspectRatio, fovy;
public:
    PerspectiveCamera(float near, float far, float aspectRatio, float fovy, const glm::vec3& position);
    virtual ~PerspectiveCamera() = default;
    float getAspectRatio() const;
    float getFovy() const;
};

class FPSCamera : public PerspectiveCamera {
    float lastx, lasty;
    float yaw, pitch;
    float movementSpeed;
    float mouseSpeed;
public:
    FPSCamera(float near, float far, float aspectRatio, const glm::vec3& position, float fovy);
    void updateEuler(float mousex, float mousey);
    void updateQuat(float mousex, float mousey);
    void processKeyboard(Bindings binding, float deltaTime);
    void setMovementSpeed(float speed);
    void setMouseSpeed(float speed);
    // TODO from direction vectors of camera set angles properly
    void setDirection(const glm::vec3& direction) = delete;
    void setDirectionLookAt(const glm::vec3& lookAtPosition) = delete;
};