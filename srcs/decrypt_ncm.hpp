#ifndef UNLOCKMUSIC_DECRYPT_NCM_HPP
#define UNLOCKMUSIC_DECRYPT_NCM_HPP

#include <openssl/bio.h>
#include <openssl/evp.h>

#include <ncmdump/base64.h>

#include "decrypt_handler.hpp"
#include "detectAudioExt.hpp"

namespace unlockmusic {
	namespace decrypt {
		namespace ncm {
			namespace {
				const char CORE_KEY[17] = { 0x68, 0x7A, 0x48, 0x52, 0x41, 0x6D, 0x73, 0x6F, 0x35, 0x6B, 0x49, 0x6E, 0x62, 0x61, 0x78, 0x57, 0 };
				const char MODIFY_KEY[17] = { 0x23, 0x31, 0x34, 0x6C, 0x6A, 0x6B, 0x5F, 0x21, 0x5C, 0x5D, 0x26, 0x30, 0x55, 0x3C, 0x27, 0x28, 0 };
				const char MagicHeader[] = { 0x43, 0x54, 0x45, 0x4E, 0x46, 0x44, 0x41, 0x4D };

				//aes-128-ecb解密
				unsigned int aes128_ecb_decrypt(const unsigned char* key, unsigned char* in, int in_len, unsigned char* out) {
					unsigned int outlen;
					int temp;

					EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
					EVP_CIPHER_CTX_init(ctx);
					EVP_CIPHER_CTX_set_padding(ctx, 1);

					if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), 0, key, NULL)) {
						return -1;
					}

					if (!EVP_DecryptUpdate(ctx, out, (int*)&outlen, in, in_len))
					{
						return -2;
					}

					if (!EVP_DecryptFinal_ex(ctx, out + outlen, &temp)) {
						return -3;
					}

					outlen += temp;

					EVP_CIPHER_CTX_cleanup(ctx);

					return outlen;
				}

				void buildKeyBox(unsigned char* keybox, unsigned char* key, int keyLen) {
					int i;
					for (i = 0; i < 256; ++i) {
						keybox[i] = (unsigned char)i;
					}

					unsigned char swap = 0;
					unsigned char c = 0;
					unsigned char last_byte = 0;
					unsigned char key_offset = 0;

					for (i = 0; i < 256; ++i)
					{
						swap = keybox[i];
						c = ((swap + last_byte + key[key_offset++]) & 0xff);
						if (key_offset >= keyLen)
							key_offset = 0;
						keybox[i] = keybox[c];
						keybox[c] = swap;
						last_byte = c;
					}
				}

				static decrypt::handler::retType Decrypt(char* data, const size_t& size) {
					//已读取字节
					size_t iByteRead = 0;

					//检查文件特征
					if (::memcmp(data, &MagicHeader, 8))
						throw std::invalid_argument("not netease protected file");
					iByteRead += 10;

					//获取AES密钥长度 4Byte
					auto iRawKeyLen = (unsigned int*)&data[iByteRead];
					if (*iRawKeyLen < 0)
						throw std::invalid_argument("broken ncm file");
					iByteRead += 4;

					//读取Key并解密
					auto rawkeydata = (char*)::malloc(sizeof(char) * *iRawKeyLen);
					::memcpy(rawkeydata, &data[iByteRead], *iRawKeyLen);
					iByteRead += *iRawKeyLen;

					for (auto i = 0; i < *iRawKeyLen; i++)
						rawkeydata[i] ^= 0x64;
					auto keydata = (char*)::malloc(sizeof(char) * *iRawKeyLen);
					auto iKeyLen = aes128_ecb_decrypt((uint8_t*)CORE_KEY, (uint8_t*)rawkeydata, *iRawKeyLen, (uint8_t*)keydata);

					//构建KeyBox
					auto keybox = (char*)::malloc(256);
					buildKeyBox((unsigned char*)keybox, (unsigned char*)keydata + 17, iKeyLen - 17);

					//获取Meta数据大小 4Byte
					auto iRawMetaSize = (unsigned int*)&data[iByteRead];
					iByteRead += 4;
					char* modifyData = nullptr;
					if (iRawMetaSize > 0) {
						modifyData = (char*)::malloc(sizeof(char) * *iRawMetaSize);
						::memcpy(modifyData, &data[iByteRead], *iRawMetaSize);
						iByteRead += *iRawMetaSize;

						//解密Meta数据
						for (auto i = 0; i < *iRawMetaSize; i++)
							modifyData[i] ^= 0x63;

						std::string modifyOutData;
						if (!Base64::Decode(std::string_view{ modifyData + 22, (size_t)*iRawMetaSize - 22 }, modifyOutData)) {
							auto modifyDecryptData = (char*)::malloc(256);
							auto iMetaSize = aes128_ecb_decrypt((uint8_t*)MODIFY_KEY, (uint8_t*)modifyOutData.c_str(), modifyOutData.size(), (uint8_t*)modifyDecryptData);
							std::string_view svJson{ &modifyDecryptData[6], (size_t)iMetaSize - 6 };
						}
						else {
							throw std::invalid_argument("broken metadata");
						}
					}

					//跳过专辑图CRC校验码
					iByteRead += 9;

					//获取专辑图片大小 4Byte
					auto iRawImageSize = (unsigned int*)&data[iByteRead];
					iByteRead += 4;
					char* imageData = nullptr;
					if (*iRawMetaSize > 0) {
						imageData = (char*)::malloc(sizeof(char) * *iRawImageSize);
						::memcpy(imageData, &data[iByteRead], *iRawImageSize);
						iByteRead += *iRawImageSize;
					}

					//剩下的都是音乐数据
					auto iMusicSize = size - iByteRead;
					::memmove(data, &data[iByteRead], iMusicSize);
					for (auto i = 0; i < iMusicSize; i++) {
						int j = (i + 1) & 0xff;
						data[i] ^= keybox[(keybox[j] + keybox[(keybox[j] + j) & 0xff]) & 0xff];
					}
					const char* ext = nullptr;

					if (::memcmp(audioheaders::flac::data, data, audioheaders::flac::size))
						ext = "mp3";
					else
						ext = "flac";
					
					free(rawkeydata);
					free(keydata);
					free(keybox);
					if (modifyData)
						free(modifyData);
					if (imageData)
						free(imageData);

					return { iMusicSize, ext };
				}
			}

		}
	}
}

#endif
