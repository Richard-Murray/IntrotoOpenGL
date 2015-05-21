#ifndef APPLICATION7_H
#define APPLICATION7_H

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "MeshArray.h"

#include "Particles.h"

//#include "tiny_obj_loader.h"
#include "FBXFile.h"

#include "Renderer.h"
#include "EntityManager.h"
#include "AssetManager.h"

#include "Loader.h"

#include "Checkers.h"

//class MeshArray;
class FlyCamera;
struct OpenGLInfo;
struct KeyFrame;

class Application7
{
public:
	Application7();
	~Application7();

	int Run();

	void Load();

	void Update(float deltaTime);
	void Draw();

private:
	void GenerateSimpleTexturePlane();

	void createOpenGLBuffers(FBXFile* fbx);
	void cleanupOpenGLBuffers(FBXFile* fbx);

private:
	Renderer* m_renderer;

	//MeshArray* m_meshArray;
	FlyCamera* m_camera;
	GLFWwindow* m_window;

	EntityManager* m_entityManager;
	AssetManager* m_assetManager;

	CheckersManager* m_checkersManager;
	
};




#endif