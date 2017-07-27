#ifndef NEWBEE_NBSTRING_H
#define NEWBEE_NBSTRING_H

//#include "newbeeUtils/tstring.h"
#include "newbeeUtils/basicChain.h"
#include "newbeeUtils/exports.h"
namespace NEWBEE_UTILS{
class NBUAPI NBString{
public:
    inline int len(){
        return m_len;
    }
    inline void setLen(int len){
        m_len = len;
    }
    static const int M_CAPACITY = 256;
    static NBString NBNULL;
    inline const char* buf(){
        return m_data;
    }
    inline char* writePtr(){
        return m_data+m_len;
    }
    bool operator ==(const char*);
    bool operator ==(const NBString &);
    const NBString& operator+=(char c);
    const NBString& operator=(char c);
    NBString operator+(char c){
        NBString sum(*this);
        return sum += c;
    }
    const NBString& operator+=(int i);
    const NBString& operator=(int i);
    NBString operator+(int i){
        NBString sum(*this);
        return sum += i;
        //return sum;
    }
    const NBString& operator+=(const char *);
    const NBString& operator=(const char *);
    NBString operator+(const char* value)
    {
        NBString sum(*this);
        sum += value;
        return sum;
    }
    NBString(const char*);
    NBString(char);
    NBString(int);
    NBString();
    virtual void clear();
protected:
    static const int M_BUFFER_LEN = M_CAPACITY+1;
    inline bool _exceed(int len){
        return m_len+len > M_CAPACITY;
    }
    int m_len;
private:
    char m_data[M_BUFFER_LEN];
};

class NBUAPI NBStrSpliter 
{//
public:
	class NBUAPI Spliter //目前它的唯一作用是解除调用模块和这个dll的编译依赖性。
	{
	public:
		Spliter(){
		}
		~Spliter();
		BasicPointerChain<char> *getChain(){
			return &m_chain;
		}
		void clear(); // 这个不能用！不能解决编译依赖性问题。为什么？
	protected:
		BasicPointerChain<char> m_chain;
	};
	static const unsigned int ASCII_CASE_DIFFERENCE = 'a' - 'A';//ASCII 码大小写字母之间的差
	enum SPLIT_ALLOCATE_ORDER{
		SAO_NORMAL,
		SAO_REVERSE,
	};
	static inline int trim(char **retPPtr, char *pSrcStr){//移除两端的空格。
		NEWBEE_UTILS::BasicChain<char> chain;
		char firstChar = ' ';
		chain.allocateLast(firstChar);
		return trim(retPPtr, pSrcStr, &chain);
	}
	static int trim(char **retPPtr, char *pSrcStr, BasicChain<char>* prefixChain);
	~NBStrSpliter();
	const inline char *c_str()
	{
		return m_str;
	}
	char *str()
	{
		return m_str;
	}
	static inline bool startWith(const char *str, const char firstChar){
		return str[0] == firstChar;
	}
	static inline bool isEqual(const char *str, const char theChar)
	{
		return theChar == str[0] && '\0' == str[1];
	}
	static inline bool isEqual(const char theChar, const char *str)
	{
		return theChar == str[0] && '\0' == str[1];
	}
	static inline void removeLn(char *str){
		removeLast(str, '\n');
	}

	static bool startWith(const char *str, const char *subStr);
	static bool endWith(const char *str, const char charactor);
	static bool isContain(const char *str, const char charactor);
	static int findSubString(const char *str, const char *subStr);
	static int findSubString(const char *str, const char ch);
	inline BasicPointerChain<char> *split(const char* str, const char *spliterStr, SPLIT_ALLOCATE_ORDER order = SAO_NORMAL, bool isCreate_for_emptyStr = false)
	{
		if(0 != NBStrSpliter::split(str, spliterStr, &m_mbStringChain, order, isCreate_for_emptyStr)){
			return 0;
		}
		return &m_mbStringChain;
	}
 	inline BasicPointerChain<char> *split(const char* str, const char c, SPLIT_ALLOCATE_ORDER order = SAO_NORMAL)
 	{
		if(0 != NBStrSpliter::split(str, c, &m_mbStringChain, order)){
			return 0;
		}
		return &m_mbStringChain;
 	}
	static int split(const char *str, const char *separatorStr, BasicPointerChain<char> *chain, SPLIT_ALLOCATE_ORDER order = SAO_NORMAL, bool isCreate_for_emptyStr = false);
	static int split(const char *str, const char c, BasicPointerChain<char> *chain, SPLIT_ALLOCATE_ORDER order = SAO_NORMAL, bool isCreate_for_emptyStr = false);
	static char* split2(char *str, const char ch);
	static void toLowerCase(char *str, size_t len);
	static void toUpperCase(char *str, size_t len);
	static void toLowerCase(char *str);
	static void toUpperCase(char *str);
	NBStrSpliter()
	{
		m_str = 0;
	}
	NBStrSpliter(int length)
	{
		m_str = new char[length+1];
		m_str[length] = '\0';
	}

protected:
	static void removeLast(char *str, const char ch);
	static void removeLast(char *str);
	static int skipSubStringsR(const char *str, const char ch);
	static int skipSubStrings(const char *str, const char ch);

	static int skipSubStrings(const char *str, const char *subStr);

	static int createNode(const char* srcStr, BasicPointerChain<char> *chain, SPLIT_ALLOCATE_ORDER order, int newSubStringLength, bool isCreateNode_for_nullStr = false);
	static int findnCharR(const char *str, const char ch, size_t len);
	static char **createChainNode(BasicPointerChain<char> *chain, SPLIT_ALLOCATE_ORDER order);
	char* m_str;
	static bool isEqual4firstNCharactor(const char *str1, const char *str2, size_t n);
	BasicPointerChain<char> m_mbStringChain;
};

}
#endif //NEWBEE_NBSTRING_H
