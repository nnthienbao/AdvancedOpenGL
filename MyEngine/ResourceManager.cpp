#include "ResourceManager.h"

namespace MyEngine
{
	CTextureCache CResourceManager::_textureCache;

	GLTexture CResourceManager::GetTexture(std::string texturePath)
	{
		return _textureCache.GetTexture(texturePath);
	}
}