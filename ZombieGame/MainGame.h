#pragma once

#include <MyEngine/Window.h>
#include <MyEngine/GLSLProgram.h>
#include <MyEngine/Camera2D.h>
#include <MyEngine/InputManager.h>
#include <MyEngine/SpriteBatch.h>
#include <MyEngine/SpriteFont.h>
#include <MyEngine/AudioEngine.h>
#include <MyEngine/ParticleEngine2D.h>
#include <MyEngine/ParticleBatch2D.h>

#include "Level.h"
#include "Player.h"
#include "Bullet.h"



class CZombie;

enum class GameState
{
	PLAY, EXIT
};

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

	void Run();

private:
	void InitSystems();

	void InitLevel();

	void InitShaders();

	void GameLoop();

	void UpdateAgents(float deltaTime);

	void UpdateBullets(float deltaTime);

	void CheckVictory();

	void ProcessInput();

	void DrawGame();

	void DrawHub();

	void AddBlood(const glm::vec2& position, int numParticles);

	MyEngine::CWindow _window;
		
	MyEngine::CGLSLProgram _textureProgram;

	MyEngine::CInputManager _inputManager;

	MyEngine::CCamera2D _camera;
	MyEngine::CCamera2D _hubCamera;

	MyEngine::CSpriteBatch _agentSpriteBatch;
	MyEngine::CSpriteBatch _hubSpriteBatch;

	MyEngine::CParticleEngine2D m_particleEngine;
	MyEngine::CParticleBatch2D* m_bloodParticleBatch;

	std::vector<CLevel*> _levels;

	int _screenWidth, _screenHeight;

	GameState _gameState;

	int _fps;

	int _currentLevel;

	CPlayer *_player;
	std::vector<CHuman*> _humans;
	std::vector<CZombie*> _zombies;
	std::vector<CBullet> _bullets;

	int _numHumansKilled;
	int _numZombiesKilled;

	MyEngine::CSpriteFont *_spriteFont;

	MyEngine::CAudioEngine m_audioEngine;

};
