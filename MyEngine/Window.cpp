#include "Window.h"
#include "MyEngineError.h"


namespace MyEngine
{
	CWindow::CWindow()
	{
	}


	CWindow::~CWindow()
	{
	}

	int CWindow::Create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);

		if (_sdlWindow == nullptr)
		{
			fatalError("SDL window could not be create!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);

		if (glContext == nullptr)
		{
			fatalError("SDL_GL context could not be create!");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			fatalError("Could not initialize glew!");
		}

		// Check the OpenGL version
		std::printf("*** OpenGL version %s\n", glGetString(GL_VERSION));

		// Set background
		glClearColor(0, 0, 1.0, 1.0);

		// Set VSYNC
		SDL_GL_SetSwapInterval(0);

		// Enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void CWindow::SwapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}
}