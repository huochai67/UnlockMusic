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
				static void default(char* data, const size_t & size) {
					qqmusic::mask::QmcMask::decrypt(data, size, qqmusic::mask::matrix::QMCDefaultMaskMatrix::getMask128());
				}

				static void mgg(char* data, const size_t & size) {

				}

				static void mflac(char* data, const size_t & size) {

				}
			}


			static const std::map<std::string, decrypt::handler::Handler> HandlerMap = {
				{std::map<std::string, decrypt::handler::Handler>::value_type{"mgg", {handlers::mgg, "ogg" }}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"mflac", {handlers::mflac, "flac" }}},

				{std::map<std::string, decrypt::handler::Handler>::value_type{"qmc0", {handlers::default, "mp3" }}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"qmc2", {handlers::default, "ogg" }}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"qmc3", {handlers::default, "mp3" }}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"qmcogg", {handlers::default, "ogg"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"qmcflac", {handlers::default, "flac" }}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"bkcmp3", {handlers::default, "mp3"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"bkcflac", {handlers::default, "flac"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"tkm", {handlers::default, "m4a"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"666c6163", {handlers::default, "flac"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"6d7033", {handlers::default, "mp3"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"6f6767", {handlers::default, "ogg"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"6d3461", {handlers::default, "flac"}}},
				{std::map<std::string, decrypt::handler::Handler>::value_type{"776176", {handlers::default, "wav"}}}
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
