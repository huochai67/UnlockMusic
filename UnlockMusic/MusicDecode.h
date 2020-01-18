#pragma once
#include <filesystem>
#include <fstream>

namespace QmcDecrypt {
	bool Decrypt(unsigned char* raw_data, const size_t raw_size);
	bool Decrypt(const std::filesystem::path&);
}