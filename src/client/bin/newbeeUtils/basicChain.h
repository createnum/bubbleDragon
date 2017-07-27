/*	
������ͷ���������ǵ���ģ�����Ŀǰɾ��������m_lastNodeָ��Ĵ������ڵ�Ч�����⡣
*/
//#pragma warning (disable: 4700)

#if !defined NEWBEE_UTILS_BASICCHAIN_H
#define NEWBEE_UTILS_BASICCHAIN_H
#include <typeinfo>
#include "newbeeUtils/exports.h"
#include "chain.h"
//�α꣺	m_browser
namespace NEWBEE_UTILS{
template <typename T>
struct Node{
	T data;
	Node* nextNode;
};
template <class T>
class NBUAPI BasicChain
{
public:
	void increase()
	{
		m_counter++;
	}
	void decrease()
	{
		m_counter--;
	}
	bool isLast(){
		return (m_browser == m_lastNode) && (0 != m_browser);
	}
	bool isFirst(){//�α��Ƿ�ָ���һ�����ݽڵ㡣
		return (m_browser == m_firstNode) && (0 != m_browser);
	}
	void ungear(BasicChain<T> *newTrain){
		//�ѹ������Լ���һ���ֳ��ᣨ�� m_browser���µĲ��֣��ѹ����ҵ���ĳ��ϡ�
		if(empty()){
			return;
		}
		//���������У����򲻿ɴ���
		//��һ����ȷ������
		Node<T> *cargo = 0;
		if(0 == m_browser){//����ʹӵ�һ���ڵ㿪ʼ
			cargo = this->m_firstNode;
		}else{//���򣬴��α����һ���ڵ㿪ʼ��
			cargo = m_browser->nextNode;
		}
		//�ڶ������ҽ�
		if(newTrain->empty()){
			newTrain->m_firstNode = cargo;
		}else{
			newTrain->m_lastNode->nextNode = cargo;
		}
		newTrain->m_lastNode = this->m_lastNode;
		//���������ѹ�, ԭ����ͻ����ѽڣ������ƺ�
		this->m_browser->nextNode = 0;
		this->m_lastNode = this->m_browser;
	}
	BasicChain &operator=(const BasicChain &obj)
	{
		Node<T> *browser = obj.m_firstNode;
		while(browser){
			this->allocateLast(browser->data);
			browser = browser->nextNode;
		}
		return *this;
	}
	BasicChain(const BasicChain& obj)
	{//
		m_counter = 0;
		m_browser = 0;
		m_firstNode = 0;
		m_lastNode = 0;
		Node<T> *browser = obj.m_firstNode;
		while(browser){
			this->allocateLast(browser->data);
			browser = browser->nextNode;
		}
	}
	void transferData( BasicChain& bc)
	{//�������캯���Ǵ���ʵ��ģ�Ҳ����˵�������һ�������ݻᴫ�ݸ�����ֵ�ߣ��Լ��ľ�û�ˡ�
		m_counter = bc.m_counter;
		m_browser = bc.m_browser;
		m_firstNode = bc.m_firstNode;
		m_lastNode = bc.m_lastNode;
		bc.m_counter = 0;
		bc.m_browser = 0;
		bc.m_firstNode = 0;
		bc.m_lastNode = 0;
	}
	BasicChain()
	{
		m_counter = 0;
		m_browser = 0;
		m_firstNode = 0;
		m_lastNode = 0;
	}
	virtual ~BasicChain()
	{
		while (m_firstNode)
		{
			Node<T>* theReleaseOne = m_firstNode;
			m_firstNode = m_firstNode->nextNode;
			releaseNode(theReleaseOne);
		}
		m_firstNode = m_lastNode = m_browser = 0;
	};
	/*virtual*/ void clear()
	{
		while (m_firstNode)
		{
			Node<T>* theReleaseOne = m_firstNode;
			m_firstNode = m_firstNode->nextNode;
			releaseNode(theReleaseOne);
		}
		m_firstNode = m_lastNode = m_browser = 0;
		m_counter = 0;
	};
	T *getLastData(){
		if(empty()){
			return 0;
		}
		return &(m_lastNode->data);
	}
	//////////////////////��������////////////////////////////////////////////////////
	T* first(){//��m_browser��Ϊ��һ�������գ�����0���α����㡣
		if(empty()){
			return 0;
		}else{
			m_browser = m_firstNode;
			return &(m_firstNode->data);
		}
	}
	T* next(){//�α�������һ�������m_browserΪ�㣬����null_node
		if(m_browser){
			m_browser = m_browser->nextNode;
		}
		if( m_browser){
			return &(m_browser->data);
		}else {
			return 0;
		}
	}
	bool isEnd(){//m_browserֵΪ�㣬��ʾ�Ѿ�û���ˡ�
		return 0 == m_browser;
	}
	T* first(T*)
	{//����ͬnext()���������α꣬�Ա���Ƕ�����⡣
		if(empty()){
			return 0;
		}else{
			return &(m_firstNode->data);
		}
	}
	T* next(T* pData)
	{//����ͬnext()���������α꣬�Ա���Ƕ�����⡣
		Node<T>* pNode = convertDataToNode(pData);
		if( 0 == pNode->nextNode){
			return 0;
		}else {
			return &(pNode->nextNode->data);
		}
	}
	Node<T>* findPre(Node<T>* pNode)
	{//�ҵ�ǰһ���ڵ�
		if (pNode)
		{
			if (pNode == m_firstNode)
			{//û��ǰһ��
				return 0;
			}
			Node<T>* p = m_firstNode;
			while (p)
			{
				if (p->nextNode == pNode)
				{
					return p;
				}
				p = p->nextNode;
			}
		}
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	T * last(){//��m_browser��Ϊ��һ�������գ�����0���α����㡣
		m_browser = m_lastNode;
		if(empty()){
			return 0;
		}else{
			return &(m_lastNode->data);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	T* allocateLast()
	{//������β������һ���µ����ݿռ�ڵ㡣���ظ����ݵ�ָ�룬�Թ����ʹ�á�
		Node<T> *theNewOne = new Node<T>;
		if (0 == theNewOne){
			return 0;
		}
		theNewOne->nextNode = 0;
		if (empty())
		{
			m_firstNode = theNewOne;
			m_lastNode = theNewOne;
		}else{
			m_lastNode->nextNode = theNewOne;
			m_lastNode = theNewOne;
		}
		return &(theNewOne->data);
	}
	T* allocateLast(T &data)
	{////������β������һ���µ����ݿռ�ڵ㡣ͬʱ���и�ֵ�����ظ����ݵ�ָ�룬0��ʾʧ�ܡ�
		T *pData = allocateLast();
		if (pData){
			convertDataToNode(pData)->data = data;
		}
		return pData;
	}
	T* allocateFirst()
	{//�ŵ�����ͷ
		Node<T> *theNewOne = new Node<T>;
		if (0 == theNewOne)	{
//			s_log->errorlog("failed to allocate New Node!");
			return 0;
		}
		theNewOne->nextNode = 0;
		if (empty())
		{
			m_lastNode = theNewOne;
			m_firstNode = theNewOne;
		}else{
			theNewOne->nextNode = m_firstNode;
			m_firstNode = theNewOne;
		}
		return &(theNewOne->data);
	}
	void printAvailable()
	{
		/*
		char oldSeparator = s_log->setSeparator('|');
		s_log->infologs("[available: %d]", sumAvailable());
		Node* p = first();
		for (int i= 1, p, i++)
		{
			s_log->infologs("->%d", i);
			p = next();
		}
		s_log->setSeparator(oldSeparator);
		*/
	}
	bool empty(){
		return 0 == m_firstNode;
	}
	bool hasData(){
		return 0 != m_firstNode;
	}
	int size(){
		if(empty()){
			return 0;
		}else{
			Node<T> *browser = m_firstNode;
			int count = 0;
			while(browser){
				browser = browser->nextNode;
				count ++;
			}
			return count;
		}
	}

	T* release(T* pData)
	{//�ͷ�ָ���ڵ㣬������һ���ڵ㣨û�з����㣩����ô�ڱ����оͱ�������next()������
		if(0 == pData){
			return 0;
		}
		Node<T> *pNode = convertDataToNode(pData);
		Node<T> *deleted = 0;
		if (pNode == m_firstNode)
		{//�����������ô��ǰһ�����Ҳ����ġ����Ա������⴦��
			deleted = m_firstNode;
			m_firstNode = m_firstNode->nextNode;
			releaseNode(deleted);
			if (empty())
			{
				m_lastNode = 0;
				return 0;
			}
			return &(m_firstNode->data);
		}else{
			Node<T>* pre = findPre(pNode);
			if (pre)
			{//�ҵ���
				if (pNode == m_lastNode){
					m_lastNode = pre;
					releaseNextNode(pre);
					return 0;
				}else{
					releaseNextNode(pre);
					return &(pre->nextNode->data);
				}
			}else{
//				s_log->errorlog("can't find previous node!");
				return 0;
			}
		}
		
	}	
	void releaseNext(T* pData)
	{//�ҵ�ǰһ���ڵ㣬�������⣬û�п��ǣ�ͷβ�ڵ�����⡣
		if(0 == pData){
			return;
		}
		Node<T> *pNode = convertDataToNode(pData);
		Node<T> *deleted = pNode->nextNode;
		if (deleted)
		{
			if (m_lastNode == deleted)
			{
				m_lastNode == pNode;
			}
			pNode->nextNode = deleted->nextNode;
			releaseNode(deleted);
		}
	}	
	void releaseTail(T* pData)
	{//��β�ͱ�����ɾ��pDataָ���������ڵĽڵ�����������нڵ㣨����pDataָ���������ڵĽڵ㣩
		if(0 == pData){
			return;
		}
		Node<T> *pNode = convertDataToNode(pData);
		if (pNode == m_firstNode)
		{//�����������ô��ǰһ�����Ҳ����ġ����Ա������⴦��
			clear();
		}else{
			Node<T>* pre = findPre(convertDataToNode(pData));
			if (pre)
			{//�ҵ���
				Node<T>* p = pre->nextNode;
				while (p)
				{
					Node<T>* theReleaseOne = p;
					p = p->nextNode;
					releaseNode(theReleaseOne);
				}
			}
			pre->nextNode = 0;
		}
	}
	////////////////////��ջ�㷨//////////////////////////////////////////////////////
	//T* push(T& topOne) //��allocateFirst�����ǵ�ͬ�ġ�
	virtual int pop(T* topOne)
	{//���ͷŵ�һ��
		if (m_firstNode)
		{//�����������ô��ǰһ�����Ҳ����ġ����Ա������⴦��
			if(topOne){
				*topOne = m_firstNode->data;
			}
			releaseFirst();
			return 0;
		}else{
			return -1;
		}
	}
	void releaseFirst()
	{//�ͷŵ�һ��
		if (m_firstNode)
		{//�����������ô��ǰһ�����Ҳ����ġ����Ա������⴦��
			if (m_lastNode == m_firstNode){
				clear();
			}else{
				Node<T> *temp = m_firstNode;
				m_firstNode = m_firstNode->nextNode;
				releaseNode(temp);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	T *get(){//�α겻�������ص�ǰ����
		if (m_browser)
		{
			return &(m_browser->data);
		}else{
			return 0;
		}
	}
protected:
	void unload(){//ж���������ݣ�����ɾ��
		m_firstNode = m_lastNode = 0;
		m_counter = 0;
	}
	virtual void releaseNode(Node<T> *node){
		node->nextNode = 0;
		delete node;
	}
	void releaseNextNode(Node<T>* pNode)
	{//����ͷ����ͷŴ���ڵ����һ���ڵ㡣
		Node<T>* theReleaseOne = pNode->nextNode;
		if (theReleaseOne)
		{
			pNode->nextNode = theReleaseOne->nextNode;
			this->releaseNode(theReleaseOne);
		}
	}
	T* searchIfEqual(T& pData){
		T* cursor = first();
		while (cursor) {
			if (*cursor == pData) {
				return cursor;
			}
			cursor = next();
		}
		return 0;
	}


	long totalNumber()
	{
		return m_counter;
	}
	long sumTotalNumber()
	{
		T* pNode = first(pNode = 0);
		long i = 0;
		for (;pNode;i++)
		{
			pNode = next(pNode);
		}
		return i;
	}
	static Node<T>* convertDataToNode(T* pData)
	{
		return reinterpret_cast<Node<T>*>(pData);
	}

	bool isLastNode(Node<T>* pNode)
	{
		return (pNode == m_lastNode);
	}
	long sumAvailable(){
		if (empty()){
			return 0;
		}
		long i = 1;
		Node<T>* p = m_firstNode;
		for(int i=1; !isLastNode(); i++){
			p = p->nextNode;
		}
		return i;
	}

	//Node<T> m_chainHead;
	Node<T>* m_browser;
	Node<T>* m_firstNode;
	Node<T>* m_lastNode;
	long m_counter;
private:
	//void operator=( const BasicChain& base );	// not allowed.
};
//����������գ��Լ��ͷ�Ԫ��ʱ�������ͷ��û����ݣ�ָ�룩��BasicChain��
//ע��	1.���ݣ�data��һ��Ϊָ�롣
//		2.�����ͷ�ָ�룬Ҳ�������洫������ָ�룡����
//		3.����ǿ���ͷ�ָ��ռ䣬���ԣ������ַ��������Ա������У���Ӧʹ��BasicChain
template<class T>
class NBUAPI BasicPointerChain :public BasicChain<T*>
{
public:
	BasicPointerChain(const BasicPointerChain& obj)
	{//
		this->m_counter = 0;
		this->m_browser = 0;
		this->m_firstNode = 0;
		this->m_lastNode = 0;
		Node<T*> *browser = obj.m_firstNode;
		while(browser){
			T *newData = 0;
			if(browser->data){
				newData = new T(*(browser->data));
			}
			this->allocateLast(newData);
			browser = browser->nextNode;
		}
	}
	T* first(){//��m_browser��Ϊ��һ�������գ�����0���α����㡣
		if(BasicChain<T*>::empty()){
			return 0;
		}else{
			BasicChain<T*>::m_browser = BasicChain<T*>::m_firstNode;
			return BasicChain<T*>::m_firstNode->data;
		}
	}
	T* next(){//�α�������һ�������m_browserΪ�㣬����null_node
		if(BasicChain<T*>::m_browser){
			BasicChain<T*>::m_browser = BasicChain<T*>::m_browser->nextNode;
		}
		if( BasicChain<T*>::m_browser){
			return BasicChain<T*>::m_browser->data;
		}else {
			return 0;
		}
	}
	T *get(){//�α겻�������ص�ǰ����
		if(BasicChain<T*>::m_browser)
		{
			return this->m_browser->data;
		}else{
			return 0;
		}
	}

	T** firstPointer(){//��m_browser��Ϊ��һ�������գ�����0���α����㡣
		return BasicChain<T*>::first();
	}
	T** nextPointer(){//�α�������һ�������m_browserΪ�㣬����null_node
		return BasicChain<T*>::next();

	}
	T** getPointer(){//�α겻�������ص�ǰ����
		return BasicChain<T*>::get();

	}
	T* getLast(){ //�����Ƿ���һ��ָ����ѡ�
		if(BasicChain<T*>::empty()){
			return 0;
		}
		return BasicChain<T*>::m_lastNode->data;
	}
	T* pop() // ������һ���ڵ�
	{
		T* retVal = 0;
		if (!BasicChain<T*>::empty())
		{//�����������ô��ǰһ�����Ҳ����ġ����Ա������⴦��
			retVal =  BasicChain<T*>::m_firstNode->data;
			//Node<T> *nodeDelete = BasicChain<T*>::m_firstNode;// old code for unix
			Node<T*> *nodeDelete = BasicChain<T*>::m_firstNode;
			if(BasicChain<T*>::m_firstNode == BasicChain<T*>::m_lastNode){
				BasicChain<T*>::m_lastNode = BasicChain<T*>::m_firstNode = 0;
			}else{
				BasicChain<T*>::m_firstNode = BasicChain<T*>::m_firstNode->nextNode;
			}
			BasicChain<T*>::releaseNode(nodeDelete);
		}
		return retVal;
	}
	BasicPointerChain()
	{

	}
	virtual ~BasicPointerChain()
	{
		Node<T*> *browser = BasicChain<T*>::m_firstNode;
		while (browser)
		{
			Node<T*>* theReleaseOne = browser;
			browser = browser->nextNode;

			if(theReleaseOne->data){
				delete theReleaseOne->data;
			}
			theReleaseOne->data = 0;
			// ���ﲻӦɾ���ڵ㱾�������ɻ�������������ɵġ�
		}
	}
protected:
	virtual void releaseNode(Node<T*> *node){
		node->nextNode = 0;
		delete node->data;
		node->data = 0;
		delete node;
	}
};
template <class T>
class NBUAPI HookChain // ��������
{// �Լ�������(new)�κοռ䣬ֻ���ڹҽ��ⲿ�ڵ㡣��ҵĽڵ�ṹ�������������
	//1. ָ������һ���ڵ��ָ����Ϊ nextNode
	//2. ���� void release() ���������� �������� ��ջ�������ʱ�ص���
 // �������Ҫ����T������bool compare(T* a)�����������ͺñȣ����ء�����ÿ���ڵ���һ�ء��½ڵ���á��ȽϺ��������������б�ڵ㣩ֻҪ����true�ͱ�ʾ���سɹ������԰ݷ���һ���ڵ㡣ע�⣺���㷨�� �ȽϺ����ĵ��ã����½ڵ���Ϊ���÷���
 // 
public:
	typedef bool (T::*COMPARE_FUNC)(T* data);
	//void printSelf(NEWBEE_UTILS::Logging *log, Logging::LogLeveL level){
	//	int oldFormat = log->setFormat(Logging::ORNAMENT_NONE);
	//	log->printlog(level, " ================= Battle Map =========================== \n");
	//	T *p = m_hook;
	//	while(p){
	//		p->data;
	//		log->debuglog(Logging::ORNAMENT_SEPARATOR);
	//	}
	//	log->debuglog("================= Battle Map end =========================== \n");
	//	log->setFormat(oldFormat);
	//}
	HookChain(){
		m_hook = 0;
		m_compareFunc = 0;
		m_counter = 0;
	}
	~HookChain(){
		clear();
	}
	inline bool empty(){
		return 0 == m_hook;
	}
	inline T* getFirst()
	{
		return m_hook;
	}
	inline T *pop(){ // �ӿսڵ������ϣ�ȡ��һ���ڵ�ʹ�á�
		if(empty()){
			return 0;
		}
		T *popNode = m_hook;
		m_hook = m_hook->nextNode;
		m_counter--;
		return popNode;
	}
	void push(T *node){
		if(empty()){
			m_hook = node;
			m_hook->nextNode = 0;
		}else{
			node->nextNode = m_hook->nextNode;
			m_hook->nextNode = node;
		}
		m_counter++;
	}
	void insert_sortly(T* newNode){ // 
		if(empty()){
			m_hook = newNode;
			m_hook->nextNode = 0;
		}else{
			if((newNode->*m_compareFunc)(m_hook)){
				T* itor = m_hook;
				while(itor->nextNode && (newNode->*m_compareFunc)(itor->nextNode)){ // û����itor����һ������ô���½ڵ����itor����һ����
					itor = itor->nextNode;
				}
				newNode->nextNode = itor->nextNode;
				itor->nextNode = newNode;
			}else{
				newNode->nextNode = m_hook;
				m_hook = newNode;
			}
		}
		m_counter++;
	}
	void setCompareFunction(COMPARE_FUNC func){
		m_compareFunc = func;
	}
	inline int size(){
		return m_counter;
	}
	int size_check(){
		T *temp = m_hook;
		int counter = 0;
		while(temp){
			counter++;
			temp = temp->nextNode;
		}
		return counter;
	}
	void clear(){
		m_compareFunc = 0;
		if(0 == m_hook){
			return;
		}
		while(m_hook->nextNode){
			T *releasedNode = m_hook->nextNode;
			m_hook->nextNode = releasedNode->nextNode;
			releasedNode->release();
			//delete releasedNode;
		}
		m_hook->release();
		m_hook = 0;
		m_counter = 0;
	}
	void shallowTruncate(){ // ��ָ�벿����ȫ�ضϣ������ⲿ��Ҫǳ������ʱ��������ֹ�ͷų���
		m_hook = 0;
		m_counter = 0;
	}
protected:
	int m_counter;
	T* m_hook;
	COMPARE_FUNC m_compareFunc;
};
}
#endif //NEWBEE_UTILS_BASICCHAIN_H

