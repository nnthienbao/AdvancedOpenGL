#pragma once

#include <string>
#include <vector>

#include <MyEngine/SpriteBatch.h>


const int TILE_WIDTH = 40;

class CLevel
{
public:
	CLevel(const std::string &fileName);
	~CLevel();

	void Draw();

	//Getters
	int GetWidth() const { return _levelData[0].size(); }
	int GetHeight() const { return _levelData.size(); }
	int GetNumHumans() const { return _numberHuman; }
	const std::vector<std::string> &GetLevelData() { return _levelData; }
	glm::vec2 GetStartPlayerPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2>& GetZombiesrPos() const { return _zombieStartPos; }

private:
	std::vector<std::string> _levelData;
	int _numberHuman;
	MyEngine::CSpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _zombieStartPos;
};

