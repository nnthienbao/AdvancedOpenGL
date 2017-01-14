#pragma once

#include <string>
#include <map>

#include <SDL/SDL_mixer.h>


namespace MyEngine
{
	class CSoundEffect
	{
	public:
		friend class CAudioEngine;
		void Play(int loops = 0);
	private:

		Mix_Chunk* m_chunk = nullptr;
	};

	class CMusic
	{
	public:
		friend class CAudioEngine;
		void Play(int loops = 1);

		static void Pause();
		static void Stop();
		static void Resume();
	private:
		Mix_Music *m_music = nullptr;
	};


	class CAudioEngine
	{
	public:
		CAudioEngine();
		~CAudioEngine();

		void Init();
		void Destroy();

		CSoundEffect LoadSoundEffect(const std::string& filePath);
		CMusic LoadMusic(const std::string& filePath);
	private:

		std::map<std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musicMap;

		bool m_isInitialized = false;
	};

}