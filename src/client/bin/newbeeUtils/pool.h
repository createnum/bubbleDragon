#ifndef NEWBEE_POOL_H
#define NEWBEE_POOL_H
#include "newbeeUtils/exports.h"
#include "newbeeUtils/queue.h"
//负责分配新资源，资源用完，不真正从内存中释放而只是标记为“空闲”。
//被分配的资源要满足如下条件：
//有void setId(int id); 和 int getId() 成员函数。id的值是一个小于池内元素总数的非负数，可以理解为自增的数组下标。
namespace NEWBEE_UTILS
{
template <class T, int MAX_POOL_SIZE>
class NBUAPI NBPool
{
public:
	int capacity(){
		return M_CAPACITY;
	}
	/*virtual*/ T* allocateResource(){//错误，返回空
		if(m_idleResourceIds.size()>0){ //有空闲的资源。
			int idleResourceId = -1;
			if(0 == m_idleResourceIds.dequeue(&idleResourceId)){
				m_container[idleResourceId]->setId(idleResourceId);//这里需要，重新设置资源的id。
				return m_container[idleResourceId];
			}
		}
		if(isFull()){
			return 0;
		}
		T* newResource = new T();
		if(newResource){
			m_container[m_first_unallocated_item_index] = newResource;
			newResource->setId(m_first_unallocated_item_index);//设置资源Id
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
	inline /*virtual*/ int size(){//资源总数
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
	int m_first_unallocated_item_index;//指向第一个未分配的资源项。其后的资源都是未分配的。
};
//template <class T, int MAX_POOL_SIZE>
//class NBUAPI NBObjectPool : public NBPool<T, MAX_POOL_SIZE>
//{
//public:
//
//};
}
#endif // NEWBEE_POOL_H


