#pragma once

#include <string>
#include <vector>

#include <MyEngine/AudioEngine.h>

#include <glm/glm.hpp>

#include "Bullet.h"

class CGun
{
public:
	CGun(std::string name, int fireRate, int bulletPerShot,
		float spread, float bulletDamage, float bulletSpeed, MyEngine::CSoundEffect fireEffect);
	~CGun();

	void Update(bool isMouseDown, const glm::vec2 &position, const glm::vec2 &direction, std::vector<CBullet> &bullets, float deltaTime);

	void Fire(const glm::vec2 &position, const glm::vec2 &direction, std::vector<CBullet> &bullets);

private:

	MyEngine::CSoundEffect m_fireEffect;
	
	std::string _name;

	int _fireRate;

	int _bulletsPerShot;

	float _spread;

	int _bulletDamage;

	int _bulletSpeed;

	int _frameCounter;

};

