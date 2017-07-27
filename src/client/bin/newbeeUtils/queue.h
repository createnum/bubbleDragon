#ifndef NEWBEE_QUEUE_H
#define NEWBEE_QUEUE_H
#include "newbeeUtils/exports.h"
//基本先进先出队列，采用的是“头永远追不上尾（尾永远在头‘前’）”的算法。
//故实际申请的空间是 最大节点数 + 1。
namespace NEWBEE_UTILS
{
template<class T, int MAX_QUEUE_SIZE> class NBStack;
template <class T, int MAX_QUEUE_SIZE>
class NBUAPI NBQueue
{
    friend class NBStack<T, MAX_QUEUE_SIZE>;
public:
    inline bool full(){
        return size() == MAX_QUEUE_SIZE;
    }
    inline int size(){
        return size(m_head);
    }
    void toString(char* str)
    {
        //char temp[40];
        //sprintf(temp,"m_head: %2d - m_end: %2d = length: %2d", m_head, m_end, length());
        //strcat(str,temp);
    }
    T *last(){
        if(empty()){
            return 0;
        }else{
            m_browser = backward(m_end);
            return &m_container[m_browser];
        }
    }
    T *pre(){
        if(size()-1 == size(m_browser)){
            return 0;
        }else{
            m_browser = backward(m_browser);
            return &m_container[m_browser];
        }
    }
    T *first(){
        if(empty()){
            return 0;
        }else{
            m_browser = forward(m_head);
            return &m_container[m_browser];
        }
    }
    T *next(){
        if(0 == size(m_browser)){
            return 0;
        }else{
            m_browser = forward(m_browser);
            return &m_container[m_browser];
        }
    }
    NBQueue()/*:M_CAPACITY(MAX_QUEUE_SIZE+1)*/
    {
        NBQueue_();
    }
    virtual ~NBQueue()
    {
    }
    inline bool empty()
    {
        return 0 == size();
    }
    inline int enqueue(const T &item)
    {//以拷贝的方式加入队列。
        // 返回0表示成功，其他为错误代码
        if(full()){
            return -1;
        }else{
            m_container[m_end] = item;
            m_end = forward(m_end);
            return 0;
        }
    }
    inline int dequeue(T* item)
    {//返回0表示成功，其他为错误代码
        if(empty()){
            return 1;
        }else{
            m_head = forward(m_head);
            *item = m_container[m_head];
            return 0;
        }
    }
    inline void clear(){
        NBQueue_();
    }
protected:
    int size(int index){
        int diff = m_end - index;
        if( diff > 0){
            return diff - 1;
        }
        return diff + MAX_QUEUE_SIZE;
    }
    inline void NBQueue_(){
        m_head = 0; // m_head在什么位置并不那么重要。
        m_end = forward(m_head); // 重要的是m_end永远比m_head领先一步。
        m_browser = -1;
    }
    inline int forward(int index){
        return (index+1)%(MAX_QUEUE_SIZE+1);
    }
    inline int backward(int index){
        index--;
        if(index<0){
            index += (MAX_QUEUE_SIZE+1);
        }
        return index;
    }
    int m_head;//队列头，初始值为1。永远指向队列头节点的前一个节点。
    int m_end;//队列尾，初始值为0。永远指向队列尾节点的下一个节点。
    int m_browser;//用来遍历的游标。
    T m_container[MAX_QUEUE_SIZE+1];
};
template <class T, int MAX_STACK_SIZE>
class NBUAPI NBStack
{
public:
    inline T *top(){ // 栈顶第一个
        return m_queue.last();
    }
    inline T *lower(){ // 栈顶第一个
        return m_queue.pre();
    }
    inline T *bottom(){ // 栈底第一个
        return m_queue.first();
    }
    inline T *upper(){ // 栈底下一个
        return m_queue.next();
    }
    inline int push(const T &data){
        return m_queue.enqueue(data);
    }
    inline int pop(T &data){
        if(!m_queue.empty()){
            m_queue.m_end = m_queue.backward(m_queue.m_end);
            data = m_queue.m_container[m_queue.m_end];
            return 0;
        }
        return -1;
    }
    inline int pop(){  // 纯弹出抛弃
        if(!m_queue.empty()){
            m_queue.m_end = m_queue.backward(m_queue.m_end);
            return 0;
        }
        return -1;
    }
    inline bool full(){
        return m_queue.full();
    }
    inline int size(){
        return m_queue.size();
    }
    inline bool empty(){
        return m_queue.empty();
    }
    inline void clear(){
        m_queue.clear();
    }
    NBQueue<T, MAX_STACK_SIZE> m_queue;
};
template <class T, int MAX_QUEUE_SIZE>
class NBUAPI NBPointerQueue : public NBQueue<T*, MAX_QUEUE_SIZE>
{ // 不管释放指针内存
public:
    T* first(){
        if(this->empty()){
            return 0;
        }else{
            this->m_browser = this->forward(this->m_head);
            return this->m_container[this->m_browser];
        }
    }
    T* next(){
        if(0 == this->size(this->m_browser)){
            return 0;
        }else{
            this->m_browser = this->forward(this->m_browser);
            return this->m_container[this->m_browser];
        }
    }
    T* last(){
        if(this->empty()){
            return 0;
        }else{
            this->m_browser = this->backward(this->m_end);
            return this->m_container[this->m_browser];
        }
    }
    T* pre(){
        if(this->size()-1 == NBQueue<T* , MAX_QUEUE_SIZE>::size(this->m_browser)){
            return 0;
        }else{
            this->m_browser = this->backward(this->m_browser);
            return this->m_container[this->m_browser];
        }
    }
    inline int enqueue(T *item)
    {//返回0表示成功，其他为错误代码
        if(NBQueue<T* , MAX_QUEUE_SIZE>::full()){
            return -1;
        }else{
            NBQueue<T* , MAX_QUEUE_SIZE>::m_container[NBQueue<T* , MAX_QUEUE_SIZE>::m_end] = item;
            NBQueue<T* , MAX_QUEUE_SIZE>::m_end = this->forward(NBQueue<T* , MAX_QUEUE_SIZE>::m_end);
            return 0;
        }
    }
    inline int dequeue(T **item)
    {//返回0表示成功，其他为错误代码
        if(NBQueue<T* , MAX_QUEUE_SIZE>::empty()){
            return -1;
        }else{
            NBQueue<T* , MAX_QUEUE_SIZE>::m_head = forward(NBQueue<T* , MAX_QUEUE_SIZE>::m_head);
            *item = NBQueue<T* , MAX_QUEUE_SIZE>::m_container[NBQueue<T* , MAX_QUEUE_SIZE>::m_head];
            return 0;
        }
    }
    inline T* dequeue()
    {// 直接返回指针
        if(NBQueue<T* , MAX_QUEUE_SIZE>::empty()){
            return 0;
        }else{
            NBQueue<T* , MAX_QUEUE_SIZE>::m_head = forward(NBQueue<T* , MAX_QUEUE_SIZE>::m_head);
            return NBQueue<T* , MAX_QUEUE_SIZE>::m_container[NBQueue<T* , MAX_QUEUE_SIZE>::m_head];
        }
    }
    NBPointerQueue(){
        memset(this->m_container, 0, sizeof(this->m_container));
    }
    ~NBPointerQueue(){
        //while (!NBQueue<T* , MAX_QUEUE_SIZE>::empty())
        //{
        //    delete dequeue();
        //}
    }
protected:
};
template <class T, int MAX_QUEUE_SIZE>
class NBUAPI NBLiablePointerQueue : public NBPointerQueue<T, MAX_QUEUE_SIZE>
{ // 负责释放队列中指针所分配的内存
public:

    NBLiablePointerQueue(){}
    ~NBLiablePointerQueue(){
        while (!NBPointerQueue<T, MAX_QUEUE_SIZE>::empty())
        {
            delete NBPointerQueue<T, MAX_QUEUE_SIZE>::dequeue();
        }
    }
protected:
};
}
#endif // NEWBEE_QUEUE_H

