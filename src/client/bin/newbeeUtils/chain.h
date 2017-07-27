/* 链表：适用于容量上限稳定
    支持快速增删查
一个数组m_container保存class T类型的数据，长度MAX_CONTAINER_LENGTH。
另一个数组m_container是两个双向链表，保存对应空节点和数据节点链表信息（即container的索引值）。
它比m_container多出两个单元m_dataNodeHead,m_emptyNodeHead，分别作为空节点链表和数据节点链表的头。
*为什么要是双向呢？简单的说，删除操作需要找到上一个节点。deleteOne()中是一个例子，否则为了支持在遍历中删掉游标所指的元素。就麻烦了。
使用必看：
Chain采用如下方式遍历：
ptr = chain.first();
while(ptr){
ptr = chain.next();
}
其实，是可以抽象成一个顺序向下遍历的迭代器的，因为无论是安全方式，都必须注意删除游标所在的节点的问题。
那么，first(), next(), deleteOne()就是代表这个迭代器。
如果需要新的迭代方式，那么如果不同的迭代器都支持删除，那么他们几乎不可能避免冲突。也就是说他们的遍历不允许嵌套，交叉操作。
*/
//#pragma warning (disable: 4700)

#if !defined SY_CHAIN_H
#define SY_CHAIN_H
#include "logging.h"

#define GET_ADDRESS(a) &m_container[a]
#define m_dataNodeHead m_container[M_DATA_NODE_HEAD_ID]
#define m_emptyNodeHead m_container[M_EMPTY_NODE_HEAD_ID]
#define m_max0Storage m_container[M_MAX_ID_OF_STORAGE]
#define m_min0Storage m_container[M_MIN_ID_OF_STORAGE]
#define P_FIRST_EMPTY_NODE m_emptyNodeHead.nextNode; // 第一个空节点
#define P_LAST_EMPTY_NODE m_emptyNodeHead.preNode; // 最后一个空节点
#define P_FIRST_DATA_NODE m_dataNodeHead.nextNode; // 第一个空节点
#define P_LAST_DATA_NODE m_dataNodeHead.preNode; // 最后一个空节点
#define INVALID_BROWSER GET_ADDRESS(M_DATA_NODE_HEAD_ID)
#define P_DATA_NODE_HEAD GET_ADDRESS(M_DATA_NODE_HEAD_ID)
#define P_EMPTY_NODE_HEAD GET_ADDRESS(M_EMPTY_NODE_HEAD_ID)

namespace NEWBEE_UTILS{

    //数据节点：full node
    //空节点：empty node
    //游标：    m_browser4DownSequenceLooping
    template <class T, int MAX_CONTAINER_LENGTH>
    class Chain
    {
    public:
        inline T* get_as_AS(int i){
            if(i<0){
                return 0;
            }
            if(i >= size()){
                return 0;
            }
            Node* n = P_FIRST_DATA_NODE;
            while(0 != i)
            {
                n = n->nextNode;
                i--;
            }
            return &(n->m_data);
        }
        //inline T* get(int i){
        //    return get_as_AS(i-1);
        //}
        int capacity(){
            return MAX_CONTAINER_LENGTH;
        }
        void printSelf(NEWBEE_UTILS::Logging *log, NEWBEE_UTILS::Logging::LogLeveL level, int mode=0/*1 表示测试模式*/)
        {
            log->printlogs(level, "========= Chain Details [capacity: %d].=================", MAX_CONTAINER_LENGTH);
            int oldFormat = log->setFormat(NEWBEE_UTILS::Logging::ORNAMENT_NONE);
            log->printlog(level, NEWBEE_UTILS::Logging::ORNAMENT_PREFIX);
            //char oldSeparator = log->setSeparator('\0');
            log->printlogs(level, "[used: %d]", size());
            Node *p = P_FIRST_DATA_NODE;
            while(p != P_DATA_NODE_HEAD){
                if(mode){
                    log->printlogs(level, "->%d(v: %d)", offset(p), p->m_data);
                }else{
                    log->printlogs(level, "->%d",offset(p));
                }
                p = p->nextNode;
            }
            log->printlog(level,'\n');
            //////////////////////////////////////////////////////////////////////////
            log->printlog(level, NEWBEE_UTILS::Logging::ORNAMENT_PREFIX);
            log->printlogs(level, "[available: %d]", sumAvailable());
            p = P_FIRST_EMPTY_NODE;
            while(p != P_EMPTY_NODE_HEAD){
                log->printlogs(level, "->%d",offset(p));
                p = p->nextNode;
            }
            log->printlog(level,'\n');
            log->setFormat(oldFormat);
            log->printlog(level, "========= Chain Details end.=================");

        }
        void printEnd() // out of date
        {
            m_log->infologs("#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^#$%^>");
        }
        void printTitle() // out of date
        {
            m_log->infologs("<#$%^#$%^链表桶 %s 【 D: %d E: %d 】#$%^#$%^", typeid(*this).name(), this->sumTotalNumber(),sumAvailable());
        }
        class Node{
        public:
            T m_data;
            Node* nextNode;
            Node* preNode;
        };

        void neutralize(T* pData){
            Node* pNode = convertDataToNode(pData);
            moveBrowser4DownSequenceLooping(pNode);
            removeDataNode(pNode);
            pNode->nextNode = 0;
            pNode->preNode = 0;
        }
        Chain<T,MAX_CONTAINER_LENGTH>& operator= (Chain<T,MAX_CONTAINER_LENGTH>& chain){
            return chain;
        }
        T* searchIfEqual(T& node){
            T* cursor = first();
            while (cursor) {
                if (*cursor == node) {
                    return cursor;
                }
                cursor = next();
            }
            return 0;
        }
        Chain() : M_EMPTY_NODE_HEAD_ID (MAX_CONTAINER_LENGTH),  // 位于整个存储数组的倒数第二个。
            M_DATA_NODE_HEAD_ID(MAX_CONTAINER_LENGTH +1),  // 位于整个存储数组的最后一个。
            M_MIN_ID_OF_STORAGE(0),
            M_MAX_ID_OF_STORAGE(MAX_CONTAINER_LENGTH -1) // 位于整个存储数组的倒数第三个。
        {
            _Chain();
        }
        virtual ~Chain(){};
        void clear(){
            _Chain();
        }
        void replace();
        T* allocateLastwithCount(){ // out of date
            this->increase();//统计计数，这会影响效率，哪怕只是一点点...
            return allocateLast();
        }
        inline T* push(T& data) // 压栈，即把数据放在链表的开头
        {//添加在空链表的头一个，数据链表的最后一个。
            T *allocatedNode = allocate_from_head();
            if(allocatedNode){
                *allocatedNode = data;
            }
            return allocatedNode;
        }
        T* allocate_from_head()
        { // 把空链的第一个节点，添加到数据链的第一个。
            if(isFull()){
                return 0;
            }else{
                Node* allocatedNode = m_emptyNodeHead.nextNode; // 被分配节点
                Node* nextEmptyNode = allocatedNode->nextNode; // 被分配节点的下一个空节点，即将成为空节点链上的第一个空节点
                Node* firstDataNode = m_dataNodeHead.nextNode; // 数据节点链上，第一个数据节点
                //设置被分配节点。
                allocatedNode->nextNode = firstDataNode; 
                allocatedNode->preNode = P_DATA_NODE_HEAD;
                //设置空节点头
                m_emptyNodeHead.nextNode = nextEmptyNode;
                //设置数据节点头
                m_dataNodeHead.nextNode = allocatedNode;
                //设置（分配之前的）第一个数据节点
                firstDataNode->preNode = allocatedNode;
                //设置 pNextEmptyNode
                nextEmptyNode->preNode = P_EMPTY_NODE_HEAD;
                m_dataNodeCounter++;
                return &(allocatedNode->m_data);
            }
        }
        T* allocateLast()
        { // 把空链的第一个节点，添加到数据链的最后一个。
            if(isFull()){
                return 0;
            }else{
                Node* allocatedNode = m_emptyNodeHead.nextNode; // 被分配节点
                Node* nextEmptyNode = allocatedNode->nextNode; // 被分配节点的下一个空节点，即将成为空节点链上的第一个空节点
                Node* lastDataNode = m_dataNodeHead.preNode; // 数据节点链上，最后一个数据节点
                //设置被分配节点
                allocatedNode->preNode = lastDataNode; // 
                allocatedNode->nextNode = P_DATA_NODE_HEAD;
                //设置空节点头
                m_emptyNodeHead.nextNode = nextEmptyNode;
                //设置数据节点头
                m_dataNodeHead.preNode = allocatedNode;
                //设置（分配之前的）最后一个数据节点
                lastDataNode->nextNode = allocatedNode;
                //设置 pNextEmptyNode
                nextEmptyNode->preNode = P_EMPTY_NODE_HEAD;
                m_dataNodeCounter++;
                return &(allocatedNode->m_data);
            }
        }
        inline T* append(T& data) // 追加，即在链表的最后添加数据
        {//添加在空链表的头一个，数据链表的最后一个。
            T *allocatedNode = allocateLast();
            if(allocatedNode){
                *allocatedNode = data;
            }
            return allocatedNode;
        }
        bool isFull(){
            return GET_ADDRESS(M_EMPTY_NODE_HEAD_ID) == m_emptyNodeHead.nextNode;
        }
        bool isEmpty(){
            return GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_dataNodeHead.nextNode;
        }

        bool isEnd(){//游标是否指向最后一个数据节点。游标指向一个数据节点，并且该数据节点的nextNode指向数据头节点，那么，就表明游标指向最后一个数据节点。
            return m_browser4DownSequenceLooping != INVALID_BROWSER && GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_browser4DownSequenceLooping->nextNode;
        }
        bool isfirst(){//游标是否指向第一个数据节点。游标指向一个数据节点，并且该数据节点的preNode指向数据头节点，那么，就表明游标指向第一个数据节点。
            return m_browser4DownSequenceLooping != INVALID_BROWSER && GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_browser4DownSequenceLooping->preNode;
        }
        bool isBrowserVaild(){
            return m_browser4DownSequenceLooping != INVALID_BROWSER;
        }
        T* get(){//游标不动，返回当前数据
            if(isBrowserVaild())
            {
                return &(m_browser4DownSequenceLooping->m_data);
            }else{
                return 0;
            }
        }
        T* first(){//若空，返回第一个node，游标指向该节点；否则返回0，游标无操作
            if(isEmpty()){
                return 0;
            }else{
                m_browser4DownSequenceLooping = m_dataNodeHead.nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }
        T* next(){//若有，返回下一个node，游标指向该节点；否则返回0，游标无操作
            if( GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_browser4DownSequenceLooping->nextNode){
                return 0;
            }else {
                m_browser4DownSequenceLooping = m_browser4DownSequenceLooping->nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }
        T* firstEmpty(){//若空，返回第一个node，游标指向该节点；否则返回0，游标无操作
            if(isFull()){
                return 0;
            }else{
                m_browser4DownSequenceLooping = m_emptyNodeHead.nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }
        T* nextEmpty(){//若有，返回下一个node，游标指向该节点；否则返回0，游标无操作
            if( GET_ADDRESS(M_EMPTY_NODE_HEAD_ID) == m_browser4DownSequenceLooping->nextNode){
                return 0;
            }else {
                m_browser4DownSequenceLooping = m_browser4DownSequenceLooping->nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }

        T* first(T*)
        {//功能同next()，但不用游标，以避免嵌套问题。
            if(isEmpty())
            {
                return 0;
            }else
            {
                return &((m_dataNodeHead.nextNode)->m_data);
            }
        }
        T* next(T* pData)
        {//功能同next()，但不用游标，以避免嵌套问题。
            Node* pNode = convertDataToNode(pData);
            if( GET_ADDRESS(M_DATA_NODE_HEAD_ID) == pNode->nextNode){
                return 0;
            }else {
                return &(pNode->nextNode->m_data);
            }
        }
        void deleteOne()
        {
            m_browser4DownSequenceLooping = m_browser4DownSequenceLooping->preNode;
            deleteOnebyChainNodePointer(m_browser4DownSequenceLooping->nextNode);
        }
        void deleteOne(T* pData)
        {
            Node* pNode = convertDataToNode(pData);
            moveBrowser4DownSequenceLooping(pNode);
            deleteOnebyChainNodePointer(pNode);
        }
        void deleteOnewithCount(T* pData)
        {
            deleteOne(pData);
            this->decrease();
        }
        int size(){
            return m_dataNodeCounter;
            //return static_cast<int>(sumTotalNumber());
        }
        int availableSize(){
            return capacity() - size();
        }
        long sumTotalNumber()
        {
            T* pNode = first(pNode = 0);
            int i = 0;
            for (;pNode;i++)
            {
                pNode = next(pNode);
            }
            return i;
        }
        void printAvailable()
        {
            //char oldSeparator = m_log->setSeparator('|');
            //m_log->infologs("[available: %d]", sumAvailable());
            //Node* p = m_emptyNodeHead.nextNode;
            //while(p != &m_emptyNodeHead){
            //    m_log->infologs("->%d",offset(p));
            //    p = p->nextNode;
            //}
            //m_log->setSeparator(oldSeparator);
        }
        void join2emptyChain(T* pData)
        {//把中立节点放入空节点链表
            Node* pNode = convertDataToNode(pData);
            addNode2EmptyChainFromtheHead(pNode);
        }
        void join2DataChain(T* pData)
        {
            Node* pNode = convertDataToNode(pData);
            addNode2DataChainFromtheTail(pNode);
        }
        bool isNeutral(T* pData)
        {
            return !(convertDataToNode(pData)->nextNode);
        }

        long offset(T* cur)
        {
            return offset(convertDataToNode(cur));
        }
        void hangOntheHook(T* hooker0Data, T* pData)
        {
            Node* pNode = convertDataToNode(pData);
            Node* hooker0Node = convertDataToNode(hooker0Data);
            hangOntheHook(hooker0Node, pNode);
        }
    protected:
        void _Chain(){
            m_dataNodeCounter = 0;
            m_browser4DownSequenceLooping = INVALID_BROWSER;
            m_emptyNodeHead.nextNode = GET_ADDRESS(M_MIN_ID_OF_STORAGE);
            m_emptyNodeHead.preNode = GET_ADDRESS(M_MAX_ID_OF_STORAGE);//数组最后一个单元
            m_dataNodeHead.preNode = GET_ADDRESS(M_DATA_NODE_HEAD_ID);
            m_dataNodeHead.nextNode = GET_ADDRESS(M_DATA_NODE_HEAD_ID);
            for(long i=M_MIN_ID_OF_STORAGE; i<= M_MAX_ID_OF_STORAGE; i++)
            {
                m_container[i].nextNode = &m_container[i+1];
                m_container[i].preNode = &m_container[i-1];
            }
            m_container[M_MAX_ID_OF_STORAGE].nextNode = GET_ADDRESS(M_EMPTY_NODE_HEAD_ID);
            m_container[M_MIN_ID_OF_STORAGE].preNode = GET_ADDRESS(M_EMPTY_NODE_HEAD_ID);
        }
        //T* firstfast(){//这个快一点，但要避免嵌套循环。not implement, yet
        //    return first();
        //}
        //T* nextfast(){
        //    return next();
        //}
        typedef struct chain_expand_node{//用于扩展链表的空间，不到万不得已最好不要扩展，而是一开始就分配足够大的空间。暂时还没有用到
            Node *pNextContainer;
            chain_expand_node *pNextExpandNode;
            
        }stChainExpandNode;
        void hangOntheHook(Node* hooker, Node* pNode)
        {//?忘了作用……………………
            //把该节点作为一个数据节点，添加到hooker的下一个。
            pNode->nextNode = hooker->nextNode;
            pNode->preNode = hooker;
            //修改hooker原下一个数据节点。
            hooker->nextNode->preNode = pNode;
            //修改数据节点头
            hooker->nextNode = pNode;
        }
        static Node* convertDataToNode(T* pData)
        {
            return reinterpret_cast<Node*>(pData);
        }
        void moveBrowser4DownSequenceLooping(Node* pNode)
        {//这个也和遍历算法有关，这个支持的顺序向下遍历。
            if (pNode == m_browser4DownSequenceLooping) {
                m_browser4DownSequenceLooping = m_browser4DownSequenceLooping->preNode;    
            }
        }
        const Node* getOriginAddress()
        {
            return m_container;
        }
        long offset(Node* cur){
            return cur - getOriginAddress();
        }
        long sumAvailable(){
            long i = 0;
            Node* p = m_emptyNodeHead.nextNode;
            while(p != &m_emptyNodeHead){
                i++;
                p = p->nextNode;
            }
            return i;
        }
        void addNode2EmptyChainFromtheHead(Node* pNode)
        {
            //把该节点作为一个空节点，添加到空节点链表开头。
            pNode->nextNode = m_emptyNodeHead.nextNode;
            pNode->preNode = GET_ADDRESS(M_EMPTY_NODE_HEAD_ID);
            //修改其下一个空节点
            (m_emptyNodeHead.nextNode)->preNode = pNode;
            //修改空节点头
            m_emptyNodeHead.nextNode = pNode;
            //重新初始化这个对象，会影响一些效率哦。
            //pNode->m_data = m_emptyNodeHead.m_data;
        }
        void addNode2DataChainFromtheTail(Node* pNode)
        {
            //把该节点作为一个空节点，添加到空节点链表开头。
            pNode->nextNode = GET_ADDRESS(M_DATA_NODE_HEAD_ID);
            pNode->preNode = m_dataNodeHead.preNode;
            //修改其下一个空节点
            (m_dataNodeHead.preNode)-> nextNode= pNode;
            //修改空节点头
            m_dataNodeHead.preNode = pNode;
        }
        void removeDataNode(Node* pNode)
        {
            //修改上一个fullnode
            (pNode->preNode)->nextNode = pNode->nextNode;
            //修改下一个fullnode
            (pNode->nextNode)->preNode = pNode->preNode;
        }
        void deleteOnebyChainNodePointer(Node* pNode)
        {//如果错误重删没有任何影响！
            removeDataNode(pNode);
            addNode2EmptyChainFromtheHead(pNode);
            m_dataNodeCounter--;
        }
        bool enlargeCapability()
        {//留待实现
        }
        Node* m_browser4DownSequenceLooping;
        const long M_EMPTY_NODE_HEAD_ID;//空节点链的头节点Id
        const long M_DATA_NODE_HEAD_ID; //数据节点链的头节点Id
        const long M_MAX_ID_OF_STORAGE;
        const long M_MIN_ID_OF_STORAGE;
        Node m_container[MAX_CONTAINER_LENGTH+2];
        //Node* m_pNextContainer;//保留待用
        int m_dataNodeCounter;
    private:
        /*static */Logging* m_log;
    };

}
#endif //SY_CHAIN_H
