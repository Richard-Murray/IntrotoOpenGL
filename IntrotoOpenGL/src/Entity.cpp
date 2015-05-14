#include "Entity.h"

Entity::Entity(const char* idName)
{
	m_worldTransform = glm::mat4(1);
	m_idName = idName;
}

Entity::~Entity()
{

}

void Entity::AttachModel(Model* pModel)
{
	m_model = pModel;
	m_attachedModel = true;
}

void Entity::AttachShader(unsigned int shader)
{
	m_shaderProgram = shader;
	m_attachedShader = true;
}

void Entity::AttachTexture(unsigned int diffuseTexture)
{
	m_diffuseTexture = diffuseTexture;
	m_attachedDiffuse = true;
}

void Entity::Update(float deltaTime)
{

}

void Entity::Draw(BaseCamera* camera)
{
	if (m_attachedModel == true && m_attachedShader == true)
	{
		glUseProgram(m_shaderProgram);

		unsigned int projectionViewUniform = glGetUniformLocation(m_shaderProgram, "ProjectionView");
		unsigned int viewUniform = glGetUniformLocation(m_shaderProgram, "View");
		unsigned int worldTransformUniform = glGetUniformLocation(m_shaderProgram, "WorldTransform");

		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &(camera->GetView()[0][0]));
		glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, &(camera->GetProjectionView()[0][0]));
		glUniformMatrix4fv(worldTransformUniform, 1, GL_FALSE, &(m_worldTransform[0][0]));

		m_model->DrawModel();
	}
}

void Entity::TestMoveTransform(glm::mat4 transform)
{
	m_worldTransform = m_worldTransform * transform;
}

const char* Entity::GetID()
{
	return m_idName;
}