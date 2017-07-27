#ifndef NEWBEE_UTILS_MAP_H
#define NEWBEE_UTILS_MAP_H
//唯一键值映射
// key 值 必须 大于 
// MAP_BASIC_SIZE 表示构造时分配的元素个数。所以，使用时要考虑，内存的使用。该值不宜巨大。或者 类T是复杂类，应保存指针。也不宜过小，否则效率极低。
// 即便基容器没元素，尾巴上不见得没元素
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
        return 0; // 没找到
    }
    T *getNextElement(){
        if( m_browser_base < MAP_BASIC_SIZE){
            if(m_browser_tail){//上一次遍历到尾节点
                if(0 != m_browser_tail->nextNode){
                    m_browser_tail = m_browser_tail->nextNode;
                    return &(m_browser_tail->data);
                }
            }else{//遍历到基节点上
                if(m_container[m_browser_base].nextNode){
                    m_browser_tail = m_container[m_browser_base].nextNode;
                    return &(m_browser_tail->data);
                }
            }
        }
        m_browser_base++;
        while(m_browser_base < MAP_BASIC_SIZE){
            if(m_container[m_browser_base].hasUnit){//遍历到基节点
                    m_browser_tail = 0;
                    return &(m_container[m_browser_base].data);
            }else{//基节点没有，看看尾节点没有
                if(m_container[m_browser_base].nextNode){
                    m_browser_tail = m_container[m_browser_base].nextNode;
                    return &(m_browser_tail->data);
                }
            }
            m_browser_base++;
        }
        return 0; // 没找到
    }
    int size(){
        return m_size;
    }
    int insert(int key, T data){ // 为了效率并不做重复值检测，也就是说不保证插入元素键值的唯一性。
        BaseNode<T> *base_4_newUnit = &m_container[positioning(key)];
        if(base_4_newUnit->hasUnit){// 基容器位置已被占据，需要在其尾巴上挂一个节点。
            TailNode<T> *newUnit = m_emptyNodeChain.pop();
            if(0 == newUnit)
            {//空节点链表中没货。创建一个新的，挂在最前面。
                newUnit = new TailNode<T>;
                if(0 == newUnit){
                    return -1;
                }
            }
            newUnit->key = key;
            newUnit->data = data;
            // 挂在基容器的尾巴上，整个链表串的头一个。
            newUnit->nextNode = base_4_newUnit->nextNode;
            base_4_newUnit->nextNode = newUnit;
        }else{// 基容器位置没人，好，简单了。
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
            if(baseUnit->key == key){//此处不能清空，尾巴上不见得没元素。
                baseUnit->hasUnit = false;
                m_size--;
                return 0;
            }
        }
        TailNode<T> *tailUnit = baseUnit->nextNode;
        if(0 == tailUnit){//没有尾巴
            return 1;
        }
        if(tailUnit->key == key){ // 尾巴上的第一个就是。
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
        return 1; // 没找到
    }
    void shallowTruncate(){ // 把指针部分完全截断，用在外部需要浅拷贝的时候，这样防止释放出错。
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
            if(baseUnit->key == key){//即便基容器没元素，尾巴上不见得没元素。
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
        return 0; // 没找到
    }
    int insertUniqueUnit(int key, T data)
    {// 返回0 表示成功，1表示有重复元素
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
    inline int positioning(int key){ //把外部键值定位到基容器上。
        if(key >= MAP_BASIC_SIZE){
            return key % MAP_BASIC_SIZE;
        }
        if(key<0){
            return key%MAP_BASIC_SIZE + MAP_BASIC_SIZE;
        }
        return key;
    }
    BaseNode<T> m_container[MAP_BASIC_SIZE];
    HookChain< TailNode<T> > m_emptyNodeChain; //空节点链表。
    int m_size;
    int m_browser_base;
    TailNode<T> *m_browser_tail;
}; // Class NBMap

template<class T, int MAP_BASIC_SIZE>
class NBUAPI NBPointerMap : public NBMap<T*, MAP_BASIC_SIZE>
{
public:
    T* pop(int key){// 弹出一个键值为key的元素，找不到返回0。
        BaseNode<T*> *baseUnit = &(this->m_container[this->positioning(key)]);
        if(baseUnit->hasUnit){
            if(baseUnit->key == key){//此处不能清空，尾巴上不见得没元素。
                baseUnit->hasUnit = false;
                this->m_size--;
                return baseUnit->data;
            }
        }
        TailNode<T*> *tailUnit = baseUnit->nextNode;
        if(0 == tailUnit){//没有尾巴
            return 0;
        }
        if(tailUnit->key == key){ // 尾巴上的第一个就是。
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
        return 0; // 没找到
    }
    T* find(int key){
        BaseNode<T*> *baseUnit = &(this->m_container[this->positioning(key)]);
        if(baseUnit->hasUnit){
            if(baseUnit->key == key){//此处不能清空，尾巴上不见得没元素。
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
        return 0; // 没找到
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
        return 0; // 没找到
    }
    T* getNextElement(){
        if( this->m_browser_base < MAP_BASIC_SIZE){
            if(this->m_browser_tail){//上一次遍历到尾节点
                if(0 != this->m_browser_tail->nextNode){
                    this->m_browser_tail = this->m_browser_tail->nextNode;
                    return this->m_browser_tail->data;
                }
            }else{//遍历到基节点上
                if(this->m_container[this->m_browser_base].nextNode){
                    this->m_browser_tail = this->m_container[this->m_browser_base].nextNode;
                    return this->m_browser_tail->data;
                }
            }
        }
        this->m_browser_base++;
        while(this->m_browser_base < MAP_BASIC_SIZE){
            if(this->m_container[this->m_browser_base].hasUnit){//遍历到基节点
                this->m_browser_tail = 0;
                return this->m_container[this->m_browser_base].data;
            }else{//基节点没有，看看尾节点没有
                if(this->m_container[this->m_browser_base].nextNode){
                    this->m_browser_tail = this->m_container[this->m_browser_base].nextNode;
                    return this->m_browser_tail->data;
                }
            }
            this->m_browser_base++;
        }
        return 0; // 没找到
    }
};
}
#endif // NEWBEE_UTILS_MAP_H
