#include <MusicDecode.h> 

const unsigned char SEED_MAP[][7] = {
{0x4a, 0xd6, 0xca, 0x90, 0x67, 0xf7, 0x52},
{0x5e, 0x95, 0x23, 0x9f, 0x13, 0x11, 0x7e},
{0x47, 0x74, 0x3d, 0x90, 0xaa, 0x3f, 0x51},
{0xc6, 0x09, 0xd5, 0x9f, 0xfa, 0x66, 0xf9},
{0xf3, 0xd6, 0xa1, 0x90, 0xa0, 0xf7, 0xf0},
{0x1d, 0x95, 0xde, 0x9f, 0x84, 0x11, 0xf4},
{0x0e, 0x74, 0xbb, 0x90, 0xbc, 0x3f, 0x92},
{0x00, 0x09, 0x5b, 0x9f, 0x62, 0x66, 0xa1} };

namespace {
	class Mask {
	public:
		int NextMask() {
			unsigned short int ret;
			this->index++;
			if (this->x < 0) {
				this->dx = 1;
				this->y = (8 - this->y) % 8;
				ret = 0xc3;
			}
			else if (this->x > 6) {
				this->dx = -1;
				this->y = 7 - this->y;
				ret = 0xd8;
			}
			else {
				ret = SEED_MAP[this->y][this->x];
			}
			this->x += this->dx;
			if (this->index == 0x8000 || (this->index > 0x8000 && (this->index + 1) % 0x8000 == 0)) {
				return this->NextMask();
			}
			return ret;
		}
	private:
		int x = -1;
		int y = 8;
		int dx = 1;
		int index = -1;
	};
}

bool QmcDecrypt::Decrypt(unsigned char* raw_data, const size_t raw_size) {
	Mask* seed = new Mask();
	for (size_t i = 0; i < raw_size; ++i)
	{
		raw_data[i] ^= seed->NextMask();
	}
	delete seed;
	return true;
}


bool QmcDecrypt::Decrypt(const std::filesystem::path& file_path) {
	const std::string file_ext = file_path.extension().string();
	std::fstream file(file_path.string(), std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size_t file_size = file.tellg();
		unsigned char* file_raw = new unsigned char[file_size];
		file.seekg(0);
		file.read((char*)file_raw, file_size);
		Mask* seed = new Mask();
		for (size_t i = 0; i < file_size; ++i)
		{
			file_raw[i] ^= seed->NextMask();
		}
		file.seekg(0);
		file.write((char*)file_raw, file_size);
		file.close();
		delete seed;
		delete[] file_raw;
		return true;
	}
	return false;
}