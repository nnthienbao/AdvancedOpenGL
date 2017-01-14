#pragma once
#include "GLTexture.h"

#include <map>

namespace MyEngine
{
	class CTextureCache
	{
	public:
		CTextureCache();
		~CTextureCache();

		GLTexture GetTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> _textureMap;
	};
}