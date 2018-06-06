#include "MyGlWindow.h"
#include "SimpleShader.h"
#include "SimpleLight.h"
#include "LightBulbShader.h"
#include "TextureShader.h"
#include "NormalMapShader.h"
#include "SkyboxShader.h"
#include "HDRShader.h"
#include "Sphere.h"
#include "Cube.h"
#include "Color.h"
#include "global.h"

#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

static float DEFAULT_VIEW_POINT[3] = { 20, 20, 20 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


MyGlWindow::MyGlWindow(int w, int h, const char *modelName)
//==========================================================================
{

	m_width = w;
	m_height = h;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
	m_rotate = 0;


	initialize(modelName);

}

glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 R;

	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);


	return R * T;
}


glm::mat4 perspective(float fov, float aspect, float n, float f)
{
	glm::mat4 P(0.0f);

	const float tanHalfFOV = 1.0f / (tan(glm::radians(fov) / 2.0f));

	float A = -(n + f) / (f - n);
	float B = -(2 * (n * f)) / (f - n);

	P[0] = glm::vec4(tanHalfFOV / aspect, 0, 0, 0);
	P[1] = glm::vec4(0, tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);


	return P;

}

void MyGlWindow::draw(void)
{
	HDRShader *hdrShader = NULL;
	LightBulbShader *LBShader = static_cast<LightBulbShader *>(Shaders["LightBulb"]);
	AShader::SModelViewProjection mvp;

	if (Shaders.find("HDR") != Shaders.end() && global::UseHDR) {
		hdrShader = static_cast<HDRShader *>(Shaders["HDR"]);
		hdrShader->beforeDraw();
	}

	glViewport(0, 0, m_width, m_height);

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();

	mvp.view = lookAt(eye, look, up);
	mvp.projection = perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 500.0f);

	// RENDER SCENE HERE


	// Render Plane
	m_model.glPushMatrix();
	m_model.glTranslate(0, -1, 0);
	m_model.glScale(100, 0.1, 100);
	mvp.model = m_model.getMatrix();
	Shaders["Texture"]->use(Geometries["Cube"], &Lights, &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	m_model.glPopMatrix();

	// Render Walls

	m_model.glPushMatrix();
	m_model.glTranslate(0, 14, -100);
	m_model.glScale(100, 15, 1);
	mvp.model = m_model.getMatrix();
	Shaders["Texture"]->use(Geometries["Cube"], &Lights, &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	m_model.glPopMatrix();
	m_model.glPushMatrix();
	m_model.glTranslate(0, 14, 100);
	m_model.glScale(100, 15, 1);
	mvp.model = m_model.getMatrix();
	Shaders["Texture"]->use(Geometries["Cube"], &Lights, &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	m_model.glPopMatrix();
	m_model.glPushMatrix();
	m_model.glTranslate(-100, 14, 0);
	m_model.glScale(1, 15, 100);
	mvp.model = m_model.getMatrix();
	Shaders["Texture"]->use(Geometries["Cube"], &Lights, &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	m_model.glPopMatrix();
	m_model.glPushMatrix();
	m_model.glTranslate(100, 14, 0);
	m_model.glScale(1, 15, 100);
	mvp.model = m_model.getMatrix();
	Shaders["Texture"]->use(Geometries["Cube"], &Lights, &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	m_model.glPopMatrix();

	// Render Light Bulbs

	// White Light
	m_model.glPushMatrix();
	m_model.glTranslate(0, 7, 0);
	m_model.glScale(0.5, 0.5, 0.5);
	mvp.model = m_model.getMatrix();
	LBShader->setLight("White");
	Shaders["LightBulb"]->use(Geometries["Cube"], &Lights, &mvp);
	m_model.glPopMatrix();

	// Red Light
	m_model.glPushMatrix();
	m_model.glTranslate(50, 5, 50);
	m_model.glScale(0.5, 0.5, 0.5);
	mvp.model = m_model.getMatrix();
	LBShader->setLight("Red");
	Shaders["LightBulb"]->use(Geometries["Cube"], &Lights, &mvp);
	m_model.glPopMatrix();

	// Blue Ligth
	m_model.glPushMatrix();
	m_model.glTranslate(-50, 5, -50);
	m_model.glScale(0.5, 0.5, 0.5);
	mvp.model = m_model.getMatrix();
	LBShader->setLight("Blue");
	Shaders["LightBulb"]->use(Geometries["Cube"], &Lights, &mvp);
	m_model.glPopMatrix();

	// Green Light
	m_model.glPushMatrix();
	m_model.glTranslate(50, 5, -50);
	m_model.glScale(0.5, 0.5, 0.5);
	mvp.model = m_model.getMatrix();
	LBShader->setLight("Green");
	Shaders["LightBulb"]->use(Geometries["Cube"], &Lights, &mvp);
	m_model.glPopMatrix();

	// Yellow Light
	m_model.glPushMatrix();
	m_model.glTranslate(-50, 5, 50);
	m_model.glScale(0.5, 0.5, 0.5);
	mvp.model = m_model.getMatrix();
	LBShader->setLight("Yellow");
	Shaders["LightBulb"]->use(Geometries["Cube"], &Lights, &mvp);
	m_model.glPopMatrix();

	if (hdrShader && global::UseHDR) {
		hdrShader->use();
	}
}

MyGlWindow::~MyGlWindow()
{
	delete skybox;
	for (auto &i : Geometries) {
		delete i.second;
	}
	for (auto &i : Shaders) {
		delete i.second;
	}
	for (auto &i : Lights) {
		delete i.second;
	}
	for (auto &i : Models) {
		delete i.second;
	}
	for (auto &i : Textures) {
		delete i.second;
	}
}


void MyGlWindow::initialize(const char *modelName)
{
	SimpleShader *Shader = new SimpleShader();
	HDRShader *HShader = new HDRShader(m_width, m_height);
	TextureShader *TShader = new TextureShader();
	LightBulbShader *LBShader = new LightBulbShader();
	Shaders["HDR"] = HShader;
	Shaders["Texture"] = TShader;
	Shaders["LightBulb"] = LBShader;
	Shaders["Simple"] = Shader;

	SColor lightColor;

	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = { 200, 200, 200 };
	SimpleLight *Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(0, 5, 0, 1));
	Lights["White"] = Light;

	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = { 200, 0, 0 };
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(50, 5, 50, 1));
	Lights["Red"] = Light;

	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = { 0, 0, 200 };
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(-50, 5, -50, 1));
	Lights["Blue"] = Light;

	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = { 0, 200, 0 };
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(50, 5, -50, 1));
	Lights["Green"] = Light;

	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = { 200, 200, 0 };
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(-50, 5, 50, 1));
	Lights["Yellow"] = Light;

	AGeometry::MaterialInfo infos;
	infos.Alpha = 1.0;
	infos.color = PremadeColor::White;

	Texture *texture = new Texture("container2.png", "", Texture::ETextureType::DIFFUSE);
	Textures["container2.png"] = texture;

	texture = new Texture("container2_specular.png", "", Texture::ETextureType::SPECULAR);
	Textures["container2_specular.png"] = texture;

	texture = new Texture("wood.jpg", "", Texture::ETextureType::DIFFUSE);
	Textures["wood.jpg"] = texture;

	Sphere *sphere = new Sphere(1, 60, 60, infos);
	Geometries["Sphere"] = sphere;

	Cube *cube = new Cube(infos);
	Geometries["Cube"] = cube;
}