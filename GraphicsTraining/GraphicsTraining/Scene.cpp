#include "Scene.h"

#include <iostream>

#include <GL/glew.h>

#include "Defs.h"
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "ComplexMaterial.h"
#include "Model.h"
#include "ModelLoader.h"


Scene::Scene(const char* name) : name(name)
{
	std::cout << "\tSCENE[" << this->name << "]: Creation." << std::endl;

	for (int i = 0; i < MAX_CAMERAS; ++i)
		cameras[i] = nullptr;
}


Scene::~Scene()
{
	std::cout << "\tSCENE: Destruction." << std::endl;
}

void Scene::Init()
{
	std::cout << "SCENE[" << name << "]: Init." << std::endl;

	OnInit();

	if(!defaultMaterial) 
		defaultMaterial = resourceManager->defaultResources.simpleMat;
}

void Scene::CleanUp()
{
	std::cout << "SCENE[" << name << "]: Clean up." << std::endl;

	OnCleanUp();

	resourceManager->RemoveAllResources();
	models.clear();
	defaultMaterial = nullptr;
}

void Scene::AddModel(Model * model)
{
	if (model) models.push_back(model);
}

void Scene::RemoveModel(const char * name)
{
	if(name)
	{
		for(auto it = models.begin(); it != models.end();)
		{
			if ((*it)->GetName() == name)
			{
				it = models.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void Scene::AddCamera(Camera * cam, int index, bool setAsActive)
{
	if(index >= 0 && index < MAX_CAMERAS)
	{
		cameras[index] = cam;
		if (setAsActive)
		{
			activeCamera = cam;
			OnActiveCameraChanged();
		}
	}
}

void Scene::RemoveCamera(int index)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		cameras[index] = nullptr;
	}
}

void Scene::SetActiveCamera(int index)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		if (cameras[index])
		{
			activeCamera = cameras[index];
			OnActiveCameraChanged();
		}
	}
}

void Scene::OnResize(int winW, int winH)
{
	viewportWidth = winW; viewportHeight = winH;

	for (int i = 0; i < MAX_CAMERAS; ++i)
		if (cameras[i]) cameras[i]->ResizeViewport(winW, winH);
}

Material * Scene::GetDefaultMaterial() const
{
	return defaultMaterial;
}

void Scene::RenderScene()
{
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (GetActiveCamera()) OnRenderScene();
}

void Scene::ProcessScroll(double yoffset)
{
	if (activeCamera) activeCamera->ProcessMouseScroll(yoffset);
}

void Scene::ProcessMouseMovement(double xoffset, double yoffset)
{
	if (activeCamera) activeCamera->ProcessMouseMovement(xoffset, yoffset);
}

void Scene::ProcessInput(Camera_Movement movement, float dt)
{
	if (activeCamera) activeCamera->ProcessKeyboard(movement, dt);
}
