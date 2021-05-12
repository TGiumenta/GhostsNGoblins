#ifdef OPENGLSTUFF_CPP
#define extern 
#endif

void EndOpenGLDrawing();
void StartOpenGLDrawing();
void InitializeOpenGLDrawing( GL_Window *window, Keys *keys, float backRed, float backGreen, float backBlue);

extern GL_Window*	g_window;
extern Keys*		g_keys;

#undef extern