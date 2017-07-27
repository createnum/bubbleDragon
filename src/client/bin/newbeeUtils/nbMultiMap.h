#ifndef NEWBEE_UTILS_MULTIMAP_H
#define NEWBEE_UTILS_MULTIMAP_H
//多键值映射
// key 值 必须 大于 
// MAP_BASIC_SIZE 表示构造时分配的元素个数。所以，使用时要考虑，内存的使用。该值不宜巨大。或者 类T是复杂类，应保存指针。也不宜过小，否则效率极低。
#include "basicChain.h"
#include "nbMap.h"
//-------------------------
//| 0 | 1 | 2 | 3 | 4 | 5 |    基容器：数组
//-------------------------
//  |
//  ^
namespace NEWBEE_UTILS
{
template<class T>
struct SameKeyNode{ // 尾巴上的每个节点上的分支，每个分支都保存了同样key值的节点。
		//int key;
		T data;
		SameKeyNode *nextNode;
		SameKeyNode(const T& d){
			this->data = d;
			this->nextNode = 0;
		}
};
template<class T, int MAP_BASIC_SIZE>
class NBUAPI NBMultiMap
{
protected:

	//struct SameKeyChain{ // 尾巴上的每个节点上的分支，每个分支都保存了同样key值的节点。
	//	//int multiKey;
	//	HookChain<SameKeyNode> nodeChain;
	//	//SameKeyChain(int key){
	//	//	this->multiKey = key;
	//	//}
	//};
	//struct TailNode{ // 基容器后面的尾巴。
	//	int key;
	//	T data;
	//	TailNode *nextNode;
	//	SameKeyChain *firstSameKeyNode;
	//};
public:
	typedef NBPointerMap<HookChain<SameKeyNode<T> >, MAP_BASIC_SIZE> UNIQUE_KEY_MAP_CONTAINER;
	//T *find(int key){
	//	HookChain<SameKeyNode>
	//}
	int insert(int key, const T& data){
		SameKeyNode<T> *newNode = allocate_a_new_node(key, data);
		return insert_a_new_node_into_map(&m_container, newNode, key);
		//SameKeyChain *e = m_container.find(key);
		//if(0 == e){ // 没找到
		//	e = allocate_a_new_node_chain(key);
		//	e->key = key;
		//	m_container.insert(key, e);
		//}
		//e->nodeChain.push(newNode);
		//return 0;
	}
	int erase(int key){
//		SameKeyNode<T> *newNode = allocate_a_new_node(key, data);
		HookChain< SameKeyNode<T> > *e = m_container.find(key);
		if(0 == e){ // 没找到
			return 1;
		}
		SameKeyNode<T> *firstNode = e->pop();
		if(0 == firstNode){
			return 2;
		}
		return insert_a_new_node_into_map(&m_idleNodeContainer, firstNode, key);
	}
	void clear(){
		m_container.clear();
		NBMultiMap_();
	}
	NBMultiMap(){
		NBMultiMap_();
	}
	~NBMultiMap(){
		NBMultiMap__();
	}
protected:
	inline HookChain<SameKeyNode<T> >* allocate_a_new_node_chain(){
		return new HookChain<SameKeyNode<T> >;
	}
	SameKeyNode<T>* allocate_a_new_node(int key, const T& data){
		HookChain<SameKeyNode<T> > *idleElement = m_idleNodeContainer.find(key);
		if(idleElement){
			SameKeyNode<T> *idleNode = idleElement->pop();
			if(idleNode){
				return idleNode;
			}
		}
		return new SameKeyNode<T>(data);
	}
	void NBMultiMap__(){
		NBMultiMap_();
	}
	void NBMultiMap_(){
	}
	int insert_a_new_node_into_map(UNIQUE_KEY_MAP_CONTAINER *bulker, SameKeyNode<T> *newNode, int key)
	{
		HookChain<SameKeyNode<T> > *e = bulker->find(key);
		if(0 == e){ // 没找到
			e = allocate_a_new_node_chain();
			if(0 == e){
				return -1;
			}
			//e->multiKey = key;
			bulker->insert(key, e);
		}
		e->push(newNode);
		return 0;
	}

	UNIQUE_KEY_MAP_CONTAINER m_container;
	UNIQUE_KEY_MAP_CONTAINER m_idleNodeContainer;
};
template<class T, int MAP_BASIC_SIZE>
class NBUAPI NBMultiPointerMap : public NBMultiMap<T*, MAP_BASIC_SIZE>
{
public:
	T* pop(int key){// 弹出一个键值为key的元素，找不到返回0。
		HookChain<SameKeyNode<T*> > *theChain = this->m_container.find(key);
		if(0 == theChain){
			return 0;
		}
		SameKeyNode<T*> *popedNode = theChain->pop();
		if(popedNode){
			T * theData = popedNode->data;
			popedNode->data = 0;
			//这里无论出错与否，都不可报出。因为，目标已正确找到，而报错（而不返回目标）将造成内存泄漏。
			insert_a_new_node_into_map(&(this->m_idleNodeContainer), popedNode, key);
			return theData;
		}
		return 0;
	}
};
}
#endif //NEWBEE_UTILS_MULTIMAP_H

