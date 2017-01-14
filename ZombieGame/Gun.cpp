#include "Gun.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>



CGun::CGun(std::string name, int fireRate, int bulletPerShot,
	float spread, float bulletDamage, float bulletSpeed, MyEngine::CSoundEffect fireEffect)
{
	_name = name;
	_fireRate = fireRate;
	_bulletsPerShot = bulletPerShot;
	_spread = spread;
	_bulletDamage = bulletDamage;
	_bulletSpeed = bulletSpeed;
	_frameCounter = 0;
	m_fireEffect = fireEffect;
}


CGun::~CGun()
{
}

void CGun::Update(bool isMouseDown, const glm::vec2 &position, const glm::vec2 &direction, std::vector<CBullet> &bullets, float deltaTime)
{
	_frameCounter += 1.0f * deltaTime;
	if (_frameCounter >= _fireRate && isMouseDown)
	{
		Fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void CGun::Fire(const glm::vec2 &position, const glm::vec2 &direction, std::vector<CBullet> &bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	m_fireEffect.Play();

	for (int i = 0; i < _bulletsPerShot; i++)
	{
		bullets.emplace_back(position,
								glm::rotate(direction, randRotate(randomEngine)),
								_bulletDamage,
								_bulletSpeed);
	}
}