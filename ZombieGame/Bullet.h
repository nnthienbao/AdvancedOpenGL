#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <MyEngine/SpriteBatch.h>

class CHuman;
class CZombie;
class CAgent;

const int BULLET_RADIUS = 5;

class CBullet
{
public:
	CBullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~CBullet();

	bool Update(const std::vector<std::string> &levelData, float deltaTime);

	void Draw(MyEngine::CSpriteBatch &spriteBatch);

	bool CollideWithAgent(CAgent *agent);

	float GetDamage() const { return _damage; }

	glm::vec2 GetPosition() const { return _position; }

private:

	bool CollideWithWorld(const std::vector<std::string> &levelData);

	glm::vec2 _position;
	glm::vec2 _direction;
	float _damage;
	float _speed;
};

