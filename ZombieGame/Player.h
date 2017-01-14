#pragma once
#include "Human.h"
#include "Gun.h"

#include <MyEngine/InputManager.h>
#include <MyEngine/Camera2D.h>

class CPlayer : public CHuman
{
public:
	CPlayer();
	~CPlayer();

	void Init(float speed, glm::vec2 pos, MyEngine::CInputManager *inputManager, MyEngine::CCamera2D *camera, std::vector<CBullet> *bullets);

	void AddGun(CGun *gun);

	void Update(const std::vector<std::string> &levelData,
				std::vector<CHuman*> &humans,
				std::vector<CZombie*> &zombies,
				float deltaTime) override;

private:
	MyEngine::CInputManager *_inputManager;

	std::vector<CGun*> _guns;
	int _currentGunIndex;

	MyEngine::CCamera2D *_camera;
	std::vector<CBullet> *_bullets;
};

