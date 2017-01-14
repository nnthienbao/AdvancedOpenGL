#include "Agent.h"
#include <MyEngine/ResourceManager.h>
#include "Level.h"

#include <algorithm>


CAgent::CAgent()
{
}


CAgent::~CAgent()
{
}


bool CAgent::CollideWithLevel(const std::vector<std::string> &levelData)
{
	std::vector<glm::vec2> collideTilePosition;

	CheckTilePosition(levelData,
						collideTilePosition,
						_position.x,
						_position.y);

	CheckTilePosition(levelData,
						collideTilePosition,
						_position.x + AGENT_WIDTH,
						_position.y);

	CheckTilePosition(levelData,
						collideTilePosition,
						_position.x,
						_position.y + AGENT_WIDTH);

	CheckTilePosition(levelData,
						collideTilePosition,
						_position.x + AGENT_WIDTH,
						_position.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0)
	{
		return false;
	}
	for (int i = 0; i < collideTilePosition.size(); i++)
	{
		CollideWithTile(collideTilePosition[i]);
	}
	return true;
}

bool CAgent::CollideWithAgents(CAgent *agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB= agent->GetPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

void CAgent::Draw(MyEngine::CSpriteBatch & _spriteBatch)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	_spriteBatch.Draw(destRect, uvRect, m_textureID, 0.0f, _color, m_direction);

}

bool CAgent::ApplyDamage(float damage)
{
	_health -= damage;
	if (_health <= 0)
	{
		return true;
	}
	return false;
}

void CAgent::CheckTilePosition(const std::vector<std::string> &levelData,
								std::vector<glm::vec2> &collideTilePosition,
								float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
										floor(y / (float)TILE_WIDTH));

	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].length() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size())
	{
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

// AABB collition
void CAgent::CollideWithTile(glm::vec2 tilePos)
{
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 || yDepth > 0)
	{
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				_position.x -= xDepth;
			}
			else
			{
				_position.x += xDepth;
			}
		}
		else
		{
			if (distVec.y < 0)
			{
				_position.y -= yDepth;
			}
			else
			{
				_position.y += yDepth;
			}
		}
	}
}
