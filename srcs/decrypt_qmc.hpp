#ifndef UNLOCKMUSIC_DECRYPT_QMC_HPP
#define UNLOCKMUSIC_DECRYPT_QMC_HPP

#include <string_view>
#include <map>

#include "decrypt_qmcMask.hpp"
#include "decrypt_handler.hpp"

namespace unlockmusic {
	namespace decrypt {
		namespace qqmusic {
			namespace handlers {
				static decrypt::handler::retType qmc(char* data, const size_t & size) {
					qqmusic::mask::QmcMask::decrypt(data, size, qqmusic::mask::matrix::QMCDefaultMaskMatrix::getMask128());
					return { size, nullptr };
				}

				static decrypt::handler::retType mgg(char* data, const size_t & size) {

				}

				static decrypt::handler::retType mflac(char* data, const size_t & size) {

				}
			}

			typedef std::map<std::string, decrypt::handler::Handler> mapType;
			static const mapType HandlerMap = {
				//{mapType::value_type{"mgg", {handlers::mgg, "ogg" }}},
				//{mapType::value_type{"mflac", {handlers::mflac, "flac" }}},
				{mapType::value_type{"qmc0", {handlers::qmc, "mp3" }}},
				{mapType::value_type{"qmc2", {handlers::qmc, "ogg" }}},
				{mapType::value_type{"qmc3", {handlers::qmc, "mp3" }}},
				{mapType::value_type{"qmcogg", {handlers::qmc, "ogg"}}},
				{mapType::value_type{"qmcflac", {handlers::qmc, "flac" }}},
				{mapType::value_type{"bkcmp3", {handlers::qmc, "mp3"}}},
				{mapType::value_type{"bkcflac", {handlers::qmc, "flac"}}},
				{mapType::value_type{"tkm", {handlers::qmc, "m4a"}}},
				{mapType::value_type{"666c6163", {handlers::qmc, "flac"}}},
				{mapType::value_type{"6d7033", {handlers::qmc, "mp3"}}},
				{mapType::value_type{"6f6767", {handlers::qmc, "ogg"}}},
				{mapType::value_type{"6d3461", {handlers::qmc, "flac"}}},
				{mapType::value_type{"776176", {handlers::qmc, "wav"}}}
			};

			static inline handler::Handler getHandler(const char* ext) {
				auto iter = HandlerMap.find(ext);
				if (iter == HandlerMap.end())
					return { nullptr, nullptr };
				auto Handler = iter->second;
				return Handler;
			}
		}
	}
}

#endif
