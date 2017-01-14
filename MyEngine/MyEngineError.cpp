#include "MyEngineError.h"
#include <SDL/SDL.h>

#include <iostream>

namespace MyEngine
{
	void fatalError(std::string error)
	{
		std::cout << error << std::endl;
		std::cout << "Enter any key to quit...";
		std::cin.get();

		SDL_Quit();
		exit(1);
	}
}