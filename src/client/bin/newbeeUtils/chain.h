/* �������������������ȶ�
    ֧�ֿ�����ɾ��
һ������m_container����class T���͵����ݣ�����MAX_CONTAINER_LENGTH��
��һ������m_container������˫�����������Ӧ�սڵ�����ݽڵ�������Ϣ����container������ֵ����
����m_container���������Ԫm_dataNodeHead,m_emptyNodeHead���ֱ���Ϊ�սڵ���������ݽڵ������ͷ��
*ΪʲôҪ��˫���أ��򵥵�˵��ɾ��������Ҫ�ҵ���һ���ڵ㡣deleteOne()����һ�����ӣ�����Ϊ��֧���ڱ�����ɾ���α���ָ��Ԫ�ء����鷳�ˡ�
ʹ�ñؿ���
Chain�������·�ʽ������
ptr = chain.first();
while(ptr){
ptr = chain.next();
}
��ʵ���ǿ��Գ����һ��˳�����±����ĵ������ģ���Ϊ�����ǰ�ȫ��ʽ��������ע��ɾ���α����ڵĽڵ�����⡣
��ô��first(), next(), deleteOne()���Ǵ��������������
�����Ҫ�µĵ�����ʽ����ô�����ͬ�ĵ�������֧��ɾ������ô���Ǽ��������ܱ����ͻ��Ҳ����˵���ǵı���������Ƕ�ף����������
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
#define P_FIRST_EMPTY_NODE m_emptyNodeHead.nextNode; // ��һ���սڵ�
#define P_LAST_EMPTY_NODE m_emptyNodeHead.preNode; // ���һ���սڵ�
#define P_FIRST_DATA_NODE m_dataNodeHead.nextNode; // ��һ���սڵ�
#define P_LAST_DATA_NODE m_dataNodeHead.preNode; // ���һ���սڵ�
#define INVALID_BROWSER GET_ADDRESS(M_DATA_NODE_HEAD_ID)
#define P_DATA_NODE_HEAD GET_ADDRESS(M_DATA_NODE_HEAD_ID)
#define P_EMPTY_NODE_HEAD GET_ADDRESS(M_EMPTY_NODE_HEAD_ID)

namespace NEWBEE_UTILS{

    //���ݽڵ㣺full node
    //�սڵ㣺empty node
    //�α꣺    m_browser4DownSequenceLooping
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
        void printSelf(NEWBEE_UTILS::Logging *log, NEWBEE_UTILS::Logging::LogLeveL level, int mode=0/*1 ��ʾ����ģʽ*/)
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
            m_log->infologs("<#$%^#$%^����Ͱ %s �� D: %d E: %d ��#$%^#$%^", typeid(*this).name(), this->sumTotalNumber(),sumAvailable());
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
        Chain() : M_EMPTY_NODE_HEAD_ID (MAX_CONTAINER_LENGTH),  // λ�������洢����ĵ����ڶ�����
            M_DATA_NODE_HEAD_ID(MAX_CONTAINER_LENGTH +1),  // λ�������洢��������һ����
            M_MIN_ID_OF_STORAGE(0),
            M_MAX_ID_OF_STORAGE(MAX_CONTAINER_LENGTH -1) // λ�������洢����ĵ�����������
        {
            _Chain();
        }
        virtual ~Chain(){};
        void clear(){
            _Chain();
        }
        void replace();
        T* allocateLastwithCount(){ // out of date
            this->increase();//ͳ�Ƽ��������Ӱ��Ч�ʣ�����ֻ��һ���...
            return allocateLast();
        }
        inline T* push(T& data) // ѹջ���������ݷ�������Ŀ�ͷ
        {//����ڿ������ͷһ����������������һ����
            T *allocatedNode = allocate_from_head();
            if(allocatedNode){
                *allocatedNode = data;
            }
            return allocatedNode;
        }
        T* allocate_from_head()
        { // �ѿ����ĵ�һ���ڵ㣬��ӵ��������ĵ�һ����
            if(isFull()){
                return 0;
            }else{
                Node* allocatedNode = m_emptyNodeHead.nextNode; // ������ڵ�
                Node* nextEmptyNode = allocatedNode->nextNode; // ������ڵ����һ���սڵ㣬������Ϊ�սڵ����ϵĵ�һ���սڵ�
                Node* firstDataNode = m_dataNodeHead.nextNode; // ���ݽڵ����ϣ���һ�����ݽڵ�
                //���ñ�����ڵ㡣
                allocatedNode->nextNode = firstDataNode; 
                allocatedNode->preNode = P_DATA_NODE_HEAD;
                //���ÿսڵ�ͷ
                m_emptyNodeHead.nextNode = nextEmptyNode;
                //�������ݽڵ�ͷ
                m_dataNodeHead.nextNode = allocatedNode;
                //���ã�����֮ǰ�ģ���һ�����ݽڵ�
                firstDataNode->preNode = allocatedNode;
                //���� pNextEmptyNode
                nextEmptyNode->preNode = P_EMPTY_NODE_HEAD;
                m_dataNodeCounter++;
                return &(allocatedNode->m_data);
            }
        }
        T* allocateLast()
        { // �ѿ����ĵ�һ���ڵ㣬��ӵ������������һ����
            if(isFull()){
                return 0;
            }else{
                Node* allocatedNode = m_emptyNodeHead.nextNode; // ������ڵ�
                Node* nextEmptyNode = allocatedNode->nextNode; // ������ڵ����һ���սڵ㣬������Ϊ�սڵ����ϵĵ�һ���սڵ�
                Node* lastDataNode = m_dataNodeHead.preNode; // ���ݽڵ����ϣ����һ�����ݽڵ�
                //���ñ�����ڵ�
                allocatedNode->preNode = lastDataNode; // 
                allocatedNode->nextNode = P_DATA_NODE_HEAD;
                //���ÿսڵ�ͷ
                m_emptyNodeHead.nextNode = nextEmptyNode;
                //�������ݽڵ�ͷ
                m_dataNodeHead.preNode = allocatedNode;
                //���ã�����֮ǰ�ģ����һ�����ݽڵ�
                lastDataNode->nextNode = allocatedNode;
                //���� pNextEmptyNode
                nextEmptyNode->preNode = P_EMPTY_NODE_HEAD;
                m_dataNodeCounter++;
                return &(allocatedNode->m_data);
            }
        }
        inline T* append(T& data) // ׷�ӣ��������������������
        {//����ڿ������ͷһ����������������һ����
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

        bool isEnd(){//�α��Ƿ�ָ�����һ�����ݽڵ㡣�α�ָ��һ�����ݽڵ㣬���Ҹ����ݽڵ��nextNodeָ������ͷ�ڵ㣬��ô���ͱ����α�ָ�����һ�����ݽڵ㡣
            return m_browser4DownSequenceLooping != INVALID_BROWSER && GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_browser4DownSequenceLooping->nextNode;
        }
        bool isfirst(){//�α��Ƿ�ָ���һ�����ݽڵ㡣�α�ָ��һ�����ݽڵ㣬���Ҹ����ݽڵ��preNodeָ������ͷ�ڵ㣬��ô���ͱ����α�ָ���һ�����ݽڵ㡣
            return m_browser4DownSequenceLooping != INVALID_BROWSER && GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_browser4DownSequenceLooping->preNode;
        }
        bool isBrowserVaild(){
            return m_browser4DownSequenceLooping != INVALID_BROWSER;
        }
        T* get(){//�α겻�������ص�ǰ����
            if(isBrowserVaild())
            {
                return &(m_browser4DownSequenceLooping->m_data);
            }else{
                return 0;
            }
        }
        T* first(){//���գ����ص�һ��node���α�ָ��ýڵ㣻���򷵻�0���α��޲���
            if(isEmpty()){
                return 0;
            }else{
                m_browser4DownSequenceLooping = m_dataNodeHead.nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }
        T* next(){//���У�������һ��node���α�ָ��ýڵ㣻���򷵻�0���α��޲���
            if( GET_ADDRESS(M_DATA_NODE_HEAD_ID) == m_browser4DownSequenceLooping->nextNode){
                return 0;
            }else {
                m_browser4DownSequenceLooping = m_browser4DownSequenceLooping->nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }
        T* firstEmpty(){//���գ����ص�һ��node���α�ָ��ýڵ㣻���򷵻�0���α��޲���
            if(isFull()){
                return 0;
            }else{
                m_browser4DownSequenceLooping = m_emptyNodeHead.nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }
        T* nextEmpty(){//���У�������һ��node���α�ָ��ýڵ㣻���򷵻�0���α��޲���
            if( GET_ADDRESS(M_EMPTY_NODE_HEAD_ID) == m_browser4DownSequenceLooping->nextNode){
                return 0;
            }else {
                m_browser4DownSequenceLooping = m_browser4DownSequenceLooping->nextNode;
                return &(m_browser4DownSequenceLooping->m_data);
            }
        }

        T* first(T*)
        {//����ͬnext()���������α꣬�Ա���Ƕ�����⡣
            if(isEmpty())
            {
                return 0;
            }else
            {
                return &((m_dataNodeHead.nextNode)->m_data);
            }
        }
        T* next(T* pData)
        {//����ͬnext()���������α꣬�Ա���Ƕ�����⡣
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
        {//�������ڵ����սڵ�����
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
            m_emptyNodeHead.preNode = GET_ADDRESS(M_MAX_ID_OF_STORAGE);//�������һ����Ԫ
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
        //T* firstfast(){//�����һ�㣬��Ҫ����Ƕ��ѭ����not implement, yet
        //    return first();
        //}
        //T* nextfast(){
        //    return next();
        //}
        typedef struct chain_expand_node{//������չ����Ŀռ䣬�����򲻵�����ò�Ҫ��չ������һ��ʼ�ͷ����㹻��Ŀռ䡣��ʱ��û���õ�
            Node *pNextContainer;
            chain_expand_node *pNextExpandNode;
            
        }stChainExpandNode;
        void hangOntheHook(Node* hooker, Node* pNode)
        {//?�������á���������������
            //�Ѹýڵ���Ϊһ�����ݽڵ㣬��ӵ�hooker����һ����
            pNode->nextNode = hooker->nextNode;
            pNode->preNode = hooker;
            //�޸�hookerԭ��һ�����ݽڵ㡣
            hooker->nextNode->preNode = pNode;
            //�޸����ݽڵ�ͷ
            hooker->nextNode = pNode;
        }
        static Node* convertDataToNode(T* pData)
        {
            return reinterpret_cast<Node*>(pData);
        }
        void moveBrowser4DownSequenceLooping(Node* pNode)
        {//���Ҳ�ͱ����㷨�йأ����֧�ֵ�˳�����±�����
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
            //�Ѹýڵ���Ϊһ���սڵ㣬��ӵ��սڵ�����ͷ��
            pNode->nextNode = m_emptyNodeHead.nextNode;
            pNode->preNode = GET_ADDRESS(M_EMPTY_NODE_HEAD_ID);
            //�޸�����һ���սڵ�
            (m_emptyNodeHead.nextNode)->preNode = pNode;
            //�޸Ŀսڵ�ͷ
            m_emptyNodeHead.nextNode = pNode;
            //���³�ʼ��������󣬻�Ӱ��һЩЧ��Ŷ��
            //pNode->m_data = m_emptyNodeHead.m_data;
        }
        void addNode2DataChainFromtheTail(Node* pNode)
        {
            //�Ѹýڵ���Ϊһ���սڵ㣬��ӵ��սڵ�����ͷ��
            pNode->nextNode = GET_ADDRESS(M_DATA_NODE_HEAD_ID);
            pNode->preNode = m_dataNodeHead.preNode;
            //�޸�����һ���սڵ�
            (m_dataNodeHead.preNode)-> nextNode= pNode;
            //�޸Ŀսڵ�ͷ
            m_dataNodeHead.preNode = pNode;
        }
        void removeDataNode(Node* pNode)
        {
            //�޸���һ��fullnode
            (pNode->preNode)->nextNode = pNode->nextNode;
            //�޸���һ��fullnode
            (pNode->nextNode)->preNode = pNode->preNode;
        }
        void deleteOnebyChainNodePointer(Node* pNode)
        {//���������ɾû���κ�Ӱ�죡
            removeDataNode(pNode);
            addNode2EmptyChainFromtheHead(pNode);
            m_dataNodeCounter--;
        }
        bool enlargeCapability()
        {//����ʵ��
        }
        Node* m_browser4DownSequenceLooping;
        const long M_EMPTY_NODE_HEAD_ID;//�սڵ�����ͷ�ڵ�Id
        const long M_DATA_NODE_HEAD_ID; //���ݽڵ�����ͷ�ڵ�Id
        const long M_MAX_ID_OF_STORAGE;
        const long M_MIN_ID_OF_STORAGE;
        Node m_container[MAX_CONTAINER_LENGTH+2];
        //Node* m_pNextContainer;//��������
        int m_dataNodeCounter;
    private:
        /*static */Logging* m_log;
    };

}
#endif //SY_CHAIN_H
