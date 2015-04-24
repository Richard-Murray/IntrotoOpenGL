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

class MeshArray;
class FlyCamera;
struct OpenGLInfo;
struct KeyFrame;

class Application7
{
public:
	Application7();
	~Application7();

	int Run();

	//unsigned int LoadShader(unsigned int type, const char* path);

	void CreateSimpleShader();
	void CreateTextureShader();
	void CreateSimpleObjShader();
	void CreateParticleShader();

	void Load();

	void Update(float deltaTime);
	void Draw();

private:
	void GenerateSimpleTexturePlane();

	void createOpenGLBuffers(FBXFile* fbx);
	void cleanupOpenGLBuffers(FBXFile* fbx);

private:
	Renderer* m_renderer;

	MeshArray* m_meshArray;
	FlyCamera* m_camera;
	GLFWwindow* m_window;
};




#endif