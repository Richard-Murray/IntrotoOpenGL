#ifndef ENTITY_H
#define ENTITY_H

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Model.h"

class Entity
{
public:
	Entity(const char* idName);
	~Entity();

	void AttachModel(Model* pModel);
	void AttachShader(unsigned int shader);
	void AttachTexture(unsigned int diffuseTexture);
	void Update(float deltaTime);
	void Draw(BaseCamera* camera);

	void TestMoveTransform(glm::mat4 transform);

	const char* GetID();

private:
	glm::mat4 m_worldTransform;

	const char* m_idName;
	
	Model* m_model;
	unsigned int m_shaderProgram;
	unsigned int m_diffuseTexture;

	bool m_attachedModel = false;
	bool m_attachedShader = false;
	bool m_attachedDiffuse = false;
};

#endif