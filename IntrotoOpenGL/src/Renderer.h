#ifndef RENDERER_H
#define RENDERER_H

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Gizmos.h"
#include <stb_image.h>
#include "tiny_obj_loader.h"
#include "FBXFile.h"
#include "AntTweakBar.h"

#include "Particles.h"
#include "MeshArray.h"
#include "Camera.h"
#include "Vertex.h"
#include "RenderTarget.h"

class Entity;

class AssetManager;
class EntityManager;
class CheckersManager;

class Renderer
{
public:
	Renderer(FlyCamera* camera, GLFWwindow* window);
	~Renderer();

	void Update(float deltaTime);
	void Draw();
	void DrawScene(BaseCamera* camera);
	void DrawLighting(BaseCamera* camera);

	void Load();

	unsigned int LoadShader(unsigned int type, const char* path);
	void CreateShader(unsigned int &shader, const char* vert, const char* frag);
	void CreateShader(unsigned int &shader, const char* vert, const char* geom, const char* frag);

	void LoadTexture(unsigned int &texture, const char* path);
	void LoadTextureRGBA(unsigned int &texture, const char* path);

	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanupOpenGLBuffers(FBXFile* fbx);

	void AddEntityManager(EntityManager* entityManager);
	void AddAssetManager(AssetManager* assetManager);
	void AddCheckersManager(CheckersManager* checkersManager);

private:
	AssetManager* m_pAssetManager;
	EntityManager* m_pEntityManager;

	CheckersManager* m_pCheckersManager;

private:

	//DEFAULTS
	unsigned int m_defaultTexture;

	//SHADERS
	//unsigned int m_programObjID;
	//unsigned int m_programTexturePlaneID;
	//unsigned int m_programTexturePlaneSimpleID;
	//
	//unsigned int m_programShadowMapID;
	//unsigned int m_programShadowMeshID;

	unsigned int m_programGeometryBufferID;
	unsigned int m_programDirectionalLightID;
	unsigned int m_programPointLightID;
	unsigned int m_programCompositeID;

	//unsigned int m_programPostProcessID;
	//
	//unsigned int m_programTerrainID;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;		
	
	//ParticleEmitter* m_particleEmitter;
	GPUParticleEmitter* m_gpuParticleEmitter;

	FBXFile* m_FBX;

	MeshArray* m_meshArray;
	FlyCamera* m_camera;
	GLFWwindow* m_window;

	std::vector<OpenGLInfo> m_gl_info;
		
	//For gizmo 'leg' animation
	struct KeyFrame
	{
		glm::vec3 position;
		glm::quat rotation;
	};
	
private:
	//Test content

	//Texture plane
	void GenerateTexturePlane();
	unsigned int m_VAOtest;
	unsigned int m_texture, m_normalMap;

	//Gizmo movement animation
	/*glm::vec3 m_positions[4];
	glm::quat m_rotations[4];

	unsigned int anim;
	unsigned int anim2;
	float animCountdown;*/

	//Gizmo test animation
	/*KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];

	glm::mat4 m_hipBone;
	glm::mat4 m_kneeBone;
	glm::mat4 m_ankleBone;*/

	//Render targets
	RenderTarget* m_pDefferedRenderTarget;
	RenderTarget* m_pLightingRenderTarget;


	//Render Target Tutorial variables
	void GenerateTexturePlaneAlt();
	unsigned int m_VAOplane;
	BaseCamera* m_targetCamera;

	//Shadow maps
	glm::vec3 m_lightDir;
	glm::mat4 m_lightMatrix;	
	//FBXFile* m_FBXBunny;
	//FBXFile* m_FBXextratest; //Example fbx file

	//Deferred rendering
	/*void CreateDeferredRenderingBuffers();
	/*unsigned int m_gpassFBO;
	/*unsigned int m_albedoTexture;
	/*unsigned int m_positionTexture;
	/*unsigned int m_normalTexture;
	/*unsigned int m_gpassDepth;
	/*unsigned int m_lightFBO;
	/*unsigned int m_lightTexture;*/

	//newstuff
	RenderTarget* m_pGeometryPassRenderTarget;
	RenderTarget* m_pLightPassRenderTarget;
	RenderTarget* m_pPostProcessRenderTarget;
	void DrawGeometryPass(BaseCamera* camera);
	void DrawLightPass(BaseCamera* camera);
	void DrawDirectionalLight(BaseCamera* camera, const glm::vec3& direction, const glm::vec3& diffuse);
	void DrawPointLight(BaseCamera* camera, const glm::vec3& position, float radius, const glm::vec3& diffuse);
	void DrawCompositePass(BaseCamera* camera);
	void DrawPostProcessingPass(BaseCamera* camera);

	void CreateCube();
	unsigned int m_VAOCube;
	
	//Postprocess
	//RenderTarget* m_pPostProcessRenderTarget;
	void CreateFullScreenQuad();
	unsigned int m_VAOfullScreenQuad;

	//GUI
	/*TwBar* m_tweakBar;
	bool m_hideObjects;
	bool m_toResetTerrain;*/

	//Procedural generation
	/*void GenerateProceduralPlane();
	void ResetProceduralPlane();
	float *m_perlinData;
	unsigned int m_perlinTexture;
	unsigned int m_proceduralVAO;

	unsigned int m_grassTexture;
	unsigned int m_rockTexture;
	unsigned int m_snowTexture;*/

	//glm::vec3 m_terrainLightDirection;

	//float m_seed;

	//Entity test
	//Entity* m_entityTest;
	//Entity* m_entity2Test;

	//ignore these
	//void CreatePostProcessFramebuffer();
	//unsigned int m_FBOPostProcess;

	//static void OnMouseButton(GLFWwindow*, int b, int a, int m) {
	//	TwEventMouseButtonGLFW(b, a);
	//}
	//static void OnMousePosition(GLFWwindow*, double x, double y) {
	//	TwEventMousePosGLFW((int)x, (int)y);
	//}
	//static void OnMouseScroll(GLFWwindow*, double x, double y) {
	//	TwEventMouseWheelGLFW((int)y);
	//}
	//static void OnKey(GLFWwindow*, int k, int s, int a, int m) {
	//	TwEventKeyGLFW(k, a);
	//}
	//static void OnChar(GLFWwindow*, unsigned int c) {
	//	TwEventCharGLFW(c, GLFW_PRESS);
	//}
	//static void OnWindowResize(GLFWwindow*, int w, int h) {
	//	TwWindowSize(w, h);
	//	glViewport(0, 0, w, h);
	//}

};



#endif