#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "camera.h"

#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)

Camera::Camera(float near, float far, const glm::vec3& position)
    : near(near), far(far), position(position), worldUp(WORLD_UP) {

}

const glm::mat4& Camera::getViewMatrix() const {
    viewMatrix = glm::lookAt(position, position + front, up);
    return viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

float Camera::getNear() const {
    return near;
}

float Camera::getFar() const {
    return far;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

const glm::vec3& Camera::getDirectionVector() const {
    return front;
}

const glm::vec3& Camera::getUpVector() const {
    return up;
}


void Camera::setDirection(const glm::vec3& direction) {
    front = direction;
    right = glm::cross(front, worldUp);
    up = glm::cross(right, front);
}

void Camera::setDirectionLookAt(const glm::vec3& lookAtPosition) {
    front = glm::normalize(lookAtPosition - position);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::cross(right, front);
}

void Camera::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

OrthographicCamera::OrthographicCamera(float near, float far, float width, float height, const glm::vec3& position)
    : Camera(near, far, position), width(width), height(height) {
    projectionMatrix = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, near, far);
}

PerspectiveCamera::PerspectiveCamera(float near, float far, float aspectRatio, float fovy, const glm::vec3& position)
    : Camera(near, far, position), aspectRatio(aspectRatio), fovy(fovy) {
    projectionMatrix = glm::perspective(fovy, aspectRatio, near, far);
}

float PerspectiveCamera::getAspectRatio() const {
    return aspectRatio;
}

float PerspectiveCamera::getFovy() const {
    return fovy;
}

FPSCamera::FPSCamera(float near, float far, float aspectRatio, const glm::vec3& position, float fovy)
    : PerspectiveCamera(near, far, aspectRatio, fovy, position) {
    lastx = 0.0f;
    lasty = 0.0f;
    yaw = -90.0f;
    pitch = 0.0f;
    movementSpeed = 1.0f;
    mouseSpeed = 2.0f;
}


void FPSCamera::updateEuler(float mousex, float mousey) {
    static bool firstTouch = false;
    float deltax = lastx - mousex, deltay = lasty - mousey;
    if (!firstTouch) {
        deltax = 0.0f;
        deltay = 0.0f;
        firstTouch = true;
    }
    yaw -= deltax * mouseSpeed;
    pitch += deltay * mouseSpeed;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    lastx = mousex;
    lasty = mousey;
    float pitchRad = glm::radians(pitch), yawRad = glm::radians(yaw);
    float cosPitch = glm::cos(pitchRad);
    front.x = glm::cos(yawRad) * cosPitch;
    front.z = glm::sin(yawRad) * cosPitch;
    front.y = glm::sin(pitchRad);
    right = glm::cross(front, worldUp);
    up = glm::cross(right, front);
}

void FPSCamera::updateQuat(float mousex, float mousey) {
    static bool firstTouch = false;
    float deltax = glm::radians(lastx - mousex) * 0.01f, deltay = glm::radians(mousey - lasty) * 0.01f;
    if (!firstTouch) {
        deltax = 0.0f;
        deltay = 0.0f;
        firstTouch = true;
    }

    lastx = mousex;
    lasty = mousey;
    glm::fquat rotation = glm::fquat(glm::cos(deltax), 0.0f, glm::sin(deltax), 0.0f) * glm::fquat(glm::cos(deltay), glm::sin(deltay), 0.0f, 0.0f);
    front = rotation * front * glm::conjugate(rotation);
    //glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void FPSCamera::processKeyboard(Bindings binding, float deltaTime) {
    switch (binding) {
    case Bindings::FORWARD:
        position += movementSpeed * deltaTime * front;
        break;
    case Bindings::BACKWARD:
        position -= movementSpeed * deltaTime * front;
        break;
    case Bindings::RIGHT:
        position += movementSpeed * deltaTime * right;
        break;
    case Bindings::LEFT:
        position -= movementSpeed * deltaTime * right;
        break;
    case Bindings::UP:
        position += movementSpeed * deltaTime * worldUp;
        break;
    case Bindings::DOWN:
        position -= movementSpeed * deltaTime * worldUp;
        break;
    }
}

void FPSCamera::setMovementSpeed(float speed) {
    movementSpeed = speed;
}

void FPSCamera::setMouseSpeed(float speed) {
    mouseSpeed = speed;
}