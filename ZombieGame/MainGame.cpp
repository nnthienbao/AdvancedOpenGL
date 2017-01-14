#include "MainGame.h"

#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

#include <MyEngine/MyEngine.h>
#include <SDL/SDL.h>
#include <glm/gtx/rotate_vector.hpp>
#include <MyEngine/Timing.h>
#include <MyEngine/MyEngineError.h>
#include <MyEngine/ResourceManager.h>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 4.0f;
const float ZOMBIE_SPEED = 3.0f;
const float PLAYER_SPEED = 5.0f;

CMainGame::CMainGame() :
	_screenWidth(800),
	_screenHeight(600),
	_gameState(GameState::PLAY),
	_fps(0),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0)
{
}

CMainGame::~CMainGame()
{
	for (int i = 0; i < _levels.size(); i++)
		delete _levels[i];
}

void CMainGame::Run()
{
	InitSystems();
	//Init level
	InitLevel();

	MyEngine::CMusic music = m_audioEngine.LoadMusic("Sound/XYZ.ogg");
	music.Play(-1);

	GameLoop();
}

// Khoi tao he thong
void CMainGame::InitSystems()
{
	// Bat SDL, double buffer
	MyEngine::Init();

	// Initialize sound
	m_audioEngine.Init();

	// Tao cua so
	_window.Create("Zombie Game", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// Khoi tao shader
	InitShaders();

	_agentSpriteBatch.Init();
	_hubSpriteBatch.Init();

	// Initialize sprite font
	_spriteFont = new MyEngine::CSpriteFont("Fonts/comic.ttf", 32);

	_camera.Init(_screenWidth, _screenHeight);
	_hubCamera.Init(_screenWidth, _screenHeight);
	_hubCamera.SetPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

	// Initialize particles
	m_bloodParticleBatch = new MyEngine::CParticleBatch2D();
	m_bloodParticleBatch->Init(1000, 0.05f,
								MyEngine::CResourceManager::GetTexture("Textures/particle.png"),
								[](MyEngine::CParticle2D& particle, float deltaTime)
							{
								particle.position += particle.velocity * deltaTime;
								particle.color.a = (GLubyte)(particle.life * 255.0f);
							});
	m_particleEngine.AddParticleBatch(m_bloodParticleBatch);
}

void CMainGame::InitLevel()
{
	//Load all maps
	_levels.push_back(new CLevel("Levels/Level1.txt"));
	_currentLevel = 0;

	_player = new CPlayer();
	_player->Init(PLAYER_SPEED, _levels[_currentLevel]->GetStartPlayerPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->GetWidth() - 2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->GetHeight() - 2);

	// Add all the random humans
	for (int i = 0; i < _levels[_currentLevel]->GetNumHumans(); i++)
	{
		_humans.push_back(new CHuman());
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->Init(HUMAN_SPEED, pos);

	}

	//Add the zombies
	const std::vector<glm::vec2> &zombiePosPositions = _levels[_currentLevel]->GetZombiesrPos();
	for (int i = 0; i < zombiePosPositions.size(); i++)
	{
		_zombies.push_back(new CZombie());
		_zombies.back()->Init(HUMAN_SPEED, zombiePosPositions[i]);
	}

	//Set up the players guns
	const float BULLET_SPEED = 20.0f;
	_player->AddGun(new CGun("Pistol", 30, 1, 0.1f, 60, BULLET_SPEED, m_audioEngine.LoadSoundEffect("Sound/shots/pistol.wav")));
	_player->AddGun(new CGun("ShotGun", 30, 20, 0.87f, 10, BULLET_SPEED, m_audioEngine.LoadSoundEffect("Sound/shots/shotgun.wav")));
	_player->AddGun(new CGun("MP5", 4, 1, 0.26f, 40, BULLET_SPEED, m_audioEngine.LoadSoundEffect("Sound/shots/cg1.wav")));
}

void CMainGame::InitShaders()
{
	_textureProgram.CompileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_textureProgram.AddAttribute("vertexPosition");
	_textureProgram.AddAttribute("vertexColor");
	_textureProgram.AddAttribute("vertexUV");
	_textureProgram.LinkShaders();
}

void CMainGame::GameLoop()
{
	_camera.SetScale(0.5f);

	const float DISIRED_FPS = 60.0f;
	const float MAX_PHYSICS_STEPS = 6;
	const float MS_PER_SECOND = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DISIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	MyEngine::CFPSLimiter fpsLimiter;
	fpsLimiter.SetMaxFPS(60.0f);

	float previoursTicks = SDL_GetTicks();

	while (_gameState == GameState::PLAY)
	{
		fpsLimiter.Begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previoursTicks;
		previoursTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		CheckVictory();

		_inputManager.Update();

		ProcessInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			UpdateAgents(deltaTime);
			UpdateBullets(deltaTime);
			m_particleEngine.Update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}

		_camera.SetPosition(_player->GetPosition());

		_camera.Update();
		_hubCamera.Update();

		DrawGame();

		std::cout << fpsLimiter.End() << std::endl;
	}
}

void CMainGame::UpdateAgents(float deltaTime)
{
	//Update humans
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->Update(_levels[_currentLevel]->GetLevelData(),
							_humans,
							_zombies,
							deltaTime);
	}

	//Update zombies
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->Update(_levels[_currentLevel]->GetLevelData(),
							_humans,
							_zombies,
							deltaTime);
	}

	//Update Zombies colisions
	for (int i = 0; i < _zombies.size(); i++)
	{
		//Collide with other zombies
		for (int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->CollideWithAgents(_zombies[j]);
		}
		//Collide with humans
		for (int j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->CollideWithAgents(_humans[j]))
			{
				// Add the new zombie
				_zombies.push_back(new CZombie());
				_zombies.back()->Init(ZOMBIE_SPEED, _humans[j]->GetPosition());
				//Delete the human
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
		
		//Collide with player
		if (_zombies[i]->CollideWithAgents(_player))
		{
			MyEngine::fatalError("YOU LOSE");
		}
	}

	//Update Humans colisions
	for (int i = 0; i < _humans.size(); i++)
	{
		for (int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->CollideWithAgents(_humans[j]);
		}
	}
}

void CMainGame::UpdateBullets(float deltaTime)
{
	for (int i = 0; i < _bullets.size();)
	{
		if (_bullets[i].Update(_levels[_currentLevel]->GetLevelData(), deltaTime))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
		{
			i++;
		}
	}

	bool wasBulletRemoved;
	//Collide with humans and zombies
	for (int i = 0; i < _bullets.size(); i++)
	{
		wasBulletRemoved = false;
		// Zombies
		for (int j = 0; j < _zombies.size();)
		{
			// Check collision
			if (_bullets[i].CollideWithAgent(_zombies[j]))
			{
				// Add blood
				AddBlood(_bullets[i].GetPosition(), 5);
				//Damage zombie
				if (_zombies[j]->ApplyDamage(_bullets[i].GetDamage()))
				{
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else
				{
					j++;
				}
				//Remove the bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--;
				break;
			}
			else
			{
				j++;
			}
		}

		//Human
		if (wasBulletRemoved == false)
		{
			for (int j = 1; j < _humans.size();)
			{
				// Check collision
				if (_bullets[i].CollideWithAgent(_humans[j]))
				{
					// Add blood
					AddBlood(_bullets[i].GetPosition(), 5);
					//Damage human
					if (_humans[j]->ApplyDamage(_bullets[i].GetDamage()))
					{
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else
					{
						j++;
					}
					//Remove the bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					i--;
					break;
				}
				else
				{
					j++;
				}
			}
		}
	}
}

void CMainGame::CheckVictory()
{
	if (_zombies.empty())
	{
		std::printf("***YOU WIN ***\n You killed %d humans and %d zombies.", _numHumansKilled, _numZombiesKilled);
		MyEngine::fatalError("");
	}
}

void CMainGame::ProcessInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.PressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.ReleaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.PressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.ReleaseKey(evnt.button.button);
			break;
		}
	}
}

void CMainGame::DrawGame()
{
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.Use();
	//Draw game

	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = _textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.GetCameraMatrix();
	GLint pUniform = _textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw level
	_levels[_currentLevel]->Draw();


	//Draw agents
	_agentSpriteBatch.Begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	//Draw the humans
	for (int i = 0; i < _humans.size(); i++)
	{
		if (_camera.IsBoxInView(_humans[i]->GetPosition(), agentDims))
		{
			_humans[i]->Draw(_agentSpriteBatch);
		}
	}

	//Draw the zombies
	for (int i = 0; i < _zombies.size(); i++)
	{
		if (_camera.IsBoxInView(_zombies[i]->GetPosition(), agentDims))
		{
			_zombies[i]->Draw(_agentSpriteBatch);
		}
	}

	//Draw the bullets
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].Draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.End();
	_agentSpriteBatch.RenderBatch();

	// Render the particles
	m_particleEngine.Draw(&_agentSpriteBatch);

	//Draw Hub
	DrawHub();

	_textureProgram.UnUse();

	_window.SwapBuffer();
}

void CMainGame::DrawHub()
{
	char buffer[256];

	glm::mat4 projectionMatrix = _hubCamera.GetCameraMatrix();
	GLint pUniform = _textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hubSpriteBatch.Begin();
	
	sprintf_s(buffer, "Num Humans: %d", _humans.size());
	_spriteFont->Draw(_hubSpriteBatch, buffer, glm::vec2(0, 0),
		glm::vec2(0.5f), 0.0f, MyEngine::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies: %d", _zombies.size());
	_spriteFont->Draw(_hubSpriteBatch, buffer, glm::vec2(0, 24),
		glm::vec2(0.5f), 0.0f, MyEngine::ColorRGBA8(255, 255, 255, 255));

	_hubSpriteBatch.End();
	_hubSpriteBatch.RenderBatch();
}

void CMainGame::AddBlood(const glm::vec2& position, int numParticles)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 2 * M_PI);

	glm::vec2 vel(2.0f, 0.0f);
	MyEngine::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++)
	{
		m_bloodParticleBatch->AddParticle(position, glm::rotate(vel, randAngle(randEngine)), color, 20.0f);
	}
}
