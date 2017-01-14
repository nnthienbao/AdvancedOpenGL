#pragma once
#include "Agent.h"

class CHuman : public CAgent
{
public:
	CHuman();
	virtual ~CHuman();

	void Init(float speed, glm::vec2 pos);

	virtual void Update(const std::vector<std::string> &levelData,
						std::vector<CHuman*> &humans,
						std::vector<CZombie*> &zombies,
						float deltaTime) override;

private:
	int _frames;
};

