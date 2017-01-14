#pragma once
#include "GLTexture.h"

#include <string>

namespace MyEngine
{
	class CImageLoader
	{
	public:
		static GLTexture LoadPNG(std::string filePath);
	};
}