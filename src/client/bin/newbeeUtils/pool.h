#ifndef NEWBEE_POOL_H
#define NEWBEE_POOL_H
#include "newbeeUtils/exports.h"
#include "newbeeUtils/queue.h"
//�����������Դ����Դ���꣬���������ڴ����ͷŶ�ֻ�Ǳ��Ϊ�����С���
//���������ԴҪ��������������
//��void setId(int id); �� int getId() ��Ա������id��ֵ��һ��С�ڳ���Ԫ�������ķǸ������������Ϊ�����������±ꡣ
namespace NEWBEE_UTILS
{
template <class T, int MAX_POOL_SIZE>
class NBUAPI NBPool
{
public:
	int capacity(){
		return M_CAPACITY;
	}
	/*virtual*/ T* allocateResource(){//���󣬷��ؿ�
		if(m_idleResourceIds.size()>0){ //�п��е���Դ��
			int idleResourceId = -1;
			if(0 == m_idleResourceIds.dequeue(&idleResourceId)){
				m_container[idleResourceId]->setId(idleResourceId);//������Ҫ������������Դ��id��
				return m_container[idleResourceId];
			}
		}
		if(isFull()){
			return 0;
		}
		T* newResource = new T();
		if(newResource){
			m_container[m_first_unallocated_item_index] = newResource;
			newResource->setId(m_first_unallocated_item_index);//������ԴId
			m_first_unallocated_item_index++;
			return newResource;
		}else{
			return 0;
		}
	}
	/*virtual*/ int recycleResource(int id){
		if(id<0 || id>M_CAPACITY-1){
			return -2;
		}
		if(0 != m_container[id]){
			return m_idleResourceIds.enqueue(id);
		}else{
			return -1;
		}
	}
	inline bool isFull(){
		return size() >= M_CAPACITY;
	}
	inline /*virtual*/ int recycled_idle_size(){
		return m_idleResourceIds.size();
	}
	inline /*virtual*/ int unallocated_size(){
		return M_CAPACITY - m_first_unallocated_item_index;
	}
	inline /*virtual*/ int size(){//��Դ����
		//return unallocated_size() + recycled_idle_size();
		return m_first_unallocated_item_index - recycled_idle_size();
	}
	NBPool():M_CAPACITY(MAX_POOL_SIZE){
		NBPool_();
	}
	virtual ~NBPool(){
		NBPool__();
	}
	void NBPool_(){
		memset(m_container, 0, sizeof(m_container));
		m_first_unallocated_item_index = 0;
	}
	void NBPool__(){
		for(int i=0; i<M_CAPACITY; i++){
			if(m_container[i]){
				delete m_container[i];
			}
		}
		NBPool_();
	}
	inline bool isEmpty(){
		return 0 == size();
	}
	inline int addItem(const T &item){
		m_container[0];
	}
	inline void clear(){
		NBPool__();
	}
protected:
	NBQueue<int, MAX_POOL_SIZE> m_idleResourceIds;
	const int M_CAPACITY;
	T* m_container[MAX_POOL_SIZE];
	int m_first_unallocated_item_index;//ָ���һ��δ�������Դ�������Դ����δ����ġ�
};
//template <class T, int MAX_POOL_SIZE>
//class NBUAPI NBObjectPool : public NBPool<T, MAX_POOL_SIZE>
//{
//public:
//
//};
}
#endif // NEWBEE_POOL_H


