#pragma once
#include <GL/glew.h>
#include <string>

#include "GLTexture.h"
#include "ResourceManager.h"

namespace MyEngine
{
	class CSprite
	{
	public:
		CSprite();
		~CSprite();

		void Init(float x, float y, float width, float height, std::string texturePath);

		void Draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;

		GLuint _vboID;	// Vertex buffer object ID

		GLTexture _texture;
	};
}