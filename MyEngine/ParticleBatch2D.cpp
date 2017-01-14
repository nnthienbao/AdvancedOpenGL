#include "ParticleBatch2D.h"


namespace MyEngine
{
	CParticleBatch2D::CParticleBatch2D()
	{
	}


	CParticleBatch2D::~CParticleBatch2D()
	{
		delete[] m_particles;
	}

	void CParticleBatch2D::Init(int maxParticle,
								float decayRate,
								GLTexture texture,
								std::function<void(CParticle2D&, float)> updateFunc)
	{
		m_maxParticles = maxParticle;
		m_particles = new CParticle2D[maxParticle];

		m_decayRate = decayRate;

		m_texture = texture;

		m_updateFunc = updateFunc;
	}

	void CParticleBatch2D::Update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; i++)
		{
			// Check if it is active
			if (m_particles[i].life > 0.0f)
			{
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].life -= m_decayRate * deltaTime;
			}
		}
	}

	void CParticleBatch2D::Draw(CSpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++)
		{
			// Check if it is active
			auto& p = m_particles[i];
			if (p.life > 0.0f)
			{
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->Draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
			}
		}
	}

	void CParticleBatch2D::AddParticle(const glm::vec2& position,
										const glm::vec2 velocity,
										const ColorRGBA8& color,
										float width)
	{
		int particleIndex = FindFreeParticle();

		auto& p = m_particles[particleIndex];

		p.life = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;
	}

	int CParticleBatch2D::FindFreeParticle()
	{
		for (int i = m_lastFreeParticle; i < m_maxParticles; i++)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastFreeParticle; i++)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}

		return 0;
	}
}