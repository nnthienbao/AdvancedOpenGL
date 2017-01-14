#include <SDL/SDL.h>
#include <GL/glew.h>

namespace MyEngine
{
	void Init()
	{
		// Khoi tao SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Bat double buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	}
}