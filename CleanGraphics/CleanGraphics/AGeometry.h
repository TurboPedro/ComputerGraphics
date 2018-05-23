#pragma once

#ifndef __A_GEOMETRY
# define __A_GEOMETRY

#include <GL/glew.h>
#include <GL/GL.h>

# include "Color.h"

class AGeometry
{
public:

	struct MaterialInfo {
		float Alpha;
		SColor color;
	};

	AGeometry(MaterialInfo info);
	~AGeometry();

	virtual GLuint GetElementCount() const = 0;
	virtual GLenum GetElementType() const = 0;

	virtual GLuint GetVAO() const;
	virtual GLuint GetIBO() const;
	virtual GLuint GetVertexVBO() const;
	virtual GLuint GetNormalVBO() const;
	virtual GLuint GetTextureVBO() const;
	virtual GLuint GetTangentVBO() const;
	virtual GLuint GetBitTangentVBO() const;
	virtual MaterialInfo GetMaterialInfo() const;
	virtual void SetMaterialInfo(MaterialInfo info);

protected:
	
	MaterialInfo materialInfo;
	GLuint VAO, VertexVBO, NormalVBO, IBO, TextureVBO, TangentVBO, BitTangentVBO;

	virtual void setup() = 0;
};

#endif