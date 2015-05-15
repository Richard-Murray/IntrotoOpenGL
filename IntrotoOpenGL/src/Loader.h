#ifndef LOADER_H
#define LOADER_H

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "EntityManager.h"
#include "AssetManager.h"

class Loader
{
public:
	Loader();
	~Loader();

	Model* FindModel(const char* idName);
	unsigned int FindTexture(const char* idName);
	unsigned int FindShader(const char* idName);

private:
	EntityManager* m_pEntityManager;
	AssetManager* m_pAssetManager;
};

#endif