#include <iostream>
#include <vector>
#include "Scene.h"
#include "Camera.h"
#include "./Objects/Sphere/Sphere.h"
#include "./Objects/Object.h"
#include "./Objects/Triangle/Triangle.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "./Window/Window.h"
#include "./Utils.h"

int main() {
    const uint32_t WIDTH = 1920;
    const uint32_t HEIGHT = 1080;

    Camera *camera = new Camera(0.0f, 2.0f, 5.0f);

    AmbientLight* ambient_light = new AmbientLight(0.1f);
    DirectionalLight* directional_light = new DirectionalLight(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), 0.2f, 2.0f);

    Scene* scene = new Scene(camera, ambient_light, directional_light);
    scene->setAA(4);
    scene->setSoftShadowRays(2);

    Sphere* sphere1 = new Sphere(glm::vec4(-5.0, 0.0, -6.0, 1.0f), glm::vec4(1.0, 0.0, 0.0, 1.0f), 1.0f, 0.2f, 0.0f);
    Sphere* sphere2 = new Sphere(glm::vec4(5.0, 0.0, 6.0, 1.0f), glm::vec4(0.0, 1.0, 0.0, 1.0f), 1.0f, -1.0f, 0.5f);
    Sphere* sphere3 = new Sphere(glm::vec4(0.0, -5002.0f, 0.0, 1.0f), glm::vec4(1.0, 1.0, 0.0, 1.0f), 5000.0, 0.2f, 0.0f);
    Sphere* sphere4 = new Sphere(glm::vec4(5.0, 5.0, 0.0, 1.0f), glm::vec4(0.0, 1.0, 0.0, 1.0f), 1.0f, -1.0f, 0.5f);
    Sphere* sphere5 = new Sphere(glm::vec4(-5.0, 5.0, 0.0, 1.0f), glm::vec4(0.0, 1.0, 1.0, 1.0f), 1.0f, -1.0f, 0.5f);

    TriangleMesh* sus = new TriangleMesh(Utils::loadOBJ("sus.obj", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), -1.0f, 0.5f));
    sus->changePosition(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    PointLight* pointLight1 = new PointLight(glm::vec4(0.0f, 5.0f, 0.0, 1.0f), 0.4f, 128.0f);
    scene->addPointLight(pointLight1);

    Animation* sphere1_animation = new Animation(sphere1->getPosition(), glm::vec4(5.0f, 0.0f, -6.0f, 1.0f), 1.0f, true);
    sphere1->setAnimation(sphere1_animation);
    
    Animation* sphere2_animation = new Animation(sphere2->getPosition(), glm::vec4(-5.0f, 0.0f, 6.0f, 1.0f), 1.0f, true);
    sphere2->setAnimation(sphere2_animation);

    scene->addSphere(sphere1);
    scene->addSphere(sphere2);
    scene->addSphere(sphere3);
    scene->addSphere(sphere4);
    scene->addSphere(sphere5);
    scene->addMesh(sus);

    Window window = Window(WIDTH, HEIGHT, scene);

    window.run();

    return 0;
}
