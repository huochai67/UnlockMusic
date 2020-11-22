#include <iostream>
#include <string_view>
#include <fstream>
#include <filesystem>

#include <cstdio>

#include "decrypt.hpp"

using std::cout;
using std::endl;


void print() {

}

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

				Handler.handler(data, file_size);

				std::string file_noext = file_path.parent_path().string() + "\\" + file_path.stem().string();
				std::fstream file2(file_noext + "." + Handler.ext, std::ios::out | std::ios::binary | std::ios::trunc);
				if (file2.is_open())
				{
					file2.write(data, file_size);
					file2.close();
				}
			}
		}
	}
}

void decrypt(const std::filesystem::path& file_path) {
	if (std::filesystem::is_directory(file_path)) {
		std::filesystem::directory_iterator list(file_path);
		for (auto x : list)
		{
			if (x.is_directory())
			{
				decrypt(x.path());
			}
			else
			{
				AutoDecrypt(file_path);
			}
		}
	}
	else {
		AutoDecrypt(file_path);
	}
}

int main(int argc, char* argv[]) {
	decrypt("D:\\test\\qmcflac.qmcflac");
	//for (int i = 1; i < argc; ++i)
	//	decrypt(argv[i]);
	return 0;
}