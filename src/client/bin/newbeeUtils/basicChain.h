/*	
单向无头链表，由于是单向的，所以目前删除操作对m_lastNode指针的处理都存在低效的问题。
*/
//#pragma warning (disable: 4700)

#if !defined NEWBEE_UTILS_BASICCHAIN_H
#define NEWBEE_UTILS_BASICCHAIN_H
#include <typeinfo>
#include "newbeeUtils/exports.h"
#include "chain.h"
//游标：	m_browser
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
	bool isFirst(){//游标是否指向第一个数据节点。
		return (m_browser == m_firstNode) && (0 != m_browser);
	}
	void ungear(BasicChain<T> *newTrain){
		//脱钩，把自己的一部分车厢（自 m_browser以下的部分）脱钩，挂到别的车上。
		if(empty()){
			return;
		}
		//分三步进行，次序不可打乱
		//第一步：确定货物
		Node<T> *cargo = 0;
		if(0 == m_browser){//货物就从第一个节点开始
			cargo = this->m_firstNode;
		}else{//否则，从游标的下一个节点开始。
			cargo = m_browser->nextNode;
		}
		//第二步：挂接
		if(newTrain->empty()){
			newTrain->m_firstNode = cargo;
		}else{
			newTrain->m_lastNode->nextNode = cargo;
		}
		newTrain->m_lastNode = this->m_lastNode;
		//第三步：脱钩, 原链表和货物脱节，做好善后。
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
	{//拷贝构造函数是传递实体的，也就是说他里面的一串串数据会传递给被赋值者，自己的就没了。
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
	//////////////////////遍历函数////////////////////////////////////////////////////
	T* first(){//把m_browser置为第一个。若空，返回0，游标置零。
		if(empty()){
			return 0;
		}else{
			m_browser = m_firstNode;
			return &(m_firstNode->data);
		}
	}
	T* next(){//游标移向下一个。如果m_browser为零，返回null_node
		if(m_browser){
			m_browser = m_browser->nextNode;
		}
		if( m_browser){
			return &(m_browser->data);
		}else {
			return 0;
		}
	}
	bool isEnd(){//m_browser值为零，表示已经没有了。
		return 0 == m_browser;
	}
	T* first(T*)
	{//功能同next()，但不用游标，以避免嵌套问题。
		if(empty()){
			return 0;
		}else{
			return &(m_firstNode->data);
		}
	}
	T* next(T* pData)
	{//功能同next()，但不用游标，以避免嵌套问题。
		Node<T>* pNode = convertDataToNode(pData);
		if( 0 == pNode->nextNode){
			return 0;
		}else {
			return &(pNode->nextNode->data);
		}
	}
	Node<T>* findPre(Node<T>* pNode)
	{//找到前一个节点
		if (pNode)
		{
			if (pNode == m_firstNode)
			{//没有前一个
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
	T * last(){//把m_browser置为第一个。若空，返回0，游标置零。
		m_browser = m_lastNode;
		if(empty()){
			return 0;
		}else{
			return &(m_lastNode->data);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	T* allocateLast()
	{//在链表尾，分配一个新的数据空间节点。返回该数据的指针，以供外界使用。
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
	{////在链表尾，分配一个新的数据空间节点。同时进行赋值。返回该数据的指针，0表示失败。
		T *pData = allocateLast();
		if (pData){
			convertDataToNode(pData)->data = data;
		}
		return pData;
	}
	T* allocateFirst()
	{//放到链表头
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
	{//释放指定节点，返回下一个节点（没有返回零），那么在遍历中就必须跳过next()操作，
		if(0 == pData){
			return 0;
		}
		Node<T> *pNode = convertDataToNode(pData);
		Node<T> *deleted = 0;
		if (pNode == m_firstNode)
		{//如果成立，那么找前一个是找不到的。所以必须特殊处理
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
			{//找到了
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
	{//找到前一个节点，似有问题，没有考虑，头尾节点的问题。
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
	{//从尾巴遍历起，删除pData指向数据所在的节点起往后的所有节点（包括pData指向数据所在的节点）
		if(0 == pData){
			return;
		}
		Node<T> *pNode = convertDataToNode(pData);
		if (pNode == m_firstNode)
		{//如果成立，那么找前一个是找不到的。所以必须特殊处理
			clear();
		}else{
			Node<T>* pre = findPre(convertDataToNode(pData));
			if (pre)
			{//找到了
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
	////////////////////堆栈算法//////////////////////////////////////////////////////
	//T* push(T& topOne) //和allocateFirst函数是等同的。
	virtual int pop(T* topOne)
	{//从释放第一个
		if (m_firstNode)
		{//如果成立，那么找前一个是找不到的。所以必须特殊处理
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
	{//释放第一个
		if (m_firstNode)
		{//如果成立，那么找前一个是找不到的。所以必须特殊处理
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

	T *get(){//游标不动，返回当前数据
		if (m_browser)
		{
			return &(m_browser->data);
		}else{
			return 0;
		}
	}
protected:
	void unload(){//卸下所有数据，并不删除
		m_firstNode = m_lastNode = 0;
		m_counter = 0;
	}
	virtual void releaseNode(Node<T> *node){
		node->nextNode = 0;
		delete node;
	}
	void releaseNextNode(Node<T>* pNode)
	{//这个释放是释放传入节点的下一个节点。
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
//析构或者清空，以及释放元素时，自行释放用户数据（指针）的BasicChain。
//注：	1.数据（data）一定为指针。
//		2.自行释放指针，也包括外面传进来的指针！！！
//		3.由于强行释放指针空间，所以，常量字符串不可以保存其中，而应使用BasicChain
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
	T* first(){//把m_browser置为第一个。若空，返回0，游标置零。
		if(BasicChain<T*>::empty()){
			return 0;
		}else{
			BasicChain<T*>::m_browser = BasicChain<T*>::m_firstNode;
			return BasicChain<T*>::m_firstNode->data;
		}
	}
	T* next(){//游标移向下一个。如果m_browser为零，返回null_node
		if(BasicChain<T*>::m_browser){
			BasicChain<T*>::m_browser = BasicChain<T*>::m_browser->nextNode;
		}
		if( BasicChain<T*>::m_browser){
			return BasicChain<T*>::m_browser->data;
		}else {
			return 0;
		}
	}
	T *get(){//游标不动，返回当前数据
		if(BasicChain<T*>::m_browser)
		{
			return this->m_browser->data;
		}else{
			return 0;
		}
	}

	T** firstPointer(){//把m_browser置为第一个。若空，返回0，游标置零。
		return BasicChain<T*>::first();
	}
	T** nextPointer(){//游标移向下一个。如果m_browser为零，返回null_node
		return BasicChain<T*>::next();

	}
	T** getPointer(){//游标不动，返回当前数据
		return BasicChain<T*>::get();

	}
	T* getLast(){ //仅仅是返回一个指针而已。
		if(BasicChain<T*>::empty()){
			return 0;
		}
		return BasicChain<T*>::m_lastNode->data;
	}
	T* pop() // 弹出第一个节点
	{
		T* retVal = 0;
		if (!BasicChain<T*>::empty())
		{//如果成立，那么找前一个是找不到的。所以必须特殊处理
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
			// 这里不应删除节点本身，这是由基类析构函数完成的。
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
class NBUAPI HookChain // 钩子链条
{// 自己不分配(new)任何空间，只用于挂接外部节点。外挂的节点结构必须符合条件：
	//1. 指向其下一个节点的指针名为 nextNode
	//2. 含有 void release() 函数，用于 钩子链条 清空或者析构时回调。
 // 排序插入要求，类T必须有bool compare(T* a)函数，其规则就好比，闯关。链表每个节点是一关。新节点调用“比较函数”（参数是列表节点）只要返回true就表示闯关成功，可以拜访下一个节点。注意：本算法内 比较函数的调用，是新节点做为调用方。
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
	inline T *pop(){ // 从空节点链表上，取下一个节点使用。
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
				while(itor->nextNode && (newNode->*m_compareFunc)(itor->nextNode)){ // 没闯过itor的下一个。那么，新节点就是itor的下一个。
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
	void shallowTruncate(){ // 把指针部分完全截断，用在外部需要浅拷贝的时候，这样防止释放出错。
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

