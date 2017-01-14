#pragma once

#include "ParticleBatch2D.h"

#include "SpriteBatch.h"

namespace MyEngine
{

	class CParticleEngine2D
	{
	public:
		CParticleEngine2D();
		~CParticleEngine2D();

		void AddParticleBatch(CParticleBatch2D* particleBatch);

		void Update(float deltaTime);

		void Draw(CSpriteBatch* spriteBatch);

	private:
		std::vector<CParticleBatch2D*> m_batches;
	};

}