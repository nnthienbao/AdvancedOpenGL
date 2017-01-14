#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

#include <MyEngine/ResourceManager.h>


CHuman::CHuman() :
	_frames(0)
{
}


CHuman::~CHuman()
{
}

void CHuman::Init(float speed, glm::vec2 pos)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_health = 20;

	_color = MyEngine::ColorRGBA8(255, 255, 255, 255);

	_speed = speed;
	_position = pos;
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (m_direction.length() == 0)
	{
		m_direction = glm::vec2(1.0f, 0.0f);
	}

	m_direction = glm::normalize(m_direction);
	m_textureID = MyEngine::CResourceManager::GetTexture("Textures/human.png").id;
}

void CHuman::Update(const std::vector<std::string> &levelData,
					std::vector<CHuman*> &humans,
					std::vector<CZombie*> &zombies,
					float deltaTime)
{
	static std::mt19937 randomEngine(time(nullptr));

	static std::uniform_real_distribution<float> randRotate(-0.34f, 0.34f);

	_position += m_direction * _speed * deltaTime;

	if (_frames == 20)
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else
	{
		_frames++;
	}

	if (CollideWithLevel(levelData))
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}
}
