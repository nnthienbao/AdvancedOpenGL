#pragma once

namespace MyEngine
{
	class CFPSLimiter
	{
	public:
		CFPSLimiter();

		void Init(float maxFPS);

		void SetMaxFPS(float maxFPS);

		void Begin();

		float End();
		
	private:
		void CaculateFPS();

		unsigned int _startTicks;

		float _fps;
		float _maxFPS;
		float _frameTime;
	};
}
