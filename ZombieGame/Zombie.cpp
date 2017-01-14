#include "Zombie.h"

#include <random>
#include <ctime>

#include <MyEngine/Vertex.h>
#include <MyEngine/ResourceManager.h>

#include "Human.h"


CZombie::CZombie()
{
}


CZombie::~CZombie()
{
}

void CZombie::Init(float speed, glm::vec2 pos)
{
	_speed = speed;
	_position = pos;
	_health = 150;
	_color = MyEngine::ColorRGBA8(255, 255, 255, 255);
	m_textureID = MyEngine::CResourceManager::GetTexture("Textures/zombie.png").id;
}

void CZombie::Update(const std::vector<std::string> &levelData,
					std::vector<CHuman*> &humans,
					std::vector<CZombie*> &zombies,
					float deltaTime)
{
	CHuman *closestHuman = GetNearesHuman(humans);

	if (closestHuman != nullptr)
	{
		m_direction = glm::normalize(closestHuman->GetPosition() - _position);
		_position += m_direction * _speed * deltaTime;
	}

	CollideWithLevel(levelData);
}

CHuman* CZombie::GetNearesHuman(std::vector<CHuman*> &humans)
{
	CHuman *closestHuman = nullptr;
	float smallestDistance = 9999999.0f;

	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->GetPosition() - _position;
		float distance = glm::length(distVec);
		if (smallestDistance > distance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}