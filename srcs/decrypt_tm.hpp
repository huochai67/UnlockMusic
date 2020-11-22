#ifndef UNLOCKMUSIC_DECRYPT_TM_HPP
#define UNLOCKMUSIC_DECRYPT_TM_HPP

#include "decrypt_handler.hpp"

namespace unlockmusic {
	namespace decrypt {
		namespace tm {
			const char TM_HEADER[] = { 0x00, 0x00, 0x00, 0x20, 0x66, 0x74, 0x79, 0x70 };

			static constexpr void Decrypt(char* data) {
				for (auto cur = 0; cur < 8; ++cur) {
					data[cur] = TM_HEADER[cur];
				}
			}

			static constexpr void Decrypt(char* data, const size_t& size) {
				Decrypt(data);
			}

			static constexpr inline decrypt::handler::Handler getHandler() {
				return { Decrypt, nullptr };
			}

		}
	}
}

#endif
