#ifndef UNLOCKMUSIC_DECRYPT_HANDLER_HPP
#define UNLOCKMUSIC_DECRYPT_HANDLER_HPP

namespace unlockmusic {
	namespace decrypt {
		namespace handler {
			typedef std::pair<size_t, const char*> retType;
			typedef retType(*handler)(char*, const size_t&);
			struct Handler
			{
				handler handler;
				const char* ext;
			};
		}
	}
}

#endif
