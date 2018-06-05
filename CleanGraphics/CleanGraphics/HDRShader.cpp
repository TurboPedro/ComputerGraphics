#include "HDRShader.h"
#include "global.h"

HDRShader::HDRShader(int w, int h)
{

	width = w;
	height = h;

	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("hdr.vert", "hdr.frag");

	shaderProgram->addUniform("ScreenTex");
	shaderProgram->addUniform("Gamma");

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create vbo for vertices
	glGenBuffers(1, &VertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,					// attribute
		2,					// number of elements per vertex, here (x,y,z)
		GL_FLOAT,			// the type of each element
		GL_FALSE,			// take our values as-is
		4 * sizeof(float),	// no extra data between each position
		0					// offset of first element
	);
	glEnableVertexAttribArray(0);

	//create vbo for textures
	glGenBuffers(1, &TextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,					// attribute
		2,					// number of elements per vertex, here (x,y)
		GL_FLOAT,			// the type of each element
		GL_FALSE,			// take our values as-is
		4 * sizeof(float),	// no extra data between each position
		(void *)(2 * sizeof(float))	// offset of first element
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Make FrameBuffer, TextureBuffer and RenderBuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


HDRShader::~HDRShader()
{
}

bool HDRShader::use(AGeometry * object, std::map<const char*, ALight*>* lights, SModelViewProjection * mvp, Texture * diffuseTexture, Texture * specularTexture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram->use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TBO);
	glUniform1i(shaderProgram->uniform("ScreenTex"), 0);
	glUniform1f(shaderProgram->uniform("Gamma"), global::gammaCorrection);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	shaderProgram->disable();

	return true;
}

void HDRShader::beforeDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}