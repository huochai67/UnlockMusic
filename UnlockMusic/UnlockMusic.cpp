#include <iostream>
#include <filesystem>

#include <MusicDecode.h>

using std::cout;
using std::endl;

bool filedecrypt(const std::filesystem::path& file_path)
{
	if (file_path.has_extension())
	{
		std::string file_ext = file_path.extension().string();
		std::string file_noext = file_path.parent_path().string() + "\\" + file_path.stem().string();
		if (file_ext == ".qmc3") {
			if (QmcDecrypt::Decrypt(file_path)) {
				std::filesystem::rename(file_path, file_noext + ".mp3");
				return true;
			}
		}
		else if (file_ext == ".qmc0") {
			if (QmcDecrypt::Decrypt(file_path)) {
				std::filesystem::rename(file_path, file_noext + ".mp3");
				return true;
			}
		}
		else if (file_ext == ".qmcflac") {
			if (QmcDecrypt::Decrypt(file_path)) {
				std::filesystem::path file_new(file_noext + ".flac");
				std::filesystem::rename(file_path, file_new);
				return true;
			}
		}
		else if (file_ext == ".qmcogg") {

		}
		else if (file_ext == ".mflac") {

		}
		else if (file_ext == ".ncm") {

		}
	}
	return false;
}

void dirdecrypt(const std::filesystem::path& file_path) {
	std::filesystem::directory_iterator list(file_path);
	for (auto x : list)
	{
		if (x.is_directory())
		{
			dirdecrypt(x.path());
		}
		else
		{
			filedecrypt(x.path());
		}
	}
}

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i)
	{
		std::filesystem::path file_path(argv[i]);
		if (std::filesystem::is_directory(file_path))
		{
			dirdecrypt(file_path);
		}
		else {
			if (filedecrypt(file_path)){
				cout << file_path.filename() << "解密成功" << endl;
			}
			else {
				cout << file_path.filename() << "解密失败" << endl;
			}
		}
	}
	return 0;
}