#pragma once

#include <glm/glm.hpp>

#include <MyEngine/SpriteBatch.h>
#include <MyEngine/Vertex.h>

const float AGENT_WIDTH = 40;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class CZombie;
class CHuman;

class CAgent
{
public:
	CAgent();
	virtual ~CAgent();

	virtual void Update(const std::vector<std::string> &levelData,
						std::vector<CHuman*> &humans,
						std::vector<CZombie*> &zombies,
						float deltaTime) = 0;

	bool CollideWithLevel(const std::vector<std::string> &levelData);

	bool CollideWithAgents(CAgent *agent);

	void Draw(MyEngine::CSpriteBatch & _spriteBatch);

	bool ApplyDamage(float damage);

	glm::vec2 GetPosition() const { return _position; }

protected:

	void CheckTilePosition(const std::vector<std::string> &levelData,
							std::vector<glm::vec2> &collideTilePosition,
							float x, float y);

	void CollideWithTile(glm::vec2 tilePos);

	glm::vec2 _position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
	MyEngine::ColorRGBA8 _color;
	float _speed;
	float _health;
	GLuint m_textureID;
};

