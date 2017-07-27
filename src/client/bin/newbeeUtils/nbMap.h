#ifndef NEWBEE_UTILS_MAP_H
#define NEWBEE_UTILS_MAP_H
//Ψһ��ֵӳ��
// key ֵ ���� ���� 
// MAP_BASIC_SIZE ��ʾ����ʱ�����Ԫ�ظ��������ԣ�ʹ��ʱҪ���ǣ��ڴ��ʹ�á���ֵ���˾޴󡣻��� ��T�Ǹ����࣬Ӧ����ָ�롣Ҳ���˹�С������Ч�ʼ��͡�
// ���������ûԪ�أ�β���ϲ�����ûԪ��
#include "basicChain.h"
namespace NEWBEE_UTILS
{
template <typename T>
class TailNode{
public:
    int key;
    T data;
    TailNode *nextNode;
    void release(){
        nextNode = 0;
        delete this;
    }
};
template <typename T>
struct NBUAPI BaseNode{
    bool hasUnit;
    int key;
    T data;
    TailNode<T> *nextNode;
};
template <class T, int MAP_BASIC_SIZE>
class NBUAPI NBMap
{
public:
    T *getFirstElement(){
        for(int i=0; i<MAP_BASIC_SIZE; i++){
            BaseNode<T> *baseUnit = &m_container[i];
            if(baseUnit->hasUnit){
                m_browser_base = i;
                m_browser_tail = 0;
                return &(baseUnit->data);
            }
            if(baseUnit->nextNode){
                m_browser_base = i;
                m_browser_tail = baseUnit->nextNode;
                return &(baseUnit->nextNode->data);
            }
        }
        return 0; // û�ҵ�
    }
    T *getNextElement(){
        if( m_browser_base < MAP_BASIC_SIZE){
            if(m_browser_tail){//��һ�α�����β�ڵ�
                if(0 != m_browser_tail->nextNode){
                    m_browser_tail = m_browser_tail->nextNode;
                    return &(m_browser_tail->data);
                }
            }else{//���������ڵ���
                if(m_container[m_browser_base].nextNode){
                    m_browser_tail = m_container[m_browser_base].nextNode;
                    return &(m_browser_tail->data);
                }
            }
        }
        m_browser_base++;
        while(m_browser_base < MAP_BASIC_SIZE){
            if(m_container[m_browser_base].hasUnit){//���������ڵ�
                    m_browser_tail = 0;
                    return &(m_container[m_browser_base].data);
            }else{//���ڵ�û�У�����β�ڵ�û��
                if(m_container[m_browser_base].nextNode){
                    m_browser_tail = m_container[m_browser_base].nextNode;
                    return &(m_browser_tail->data);
                }
            }
            m_browser_base++;
        }
        return 0; // û�ҵ�
    }
    int size(){
        return m_size;
    }
    int insert(int key, T data){ // Ϊ��Ч�ʲ������ظ�ֵ��⣬Ҳ����˵����֤����Ԫ�ؼ�ֵ��Ψһ�ԡ�
        BaseNode<T> *base_4_newUnit = &m_container[positioning(key)];
        if(base_4_newUnit->hasUnit){// ������λ���ѱ�ռ�ݣ���Ҫ����β���Ϲ�һ���ڵ㡣
            TailNode<T> *newUnit = m_emptyNodeChain.pop();
            if(0 == newUnit)
            {//�սڵ�������û��������һ���µģ�������ǰ�档
                newUnit = new TailNode<T>;
                if(0 == newUnit){
                    return -1;
                }
            }
            newUnit->key = key;
            newUnit->data = data;
            // ���ڻ�������β���ϣ�����������ͷһ����
            newUnit->nextNode = base_4_newUnit->nextNode;
            base_4_newUnit->nextNode = newUnit;
        }else{// ������λ��û�ˣ��ã����ˡ�
            base_4_newUnit->hasUnit = true;
            base_4_newUnit->key = key;
            base_4_newUnit->data = data;
        }
        m_size++;
        return 0;
    }
    inline bool if_browser_point_base(int pos){
        return pos == m_browser_base && 0 == m_browser_tail;
    }

    int erase(int key){
        BaseNode<T> *baseUnit = &m_container[positioning(key)];
        if(baseUnit->hasUnit){
            if(baseUnit->key == key){//�˴�������գ�β���ϲ�����ûԪ�ء�
                baseUnit->hasUnit = false;
                m_size--;
                return 0;
            }
        }
        TailNode<T> *tailUnit = baseUnit->nextNode;
        if(0 == tailUnit){//û��β��
            return 1;
        }
        if(tailUnit->key == key){ // β���ϵĵ�һ�����ǡ�
            baseUnit->nextNode = tailUnit->nextNode;
            m_emptyNodeChain.push(tailUnit);
            m_size--;
            return 0;
        }

        while(tailUnit->nextNode)
        {
            if(tailUnit->nextNode->key == key){
                TailNode<T> *removedUnit = tailUnit->nextNode;
                tailUnit->nextNode = removedUnit->nextNode;
                m_emptyNodeChain.push(removedUnit);
                m_size--;
                return 0;
            }
            tailUnit = tailUnit->nextNode;
        }
        return 1; // û�ҵ�
    }
    void shallowTruncate(){ // ��ָ�벿����ȫ�ضϣ������ⲿ��Ҫǳ������ʱ��������ֹ�ͷų���
        for(int i=0; i<MAP_BASIC_SIZE; i++){
            m_container[i].nextNode = 0;
        }
        m_emptyNodeChain.shallowTruncate();
    }
    void clear(){
        NBMap__();
        m_emptyNodeChain.clear();
    }
    T *findPointer(int key){
        return &find();
    }
    T *find(int key){
        BaseNode<T> *baseUnit = &m_container[positioning(key)];
        if(baseUnit->hasUnit){
            if(baseUnit->key == key){//���������ûԪ�أ�β���ϲ�����ûԪ�ء�
                return &(baseUnit->data);
            }
        }
        TailNode<T> *tailUnit = baseUnit->nextNode;
        while(tailUnit)
        {
            if(tailUnit->key == key){
                return &(baseUnit->data);
            }
            tailUnit = tailUnit->nextNode;
        }
        return 0; // û�ҵ�
    }
    int insertUniqueUnit(int key, T data)
    {// ����0 ��ʾ�ɹ���1��ʾ���ظ�Ԫ��
        if(0 == find(key)){
            return insert(key, data);
        }
        return 1;
    }
    NBMap(){
        NBMap_();
    }
    ~NBMap(){
        NBMap__();
    }
protected:
    NBMap(const NBMap<T, MAP_BASIC_SIZE>& m){
        NBMap_();
    }
    void NBMap__(){
        TailNode<T> *p =0, *delP =0;
        for(int i=0; i<MAP_BASIC_SIZE; i++){
            p = m_container[i].nextNode;
            while (p){
                delP = p;
                p = p->nextNode;
                delete delP;
            }
        }
        NBMap_();
    }
    void NBMap_(){
        memset(m_container, 0, sizeof(m_container));
        m_size = 0;
        m_browser_tail = 0;
        m_browser_base = -1;
    }
    inline int positioning(int key){ //���ⲿ��ֵ��λ���������ϡ�
        if(key >= MAP_BASIC_SIZE){
            return key % MAP_BASIC_SIZE;
        }
        if(key<0){
            return key%MAP_BASIC_SIZE + MAP_BASIC_SIZE;
        }
        return key;
    }
    BaseNode<T> m_container[MAP_BASIC_SIZE];
    HookChain< TailNode<T> > m_emptyNodeChain; //�սڵ�����
    int m_size;
    int m_browser_base;
    TailNode<T> *m_browser_tail;
}; // Class NBMap

template<class T, int MAP_BASIC_SIZE>
class NBUAPI NBPointerMap : public NBMap<T*, MAP_BASIC_SIZE>
{
public:
    T* pop(int key){// ����һ����ֵΪkey��Ԫ�أ��Ҳ�������0��
        BaseNode<T*> *baseUnit = &(this->m_container[this->positioning(key)]);
        if(baseUnit->hasUnit){
            if(baseUnit->key == key){//�˴�������գ�β���ϲ�����ûԪ�ء�
                baseUnit->hasUnit = false;
                this->m_size--;
                return baseUnit->data;
            }
        }
        TailNode<T*> *tailUnit = baseUnit->nextNode;
        if(0 == tailUnit){//û��β��
            return 0;
        }
        if(tailUnit->key == key){ // β���ϵĵ�һ�����ǡ�
            baseUnit->nextNode = tailUnit->nextNode;
            this->m_emptyNodeChain.push(tailUnit);
            this->m_size--;
            return tailUnit->data;
        }
        while(tailUnit->nextNode)
        {
            if(tailUnit->nextNode->key == key){
                TailNode<T*> *removedUnit = tailUnit->nextNode;
                tailUnit->nextNode = removedUnit->nextNode;
                this->m_emptyNodeChain.push(removedUnit);
                this->m_size--;
                return removedUnit->data;
            }
            tailUnit = tailUnit->nextNode;
        }
        return 0; // û�ҵ�
    }
    T* find(int key){
        BaseNode<T*> *baseUnit = &(this->m_container[this->positioning(key)]);
        if(baseUnit->hasUnit){
            if(baseUnit->key == key){//�˴�������գ�β���ϲ�����ûԪ�ء�
                return baseUnit->data;
            }
        }
        TailNode<T*> *tailUnit = baseUnit->nextNode;
        while(tailUnit)
        {
            if(tailUnit->key == key){
                return tailUnit->data;
            }
            tailUnit = tailUnit->nextNode;
        }
        return 0; // û�ҵ�
    }
    T* getFirstElement(){
        for(int i=0; i<MAP_BASIC_SIZE; i++){
            BaseNode<T*> *baseUnit = &(this->m_container[i]);
            if(baseUnit->hasUnit){
                this->m_browser_base = i;
                this->m_browser_tail = 0;
                return baseUnit->data;
            }
            if(baseUnit->nextNode){
                this->m_browser_base = i;
                this->m_browser_tail = baseUnit->nextNode;
                return baseUnit->nextNode->data;
            }
        }
        return 0; // û�ҵ�
    }
    T* getNextElement(){
        if( this->m_browser_base < MAP_BASIC_SIZE){
            if(this->m_browser_tail){//��һ�α�����β�ڵ�
                if(0 != this->m_browser_tail->nextNode){
                    this->m_browser_tail = this->m_browser_tail->nextNode;
                    return this->m_browser_tail->data;
                }
            }else{//���������ڵ���
                if(this->m_container[this->m_browser_base].nextNode){
                    this->m_browser_tail = this->m_container[this->m_browser_base].nextNode;
                    return this->m_browser_tail->data;
                }
            }
        }
        this->m_browser_base++;
        while(this->m_browser_base < MAP_BASIC_SIZE){
            if(this->m_container[this->m_browser_base].hasUnit){//���������ڵ�
                this->m_browser_tail = 0;
                return this->m_container[this->m_browser_base].data;
            }else{//���ڵ�û�У�����β�ڵ�û��
                if(this->m_container[this->m_browser_base].nextNode){
                    this->m_browser_tail = this->m_container[this->m_browser_base].nextNode;
                    return this->m_browser_tail->data;
                }
            }
            this->m_browser_base++;
        }
        return 0; // û�ҵ�
    }
};
}
#endif // NEWBEE_UTILS_MAP_H
