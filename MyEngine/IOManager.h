#pragma once
#include <vector>

namespace MyEngine
{
	class CIOManager
	{
	public:
		static bool ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
		static bool ReadFileToBuffer(std::string filePath, std::string& buffer);

	};
}