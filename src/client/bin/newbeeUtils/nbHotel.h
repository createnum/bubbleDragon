#ifndef NEWBEE_UTILS_HOTEL_H
#define NEWBEE_UTILS_HOTEL_H
#include "newbeeUtils/exports.h"
#include <memory.h>
// 一个有 固定“房间”数的“酒店”，只负责临时的“住宿”，不问“生老病死”。
namespace NEWBEE_UTILS
{
    template <class T, int NUMBER_OF_ROOMS>
    class NBUAPI NBPointerHotel
    { // 一个房间链表，挂接所有“离异”的单元。添加对象时，先试图从空房间链表中，找到一个房间。如果没有，以一个数值游标分配新房间。
    public:
        /*typedef*/ struct PoolRoom{
            T *tourist;
            PoolRoom *nextRoom;
            PoolRoom *preRoom;
            int containerIndex; // 代为记录数组下标，在添加的时候赋值
        };
        NBPointerHotel(){
            m_lobby = myContainer; // 第一个房间是头节点，不住客。
            myFirstEmptyRoom = myContainer+max_id()+1;
            myBrowser = 0;
            memset(myContainer, 0, sizeof(myContainer));
            min_unallocated_element_index = min_id();
        }
        int checkIn(T *newComer){
            PoolRoom *curRoom = 0;
            if(isFull()){
                return 1;
            }
            if(0 == myFirstEmptyRoom->nextRoom){ // 空房间链表为空，
                //myContainer[min_unallocated_element_index] = new Unit;
                curRoom = &myContainer[min_unallocated_element_index];
                curRoom->containerIndex = min_unallocated_element_index;
                min_unallocated_element_index++;
            }else{
                curRoom = myFirstEmptyRoom->nextRoom;
                myFirstEmptyRoom->nextRoom = curRoom->nextRoom;
            }
            curRoom->tourist = newComer;
            curRoom->tourist->setRoomId(curRoom->containerIndex); // !!
            curRoom->nextRoom = m_lobby->nextRoom;
            if(m_lobby->nextRoom){
                m_lobby->nextRoom->preRoom = curRoom;
            }
            curRoom->preRoom = m_lobby;
            m_lobby->nextRoom = curRoom;
            checkStatus();
            return 0;
        }
        int checkOut(T *leaver){
            int leaverRoomId = leaver->getRoomId();
            if(is_id_invalid(leaverRoomId)){
                return 1;
            }
            PoolRoom *curRoom = &myContainer[leaverRoomId];
            if(0 == curRoom->tourist || curRoom->tourist != leaver){ // 允许外界有删除出错的情况。这在外界是可接受的，比如多线程并发删除时。
                return 1;
            }
            curRoom->preRoom->nextRoom = curRoom->nextRoom;
            if(curRoom == myBrowser){
                myBrowser = myBrowser->preRoom;
            }
            if(curRoom->nextRoom){
                curRoom->nextRoom->preRoom = curRoom->preRoom;
            }
            curRoom->nextRoom = myFirstEmptyRoom->nextRoom;
            curRoom->preRoom = myFirstEmptyRoom;
            if(myFirstEmptyRoom->nextRoom){
                myFirstEmptyRoom->nextRoom->preRoom = curRoom;
            }
            myFirstEmptyRoom->nextRoom = curRoom;
            curRoom->tourist = 0;
            checkStatus();
            return 0;
        }
        T *firstRoom(){
            if(empty()){
                return 0;
            }
            myBrowser = m_lobby->nextRoom;
            return myBrowser->tourist;
        }
        T *nextRoom(){
            if(0 == myBrowser->nextRoom){
                return 0;
            }
            myBrowser = myBrowser->nextRoom;
            return myBrowser->tourist;
        }
        bool isFull(){
            return (0 == myFirstEmptyRoom->nextRoom) && (max_id()+1 == min_unallocated_element_index);
        }
        bool empty(){
            return 0 == m_lobby->nextRoom;
        }
    protected:
        bool is_id_invalid(int id){
            return id <min_id() || id > max_id();
        }
        bool is_container_index_used(int index){
            return index >=min_id() && index < min_unallocated_element_index;
        }
        inline int capacity(){
            return NUMBER_OF_ROOMS;
        }
        inline int max_id(){
            return NUMBER_OF_ROOMS;
        }
        inline int min_id(){
            return 1;
        }
        bool checkStatus(){
            int checkContainer[NUMBER_OF_ROOMS+1]; // 其值含义为，-1: 空元素; 0: 初始值; 1: 数据元素
            PoolRoom *curRoom = 0, *preRoom = 0;
            curRoom = m_lobby->nextRoom;
            preRoom = m_lobby;
            while(curRoom){
                int curIndex = curRoom->containerIndex;
                if(!is_container_index_used(curIndex)){
                    return false;
                }
                if(0 != checkContainer[curIndex]){
                    //new LogRuntime().error("skt bucket internal error - data element has duplicated ele of type: "+ checkContainer[curIndex] +" at: "+ curIndex);
                    return false;
                }
                if(preRoom != curRoom->preRoom){ 
                    //new LogRuntime().error("skt bucket internal error - data element's preRoom value is wrong: "+ curRoom->preRoom.containerIndex +", should be: "+ preRoom.containerIndex);
                    return false;
                }
                checkContainer[curIndex] = 1;
                curRoom = curRoom->nextRoom;
                preRoom = preRoom->nextRoom;
            }
            curRoom = myFirstEmptyRoom->nextRoom;
            while(0 != curRoom){
                int curIndex = curRoom->containerIndex;
                if(!is_container_index_used(curIndex)){
                    //new LogRuntime().error("skt bucket internal error - empty element has invalid container index: "+ curIndex);
                    return false;
                }
                if(0 != checkContainer[curIndex]){
                    //new LogRuntime().error("skt bucket internal error - empty element has duplicated ele of type: "+ checkContainer[curIndex] +" at: "+ curIndex);
                    return false;
                }
                checkContainer[curIndex] = -1;
                curRoom = curRoom->nextRoom;
            }
            for(int i=min_unallocated_element_index; i<=max_id(); i++){
                if(0 != checkContainer[i]){
                    //new LogRuntime().error("skt bucket internal error - unallocated element has unexpected ele at index: "+ i);
                    return false;                
                }
            }
            return true;
        }
    protected:
        PoolRoom myContainer[NUMBER_OF_ROOMS+2];
        PoolRoom *m_lobby; // 头节点，浪费掉
        PoolRoom *myFirstEmptyRoom; // 间隙回收的元素链表
        PoolRoom *myBrowser; // 游标
        int min_unallocated_element_index; // 当前未分配元素下标最小值，超过它的都是未分配的元素。
    };
}
#endif // NEWBEE_UTILS_HOTEL_H


