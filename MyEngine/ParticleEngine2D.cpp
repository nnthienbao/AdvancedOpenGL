#include "ParticleEngine2D.h"

namespace MyEngine
{

	CParticleEngine2D::CParticleEngine2D()
	{
	}


	CParticleEngine2D::~CParticleEngine2D()
	{
		for (auto& b : m_batches)
		{
			delete b;
		}
	}

	void CParticleEngine2D::AddParticleBatch(CParticleBatch2D* particleBatch)
	{
		m_batches.push_back(particleBatch);
	}

	void CParticleEngine2D::Update(float deltaTime)
	{
		for (auto& b : m_batches)
		{
			b->Update(deltaTime);
		}
	}

	void CParticleEngine2D::Draw(CSpriteBatch* spriteBatch)
	{
		for (auto& b : m_batches)
		{
			spriteBatch->Begin();
			b->Draw(spriteBatch);

			spriteBatch->End();
			spriteBatch->RenderBatch();
		}
	}
}