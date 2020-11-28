#ifndef UNLOCKMUSIC_DETECTAUDIOEXT_HPP
#define UNLOCKMUSIC_DETECTAUDIOEXT_HPP

namespace unlockmusic {
	namespace audioheaders {
		namespace flac {
			const char data[] = { 0x66, 0x4C, 0x61, 0x43 };
			const size_t size = 4;
		}
		namespace mp3 {
			const char data[] = { 0x49, 0x44, 0x33 };
			const size_t size = 3;
		}
		namespace ogg {
			const char data[] = { 0x4F, 0x67, 0x67, 0x53 };
			const size_t size = 4;
		}
		namespace m4a {
			const char data[] = { 0x66, 0x74, 0x79, 0x70 };
			const size_t size = 4;
		}
		namespace wma {
			const char data[] = { 0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
				0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
			const size_t size = 16;
		}
		namespace wav {
			const char data[] = { 0x52, 0x49, 0x46, 0x46 };
			const size_t size = 4;
		}
	}
}

#endif
