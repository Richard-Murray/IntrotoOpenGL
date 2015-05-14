#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void Update(float deltaTime);
	void Draw(BaseCamera* camera);

	void CreateEntity(const char* idName);
	Entity* GetEntity(const char* idName);
	Entity* GetNewEntity();

private:
	std::vector<Entity*> m_entityList;
};


#endif