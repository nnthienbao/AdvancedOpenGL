#include "Player.h"

#include <SDL/SDL.h>

#include <MyEngine/ResourceManager.h>


CPlayer::CPlayer() :
	_currentGunIndex(-1)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init(float speed, glm::vec2 pos, MyEngine::CInputManager *inputManager, MyEngine::CCamera2D *camera, std::vector<CBullet> *bullets)
{
	_inputManager = inputManager;
	_speed = speed;
	_position = pos;
	_color = MyEngine::ColorRGBA8(255, 255, 255, 255);
	_camera = camera;
	_bullets = bullets;
	_health = 150;
	m_textureID = MyEngine::CResourceManager::GetTexture("Textures/player.png").id;
}


void CPlayer::AddGun(CGun *gun)
{
	_guns.push_back(gun);

	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}
}

void CPlayer::Update(const std::vector<std::string> &levelData,
					std::vector<CHuman*> &humans,
					std::vector<CZombie*> &zombies,
					float deltaTime)
{
	if (_inputManager->IsKeyDown(SDLK_w))
	{
		_position.y += _speed * deltaTime;
	}
	else if (_inputManager->IsKeyDown(SDLK_s))
	{
		_position.y -= _speed * deltaTime;
	}
	if (_inputManager->IsKeyDown(SDLK_d))
	{
		_position.x += _speed * deltaTime;
	}
	else if (_inputManager->IsKeyDown(SDLK_a))
	{
		_position.x -= _speed * deltaTime;
	}

	if (_inputManager->IsKeyDown(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	else if (_inputManager->IsKeyDown(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
	}
	else if (_inputManager->IsKeyDown(SDLK_3) && _guns.size() >= 2)
	{
		_currentGunIndex = 2;
	}

	glm::vec2 mouseCoords = _inputManager->GetMouseCoords();
	mouseCoords = _camera->ConvertScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

	m_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1)
	{
		_guns[_currentGunIndex]->Update(_inputManager->IsKeyDown(SDL_BUTTON_LEFT),
			centerPosition,
			m_direction,
			*_bullets,
			deltaTime);
	}

	CollideWithLevel(levelData);
}