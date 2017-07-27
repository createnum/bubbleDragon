#ifndef NEWBEE_UTILS_POINT_H
#define NEWBEE_UTILS_POINT_H
//简单数值类型的坐标点(x,y)封装
namespace NEWBEE_UTILS
{
template <typename T>
struct NBPoint{
    inline void set(T px, T py){
        x = px, y = py;
    }
    NBPoint(): x(-1), y(-1){
    }
    NBPoint(T px, T py) : x(px), y(py){
    }
    inline bool operator == (const NBPoint& p){
         return this->x == p.x && this->y == p.y;
    }
    T x;
    T y;
};
typedef NBPoint<int> NBPointI;
//inline bool NBPointI::operator == (NBPointI& p){
//    return this->x == p.x && this->y == p.y;
//}

typedef NBPoint<float> NBPointF;
typedef NBPoint<double> NBPointD;
}
#endif // NEWBEE_UTILS_POINT_H