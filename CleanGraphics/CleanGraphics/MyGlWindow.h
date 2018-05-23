#pragma once
//#define  FREEGLUT_LIB_PRAGMAS  0



#pragma warning(push)
#pragma warning(disable:4311)		// convert void* to long
#pragma warning(disable:4312)		// convert long to void*

#include <iostream>
#include <GL/glew.h>
#include <string>
#include <stack>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Viewer.h"
#include "ModelView.h"
#include "AShader.h"
#include "AGeometry.h"
#include "ALight.h"
#include "LoadedModel.h"

#pragma warning(pop)


class MyGlWindow {
public:
	MyGlWindow(int w, int h, const char *modelName);
	~MyGlWindow();
	void draw();
	void setSize(int w, int h) { m_width = w; m_height = h; }
	void setAspect(float r) { m_viewer->setAspectRatio(r); }
	Viewer *m_viewer;
	float m_rotate;
private:
	int m_width;
	int m_height;
	void initialize(const char *);

	Model m_model;
	AShader *Shader;
	Skybox *skybox;
	std::vector<AGeometry *> Geometries;
	std::vector<LoadedModel *> Models;
	std::map<const char *, Texture *> Textures;
	ALight *Light;
};