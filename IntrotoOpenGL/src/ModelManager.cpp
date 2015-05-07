#include "ModelManager.h"

ModelManager::ModelManager(Renderer* pRender)
{
	m_pRender = pRender;
}

ModelManager::~ModelManager()
{

}

void ModelManager::AddModel(const char* idName, const char* path)
{
	if (!DoesModelExist(idName))
	{
		m_modelList.push_back(new Model());
		m_modelList.back()->LoadModel(idName, path, m_pRender);
	}
}

Model* ModelManager::GetModel(const char* idName)
{
	if (DoesModelExist(idName))
	{
		for (auto iter = m_modelList.begin(); iter != m_modelList.end(); iter++)
		{
			if ((*iter)->GetID() == idName)
			{
				return (*iter);
			}
		}
		//search and return model
	}
	else
	{
		return nullptr;
	}
}

bool ModelManager::DoesModelExist(const char* idName)
{
	for (auto iter = m_modelList.begin(); iter != m_modelList.end(); iter++)
	{
		if ((*iter)->GetID() == idName)
		{
			return true;
		}
	}
	return false;
}