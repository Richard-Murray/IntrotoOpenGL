#include "Application7.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Vertex.h"

#include <stb_image.h>
#include "tiny_obj_loader.h"

#include "ModelManager.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera;
class FlyCamera;
struct Vertex;

Application7::Application7()
{}

Application7::~Application7()
{
	delete m_entityManager;
	delete m_renderer;
	delete m_camera;
}

int Application7::Run()
{
	if (glfwInit() == false)
		return -1;

	m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (m_window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return -3;
	}

	Gizmos::create();

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.25f, 0.25f, 1.0f, 1);
	glEnable(GL_DEPTH_TEST);

	float previousTime = 0.0f;

	Load();

	while (glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		Gizmos::clear();

		Update(deltaTime);
		Draw();

		Gizmos::draw(m_camera->GetProjectionView());

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	//delete m_meshArray;
	delete m_camera;

	Gizmos::destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
	return 0;
}

void Application7::Update(float deltaTime)
{
	m_renderer->Update(deltaTime);
	m_camera->Update(deltaTime);
	m_entityManager->Update(deltaTime);
}

void Application7::Draw()
{
	m_renderer->Draw();
}

void Application7::Load()
{
	m_camera = new FlyCamera(10.0f);
	m_camera->SetInputWindow(m_window);
	m_camera->SetUpPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 10000.f);

	m_renderer = new Renderer(m_camera, m_window);
	m_renderer->Load();

	m_entityManager = new EntityManager();
	m_renderer->AddEntityManager(m_entityManager);
	m_entityManager->CreateEntity("Test1");
	m_entityManager->GetNewEntity()->AttachModel(m_renderer->GetModelManager()->GetModel("Cube1"));
}