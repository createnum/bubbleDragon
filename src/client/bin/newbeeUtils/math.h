#ifndef MATH_NEWBEE_UTILS_H
#define MATH_NEWBEE_UTILS_H
#include "newbeeUtils/exports.h"
namespace NEWBEE_UTILS{

    const int BYTE_CAPACITY = 256;
    const int BYTES_OCUPPIED_INT = 4;
    const int BYTES_OCUPPIED_SHORT = 2;
    const int BYTES_OCUPPIED_CHAR = 1;
    NBUAPI int nbDigits(int num); // ��������λ��������1λ��
    NBUAPI int roundforPositive(float num);
    NBUAPI int roundforPositive(double num);
    NBUAPI inline int nbCelling(float number){
        int intNumber = static_cast<int>(number);
        if(number > static_cast<float>(intNumber)){
            return intNumber + 1;
        }else{
            return intNumber;
        }
    }
    NBUAPI inline int nbFloor(float number){
        return static_cast<int>(number);
    }
    //template<class T>
    //bool contain(T x, T y, T minX, T minY, T maxX, T maxY){ // �Ƿ����ĳ��- С�մ�����
    //    return (x >= minX) && (y >= minY) && (x <= maxX) && (y <= maxY);
    //}
    template<class T>
    T abs(T s1)
    {
        if(s1>0){
            return s1;
        }
        return -s1;
    }
    #define _nbMin(a,b)            (((a) < (b)) ? (a) : (b))
    #define _nbMax(a,b)            (((a) > (b)) ? (a) : (b))
    template<class T>
    inline T nbMax(T s1,T s2)
    {
        return (s1 > s2) ? s1: s2;
    }
    template<class T>
    inline T nbMax(T s1,T s2,T s3){// not commented
        if(s1 > s2){
            return nbMax(s1, s3);
        }else{//s1 > s2
            return nbMax(s2, s3);
        }
    }
    //template<class R,class S>
    //R _max(R& s1,S& s2)
    //{// not commented
    //    return (s1 > s2) ? s1: s2;
    //}
    //template<class R,class S,class T>
    //inline R _max(R& s1,S& s2,T& s3){// not commented
    //    if(s1 > s2){
    //        return _max(s1,s3);
    //    }else{//s1 > s2
    //        return _max(s2,s3);
    //    }
    //}
    template<class T>
    inline T nbMin(T s1,T s2)
    {
        return (s1 < s2) ? s1: s2;
    }
    template<class T>
    T nbMin(T s1,T s2,T s3){
        //    return ((s1 > s2)? s1: s2) > s3 ? ((s1 > s2)? s1: s2) : s3;
        if(s1 < s2){
            return nbMin(s1, s3);
        }else{//s1 > s2
            return nbMin(s2, s3);
        }
    }
    inline int roundforPositive(float num)
    {//��������
        return static_cast<int>( num +0.5);
    }
    inline int round(float num)
    {//
        if(num >=0){
            return static_cast<int>(num+0.5);
        }else{
            return static_cast<int>(num -0.5);
        }
    }
    inline int round(double num)
    {//
        if(num >=0){
            return static_cast<int>(num+0.5);
        }else{
            return static_cast<int>(num -0.5);
        }
    }
    inline int roundforPositive(double num)
    {//��������
        return static_cast<int>(num+0.5);
    }
    inline int digit(int i)
    {
        int digit = 1;
        while ( i= (i/10) )
        {
            digit++;
        }
        return digit;
    }
    inline int digit(long i)
    {
        int digit = 1;
        while ( i= (i/10) )
        {
            digit++;
        }
        return digit;
    }
    inline int digit(double i)
    {
        return digit(static_cast<long>(i));
    }
    class NBUAPI RandomDigital
    {
    public:
        void clear();
        int getRandomValue();
        int initRandomDigital(int scope); //���ݼ���Ԫ����������������
        int scope(){
            return m_scope;
        }
        RandomDigital()
        {
            m_scope = -1;
        }
        ~RandomDigital()
        {
        }
    protected:
        //int m_MIN, m_MAX;
        int m_scope;

    };
    class NBUAPI RandomAccess
    {// �����������ĳһ���ݼ��ϡ�
    public:
        int size(){
            return m_size;
        }
        RandomAccess()
        {
            RandomAccess_();
        }
        ~RandomAccess()
        {
            RandomAccess__();
        }
        int initCollection(int max, int min); //���ݼ���Ԫ����������������
        int getFirstUnit(int *pUnit);
        int getNextUnit(int *pUnit);
        void clear();
    protected:
        void RandomAccess_();
        void RandomAccess__();
        int *m_container;//Ԫ�ر��洦
        //bool m_isInitialized;
        int m_unSelectedLimit; //��ѡ��Ԫ�ص��Ͻ硣ָ�����һ����ѡԪ�ء�
        int m_size;
    };
}

#endif //(MATH_NEWBEE_UTILS_H)
