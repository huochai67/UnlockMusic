#ifndef UNLOCKMUSIC_DECRYPT_HANDLER_HPP
#define UNLOCKMUSIC_DECRYPT_HANDLER_HPP

namespace unlockmusic {
	namespace decrypt {
		namespace handler {
			typedef void(*handler)(char*, const size_t&);
			struct Handler
			{
				handler handler;
				const char* ext;
			};
		}
	}
}

#endif
