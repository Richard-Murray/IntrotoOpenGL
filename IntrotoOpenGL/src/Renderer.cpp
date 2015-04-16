#include "Renderer.h"

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
	delete m_targetCamera;

	delete m_pLightingRenderTarget;
	delete m_pDefferedRenderTarget;

	delete m_FBX;
	delete m_gpuParticleEmitter;


	glDeleteProgram(m_programObjID);
	glDeleteProgram(m_programTexturePlaneID);

	CleanupOpenGLBuffers(m_FBX);
}

void Renderer::Update(float deltaTime)
{
	float s = glm::cos((float)glfwGetTime()) * 0.5f + 0.5f;


	//All stuff for test skeleton
	/*glm::vec3 p = (1.0f - s) * m_hipFrames[0].position +
		s * m_hipFrames[1].position;

	glm::quat r = glm::slerp(m_hipFrames[0].rotation,
		m_hipFrames[1].rotation, s);

	m_hipBone = glm::translate(p) * glm::toMat4(r);

	p = (1.0f - s) * m_kneeFrames[0].position +
		s * m_kneeFrames[1].position;

	r = glm::slerp(m_kneeFrames[0].rotation,
		m_kneeFrames[1].rotation, s);

	m_kneeBone = m_hipBone * glm::translate(p) * glm::toMat4(r);

	p = (1.0f - s) * m_ankleFrames[0].position +
		s * m_ankleFrames[1].position;

	r = glm::slerp(m_ankleFrames[0].rotation,
		m_ankleFrames[1].rotation, s);

	m_ankleBone = m_kneeBone * glm::translate(p) * glm::toMat4(r);

	glm::vec3 hipPos = glm::vec3(m_hipBone[3].x, m_hipBone[3].y, m_hipBone[3].z);
	glm::vec3 kneePos = glm::vec3(m_kneeBone[3].x, m_kneeBone[3].y, m_kneeBone[3].z);
	glm::vec3 anklePos = glm::vec3(m_ankleBone[3].x, m_ankleBone[3].y, m_ankleBone[3].z);

	glm::vec3 half(0.5f);
	glm::vec4 pink(1, 0, 1, 1);

	Gizmos::addAABBFilled(hipPos, half, pink, &m_hipBone);
	Gizmos::addAABBFilled(kneePos, half, pink, &m_kneeBone);
	Gizmos::addAABBFilled(anklePos, half, pink, &m_ankleBone);*/

	//SKELETON STUFF
	FBXSkeleton* skeleton = m_FBX->getSkeletonByIndex(0);
	FBXAnimation* animation = m_FBX->getAnimationByIndex(0);

	skeleton->evaluate(animation, glfwGetTime());

	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	{
		skeleton->m_nodes[bone_index]->updateGlobalTransform();
	}

	//m_particleEmitter->Update(deltaTime, m_camera->GetTransform());
	m_gpuParticleEmitter->Update(deltaTime);

	//Render target code
	//Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));

	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)
	{
		m_targetCamera->SetTransform(m_camera->GetTransform());
	}
}

void Renderer::Draw()
{
	////Regular drawing
	m_pLightingRenderTarget->SetAsActiveRenderTarget();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	DrawLighting(m_targetCamera);
	
	m_pDefferedRenderTarget->SetAsActiveRenderTarget();
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawScene(m_targetCamera);
	//Gizmos::draw(m_targetCamera->GetProjectionView());

	m_pDefferedRenderTarget->ClearAsActiveRenderTarget();
	glClearColor(0.55f, 0.55f, 0.55f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programTexturePlaneSimpleID);
	
	int loc = glGetUniformLocation(m_programTexturePlaneSimpleID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pDefferedRenderTarget->GetRenderTexture(0));
	glUniform1i(glGetUniformLocation(m_programTexturePlaneSimpleID, "diffuse"), 0);
	
	glBindVertexArray(m_VAOplane);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	DrawScene(m_camera);

	////Deferred rendering drawing

	////Post process drawing
	//// bind our target
	//glBindFramebuffer(GL_FRAMEBUFFER, m_pPostProcessRenderTarget->GetFBO());
	//glViewport(0, 0, 1280, 720);

	//// clear the target
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// draw our 3D scene
	//DrawScene(m_camera);
	//// gizmos for now
	//Gizmos::draw(m_camera->GetProjectionView());

	//// bind the back-buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 1280, 720);

	//// just clear the back-buffer depth as
	//// each pixel will be filled
	//glClear(GL_DEPTH_BUFFER_BIT);

	//// draw out full-screen quad
	//glUseProgram(m_programPostProcessID);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_pPostProcessRenderTarget->GetRenderTexture(0));

	//int loc = glGetUniformLocation(m_programPostProcessID, "target");
	//glUniform1i(loc, 0);

	//glBindVertexArray(m_VAOfullScreenQuad);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer::DrawLighting(BaseCamera* camera)
{
	glUseProgram(m_programShadowMapID);

	//bind light matrix
	int loc = glGetUniformLocation(m_programShadowMapID, "LightMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_lightMatrix[0][0]));

	//draw all shadow-casting geometry
	for (unsigned int i = 0; i < m_FBXBunny->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_FBXBunny->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Renderer::DrawScene(BaseCamera* camera)
{
	//REGULAR DRAWING

	
	//Shadowing stuff
	//----------
	glUseProgram(m_programShadowMeshID);

	//bind the camera
	int loc = glGetUniformLocation(m_programShadowMeshID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));

	//bind light matrix
	glm::mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;

	float shadowBias = 0.01f;

	loc = glGetUniformLocation(m_programShadowMeshID, "LightMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightMatrix[0][0]));

	loc = glGetUniformLocation(m_programShadowMeshID, "LightDir");
	glUniform3fv(loc, 1, &m_lightDir[0]);

	loc = glGetUniformLocation(m_programShadowMeshID, "shadowMap");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(m_programShadowMeshID, "shadowBias");
	glUniform1f(loc, shadowBias);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pLightingRenderTarget->GetDepthTexture());

	//draw all shadow-recieving
	for (unsigned int i = 0; i < m_FBXBunny->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_FBXBunny->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}


	//----------


	//Hardcoded test values
	vec3 lightDirection = vec3(-0.5f, 0.5f/*glfwGetTime() * 0.1f*/, 0.5f);
	vec3 lightColour = vec3(1.0f, 0.7f, 0.0f);
	float specularPower = 1.0f;
	FBXSkeleton* skeleton = m_FBX->getSkeletonByIndex(0);
	skeleton->updateBones();
	
	glUseProgram(m_programObjID);
	unsigned int viewProjectionUniform = glGetUniformLocation(m_programObjID, "ProjectionView");
	unsigned int lightDirectionUniform = glGetUniformLocation(m_programObjID, "LightDirection");
	unsigned int lightColourUniform = glGetUniformLocation(m_programObjID, "LightColour");
	unsigned int cameraPositionUniform = glGetUniformLocation(m_programObjID, "CameraPosition");
	unsigned int specularPowerUniform = glGetUniformLocation(m_programObjID, "SpecularPower");
	unsigned int bonesUniform = glGetUniformLocation(m_programObjID, "Bones");
	
	glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionView()));
	glUniform3fv(lightDirectionUniform, 1, glm::value_ptr(lightDirection));
	glUniform3fv(lightColourUniform, 1, glm::value_ptr(lightColour));
	glUniform3fv(cameraPositionUniform, 1, glm::value_ptr(camera->GetPosition()));
	glUniform1f(specularPowerUniform, specularPower);
	glUniformMatrix4fv(bonesUniform, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);
	
	int iDiffuseLoc = glGetUniformLocation(m_programObjID, "Diffuse");
	glUniform1i(iDiffuseLoc, 0);
	
	for (unsigned int i = 0; i < m_FBX->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_FBX->getMeshByIndex(i);
	
		unsigned int* glData = (unsigned int*)mesh->m_userData;
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->m_material->textures[FBXMaterial::DiffuseTexture]->handle);
	
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	//Textured plane
	//----------
	glUseProgram(m_programTexturePlaneID);
	
	//set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalMap);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_pLightingRenderTarget->GetDepthTexture());

	//bind camera
	loc = glGetUniformLocation(m_programTexturePlaneID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));

	loc = glGetUniformLocation(m_programTexturePlaneID, "LightMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightMatrix[0][0]));

	loc = glGetUniformLocation(m_programTexturePlaneID, "shadowMap");
	glUniform1i(loc, 2);

	loc = glGetUniformLocation(m_programTexturePlaneID, "shadowBias");
	glUniform1f(loc, shadowBias);


	//tell shader where it is
	loc = glGetUniformLocation(m_programTexturePlaneID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_programTexturePlaneID, "normal");
	glUniform1i(loc, 1);

	//bind light
	vec3 light(m_lightDir);
	loc = glGetUniformLocation(m_programTexturePlaneID, "LightDirection");
	glUniform3f(loc, light.x, light.y, light.z);

	//draw
	glBindVertexArray(m_VAOtest);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//----------

	m_gpuParticleEmitter->Draw((float)glfwGetTime(), camera->GetTransform(), camera->GetProjectionView());

	
}

//void Renderer::DrawGeometryPass(BaseCamera* camera)
//{
//	// G-Pass: render out the albedo, position and normal
//	glEnable(GL_DEPTH_TEST);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO);
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glUseProgram(m_gpassShader);
//	// bind camera transforms
//	int loc = glGetUniformLocation(m_gpassShader, "ProjectionView");
//	glUniformMatrix4fv(loc, 1, GL_FALSE,
//		&(m_camera->getProjectionView()[0][0]));
//	loc = glGetUniformLocation(m_gpassShader, "View");
//	glUniformMatrix4fv(loc, 1, GL_FALSE,
//		&(m_camera->getView()[0][0]));
//	// draw our scene, in this example just the Stanford Bunny
//	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {
//		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//		glBindVertexArray(glData[0]);
//		glDrawElements(GL_TRIANGLES,
//			(unsigned int)mesh->m_indices.size(),
//			GL_UNSIGNED_INT, 0);
//	}
//}
//
//void Renderer::DrawLightPass(BaseCamera* camera)
//{}
//
//void Renderer::DrawCompositePass(BaseCamera* camera)
//{}

//void Renderer::DrawDirectionalLight(const glm::vec3& direction, const glm::vec3& diffuse)
//{
//	glm::vec4 viewSpaceLight = m_camera->GetView() *glm::vec4(glm::normalize(direction), 0);
//
//	int loc = glGetUniformLocation(m_programDirectionalLightID, "lightDirection");
//	glUniform3fv(loc, 1, &viewSpaceLight[0]);
//
//	loc = glGetUniformLocation(m_programDirectionalLightID, "lightDiffuse");
//	glUniform3fv(loc, 1, &diffuse[0]);
//
//	glBindVertexArray(m_quadVAO);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//}

//UNUSED
void Renderer::DrawDeferredRenderingContent(BaseCamera* camera)
{
	//// G-Pass: render out the albedo, position and normal
	//glEnable(GL_DEPTH_TEST);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO);
	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(m_programGBufferID);
	//// bind camera transforms
	//int loc = glGetUniformLocation(m_programGBufferID, "ProjectionView");
	//glUniformMatrix4fv(loc, 1, GL_FALSE,
	//	&(m_camera->GetProjectionView()[0][0]));
	//loc = glGetUniformLocation(m_programGBufferID, "View");
	//glUniformMatrix4fv(loc, 1, GL_FALSE,
	//	&(m_camera->GetView()[0][0]));
	//// draw our scene, in this example just the Stanford Bunny
	//for (unsigned int i = 0; i < m_FBXBunny->getMeshCount(); ++i) {
	//	FBXMeshNode* mesh = m_FBXBunny->getMeshByIndex(i);
	//	unsigned int* glData = (unsigned int*)mesh->m_userData;
	//	glBindVertexArray(glData[0]);
	//	glDrawElements(GL_TRIANGLES,
	//		(unsigned int)mesh->m_indices.size(),
	//		GL_UNSIGNED_INT, 0);
	//}







}

void Renderer::Load()
{
	CreateShader(m_programObjID, "./data/shaders/obj.vert", "./data/shaders/obj.frag");
	CreateShader(m_programTexturePlaneID, "./data/shaders/texPlane.vert", "./data/shaders/texPlane.frag");
	CreateShader(m_programTexturePlaneSimpleID, "./data/shaders/texPlaneSimple.vert", "./data/shaders/texPlaneSimple.frag");

	CreateShader(m_programShadowMeshID, "./data/shaders/shadowMesh.vert", "./data/shaders/shadowMesh.frag");
	CreateShader(m_programShadowMapID, "./data/shaders/shadowMap.vert", "./data/shaders/shadowMap.frag");

	CreateShader(m_programGBufferID, "./data/shaders/deferredGbuffer.vert", "./data/shaders/deferredGbuffer.frag");
	CreateShader(m_programDirectionalLightID, "./data/shaders/deferredDirectionalLight.vert", "./data/shaders/deferredDirectionalLight.frag");
	CreateShader(m_programCompositeID, "./data/shaders/deferredComposite.vert", "./data/shaders/deferredComposite.frag");

	CreateShader(m_programPostProcessID, "./data/shaders/postProcess.vert", "./data/shaders/postProcess.frag");

	//m_meshArray = new MeshArray();
	GenerateTexturePlane();

	m_gpuParticleEmitter = new GPUParticleEmitter();
	m_gpuParticleEmitter->Initialise(100000, 0.1f, 5.0f, 5, 20, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	m_FBX = new FBXFile();
	m_FBX->load("data/characters/Pyro/pyro.fbx");
	m_FBX->initialiseOpenGLTextures();
	CreateOpenGLBuffers(m_FBX);

	//Textured plane
	//----------
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	//load diffuse map
	unsigned char* data = stbi_load("./data/textures/rock_diffuse.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB /*REMEMBER RGBA*/, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	//load normal map
	data = stbi_load("./data/textures/rock_normal.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_normalMap);
	glBindTexture(GL_TEXTURE_2D, m_normalMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
	//----------
	
	//Test content for gizmos animation
	//----------
	/*m_positions[0] = glm::vec3(10, 5, 10);
	m_positions[1] = glm::vec3(-10, 0, -10);
	m_positions[2] = glm::vec3(-10, 10, -10);
	m_positions[3] = glm::vec3(0, 0, 40);
	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));
	m_rotations[2] = glm::quat(glm::vec3(0, 0, 1));
	m_rotations[3] = glm::quat(glm::vec3(1, 0, 0));
	anim = 0;
	anim2 = 1;
	animCountdown = 0.0f;

	m_hipFrames[0].position = glm::vec3(0, 5, 0);
	m_hipFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_hipFrames[1].position = glm::vec3(0, 5, 0);
	m_hipFrames[1].rotation = glm::quat(glm::vec3(-1, 0, 0));

	m_kneeFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_kneeFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_ankleFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_ankleFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));*/
	//----------

	//Render target loading
	//----------
	m_pDefferedRenderTarget = new RenderTarget();
	m_pDefferedRenderTarget->SetSize(1024, 1024);
	m_pDefferedRenderTarget->Initialise();
	m_pDefferedRenderTarget->AttachColourBuffer(0, GL_RGBA8);
	m_pDefferedRenderTarget->AttachDepthBuffer();
	m_pDefferedRenderTarget->SetDrawBuffers();

	m_pLightingRenderTarget = new RenderTarget();
	m_pLightingRenderTarget->SetSize(1024, 1024);
	m_pLightingRenderTarget->Initialise();
	m_pLightingRenderTarget->AttachDepthBuffer(GL_DEPTH_COMPONENT16, true);
	m_pLightingRenderTarget->SetDrawBuffers();

	GenerateTexturePlaneAlt();

	m_pGeometryPassRenderTarget = new RenderTarget();
	m_pGeometryPassRenderTarget->SetSize(1280, 720);
	m_pGeometryPassRenderTarget->Initialise();
	m_pGeometryPassRenderTarget->AttachColourBuffer(0, GL_RGB8); //albedo
	m_pGeometryPassRenderTarget->AttachColourBuffer(1, GL_RGB32F); //position
	m_pGeometryPassRenderTarget->AttachColourBuffer(2, GL_RGB32F); //normal
	m_pGeometryPassRenderTarget->AttachDepthBuffer();
	m_pGeometryPassRenderTarget->SetDrawBuffers();

	m_pLightBufferRenderTarget = new RenderTarget();
	m_pLightBufferRenderTarget->SetSize(1280, 720);
	m_pLightBufferRenderTarget->AttachColourBuffer(0, GL_RGB8);
	m_pLightBufferRenderTarget->SetDrawBuffers();

	m_pPostProcessRenderTarget = new RenderTarget();
	m_pPostProcessRenderTarget->SetSize(1280, 720);
	m_pPostProcessRenderTarget->AttachColourBuffer(0, GL_RGBA8);
	m_pPostProcessRenderTarget->AttachDepthBuffer();
	m_pPostProcessRenderTarget->SetDrawBuffers();

	m_targetCamera = new BaseCamera(vec3(5, 5, 5));
	m_targetCamera->SetUpPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 10000.f);

	CreateFullScreenQuad();
	//----------

	//shadow maps
	//----------
	m_FBXBunny = new FBXFile();
	m_FBXBunny->load("./data/models/Bunny.fbx");
//	m_FBXBunny->initialiseOpenGLTextures();
	CreateOpenGLBuffers(m_FBXBunny);
	
	m_lightDir = glm::normalize(glm::vec3(1, 2.5f, 1));

	glm::mat4 lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 lightView = glm::lookAt(m_lightDir, glm::vec3(0), glm::vec3(0, 1, 0));

	m_lightMatrix = lightProjection * lightView;

	/*glm::mat4 textureSpaceOffset(
	0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f);

	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;*/
	//----------

	//deferred rendering
	//----------
	CreateDeferredRenderingBuffers();

	//----------

	//GUI
	//----------
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	glfwSetMouseButtonCallback(m_window, OnMouseButton);
	glfwSetCursorPosCallback(m_window, OnMousePosition);
	glfwSetScrollCallback(m_window, OnMouseScroll);
	glfwSetKeyCallback(m_window, OnKey);
	glfwSetCharCallback(m_window, OnChar);
	glfwSetWindowSizeCallback(m_window, OnWindowResize);

	//----------

	//Procedural gen
	//----------
	GenerateProceduralPlane();
	
	//----------
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
	//
	//unsigned int shader;

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
		glEnableVertexAttribArray(3); //tangents
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

void Renderer::CreateDeferredRenderingBuffers()
{
	//setup gpass framebuffer
	glGenFramebuffers(1, &m_gpassFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO);

	glGenTextures(1, &m_albedoTexture);
	glBindTexture(GL_TEXTURE_2D, m_albedoTexture);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &m_positionTexture);
	glBindTexture(GL_TEXTURE_2D, m_positionTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &m_normalTexture);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &m_gpassDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gpassDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,1280, 720);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_albedoTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_positionTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_normalTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gpassDepth);

	GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, gpassTargets);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//set up light framebuffer
	glGenFramebuffers(1, &m_lightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	glGenTextures(1, &m_lightTexture);
	glBindTexture(GL_TEXTURE_2D, m_lightTexture);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightTexture, 0);

	GLenum lightTargets[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightTargets);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

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

void Renderer::GenerateProceduralPlane()
{
	//m_proceduralVAO

	/*glGenVertexArrays(1, &m_VAO);
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

	m_proceduralVAO = m_VAO;*/

	int dims = 64;
	m_perlinData = new float[dims * dims];

	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			m_perlinData[y * dims + x] = glm::perlin(glm::vec2(x, y) * 10) * 0.5f + 0.5f;
		}
	}

	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, m_perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}