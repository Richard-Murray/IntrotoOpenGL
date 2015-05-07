#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include <vector>

#include "Renderer.h"
#include "Model.h"

class ModelManager
{
public:
	ModelManager(Renderer* pRender);
	~ModelManager();

	void AddModel(const char* idName, const char* path);
	Model* GetModel(const char* idName);
	bool DoesModelExist(const char* idName);

private:
	std::vector<Model*> m_modelList;

	Renderer* m_pRender;
};

#endif