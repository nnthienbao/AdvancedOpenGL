#pragma once
#include "Agent.h"

class CHuman;

class CZombie : public CAgent
{
public:
	CZombie();
	~CZombie();

	void Init(float speed, glm::vec2 pos);

	virtual void Update(const std::vector<std::string> &levelData,
						std::vector<CHuman*> &humans,
						std::vector<CZombie*> &zombies,
						float deltaTime) override;

private:
	CHuman *GetNearesHuman(std::vector<CHuman*> &humans);
};

