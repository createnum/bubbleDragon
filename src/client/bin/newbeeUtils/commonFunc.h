#ifndef NEWBEE_UTILS_COMMONFUNC_H
#define NEWBEE_UTILS_COMMONFUNC_H
#include "newbeeUtils/exports.h"
#include "newbeeUtils/basicChain.h"
#define RELEASE_POINTER(obj)             {if(obj){delete obj;obj = 0;}}
#define RELEASE_ARRAY_POINTER(obj)             {if(obj){delete[] obj;obj = 0;}}

namespace NEWBEE_UTILS{

	NBUAPI char *allocateMem(int len);
	template<class T>
	NBUAPI T *allocateMem1(int len){
		return new T[len];
	}
	/*
	enum OS_ENDIAN {
		NEWBEE_LITTLE_ENDIAN,
		NEWBEE_BIG_ENDIAN
	};
	*/
	enum OS_ENDIAN{NEWBEE_LITTLE_ENDIAN, NEWBEE_BIG_ENDIAN};
	NBUAPI void releaseMem(const char *ptr);
	NBUAPI bool isMemZero(const char * ptr, int len);
	NBUAPI int mem_swap_cpy_by_byte(char* des,char* src, int len);
	NBUAPI bool memcmp_byte_by_byte(const char * ptr, unsigned char content, int len);
	NBUAPI bool sign_of_bin(const char * ptr, int len_of_bytes, OS_ENDIAN endianWay);
	NBUAPI int count_of_effective_byte(const char * const src, const int len, OS_ENDIAN desEndianWay, bool isUnsigned);

	NBUAPI 	int copy_digital_rightly(char *desPtr, int desLen, OS_ENDIAN desEndianWay,
		const char *srcPtr, int srcLen, OS_ENDIAN srcEndianWay,
		bool isUnsigned); //把二进制里的数当成有符号数还是无符号数来看待。
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline int convert_unsigned_digital_2_bin(char *desPtr,const T srcDigital, 
		OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
			return copy_digital_rightly(desPtr, sizeof(T), desEndianWay, 
				reinterpret_cast<const char*>(&srcDigital), sizeof(T), srcEndianWay, true);
	}
	template<class T>
	inline int convert_signed_digital_2_bin(char *desPtr, const T srcDigital, 
		OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
			return copy_digital_rightly(desPtr, sizeof(T), desEndianWay, 
				reinterpret_cast<const char*>(&srcDigital), sizeof(T), srcEndianWay, false);
	}
	template<class T>
	inline int convert_bin_2_unsigned_digital(T *desDigital, const char * srcPtr, unsigned int srcLen_of_bytes, 
		OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
			return copy_digital_rightly(reinterpret_cast<char*>(desDigital), sizeof(T), desEndianWay, 
				srcPtr, srcLen_of_bytes, srcEndianWay, true);
	}
	template<class T>
	inline int convert_bin_2_unsigned_digital(T *desDigital, const char * srcPtr, 
		OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
			return copy_digital_rightly(reinterpret_cast<char*>(desDigital), sizeof(T), desEndianWay, 
				srcPtr, sizeof(T), srcEndianWay, true);
	}
	template<class T>
	inline int convert_bin_2_signed_digital(T *desDigital, const char * srcPtr, int srcLen_of_bytes, 
		OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
			return copy_digital_rightly(reinterpret_cast<char*>(desDigital), sizeof(T), desEndianWay, 
				srcPtr, srcLen_of_bytes, srcEndianWay, false);
	}
	//NBUAPI inline int convert_bin_2_digital(int *desDigital, const char * srcPtr, int srcLen_of_bytes, 
	//	OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
	//		return convert_bin_2_digital_mem(reinterpret_cast<char*>(desDigital), sizeof(int), desEndianWay, 
	//			srcPtr, srcLen_of_bytes, srcEndianWay, true);
	//}
	//NBUAPI inline int convert_bin_2_digital(unsigned int *desDigital, const char * srcPtr, int srcLen_of_bytes, 
	//	OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
	//		return convert_bin_2_digital_mem(reinterpret_cast<char*>(desDigital), sizeof(unsigned int), desEndianWay, 
	//			srcPtr, srcLen_of_bytes, srcEndianWay, true);
	//}
	//NBUAPI inline int convert_bin_2_digital(short *desDigital, const char * srcPtr, int srcLen_of_bytes, 
	//	OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
	//		return convert_bin_2_digital_mem(reinterpret_cast<char*>(desDigital), sizeof(short), desEndianWay, 
	//			srcPtr, srcLen_of_bytes, srcEndianWay, false);
	//}
	//NBUAPI inline int convert_bin_2_digital(unsigned short *desDigital, const char * srcPtr, int srcLen_of_bytes, 
	//	OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
	//		return convert_bin_2_digital_mem(reinterpret_cast<char*>(desDigital), sizeof(unsigned short), desEndianWay, 
	//			srcPtr, srcLen_of_bytes, srcEndianWay, true);
	//}
	//NBUAPI inline int convert_bin_2_digital(char *desDigital, const char * srcPtr, int srcLen_of_bytes, 
	//	OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
	//		return convert_bin_2_digital_mem(reinterpret_cast<char*>(desDigital), sizeof(char), desEndianWay, 
	//			srcPtr, srcLen_of_bytes, srcEndianWay, false);
	//}
	//NBUAPI inline int convert_bin_2_digital(char *desDigital, const char * srcPtr, int srcLen_of_bytes, 
	//	OS_ENDIAN desEndianWay = NEWBEE_LITTLE_ENDIAN, OS_ENDIAN srcEndianWay = NEWBEE_LITTLE_ENDIAN){
	//		return convert_bin_2_digital_mem(reinterpret_cast<char*>(desDigital), sizeof(unsigned char), desEndianWay, 
	//			srcPtr, srcLen_of_bytes, srcEndianWay, true);
	//}
  
	NBUAPI OS_ENDIAN endian();
	NBUAPI bool is_little_endian();

	NBUAPI const unsigned int MAX_ERROR_MSG_LENGTH = 1024;
	NBUAPI const unsigned int MAX_DIR_LENGTH =1024;
	NBUAPI int memswap(char *des, const char*src,  int len_of_bytes);
	//extern const unsigned int MAX_ERROR_MSG_LENGTH =1024;
	//extern const unsigned int MAX_DIR_LENGTH = 256;
	//extern NBUAPI const unsigned int MAX_ERROR_MSG_LENGTH;
	//extern NBUAPI const unsigned int MAX_DIR_LENGTH;
	NBUAPI BasicPointerChain<char> *split(const char* str, const char c);
	//NBUAPI bool getCurDir(char *dirBuffer, unsigned int bufferCapacity, char* errorMsg=0);
	//NBUAPI bool getCurDir(char *dirBuffer, unsigned int bufferCapacity, char* errorMsg, unsigned int errorMsgLen);
	NBUAPI int readLine(FILE *fp, char * lineBuffer, int len);

}
#endif // !defined(NEWBEE_UTILS_COMMONFUNC_H)


