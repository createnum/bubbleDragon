#ifndef NEWBEE_QUEUE_H
#define NEWBEE_QUEUE_H
#include "newbeeUtils/exports.h"
//�����Ƚ��ȳ����У����õ��ǡ�ͷ��Զ׷����β��β��Զ��ͷ��ǰ���������㷨��
//��ʵ������Ŀռ��� ���ڵ��� + 1��
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
    {//�Կ����ķ�ʽ������С�
        // ����0��ʾ�ɹ�������Ϊ�������
        if(full()){
            return -1;
        }else{
            m_container[m_end] = item;
            m_end = forward(m_end);
            return 0;
        }
    }
    inline int dequeue(T* item)
    {//����0��ʾ�ɹ�������Ϊ�������
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
        m_head = 0; // m_head��ʲôλ�ò�����ô��Ҫ��
        m_end = forward(m_head); // ��Ҫ����m_end��Զ��m_head����һ����
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
    int m_head;//����ͷ����ʼֵΪ1����Զָ�����ͷ�ڵ��ǰһ���ڵ㡣
    int m_end;//����β����ʼֵΪ0����Զָ�����β�ڵ����һ���ڵ㡣
    int m_browser;//�����������αꡣ
    T m_container[MAX_QUEUE_SIZE+1];
};
template <class T, int MAX_STACK_SIZE>
class NBUAPI NBStack
{
public:
    inline T *top(){ // ջ����һ��
        return m_queue.last();
    }
    inline T *lower(){ // ջ����һ��
        return m_queue.pre();
    }
    inline T *bottom(){ // ջ�׵�һ��
        return m_queue.first();
    }
    inline T *upper(){ // ջ����һ��
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
    inline int pop(){  // ����������
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
{ // �����ͷ�ָ���ڴ�
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
    {//����0��ʾ�ɹ�������Ϊ�������
        if(NBQueue<T* , MAX_QUEUE_SIZE>::full()){
            return -1;
        }else{
            NBQueue<T* , MAX_QUEUE_SIZE>::m_container[NBQueue<T* , MAX_QUEUE_SIZE>::m_end] = item;
            NBQueue<T* , MAX_QUEUE_SIZE>::m_end = this->forward(NBQueue<T* , MAX_QUEUE_SIZE>::m_end);
            return 0;
        }
    }
    inline int dequeue(T **item)
    {//����0��ʾ�ɹ�������Ϊ�������
        if(NBQueue<T* , MAX_QUEUE_SIZE>::empty()){
            return -1;
        }else{
            NBQueue<T* , MAX_QUEUE_SIZE>::m_head = forward(NBQueue<T* , MAX_QUEUE_SIZE>::m_head);
            *item = NBQueue<T* , MAX_QUEUE_SIZE>::m_container[NBQueue<T* , MAX_QUEUE_SIZE>::m_head];
            return 0;
        }
    }
    inline T* dequeue()
    {// ֱ�ӷ���ָ��
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
{ // �����ͷŶ�����ָ����������ڴ�
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

