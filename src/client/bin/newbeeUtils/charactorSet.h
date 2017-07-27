#ifndef NEWBEE_CHARACTORSET_H
#define NEWBEE_CHARACTORSET_H
#include "newbeeUtils/exports.h"
namespace NEWBEE_UTILS{
class NBUAPI CharactorSet
{
public:
	enum CHARACTOR_SET{
		CST_NONE,
		CST_ANSI = 1, //ansi编码　　可以理解为各种国标码的统称。
		CST_GBK, //
		CST_JIS,
		CST_SBCS, //单字节字符集，不超过256个字符。例如纯西文ascii字符集，
		CST_ASCII, //单字节字符集的一种
		CST_UTF8,
		CST_UTF16LE,
		CST_UNICODE16,
		CST_UNKNOWN,
		//CST_NOT_SURE,
		//CST_NOT_CARE,
	};
	static const unsigned short MAX_CHARACTOR_SIZE_UTF8 = 4;
	static const unsigned short MAX_CHARACTOR_SIZE_ANSI = 2;
	static const unsigned short MAX_CHARACTOR_SIZE_SBCS = 1;
	static const unsigned short MAX_CHARACTOR_SIZE_UNICODE16 = 2;

	static const char *CHARACTOR_SET_UTF8;
	static const char *CHARACTOR_SET_UTF8_UPPERCASE;
	static const char *CHARACTOR_SET_UTF8_LOWERCASE;
	static const char *CHARACTOR_SET_UTF8_alias1;
	static const char *CHARACTOR_SET_UNICODE;
	static const char *CHARACTOR_SET_GBK;
	static CHARACTOR_SET convert_setTypeTextName_to_setTypeCode(const char *setTypeTextName);
	CharactorSet();
	~CharactorSet();
	static int UTF8CharLength(const char *buffer){
		return UTF8CharLength(static_cast<unsigned char>(buffer[0]));
	}
	static int UTF8CharLength(unsigned char firstUTF8Char);

	static CHARACTOR_SET getExecutableFileCharactorSet();
	static inline unsigned short queryCharactorSetMaxCharSize(CHARACTOR_SET set)
	{
		switch(set)
		{
		case CharactorSet::CST_GBK:
		case CharactorSet::CST_JIS:
		case CharactorSet::CST_ANSI:
			return MAX_CHARACTOR_SIZE_ANSI;
		case CharactorSet::CST_ASCII:
		case CharactorSet::CST_SBCS:
			return MAX_CHARACTOR_SIZE_SBCS;
		case CharactorSet::CST_UTF8:
			return MAX_CHARACTOR_SIZE_UTF8;
		case CharactorSet::CST_UNICODE16:
			return MAX_CHARACTOR_SIZE_UNICODE16;
		default:
			return 4;
		}
	}
	static inline unsigned short queryWesternCharSize(CHARACTOR_SET set)
	{
		switch(set)
		{
		case CharactorSet::CST_GBK:
		case CharactorSet::CST_JIS:
		case CharactorSet::CST_ANSI:
		case CharactorSet::CST_ASCII:
		case CharactorSet::CST_SBCS:
		case CharactorSet::CST_UTF8:
			return 1;
		case CharactorSet::CST_UNICODE16:
			return 2;
		default:
			return 1;
		}
	}
	static inline void convertChar_from_Unicode16_to_Utf8(const wchar_t unicodeChar, char *utf8Buffer)
	{//这个函数是非常不安全的。没有任何安全检查，但他是最快的。
		utf8Buffer[0] = 0xE0|(unicodeChar>>12);
		utf8Buffer[1] = 0x80|((unicodeChar>>6)&0x3F);
		utf8Buffer[2] = 0x80|(unicodeChar&0x3F);
		utf8Buffer[3] = '\0';
	}
	static inline wchar_t convertChar_from_Utf8_to_Unicode16(const char *utf8Buffer)
	{
		wchar_t unicodeChar = L'\0';
		unicodeChar = (utf8Buffer[0] & 0x1F) << 12;
		unicodeChar |= (utf8Buffer[1] & 0x3F) << 6;
		unicodeChar |= (utf8Buffer[2] & 0x3F);
		return unicodeChar;
	}
};
}
#endif //NEWBEE_CHARACTORSET_H
