#include <MusicDecode.h>
const unsigned char FLAC_HEADER[] = { 0x66, 0x4C, 0x61, 0x43, 0x00 };


namespace {
	class Mask {
	public:
		Mask() {
			for (int i = 0; i < 128; ++i)
				this->mask[i] = 0;
		}
		int NextMask() {
			this->index++;
			this->mask_index++;
			if (this->index == 0x8000 || (this->index > 0x8000 && (this->index + 1) % 0x8000 == 0)) {
				this->index++;
				this->mask_index++;
			}
			if (this->mask_index >= 128) {
				this->mask_index -= 128;
			}
			return this->mask[this->mask_index];
		}
	private:
		int index = -1;
		int mask_index = -1;
		int mask[128];
	};
}