#include "EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

void EntityManager::Update(float deltaTime)
{
	for (auto iter = m_entityList.begin(); iter != m_entityList.end(); iter++)
	{
		(*iter)->Update(deltaTime);
	}
}

void EntityManager::Draw(BaseCamera* camera)
{
	for (auto iter = m_entityList.begin(); iter != m_entityList.end(); iter++)
	{
		(*iter)->Draw(camera);
	}
}

void EntityManager::CreateEntity(const char* idName)
{
	m_entityList.push_back(new Entity(idName));
}

Entity* EntityManager::GetEntity(const char* idName)
{
	for (auto iter = m_entityList.begin(); iter != m_entityList.end(); iter++)
	{
		if ((*iter)->GetID() == idName)
			return (*iter);
	}

	return nullptr;
}

Entity* EntityManager::GetNewEntity()
{
	return m_entityList.back();
}