#include <iostream>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <format>

#include <cstdio>

#include "decrypt.hpp"

void AutoDecrypt(const std::filesystem::path& file_path)
{
	if (file_path.has_extension())
	{
		auto str_ext = file_path.extension().string();
		str_ext = str_ext.substr(1, str_ext.size() - 1);
		auto Handler = unlockmusic::decrypt::getHandler(str_ext);

		if (Handler.handler) {
			std::fstream file(file_path.string(), std::ios::in | std::ios::binary | std::ios::ate);
			if (file.is_open())
			{
				size_t file_size = file.tellg();
				file.seekg(0);

				char* data = (char*)::malloc(file_size);
				file.read(data, file_size);
				file.close();

				auto ret = Handler.handler(data, file_size);

				std::string file_noext = file_path.parent_path().string() + "\\" + file_path.stem().string();
				std::string outfilename = file_noext + "." + (Handler.ext ? Handler.ext : ret.second);
				std::fstream file2(outfilename, std::ios::out | std::ios::binary | std::ios::trunc);
				if (file2.is_open())
				{
					file2.write(data, ret.first);
					file2.close();
				}
				free(data);
			}
		}
	}
}

int iTask = 0;
int iFinished = 0;
#define PBWIDTH 60
void printprocessbar(const char* name) {

	auto percent = (float)iFinished / (float)iTask;
	int i = PBWIDTH * percent;
	std::cout << std::format("\r{} {:.1f}% [{:#>{}}]", name, percent * 100, std::format("{: >{}}", " ", PBWIDTH - i), PBWIDTH);
	fflush(stdout);
}

void decrypt(const std::filesystem::path& file_path) {
	if (std::filesystem::is_directory(file_path)) {
		std::filesystem::directory_iterator list(file_path);
		for (auto x : list)
			if (x.is_directory())
			{
				decrypt(x.path());
				return;
			}
	}
	iTask++;
	printprocessbar(file_path.string().c_str());
	AutoDecrypt(file_path);
	iFinished++;
}

int main(int argc, char* argv[]) {
#ifdef _DEBUG
	decrypt("sample/sample.ncm");
#else
	for (int i = 1; i < argc; ++i)
		decrypt(argv[i]);
#endif // _DEBUG
	printprocessbar("finished");
	return 0;
}