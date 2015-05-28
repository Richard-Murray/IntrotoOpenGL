#include "Renderer.h"
#include "AssetManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Checkers.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

Renderer::Renderer(FlyCamera* camera, GLFWwindow* window)
{
	m_camera = camera;
	m_window = window;
}

Renderer::~Renderer()
{
	//TwDeleteAllBars();
	//TwTerminate();
	//
	//delete m_targetCamera;
	//
	//delete m_pLightingRenderTarget;
	//delete m_pDefferedRenderTarget;
	//
	//delete m_FBX;
	//delete m_gpuParticleEmitter;
	//
	//glDeleteProgram(m_programObjID);
	//glDeleteProgram(m_programTexturePlaneID);
	//
	//CleanupOpenGLBuffers(m_FBX);
}

void Renderer::Update(float deltaTime)
{
	float s = glm::cos((float)glfwGetTime()) * 0.5f + 0.5f;

	//SKELETON STUFF
	//FBXSkeleton* skeleton = m_FBX->getSkeletonByIndex(0);
	//FBXAnimation* animation = m_FBX->getAnimationByIndex(0);
	//
	//skeleton->evaluate(animation, glfwGetTime());
	//
	//for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	//{
	//	skeleton->m_nodes[bone_index]->updateGlobalTransform();
	//}
	//
	//m_gpuParticleEmitter->Update(deltaTime);
	//
	//if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)
	//{
	//	m_targetCamera->SetTransform(m_camera->GetTransform());
	//}
	//
	//if (m_toResetTerrain == true)
	//{
	//	ResetProceduralPlane();
	//	m_toResetTerrain = false;
	//}
	mat4 moveTransform = mat4(	1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								-0.01f, 0, 0, 1);

	//m_entityTest->TestMoveTransform(moveTransform);
}

void Renderer::Draw()
{
	//////Regular drawing
	//m_pLightingRenderTarget->SetAsActiveRenderTarget();
	//glClear(GL_DEPTH_BUFFER_BIT);
	//
	//DrawLighting(m_targetCamera);
	//
	//m_pDefferedRenderTarget->SetAsActiveRenderTarget();
	//glClearColor(0.25f, 0.25f, 0.25f, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//DrawScene(m_targetCamera);
	////Gizmos::draw(m_targetCamera->GetProjectionView());
	//
	//m_pDefferedRenderTarget->ClearAsActiveRenderTarget();
	//glClearColor(0.55f, 0.55f, 0.55f, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//glUseProgram(m_programTexturePlaneSimpleID);
	//
	//int loc = glGetUniformLocation(m_programTexturePlaneSimpleID, "ProjectionView");
	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_pDefferedRenderTarget->GetRenderTexture(0));
	//glUniform1i(glGetUniformLocation(m_programTexturePlaneSimpleID, "diffuse"), 0);
	//
	//glBindVertexArray(m_VAOplane);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//
	//DrawScene(m_camera);
	//
	//TwDraw();

	////Deferred rendering drawing
	m_pGeometryPassRenderTarget->SetAsActiveRenderTarget();
	//m_pGeometryPassRenderTarget->ClearAsActiveRenderTarget();//
	// G-Pass: render out the albedo, position and normal
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawGeometryPass(m_camera);

	glDisable(GL_DEPTH_TEST);

	m_pLightPassRenderTarget->SetAsActiveRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT);
	DrawLightPass(m_camera);

	m_pLightPassRenderTarget->ClearAsActiveRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT);
	DrawCompositePass(m_camera);

	glEnable(GL_DEPTH_TEST);

	//////Post process drawing
	//// bind our target
	//glBindFramebuffer(GL_FRAMEBUFFER, m_pPostProcessRenderTarget->GetFBO());
	//glViewport(0, 0, 1280, 720);
	//
	//// clear the target
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//// draw our 3D scene
	//DrawScene(m_camera);
	//// gizmos for now
	//Gizmos::draw(m_camera->GetProjectionView());
	//
	//// bind the back-buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 1280, 720);
	//
	//// just clear the back-buffer depth as
	//// each pixel will be filled
	//glClear(GL_DEPTH_BUFFER_BIT);
	//
	//// draw out full-screen quad
	//glUseProgram(m_programPostProcessID);
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_pPostProcessRenderTarget->GetRenderTexture(0));
	//
	//int loc = glGetUniformLocation(m_programPostProcessID, "target");
	//glUniform1i(loc, 0);
	//
	//glBindVertexArray(m_VAOfullScreenQuad);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

//void Renderer::DrawLighting(BaseCamera* camera)
//{
//	glUseProgram(m_programShadowMapID);
//
//	//bind light matrix
//	int loc = glGetUniformLocation(m_programShadowMapID, "LightMatrix");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_lightMatrix[0][0]));
//
//	//draw all shadow-casting geometry
//	/*for (unsigned int i = 0; i < m_FBXBunny->getMeshCount(); ++i)
//	{
//		FBXMeshNode* mesh = m_FBXBunny->getMeshByIndex(i);
//
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//
//		glBindVertexArray(glData[0]);
//		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
//	}*/
//	m_modelManager->GetModel("BedSet")->DrawModel();
//	m_modelManager->GetModel("Cube1")->DrawModel();
//}
//
//void Renderer::DrawScene(BaseCamera* camera)
//{
//	//REGULAR DRAWING
//
//	int loc;
//
//	
//	//Shadowing stuff
//	//----------
//	glUseProgram(m_programShadowMeshID);
//
//	//bind the camera
//	loc = glGetUniformLocation(m_programShadowMeshID, "ProjectionView");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));
//
//	//bind light matrix
//	glm::mat4 textureSpaceOffset(
//		0.5f, 0.0f, 0.0f, 0.0f,
//		0.0f, 0.5f, 0.0f, 0.0f,
//		0.0f, 0.0f, 0.5f, 0.0f,
//		0.5f, 0.5f, 0.5f, 1.0f);
//
//	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;
//
//	float shadowBias = 0.01f;
//
//	loc = glGetUniformLocation(m_programShadowMeshID, "LightMatrix");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightMatrix[0][0]));
//
//	loc = glGetUniformLocation(m_programShadowMeshID, "LightDir");
//	glUniform3fv(loc, 1, &m_lightDir[0]);
//
//	loc = glGetUniformLocation(m_programShadowMeshID, "shadowMap");
//	glUniform1i(loc, 0);
//
//	loc = glGetUniformLocation(m_programShadowMeshID, "shadowBias");
//	glUniform1f(loc, shadowBias);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, m_pLightingRenderTarget->GetDepthTexture());
//
//	//draw all shadow-recieving
//	/*for (unsigned int i = 0; i < m_FBXBunny->getMeshCount(); ++i)
//	{
//		FBXMeshNode* mesh = m_FBXBunny->getMeshByIndex(i);
//
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//
//		glBindVertexArray(glData[0]);
//		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
//	}*/
//
//	m_modelManager->GetModel("BedSet")->DrawModel();
//	m_modelManager->GetModel("Cube1")->DrawModel();
//
//	/*for (unsigned int i = 0; i < m_FBXextratest->getMeshCount(); ++i)
//	{
//		FBXMeshNode* mesh = m_FBXextratest->getMeshByIndex(i);
//
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//
//		glBindVertexArray(glData[0]);
//		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
//	}*/
//
//
//	//----------
//
//
//	//Hardcoded test values
//	vec3 lightDirection = vec3(-0.5f, 0.5f/*glfwGetTime() * 0.1f*/, 0.5f);
//	vec3 lightColour = vec3(1.0f, 0.7f, 0.0f);
//	float specularPower = 1.0f;
//	FBXSkeleton* skeleton = m_FBX->getSkeletonByIndex(0);
//	skeleton->updateBones();
//
//	glUseProgram(m_programObjID);
//	unsigned int viewProjectionUniform = glGetUniformLocation(m_programObjID, "ProjectionView");
//	unsigned int lightDirectionUniform = glGetUniformLocation(m_programObjID, "LightDirection");
//	unsigned int lightColourUniform = glGetUniformLocation(m_programObjID, "LightColour");
//	unsigned int cameraPositionUniform = glGetUniformLocation(m_programObjID, "CameraPosition");
//	unsigned int specularPowerUniform = glGetUniformLocation(m_programObjID, "SpecularPower");
//	unsigned int bonesUniform = glGetUniformLocation(m_programObjID, "Bones");
//
//	glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionView()));
//	glUniform3fv(lightDirectionUniform, 1, glm::value_ptr(lightDirection));
//	glUniform3fv(lightColourUniform, 1, glm::value_ptr(lightColour));
//	glUniform3fv(cameraPositionUniform, 1, glm::value_ptr(camera->GetPosition()));
//	glUniform1f(specularPowerUniform, specularPower);
//	glUniformMatrix4fv(bonesUniform, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);
//
//	int iDiffuseLoc = glGetUniformLocation(m_programObjID, "Diffuse");
//	glUniform1i(iDiffuseLoc, 0);
//
//	for (unsigned int i = 0; i < m_FBX->getMeshCount(); ++i)
//	{
//		FBXMeshNode* mesh = m_FBX->getMeshByIndex(i);
//
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, mesh->m_material->textures[FBXMaterial::DiffuseTexture]->handle);
//
//		glBindVertexArray(glData[0]);
//		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
//	}
//
//	//Textured plane
//	//----------
//	glUseProgram(m_programTexturePlaneID);
//
//	//set texture slots
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, m_texture);
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, m_normalMap);
//
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, m_pLightingRenderTarget->GetDepthTexture());
//
//	//bind camera
//	loc = glGetUniformLocation(m_programTexturePlaneID, "ProjectionView");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));
//
//	loc = glGetUniformLocation(m_programTexturePlaneID, "LightMatrix");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightMatrix[0][0]));
//
//	loc = glGetUniformLocation(m_programTexturePlaneID, "shadowMap");
//	glUniform1i(loc, 2);
//
//	loc = glGetUniformLocation(m_programTexturePlaneID, "shadowBias");
//	glUniform1f(loc, shadowBias);
//
//	//tell shader where it is
//	loc = glGetUniformLocation(m_programTexturePlaneID, "diffuse");
//	glUniform1i(loc, 0);
//	loc = glGetUniformLocation(m_programTexturePlaneID, "normal");
//	glUniform1i(loc, 1);
//
//	//bind light
//	vec3 light(m_lightDir);
//	loc = glGetUniformLocation(m_programTexturePlaneID, "LightDirection");
//	glUniform3f(loc, light.x, light.y, light.z);
//
//	//draw
//	glBindVertexArray(m_VAOtest);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//	//----------
//
//	m_gpuParticleEmitter->Draw((float)glfwGetTime(), camera->GetTransform(), camera->GetProjectionView());
//
//
//	//Procedurally generated plane
//	//----------
//	//glUseProgram(m_programTerrainID);
//	//
//	//glActiveTexture(GL_TEXTURE0);
//	//glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
//	//
//	//glActiveTexture(GL_TEXTURE1);
//	//glBindTexture(GL_TEXTURE_2D, m_grassTexture);
//	//
//	//glActiveTexture(GL_TEXTURE2);
//	//glBindTexture(GL_TEXTURE_2D, m_rockTexture);
//	//
//	//glActiveTexture(GL_TEXTURE3);
//	//glBindTexture(GL_TEXTURE_2D, m_snowTexture);
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "view_proj");
//	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "perlin_texture");
//	//glUniform1i(loc, 0);
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "grass_texture");
//	//glUniform1i(loc, 1);
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "rock_texture");
//	//glUniform1i(loc, 2);
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "snow_texture");
//	//glUniform1i(loc, 3);
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "light_direction");
//	//glUniform3fv(loc, 1, glm::value_ptr(m_terrainLightDirection));
//	//
//	//loc = glGetUniformLocation(m_programTerrainID, "camera_position");
//	//glUniform3fv(loc, 1, glm::value_ptr(camera->GetPosition()));
//	//
//	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	//glBindVertexArray(m_proceduralVAO);
//	//glDrawElements(GL_TRIANGLES, 64 * 64 * 6, GL_UNSIGNED_INT, nullptr);
//}

void Renderer::DrawGeometryPass(BaseCamera* camera)
{
	//// G-Pass: render out the albedo, position and normal

	//glUseProgram(m_programGeometryBufferID);
	//
	//// bind camera transforms
	//int loc = glGetUniformLocation(m_programGeometryBufferID, "ProjectionView");
	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));
	//loc = glGetUniformLocation(m_programGeometryBufferID, "View");
	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetView()[0][0]));
	//
	// draw our scene
	//for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {
	//
	//	FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
	//
	//	unsigned int* glData = (unsigned int*)mesh->m_userData;
	//
	//	glBindVertexArray(glData[0]);
	//
	//	glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	//}
	//m_modelManager->GetModel("BedSet")->DrawModel();
	//m_modelManager->GetModel("Cube1")->DrawModel();

	m_pEntityManager->Draw(camera);
	m_pCheckersManager->Draw(camera);

	glUseProgram(m_programGeometryBufferID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pAssetManager->GetTexture("Snow"));

	unsigned int projectionViewUniform = glGetUniformLocation(m_programGeometryBufferID, "ProjectionView");
	unsigned int viewUniform = glGetUniformLocation(m_programGeometryBufferID, "View");
	unsigned int worldTransformUniform = glGetUniformLocation(m_programGeometryBufferID, "WorldTransform");
	unsigned int diffuseUniform = glGetUniformLocation(m_programGeometryBufferID, "Diffuse");

	mat4 transform = mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, 1, 1);

	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &(camera->GetView()[0][0]));
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));
	glUniformMatrix4fv(worldTransformUniform, 1, GL_FALSE, &(transform[0][0]));
	glUniform1i(diffuseUniform, 0);

	glBindVertexArray(m_VAOCube);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	//m_entityTest->Draw(camera);
	//m_entity2Test->Draw(camera);
}

void Renderer::DrawLightPass(BaseCamera* camera)
{
	// Light Pass: render lights as geometry, sampling position and
	// normals disable depth testing and enable additive blending

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//Directional Lights
	glUseProgram(m_programDirectionalLightID);

	unsigned int positionTextureUniform = glGetUniformLocation(m_programDirectionalLightID,"positionTexture");
	unsigned int normalTextureUniform = glGetUniformLocation(m_programDirectionalLightID, "normalTexture");

	glUniform1i(positionTextureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pGeometryPassRenderTarget->GetRenderTexture(1));
	
	glUniform1i(normalTextureUniform, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pGeometryPassRenderTarget->GetRenderTexture(2));

	// draw lights as fullscreen quads
	DrawDirectionalLight(camera, glm::vec3(-1), glm::vec3(1));

	//Point Lights
	glUseProgram(m_programPointLightID);

	positionTextureUniform = glGetUniformLocation(m_programPointLightID, "positionTexture");
	normalTextureUniform = glGetUniformLocation(m_programPointLightID, "normalTexture");

	glUniform1i(positionTextureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pGeometryPassRenderTarget->GetRenderTexture(1));

	glUniform1i(normalTextureUniform, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pGeometryPassRenderTarget->GetRenderTexture(2));

	glCullFace(GL_FRONT);

	DrawPointLight(camera, vec3(sinf(glfwGetTime()) * 5, 3, cosf(glfwGetTime()) * 5), 5, vec3(1, 0, 0));
	DrawPointLight(camera, vec3(2, 1, 2), 500, vec3(1, 0, 0));

	glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}

void Renderer::DrawCompositePass(BaseCamera* camera)
{
	// Composite Pass: render a quad and combine albedo and light

	glUseProgram(m_programCompositeID);

	unsigned int albedoTextureUniform = glGetUniformLocation(m_programCompositeID, "albedoTexture");
	unsigned int lightTextureUniform = glGetUniformLocation(m_programCompositeID, "lightTexture");

	glUniform1i(albedoTextureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pGeometryPassRenderTarget->GetRenderTexture(0));
		
	glUniform1i(lightTextureUniform, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pLightPassRenderTarget->GetRenderTexture(0));

	glBindVertexArray(m_VAOfullScreenQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawDirectionalLight(BaseCamera* camera, const glm::vec3& direction, const glm::vec3& diffuse)
{
	glm::vec4 viewSpaceLight = m_camera->GetView() *glm::vec4(glm::normalize(direction), 0);

	unsigned int lightDirectionUniform = glGetUniformLocation(m_programDirectionalLightID, "lightDirection");
	unsigned int lightDiffuseUniform = glGetUniformLocation(m_programDirectionalLightID, "lightDiffuse");

	glUniform3fv(lightDirectionUniform, 1, &viewSpaceLight[0]);	
	glUniform3fv(lightDiffuseUniform, 1, &diffuse[0]);

	glBindVertexArray(m_VAOfullScreenQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawPointLight(BaseCamera* camera, const glm::vec3& position, float radius, const glm::vec3& diffuse)
{
	glm::vec4 viewSpacePosition = camera->GetView() * glm::vec4(position, 1);

	unsigned int lightPositionUniform = glGetUniformLocation(m_programPointLightID, "lightPosition");
	unsigned int lightPositionViewUniform = glGetUniformLocation(m_programPointLightID, "lightPositionView");
	unsigned int lightRadiusUniform = glGetUniformLocation(m_programPointLightID, "lightRadius");
	unsigned int lightDiffuseUniform = glGetUniformLocation(m_programPointLightID, "lightDiffuse");

	glUniform3fv(lightPositionUniform, 1, &position[0]);	
	glUniform3fv(lightPositionViewUniform, 1, &viewSpacePosition[0]);
	glUniform1f(lightRadiusUniform, radius);	
	glUniform3fv(lightDiffuseUniform, 1, &diffuse[0]);

	glBindVertexArray(m_VAOCube);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Load()
{
	//LoadTextureRGBA(m_defaultTexture, "data/textures/default.png");
	//
	//m_pAssetManager = new AssetManager(this);
	//m_pAssetManager->LoadModel("Cube1", "data/models/Cube.fbx");
	//m_pAssetManager->LoadModel("BedSet", "data/models/Crate.fbx");
	
	//CreateShader(m_programObjID, "./data/shaders/obj.vert", "./data/shaders/obj.frag");
	//CreateShader(m_programTexturePlaneID, "./data/shaders/texPlane.vert", "./data/shaders/texPlane.frag");
	//CreateShader(m_programTexturePlaneSimpleID, "./data/shaders/texPlaneSimple.vert", "./data/shaders/texPlaneSimple.frag");
	//
	//CreateShader(m_programShadowMeshID, "./data/shaders/shadowMesh.vert", "./data/shaders/shadowMesh.frag");
	//CreateShader(m_programShadowMapID, "./data/shaders/shadowMap.vert", "./data/shaders/shadowMap.frag");

	//CreateShader(m_programGeometryBufferID, "./data/shaders/deferredGbuffer.vert", "./data/shaders/deferredGbuffer.frag");
	//CreateShader(m_programDirectionalLightID, "./data/shaders/deferredDirectionalLight.vert", "./data/shaders/deferredDirectionalLight.frag");
	//CreateShader(m_programCompositeID, "./data/shaders/deferredComposite.vert", "./data/shaders/deferredComposite.frag");
	//
	//m_entityTest = new Entity("test");
	//m_entityTest->AttachModel(m_pAssetManager->GetModel("Cube1"));
	//m_entityTest->AttachShader(m_programGeometryBufferID);
	//m_entityTest->AttachTexture(m_defaultTexture);
	//m_entity2Test = new Entity("test2");
	//m_entity2Test->AttachModel(m_pAssetManager->GetModel("BedSet"));
	//m_entity2Test->AttachShader(m_programGeometryBufferID);
	//m_entity2Test->AttachTexture(m_defaultTexture);

	//CreateShader(m_programPostProcessID, "./data/shaders/postProcess.vert", "./data/shaders/postProcess.frag");
	//
	//CreateShader(m_programTerrainID, "./data/shaders/proceduralPlane.vert", "./data/shaders/proceduralPlane.geom", "./data/shaders/proceduralPlane.frag");
	//
	////m_meshArray = new MeshArray();
	////GenerateTexturePlane();
	//
	////m_gpuParticleEmitter = new GPUParticleEmitter();
	////m_gpuParticleEmitter->Initialise(100000, 0.1f, 5.0f, 5, 20, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	//
	////Animated model
	////m_FBX = new FBXFile();
	////m_FBX->load("data/characters/Pyro/pyro.fbx");
	////m_FBX->initialiseOpenGLTextures();
	////CreateOpenGLBuffers(m_FBX);
	//
	////Textured plane
	////----------
	////int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	//
	////LoadTexture(m_texture, "./data/textures/rock_diffuse.tga");
	////LoadTexture(m_normalMap, "./data/textures/rock_normal.tga");
	////----------
	//
	////Render target loading
	////----------
	//m_pDefferedRenderTarget = new RenderTarget();
	//m_pDefferedRenderTarget->SetSize(1024, 1024);
	//m_pDefferedRenderTarget->Initialise();
	//m_pDefferedRenderTarget->AttachColourBuffer(0, GL_RGBA8);
	//m_pDefferedRenderTarget->AttachDepthBuffer();
	//m_pDefferedRenderTarget->SetDrawBuffers();
	//
	//m_pLightingRenderTarget = new RenderTarget();
	//m_pLightingRenderTarget->SetSize(1024, 1024);
	//m_pLightingRenderTarget->Initialise();
	//m_pLightingRenderTarget->AttachDepthBuffer(GL_DEPTH_COMPONENT16, true);
	//m_pLightingRenderTarget->SetDrawBuffers();
	//
	//GenerateTexturePlaneAlt();

	m_programGeometryBufferID = m_pAssetManager->GetShader("GeometryPass");
	m_programDirectionalLightID = m_pAssetManager->GetShader("DirectionalLight");
	m_programPointLightID = m_pAssetManager->GetShader("PointLight");
	m_programCompositeID = m_pAssetManager->GetShader("CompositePass");

	m_pGeometryPassRenderTarget = new RenderTarget();
	m_pGeometryPassRenderTarget->SetSize(1280, 720);
	m_pGeometryPassRenderTarget->Initialise();
	m_pGeometryPassRenderTarget->AttachColourBuffer(0, GL_RGB8); //albedo
	m_pGeometryPassRenderTarget->AttachColourBuffer(1, GL_RGB32F); //position
	m_pGeometryPassRenderTarget->AttachColourBuffer(2, GL_RGB32F); //normal
	m_pGeometryPassRenderTarget->AttachDepthBuffer();
	m_pGeometryPassRenderTarget->SetDrawBuffers();

	m_pLightPassRenderTarget = new RenderTarget();
	m_pLightPassRenderTarget->SetSize(1280, 720);
	m_pLightPassRenderTarget->Initialise();
	m_pLightPassRenderTarget->AttachColourBuffer(0, GL_RGB8);
	m_pLightPassRenderTarget->SetDrawBuffers();

	m_pPostProcessRenderTarget = new RenderTarget();
	m_pPostProcessRenderTarget->SetSize(1280, 720);
	m_pPostProcessRenderTarget->Initialise();
	m_pPostProcessRenderTarget->AttachColourBuffer(0, GL_RGB8);
	m_pPostProcessRenderTarget->AttachDepthBuffer();
	m_pPostProcessRenderTarget->SetDrawBuffers();


	//make post process stuff later

	//m_pPostProcessRenderTarget = new RenderTarget();
	//m_pPostProcessRenderTarget->SetSize(1280, 720);
	//m_pPostProcessRenderTarget->AttachColourBuffer(0, GL_RGBA8);
	//m_pPostProcessRenderTarget->AttachDepthBuffer();
	//m_pPostProcessRenderTarget->SetDrawBuffers();

	/*m_targetCamera = new BaseCamera(vec3(5, 5, 5));
	m_targetCamera->SetUpPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 10000.f);*/

	CreateFullScreenQuad();
	CreateCube();
	
	////----------
	//
	////shadow maps
	////----------
	////m_FBXBunny = new FBXFile();
	////m_FBXBunny->load("data/models/BedSetMediumPoly.fbx");
	////m_FBXBunny->initialiseOpenGLTextures();
	////CreateOpenGLBuffers(m_FBXBunny);
	//
	//m_FBXextratest = new FBXFile();
	//m_FBXextratest->load("data/models/Cube.fbx");
	//m_FBXextratest->initialiseOpenGLTextures();
	//CreateOpenGLBuffers(m_FBXextratest);
	//
	//m_lightDir = glm::normalize(glm::vec3(1, 2.5f, 1));
	//
	//glm::mat4 lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	//glm::mat4 lightView = glm::lookAt(m_lightDir, glm::vec3(0), glm::vec3(0, 1, 0));
	//
	//m_lightMatrix = lightProjection * lightView;
	//
	//glm::mat4 textureSpaceOffset(
	//0.5f, 0.0f, 0.0f, 0.0f,
	//0.0f, 0.5f, 0.0f, 0.0f,
	//0.0f, 0.0f, 0.5f, 0.0f,
	//0.5f, 0.5f, 0.5f, 1.0f);
	//
	//glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;
	////----------
	//
	////GUI
	////----------
	//TwInit(TW_OPENGL_CORE, nullptr);
	//TwWindowSize(1280, 720);
	//
	//glfwSetMouseButtonCallback(m_window, OnMouseButton);
	//glfwSetCursorPosCallback(m_window, OnMousePosition);
	//glfwSetScrollCallback(m_window, OnMouseScroll);
	//glfwSetKeyCallback(m_window, OnKey);
	//glfwSetCharCallback(m_window, OnChar);
	//glfwSetWindowSizeCallback(m_window, OnWindowResize);
	//
	//m_tweakBar = TwNewBar("Options");
	//m_hideObjects = false;
	//m_toResetTerrain = false;
	//TwAddVarRW(m_tweakBar, "Show Terrain Only", TW_TYPE_BOOLCPP, &m_hideObjects, "");
	//TwAddVarRW(m_tweakBar, "Reset Terrain", TW_TYPE_BOOLCPP, &m_toResetTerrain, "");
	//TwAddVarRW(m_tweakBar, "Terrain Seed", TW_TYPE_FLOAT, &m_seed, "");
	//TwAddVarRW(m_tweakBar, "Light X", TW_TYPE_FLOAT, &m_terrainLightDirection.x, "");
	//TwAddVarRW(m_tweakBar, "Light Y", TW_TYPE_FLOAT, &m_terrainLightDirection.y, "");
	//TwAddVarRW(m_tweakBar, "Light Z", TW_TYPE_FLOAT, &m_terrainLightDirection.z, "");
	//
	////----------
	//
	////Procedural gen
	////----------
	//GenerateProceduralPlane();
	//
	//LoadTexture(m_grassTexture, "./data/textures/grass.jpg");
	//LoadTexture(m_rockTexture, "./data/textures/rock.jpg");
	//LoadTexture(m_snowTexture, "./data/textures/snow.jpg");
	//
	////----------
}

void Renderer::LoadTexture(unsigned int &texture, const char* path)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

void Renderer::LoadTextureRGBA(unsigned int &texture, const char* path)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

unsigned int Renderer::LoadShader(unsigned int type, const char* path)
{
	FILE* file = fopen(path, "rb");
	if (file == nullptr)
		return 0;

	//read shader source
	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	delete[] source;

	return shader;
}

void Renderer::CreateShader(unsigned int &shader, const char* vert, const char* frag)
{
	unsigned int vs = LoadShader(GL_VERTEX_SHADER, vert);
	unsigned int fs = LoadShader(GL_FRAGMENT_SHADER, frag);

	int success = GL_FALSE;

	shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fs);
	glDeleteShader(vs);
}

void Renderer::CreateShader(unsigned int &shader, const char* vert, const char* geom, const char* frag)
{
	unsigned int vs = LoadShader(GL_VERTEX_SHADER, vert);
	unsigned int gs = LoadShader(GL_GEOMETRY_SHADER, geom);
	unsigned int fs = LoadShader(GL_FRAGMENT_SHADER, frag);


	int success = GL_FALSE;
	//
	//unsigned int shader;

	shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glAttachShader(shader, gs);
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fs);
	glDeleteShader(vs);
	glDeleteShader(gs);
}

void Renderer::CreateOpenGLBuffers(FBXFile* fbx)
{
	int numMeshes = fbx->getMeshCount();

	for (int i = 0; i < numMeshes; i++)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER,
			mesh->m_vertices.size() * sizeof(FBXVertex),
			mesh->m_vertices.data(), GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->m_indices.size() * sizeof(unsigned int),
			mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal
		glEnableVertexAttribArray(2); //texture coordinates
		glEnableVertexAttribArray(3); //tangentsss
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TangentOffset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}

void Renderer::CleanupOpenGLBuffers(FBXFile* fbx)
{
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}

//void Renderer::CreateDeferredRenderingBuffers()
//{
//	//setup gpass framebuffer
//	glGenFramebuffers(1, &m_gpassFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO);
//
//	glGenTextures(1, &m_albedoTexture);
//	glBindTexture(GL_TEXTURE_2D, m_albedoTexture);
//
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	glGenTextures(1, &m_positionTexture);
//	glBindTexture(GL_TEXTURE_2D, m_positionTexture);
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	glGenTextures(1, &m_normalTexture);
//	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	glGenRenderbuffers(1, &m_gpassDepth);
//	glBindRenderbuffer(GL_RENDERBUFFER, m_gpassDepth);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,1280, 720);
//
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_albedoTexture, 0);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_positionTexture, 0);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_normalTexture, 0);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gpassDepth);
//
//	GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, gpassTargets);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	//set up light framebuffer
//	glGenFramebuffers(1, &m_lightFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
//
//	glGenTextures(1, &m_lightTexture);
//	glBindTexture(GL_TEXTURE_2D, m_lightTexture);
//
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightTexture, 0);
//
//	GLenum lightTargets[] = { GL_COLOR_ATTACHMENT0 };
//	glDrawBuffers(1, lightTargets);
//
//	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	if (status != GL_FRAMEBUFFER_COMPLETE)
//		printf("Framebuffer Error!\n");
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void Renderer::CreateFullScreenQuad()
{
	glm::vec2 halfTexel = 1.0f / glm::vec2(1280, 720) * 0.5f;

	float vertexData[] = {
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,

		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6,vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_VAOfullScreenQuad = m_VAO;

}

void Renderer::CreateCube()
{
	float vertexData[] = {
		-1, -1, 1, 1,
		1, -1, 1, 1,
		1, -1, -1, 1,
		-1, -1, -1, 1,
		-1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, -1, 1,
		-1, 1, -1, 1,
	};

	unsigned int indexData[] = {
		0, 5, 4,
		0, 1, 5,
		1, 6, 5,
		1, 2, 6,
		2, 7, 6,
		2, 3, 7,
		3, 4, 7,
		3, 0, 4,
		4, 6, 7,
		4, 5, 6,
		3, 1, 0,
		3, 2, 1
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 8, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);	

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_VAOCube = m_VAO;

	/*
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAdv) * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0) + 48);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0) + 16);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
}

void Renderer::GenerateTexturePlane()
{

	VertexAdv vertexData[] = {
		{ -5, 0, 5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 5, 0, 5, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 5, 0, -5, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -5, 0, -5, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
	};

	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAdv) * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0) + 48);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0) + 16);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAdv), ((char*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//test
	m_VAOtest = m_VAO;
	
	/*	m_meshArray->SetVAO(m_VAO);
	m_meshArray->SetIndexCount(6);
	m_meshArray->AddedMesh();
	m_meshArray->Iterate();*/

	

	//5721
}

void Renderer::GenerateTexturePlaneAlt()
{
	float vertexData[] = {
		-5, 0, -5, 1, 0, 0,
		5, 0, -5, 1, 1, 0,
		5, 10, -5, 1, 1, 1,
		-5, 10, -5, 1, 0, 1,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,	indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_VAOplane = m_VAO;
}

//void Renderer::GenerateProceduralPlane()
//{
//	//m_proceduralVAO
//
//	const int width = 65;
//	const int height = 65;
//
//	VertexUVs vertexData[width * height];
//
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			vertexData[y * width + x].position = vec4(x * 10, 0, y * 10, 1);
//			vertexData[y * width + x].colour = vec4(0, 0, 0, 1);
//			vertexData[y * width + x].normal = vec3(0, 1, 0);
//			vertexData[y * width + x].UV = vec2((float)x / (width - 1), (float)y / (height - 1));
//		}
//	}
//
//	unsigned int indexData[(width - 1) * (height - 1) * 6];
//
//	for (int y = 0; y < height - 1; y++)
//	{
//		for (int x = 0; x < width - 1; x++)
//		{
//			indexData[(y * (width - 1) + x) * 6] = x + width * y;					//0
//			indexData[(y * (width - 1) + x) * 6 + 1] = x + 1 + width * y;			//1
//			indexData[(y * (width - 1) + x) * 6 + 2] = x + 1 + width * (y + 1);	//2
//			indexData[(y * (width - 1) + x) * 6 + 3] = x + width * y;				//0
//			indexData[(y * (width - 1) + x) * 6 + 4] = x + 1 + width * (y + 1);	//2
//			indexData[(y * (width - 1) + x) * 6 + 5] = x + width * (y + 1);		//3
//		}
//	}
//
//	glGenVertexArrays(1, &m_VAO);
//	glBindVertexArray(m_VAO);
//
//	glGenBuffers(1, &m_VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVs) * width * height, vertexData, GL_STATIC_DRAW);
//
//	glGenBuffers(1, &m_IBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * (width - 1) * (height - 1), indexData, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
//	glEnableVertexAttribArray(3);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUVs), ((char*)0));
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUVs), ((char*)0) + 16);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexUVs), ((char*)0) + 32);
//	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUVs), ((char*)0) + 44);
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	m_proceduralVAO = m_VAO;
//
//	m_seed = 7.0f;
//
//	m_terrainLightDirection = vec3(0, 1, 0);
//
//	ResetProceduralPlane();
//}
//
//void Renderer::ResetProceduralPlane()
//{
//	int dims = 64;
//	m_perlinData = new float[dims * dims];
//	float scale = (1.0f / dims) * 3;
//	int octaves = 6;
//	
//	for (int x = 0; x < 64; ++x)
//	{
//		for (int y = 0; y < 64; ++y)
//		{
//			//m_perlinData[y * dims + x] = glm::perlin(vec2(x, y) * 0.1) * 0.5f + 0.5f;
//			float amplitude = 1.f;
//			float persistence = 0.3f;
//			m_perlinData[y * dims + x] = 0;
//
//			for (int o = 0; o < octaves; ++o)
//			{
//				float freq = powf(2, (float)o);
//				float perlin_sample = glm::perlin(vec3((float)x, (float)y, (float)m_seed) * scale * freq) * 0.5f + 0.5f;
//				m_perlinData[y * dims + x] += perlin_sample * amplitude;
//				amplitude *= persistence;
//			}
//		}
//	}
//
//	glGenTextures(1, &m_perlinTexture);
//	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, m_perlinData);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//}

void Renderer::AddAssetManager(AssetManager* assetManager)
{
	m_pAssetManager = assetManager;
}

void Renderer::AddEntityManager(EntityManager* entityManager)
{
	m_pEntityManager = entityManager;
}

void Renderer::AddCheckersManager(CheckersManager* checkersManager)
{
	m_pCheckersManager = checkersManager;
}