#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

namespace MyEngine
{
	class CInputManager
	{
	public:
		CInputManager();
		~CInputManager();

		void Update();


		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		void SetMouseCoords(float x, float y);

		bool IsKeyDown(unsigned int keyID);

		bool IsKeyPressed(unsigned int keyID);


		glm::vec2 GetMouseCoords() const { return _mouseCoords; }
	private:

		bool WasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previourKeyMap;
		glm::vec2 _mouseCoords;
	};
}