#pragma once

#include <functional>

#include <glm/glm.hpp>

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace MyEngine
{
	class CParticle2D
	{
	public:

		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float life = 0.0f;
		float width = 0.0f;
	};

	inline void DefaultParticleUpdate(CParticle2D& particle, float deltaTime)
	{
		particle.position += particle.velocity * deltaTime;
	}

	class CParticleBatch2D
	{
	public:
		CParticleBatch2D();
		~CParticleBatch2D();

		void Init(int maxParticle,
					float decayRate,
					GLTexture texture,
					std::function<void(CParticle2D&, float)> updateFunc = DefaultParticleUpdate);

		void Update(float deltaTime);

		void Draw(CSpriteBatch* spriteBatch);

		void AddParticle(const glm::vec2& position,
							const glm::vec2 velocity,
							const ColorRGBA8& color,
							float width);

	private:
		int FindFreeParticle();

		std::function<void(CParticle2D&, float)> m_updateFunc;

		float m_decayRate;
		CParticle2D* m_particles = nullptr;
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		GLTexture m_texture;
	};

}