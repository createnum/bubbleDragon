#ifndef NEWBEE_UTILS_MULTIMAP_H
#define NEWBEE_UTILS_MULTIMAP_H
//���ֵӳ��
// key ֵ ���� ���� 
// MAP_BASIC_SIZE ��ʾ����ʱ�����Ԫ�ظ��������ԣ�ʹ��ʱҪ���ǣ��ڴ��ʹ�á���ֵ���˾޴󡣻��� ��T�Ǹ����࣬Ӧ����ָ�롣Ҳ���˹�С������Ч�ʼ��͡�
#include "basicChain.h"
#include "nbMap.h"
//-------------------------
//| 0 | 1 | 2 | 3 | 4 | 5 |    ������������
//-------------------------
//  |
//  ^
namespace NEWBEE_UTILS
{
template<class T>
struct SameKeyNode{ // β���ϵ�ÿ���ڵ��ϵķ�֧��ÿ����֧��������ͬ��keyֵ�Ľڵ㡣
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

	//struct SameKeyChain{ // β���ϵ�ÿ���ڵ��ϵķ�֧��ÿ����֧��������ͬ��keyֵ�Ľڵ㡣
	//	//int multiKey;
	//	HookChain<SameKeyNode> nodeChain;
	//	//SameKeyChain(int key){
	//	//	this->multiKey = key;
	//	//}
	//};
	//struct TailNode{ // �����������β�͡�
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
		//if(0 == e){ // û�ҵ�
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
		if(0 == e){ // û�ҵ�
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
		if(0 == e){ // û�ҵ�
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
	T* pop(int key){// ����һ����ֵΪkey��Ԫ�أ��Ҳ�������0��
		HookChain<SameKeyNode<T*> > *theChain = this->m_container.find(key);
		if(0 == theChain){
			return 0;
		}
		SameKeyNode<T*> *popedNode = theChain->pop();
		if(popedNode){
			T * theData = popedNode->data;
			popedNode->data = 0;
			//�������۳�����񣬶����ɱ�������Ϊ��Ŀ������ȷ�ҵ�����������������Ŀ�꣩������ڴ�й©��
			insert_a_new_node_into_map(&(this->m_idleNodeContainer), popedNode, key);
			return theData;
		}
		return 0;
	}
};
}
#endif //NEWBEE_UTILS_MULTIMAP_H

