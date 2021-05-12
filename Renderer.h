#pragma once

typedef struct {
	GLfloat xLeftCoord;
	GLfloat xRightCoord;
	GLfloat yBottomTextCoord;
	GLfloat yTopTextCoord;
}UVRectangle;

typedef struct {
	GLfloat vertexLeft;
	GLfloat vertexRight;
	GLfloat vertexBottom;
	GLfloat vertexTop;
}VertexRectangle;

void RenderQuads(const GLuint textureIndex, const UVRectangle uvRectangle, const VertexRectangle vertexRectangle) noexcept;