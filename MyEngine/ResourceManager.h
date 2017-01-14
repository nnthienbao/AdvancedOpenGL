#pragma once
#include "TextureCache.h"
#include <string>

namespace MyEngine
{
	class CResourceManager
	{
	public:
		static GLTexture GetTexture(std::string texturePath);

	private:
		static CTextureCache _textureCache;
	};
}