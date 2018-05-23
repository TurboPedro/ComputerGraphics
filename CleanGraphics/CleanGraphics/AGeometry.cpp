#include "AGeometry.h"



AGeometry::AGeometry(AGeometry::MaterialInfo info)
{
	materialInfo = info;
}


AGeometry::~AGeometry()
{
}

GLuint AGeometry::GetVAO() const
{
	return VAO;
}

GLuint AGeometry::GetIBO() const
{
	return IBO;
}

GLuint AGeometry::GetVertexVBO() const
{
	return VertexVBO;
}

GLuint AGeometry::GetNormalVBO() const
{
	return NormalVBO;
}

GLuint AGeometry::GetTextureVBO() const
{
	return TextureVBO;
}

GLuint AGeometry::GetTangentVBO() const
{
	return TangentVBO;
}

GLuint AGeometry::GetBitTangentVBO() const
{
	return BitTangentVBO;
}

AGeometry::MaterialInfo AGeometry::GetMaterialInfo() const
{
	return materialInfo;
}

void AGeometry::SetMaterialInfo(AGeometry::MaterialInfo info)
{
	materialInfo = info;
}
