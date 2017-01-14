#include "InputManager.h"


namespace MyEngine
{
	CInputManager::CInputManager() : _mouseCoords(0.0f, 0.0f)
	{
	}


	CInputManager::~CInputManager()
	{
	}

	void CInputManager::Update()
	{
		for (auto& it : _keyMap)
		{
			_previourKeyMap[it.first] = it.second;
		}
	}

	void CInputManager::PressKey(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}

	void CInputManager::ReleaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void CInputManager::SetMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool CInputManager::IsKeyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	bool CInputManager::IsKeyPressed(unsigned int keyID)
	{
		bool isPressed;
		if (IsKeyDown(keyID) == true && WasKeyDown(keyID) == false)
		{
			return true;
		}
		return false;
	}

	bool CInputManager::WasKeyDown(unsigned int keyID)
	{
		auto it = _previourKeyMap.find(keyID);
		if (it != _previourKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}
}