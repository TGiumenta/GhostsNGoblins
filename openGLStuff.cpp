#define OPENGLSTUFF_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>		
#include "glut.h"
#include "openglframework.h"
#include "openglstuff.h"

void InitializeOpenGLDrawing(GL_Window *window, Keys *keys, float backRed, float backGreen, float backBlue)
{
	glClearColor(backRed, backGreen, backGreen, 0.0f);			// Background Color
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending
	glEnable(GL_BLEND);											// Enable Blending
	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping
	glEnable(GL_CULL_FACE);										// Remove Back Face
	
	g_window	= window;
	g_keys		= keys;
	return;
}

void EndOpenGLDrawing()
{
	glFlush();
	return;
}

void StartOpenGLDrawing()
{
	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT);
	// Set the modelview matrix to be the identity matrix
	glLoadIdentity();

	return;
}