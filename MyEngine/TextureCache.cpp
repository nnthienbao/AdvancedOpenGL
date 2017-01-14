#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace MyEngine
{
	CTextureCache::CTextureCache()
	{
	}


	CTextureCache::~CTextureCache()
	{
	}

	GLTexture CTextureCache::GetTexture(std::string texturePath)
	{
		// lookup the texture and see if its in the map
		auto mit = _textureMap.find(texturePath);

		// check if its not in the map
		if (mit == _textureMap.end())
		{
			// Load the texture
			GLTexture newTexture = CImageLoader::LoadPNG(texturePath);

			std::pair<std::string, GLTexture> newPair(texturePath, newTexture);

			//Insert it into the map
			_textureMap.insert(newPair);

			std::cout << "Loader texture!\n";

			return newTexture;
		}

		//std::cout << "Used cached texture!\n";

		return mit->second;
	}
}