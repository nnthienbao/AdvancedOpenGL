#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

namespace MyEngine
{
	CSprite::CSprite()
	{
		_vboID = 0;
	}


	CSprite::~CSprite()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}

	void CSprite::Init(float x, float y, float width, float height, std::string texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = CResourceManager::GetTexture(texturePath);

		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}

		Vertex vertexData[6];

		// First triangle
		vertexData[0].SetPosition(x + width, y + height);
		vertexData[0].SetUV(1.0f, 1.0f);

		vertexData[1].SetPosition(x, y + height);
		vertexData[1].SetUV(0.0f, 1.0f);

		vertexData[2].SetPosition(x, y);
		vertexData[2].SetUV(0.0f, 0.0f);

		// Second triangle
		vertexData[3].SetPosition(x, y);
		vertexData[3].SetUV(0.0f, 0.0f);

		vertexData[4].SetPosition(x + width, y);
		vertexData[4].SetUV(1.0f, 0.0f);

		vertexData[5].SetPosition(x + width, y + height);
		vertexData[5].SetUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++)
		{
			vertexData[i].SetColor(255, 0, 255, 255);
		}

		vertexData[1].SetColor(0, 0, 255, 255);

		vertexData[4].SetColor(0, 255, 0, 255);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void CSprite::Draw()
	{
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		//Bind the buffer object
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		//Tell opengl that we want to use the first
		//attibute array. We only need one array right now
		//since we are only using position
		glEnableVertexAttribArray(0);

		// Position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color attribute
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// UV attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//Draw the 6 vertices to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Disable the vertex attibute array
		glDisableVertexAttribArray(0);

		//Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}