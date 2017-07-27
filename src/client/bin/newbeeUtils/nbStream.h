#ifndef NEWBEEUTILS_NBSTREAM_H
#define NEWBEEUTILS_NBSTREAM_H
#include "exports.h"
namespace NEWBEE_UTILS{
class NBUAPI NBStream
{ // 一个固定大小的buffer，头尾两个指针，头尾相同为空，尾不可以追上头，差一即为满。
public:
	int available(); // 空闲多少字节
	int write(const char* buf, int len);
	int read(char* buf, int len);
	//BOOL	ReadPacket( Packet* p ) ;
	int peek(char* buf, int len);
	int skip( int len ) ;
	inline static int capacity(){
		return BUFFER_CAPACITY-1;
	}
	int size() const;
	inline char* getBuffer(){
		return m_buffer;
	}
	inline bool	empty() const {
		return m_head == m_tail;
	}
    NBStream(){
        NBStream_();
    }
    virtual ~NBStream(){
        NBStream__();
    }
    int clear();
protected: //simple object(including unliable pointer) - (1) liable pointer - (2) complex object - (3) father class - (4)
	inline void head_forward(int len){
		m_head = (m_head+len)%BUFFER_CAPACITY;
	}
	inline void tail_forward(int len){
		m_tail = (m_tail+len)%BUFFER_CAPACITY;
	}
	static const int BUFFER_CAPACITY = 8;
    void NBStream_();
    int NBStream__();
	char m_buffer[BUFFER_CAPACITY];
	int	m_head;
	int m_tail;
	//bool empty_or_full; // 当头尾重合时，是空还是满，false代表空，true代表满。
}; //class NBStream
}  // namespace NEWBEE_UTILS
#endif //NEWBEEUTILS_NBSTREAM_H
