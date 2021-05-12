/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A file which contains a singular method (for now) which will render textures.
				  This method draws a quadrilateral base on the UVs and vertexes of each sprite. This 
				  is used by every manager (and therefore sprite) to render textures.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>										// Header File For Windows
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include "Renderer.h"

// Draws a quadrilateral to the screen based on the UV and Vertex
void RenderQuads(const GLuint textureIndex, const UVRectangle uvRectangle, const VertexRectangle vertexRectangle) noexcept
{
	// Enable the drawing setting for the texture and set that texture based on the parameter
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glBegin(GL_QUADS);

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);										// Display background colors/alpha
	glTexCoord2f(uvRectangle.xLeftCoord, uvRectangle.yBottomTextCoord);		// U,V
	glVertex2f(vertexRectangle.vertexLeft, vertexRectangle.vertexBottom);	// X,Y on screen

	glTexCoord2f(uvRectangle.xRightCoord, uvRectangle.yBottomTextCoord);	// U,V
	glVertex2f(vertexRectangle.vertexRight, vertexRectangle.vertexBottom);	// X,Y on screen

	glTexCoord2f(uvRectangle.xRightCoord, uvRectangle.yTopTextCoord);		// U,V
	glVertex2f(vertexRectangle.vertexRight, vertexRectangle.vertexTop);		// X,Y on screen

	glTexCoord2f(uvRectangle.xLeftCoord, uvRectangle.yTopTextCoord);		// U,V
	glVertex2f(vertexRectangle.vertexLeft, vertexRectangle.vertexTop);		// X,Y on screen

	glEnd();
	glDisable(GL_TEXTURE_2D);
}