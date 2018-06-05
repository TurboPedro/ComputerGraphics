#include "MyGlWindow.h"
#include "SimpleShader.h"
#include "SimpleLight.h"
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

static float DEFAULT_VIEW_POINT[3] = { 0, 0, -5 };
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
	AShader::SModelViewProjection mvp;

	if (Shaders.find("HDR") != Shaders.end() && global::useHDR) {
		hdrShader = static_cast<HDRShader *>(Shaders["HDR"]);
		hdrShader->beforeDraw();
	}

//	Lights["SimpleBigLight"]->SetPosition(glm::vec4(global::lightPosition, 1.0));
//	Lights["SimpleBigLight"]->SetAttenuation({ 1.0, global::Linear, global::Quadratic });

	glViewport(0, 0, m_width, m_height);

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();

	mvp.view = lookAt(eye, look, up);
	mvp.projection = perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 500.0f);

	// Attenuation scene :

	//m_model.glPushMatrix();
	//m_model.glScale(0.5, 0.5, 0.5);
	//m_model.glPushMatrix();
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(2, 5, -15);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(-1.5f, -2.2f, -2.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(-3.8f, -2.0f, -12.3f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(2.4f, -0.4f, -3.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(-1.7f, 3.0f, -7.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(1.3f, -2.0f, -2.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(1.5f, 2.0f, -2.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(1.5f, 0.2f, -1.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();
	//m_model.glPushMatrix();
	//m_model.glTranslate(-1.3f, 1.0f, -1.5f);
	//mvp.model = m_model.getMatrix();
	//Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBigLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	//m_model.glPopMatrix();

	// HDR Scene LightBulb

	//m_model.glPushMatrix();
	//m_model.glTranslate(0, 0, 104);
	//m_model.glScale(0.1, 0.1, 0.1);
	//mvp.model = m_model.getMatrix();
	//Shaders["Simple"]->use(Geometries["Sphere"], Lights["SimpleBigLight"], &mvp);
	//m_model.glPopMatrix();

	// HDR Scene

	m_model.glPushMatrix();
	m_model.glTranslate(0, 0, 50);
	m_model.glScale(5, 5, 55);
	mvp.model = m_model.getMatrix();
	Shaders["Texture"]->use(Geometries["Cube"], &Lights, &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
//	Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleRedLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
//	Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleBlueLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
//	Shaders["Texture"]->use(Geometries["Cube"], Lights["SimpleGreenLight"], &mvp, Textures["container2.png"], Textures["container2_specular.png"]);
	m_model.glPopMatrix();

	if (hdrShader && global::useHDR) {
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
	HDRShader *HShader = new HDRShader(m_width - 100, m_height - 100);
	SimpleShader *Shader = new SimpleShader;
	TextureShader *TShader = new TextureShader();
	Shaders["HDR"] = HShader;
	Shaders["Texture"] = TShader;
	Shaders["Simple"] = Shader;

	SColor lightColor;

	glm::vec3 intensity = { 200, 200, 200};
//	lightColor.Ambient = { 0.2, 0.2, 0.2 };
//	lightColor.Diffuse = { 0.5, 0.5, 0.5 };
//	lightColor.Specular = { 1, 1, 1 };
	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = intensity;
	SimpleLight *Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(0, 0, 104, 1));
	Lights["SimpleBigLight"] = Light;

	intensity = { .9, .0, .0 };
	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = intensity;
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(-1.4, -1.9, 9.0, 1));
	Lights["SimpleRedLight"] = Light;

	intensity = { .0, .0, 1.8 };
	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = intensity;
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(0, -1.8, 4.0, 1));
	Lights["SimpleBlueLight"] = Light;

	intensity = { .0, .9, .0 };
	lightColor.Ambient = lightColor.Diffuse = lightColor.Specular = intensity;
	Light = new SimpleLight(lightColor, PremadeAttenuation::d20, glm::vec4(0.8, -1.7, 6.0, 1));
	Lights["SimpleGreenLight"] = Light;

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