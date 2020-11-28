#ifndef UNLOCKMUSIC_DECRYPT_HPP
#define UNLOCKMUSIC_DECRYPT_HPP

#include <string>
#include <string_view>
#include <functional>

#include "decrypt_handler.hpp"

#include "decrypt_qmc.hpp"
#include "decrypt_tm.hpp"
#include "decrypt_ncm.hpp"

namespace unlockmusic {
	namespace decrypt {
        namespace {
            //¼ÆËãdataµÄHash
            constexpr std::uint32_t hash(const char* data)
            {
                std::uint32_t h(0);
                for (int i = 0; data && ('\0' != data[i]); i++)
                    h = (h << 6) ^ (h >> 26) ^ data[i];
                return h;
            }

            constexpr inline std::uint32_t hash(const std::string_view& str)
            {
                return hash(str.data());
            }
        }

		static handler::Handler getHandler(const std::string_view& ext) {
			switch (hash(ext))
			{
            case hash("tm0"):// QQ Music IOS Mp3
            case hash("tm3"):// QQ Music IOS Mp3
                return { decrypt::tm::Decrypt, "mp3" };
                break;
            case hash("tm2"):// QQ Music IOS M4a
            case hash("tm6"):// QQ Music IOS M4a
                return { decrypt::tm::Decrypt, "m4a" };
                break;
            case hash("qmc3")://QQ Music Android Mp3
            case hash("qmc2")://QQ Music Android Ogg
            case hash("qmc0")://QQ Music Android Mp3
            case hash("qmcflac")://QQ Music Android Flac
            case hash("qmcogg")://QQ Music Android Ogg
            case hash("tkm")://QQ Music Accompaniment M4a
            case hash("bkcmp3")://Moo Music Mp3
            case hash("bkcflac")://Moo Music Flac
            case hash("mflac")://QQ Music Desktop Flac
            case hash("mgg"): //QQ Music Desktop Ogg
            case hash("666c6163")://QQ Music Weiyun Flac
            case hash("6d7033")://QQ Music Weiyun Mp3
            case hash("6f6767")://QQ Music Weiyun Ogg
            case hash("6d3461")://QQ Music Weiyun M4a
            case hash("776176")://QQ Music Weiyun Wav
                return decrypt::qqmusic::getHandler(ext.data());
                break;
            case hash("ncm"):// Netease Mp3/Flac
                return {decrypt::ncm::Decrypt, nullptr};
                break;
            case hash("kwm"):// Kuwo Mp3/Flac
                //rt_data = await KwmDecrypt.Decrypt(file.raw, raw_filename, raw_ext);
                break;
            case hash("xm"): // Xiami Wav/M4a/Mp3/Flac
            case hash("wav"):// Xiami/Raw Wav
            case hash("mp3"):// Xiami/Raw Mp3
            case hash("flac"):// Xiami/Raw Flac
            case hash("m4a"):// Xiami/Raw M4a
                //rt_data = await XmDecrypt.Decrypt(file.raw, raw_filename, raw_ext);
                break;
            case hash("ogg"):// Raw Ogg
                //rt_data = await RawDecrypt.Decrypt(file.raw, raw_filename, raw_ext);
                break;
            case hash("vpr"):
            case hash("kgm"):
            case hash("kgma"):
                //rt_data = await KgmDecrypt.Decrypt(file.raw, raw_filename, raw_ext);
                break;
			default:
				break;
			}
            return {nullptr, nullptr};
		}
	}
}

#endif
