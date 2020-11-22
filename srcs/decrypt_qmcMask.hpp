#ifndef UNLOCKMUSIC_DECRYPT_QMCMASK_HPP
#define UNLOCKMUSIC_DECRYPT_QMCMASK_HPP

#include <vector>
#include <memory>
#include <mutex>
#include <map>

namespace unlockmusic {
	namespace decrypt {
		namespace qqmusic {
			namespace mask {
				class QmcMask {
				public:
					~QmcMask() {}

					static QmcMask* getInstance()
					{
						static std::once_flag onceFlag;
						std::call_once(onceFlag, [&] {m_qmcmask = new QmcMask(); });
						return m_qmcmask;
					};

					//inline std::map<int, std::vector<int>> get_allMapping() { return this->m_allMapping; }

					static void decrypt(char* data, const size_t& size, const char* matrix128) {
						int index = -1;
						int maskIdx = -1;
						for (int cur = 0; cur < size; cur++) {
							index++;
							maskIdx++;
							if (index == 0x8000 || (index > 0x8000 && (index + 1) % 0x8000 == 0)) {
								index++;
								maskIdx++;
							}
							if (maskIdx >= 128)
								maskIdx -= 128;
							data[cur] ^= matrix128[maskIdx];
						}
					}

					void generateMask128from44(char* dst, const char* src) {
						auto idx44 = 0;
						for (auto& it256 : this->m_allMapping) {
							for (auto& m : it256.second)
								dst[m] = src[idx44];
							idx44++;
						};
					}

				private:
					QmcMask() {
						for (auto i = 0; i < 128; i++) {
							auto realIdx = (i * i + 27) % 256;
							if (this->m_allMapping.find(realIdx) != this->m_allMapping.end())
								this->m_allMapping[realIdx].push_back(i);
							else
								this->m_allMapping[realIdx] = { i };
						}

						auto idx44 = 0;
						for (auto& all128 : this->m_allMapping) {
							for (auto& _i128 : this->m_allMapping)
								this->m_mask128to44[_i128.first] = { idx44 };
							idx44++;
						};
					};
					QmcMask(QmcMask const&) = delete;
					QmcMask& operator=(QmcMask const&) = delete;

					static QmcMask* m_qmcmask;

					std::map<int, std::vector<int>> m_allMapping;
					std::map<int, std::vector<int>> m_mask128to44;
				};
				QmcMask* QmcMask::m_qmcmask = nullptr;

				//¼ÓÃÜÃÜ³×
				namespace matrix {
					namespace QMOggPublicHeader1
					{
						const char data[84] = {
							0x4f, 0x67, 0x67, 0x53, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
							0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x1e, 0x01, 0x76, 0x6f, 0x72,
								0x62, 0x69, 0x73, 0x00, 0x00, 0x00, 0x00, 0x02, 0x44, 0xac, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0xee, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x4f, 0x67, 0x67, 0x53, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
								0xff, 0xff, 0xff, 0xff };
						const size_t size = 84;
					};
					namespace QMOggPublicHeader2
					{
						const char data[69] = {
							0x03, 0x76, 0x6f, 0x72, 0x62, 0x69, 0x73, 0x2c, 0x00, 0x00, 0x00, 0x58, 0x69, 0x70, 0x68, 0x2e,
							0x4f, 0x72, 0x67, 0x20, 0x6c, 0x69, 0x62, 0x56, 0x6f, 0x72, 0x62, 0x69, 0x73, 0x20, 0x49, 0x20,
							0x32, 0x30, 0x31, 0x35, 0x30, 0x31, 0x30, 0x35, 0x20, 0x28, 0xe2, 0x9b, 0x84, 0xe2, 0x9b, 0x84,
							0xe2, 0x9b, 0x84, 0xe2, 0x9b, 0x84, 0x29, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x54,
							0x49, 0x54, 0x4c, 0x45, 0x3d };
						const size_t size = 69;
					};
					namespace QMOggPublicConf1
					{
						const char data[84] = {
							9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0,
							0, 0, 9, 9, 9, 9, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9,
							9, 9, 9, 9, 9, 9, 9, 6, 3, 3, 3, 3, 6, 6, 6, 6,
							3, 3, 3, 3, 6, 6, 6, 6, 6, 9, 9, 9, 9, 9, 9, 9,
							9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 9, 9, 9, 9,
							0, 0, 0, 0 };
						const size_t size = 84;
					};
					namespace QMOggPublicConf2
					{
						const char data[69] = {
							3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
							3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
							3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
							3, 3, 3, 3, 3, 3, 3, 0, 1, 3, 3, 0, 1, 3, 3, 3,
							3, 3, 3, 3, 3 };
						const size_t size = 69;
					};
					namespace QMCDefaultMaskMatrix
					{
						const char data[44] = {
							0xde, 0x51, 0xfa, 0xc3, 0x4a, 0xd6, 0xca, 0x90,
							0x7e, 0x67, 0x5e, 0xf7, 0xd5, 0x52, 0x84, 0xd8,
							0x47, 0x95, 0xbb, 0xa1, 0xaa, 0xc6, 0x66, 0x23,
							0x92, 0x62, 0xf3, 0x74, 0xa1, 0x9f, 0xf4, 0xa0,
							0x1d, 0x3f, 0x5b, 0xf0, 0x13, 0x0e, 0x09, 0x3d,
							0xf9, 0xbc, 0x00, 0x11 };
						const size_t size = 44;

						namespace {
							static char* Mask128 = nullptr;
						}
						static char* getMask128() {
							if (!Mask128){
								Mask128 = (char*)::malloc(128);
								unlockmusic::decrypt::qqmusic::mask::QmcMask::getInstance()->generateMask128from44(Mask128, unlockmusic::decrypt::qqmusic::mask::matrix::QMCDefaultMaskMatrix::data);
							}
							return Mask128;
						}
					};
				}
			}
		}
	}
}

#endif
