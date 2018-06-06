#include "HDRShader.h"
#include "global.h"

HDRShader::HDRShader(int w, int h)
{

	width = w;
	height = h;

	blurShader = new ShaderProgram();
	blurShader->initFromFiles("blur.vert", "blur.frag");

	blurShader->addUniform("Image");
	blurShader->addUniform("Horizontal");

	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("hdr.vert", "hdr.frag");

	shaderProgram->addUniform("SceneTex");
	shaderProgram->addUniform("BloomTex");
	shaderProgram->addUniform("Gamma");
	shaderProgram->addUniform("Exposure");

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

	// Make pingpong FrameBuffers and their TextureBuffers

	glGenFramebuffers(2, pingpongFBOs);
	glGenTextures(2, pingpongTBOs);

	for (int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongTBOs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTBOs[i], 0);
	}

	// Make FrameBuffer, TextureBuffers and RenderBuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(2, TBOs);
	for (int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, TBOs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, TBOs[i], 0);
	}
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

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
	bool horizontal = true;
	bool first_iteration = true;

	unsigned int amount = 10;
	blurShader->use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[horizontal]);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? TBOs[1] : pingpongTBOs[!horizontal]);
		glUniform1i(blurShader->uniform("Image"), 0);
		glUniform1i(blurShader->uniform("Horizontal"), horizontal);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	blurShader->disable();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram->use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TBOs[0]);
	glUniform1i(shaderProgram->uniform("SceneTex"), 0);
	if (global::UseBloom) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongTBOs[!horizontal]);
		glUniform1i(shaderProgram->uniform("BloomTex"), 1);
	}
	glUniform1f(shaderProgram->uniform("Exposure"), global::Exposure);
	glUniform1f(shaderProgram->uniform("Gamma"), global::GammaCorrection);
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
