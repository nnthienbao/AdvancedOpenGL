#include "AudioEngine.h"

#include "MyEngineError.h"


namespace MyEngine
{
	void CSoundEffect::Play(int loops)
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1)
		{
			if (Mix_PlayChannel(0, m_chunk, loops) == -1)
			{
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void CMusic::Play(int loops)
	{
		Mix_PlayMusic(m_music, loops);
	}

	void CMusic::Pause()
	{
		Mix_PauseMusic();
	}
	void CMusic::Stop()
	{
		Mix_HaltMusic();
	}
	void CMusic::Resume()
	{
		Mix_ResumeMusic();
	}

	CAudioEngine::CAudioEngine()
	{
		// Empty
	}


	CAudioEngine::~CAudioEngine()
	{
		Destroy();
	}

	void CAudioEngine::Init()
	{
		if (m_isInitialized)
		{
			fatalError("Tried to initialize Audio Engine twice!");
		}
		// Parameter can be a bitwise combination of MIX_INIT_FAC,
		// MIX_INIT_MOD, MID_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}
	void CAudioEngine::Destroy()
	{
		if (m_isInitialized)
		{
			m_isInitialized = false;

			for (auto& it : m_effectMap)
			{
				Mix_FreeChunk(it.second);
			}
			for (auto& it : m_musicMap)
			{
				Mix_FreeMusic(it.second);
			}

			m_effectMap.clear();
			m_musicMap.clear();
			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	CSoundEffect CAudioEngine::LoadSoundEffect(const std::string& filePath)
	{
		// Try to find the audio in the cache
		auto it = m_effectMap.find(filePath);

		CSoundEffect effect;

		if (it == m_effectMap.end())
		{
			// Failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
			{
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;
			m_effectMap[filePath] = chunk;
		}
		else
		{
			// Its already cached
			effect.m_chunk = it->second;
		}

		return effect;
	}
	CMusic CAudioEngine::LoadMusic(const std::string& filePath)
	{
		// Try to find the audio in the cache
		auto it = m_musicMap.find(filePath);

		CMusic music;

		if (it == m_musicMap.end())
		{
			// Failed to find it, must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
			{
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}
			music.m_music = mixMusic;
			m_musicMap[filePath] = mixMusic;
		}
		else
		{
			// Its already cached
			music.m_music = it->second;
		}

		return music;
	}
}