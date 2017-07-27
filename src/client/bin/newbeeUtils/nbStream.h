#ifndef NEWBEEUTILS_NBSTREAM_H
#define NEWBEEUTILS_NBSTREAM_H
#include "exports.h"
namespace NEWBEE_UTILS{
class NBUAPI NBStream
{ // һ���̶���С��buffer��ͷβ����ָ�룬ͷβ��ͬΪ�գ�β������׷��ͷ����һ��Ϊ����
public:
	int available(); // ���ж����ֽ�
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
	//bool empty_or_full; // ��ͷβ�غ�ʱ���ǿջ�������false����գ�true��������
}; //class NBStream
}  // namespace NEWBEE_UTILS
#endif //NEWBEEUTILS_NBSTREAM_H
