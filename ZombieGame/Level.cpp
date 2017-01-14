#include "Level.h"

#include <fstream>
#include <iostream>
#include <MyEngine/MyEngineError.h>
#include <MyEngine/ResourceManager.h>

CLevel::CLevel(const std::string &fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file.fail())
	{
		MyEngine::fatalError("Failed to open " + fileName);
	}

	std::string tmp;
	file >> tmp >> _numberHuman;

	std::getline(file, tmp);

	while (std::getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}

	_spriteBatch.Init();
	_spriteBatch.Begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	MyEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			char tile = _levelData[y][x];

			glm::vec4 desRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile)
			{
			case 'B':
			case 'R':
				_spriteBatch.Draw(desRect,
									uvRect,
									MyEngine::CResourceManager::GetTexture("Textures/red_bricks.png").id,
									0.0f,
									whiteColor);
				break;
			case 'G':
				_spriteBatch.Draw(desRect,
					uvRect,
					MyEngine::CResourceManager::GetTexture("Textures/glass.png").id,
					0.0f,
					whiteColor);
				break;
			case 'L':
				_spriteBatch.Draw(desRect,
					uvRect,
					MyEngine::CResourceManager::GetTexture("Textures/light_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
			}
		}
	}
	_spriteBatch.End();
	file.close();
}


CLevel::~CLevel()
{
}

void CLevel::Draw()
{
	_spriteBatch.RenderBatch();
}
