#ifndef NEWBEE_UTILS_RECT_H
#define NEWBEE_UTILS_RECT_H
#include "nbPoint.h"
namespace NEWBEE_UTILS
{
template <class T>
class NBRect
{ // 默认左上角为最小点
public:
    NBRect(T x, T y, T w, T h){
        minX(x);
        minY(y);
        width(w);
        height(h);
    }
    void set(T x, T y, T w, T h){
        minX(x);
        minY(y);
        width(w);
        height(h);
    }
    void setOutline(const NBRect* r){ // 只复制轮廓
        minX(0);
        minY(0);
        width(r->width());
        height(r->height());
    }
    bool contain(T x, T y){ // 是否包含某点- 小闭大开区间
        return x >=minX() && x < maxX() && y >=minY() && y < maxY();
    }
    virtual inline T maxX() const {
        return minX() + width();
    }
    virtual inline T maxY() const {
        return minY() + height();
    }
    inline void calcCenterRect(NBRect<T>* cRect, const T width) const {
        return calcCenterRect(cRect, width, width);
    }
    void calcCenterRect(NBRect<T>* cRect, const T w, const T h) const {
        cRect->minX((this->width()-w)/2);
        cRect->minY((this->height()-h)/2);
        cRect->width(w);
        cRect->height(h);
    }
    virtual NBPoint<T> minPoint() const {
        NBPoint<T> tempPoint;
        tempPoint.x = minX();
        tempPoint.y = minY();
        return tempPoint;
    }
    virtual void minPoint(NBPoint<T> mp){
        minX(mp.x);
        minY(mp.y);
    }
    virtual NBPoint<T> maxPoint() const {
        NBPoint<T> tempPoint;
        tempPoint.x = maxX();
        tempPoint.y = maxY();
        return tempPoint;
    }
    virtual NBPoint<T> leftBottomPoint() const {
        NBPoint<T> tempPoint;
        tempPoint.x = left();
        tempPoint.y = bottom();
        return tempPoint;
    }
    virtual NBPoint<T> leftTopPoint() const {
        NBPoint<T> tempPoint;
        tempPoint.x = left();
        tempPoint.y = top();
        return tempPoint;
    }
    NBPoint<T> rightTopPoint() const {
        NBPoint<T> tempPoint;
        tempPoint.x = right();
        tempPoint.y = top();
        return tempPoint;
    }
    NBPoint<T> rightBottomPoint() const {
        NBPoint<T> tempPoint;
        tempPoint.x = right();
        tempPoint.y = bottom();
        return tempPoint;
    }
    virtual inline T centerX() const {
        return (left() + right()) / 2;
    }
    virtual inline T centerY() const {
        return (top() + bottom()) / 2;
    }
    virtual inline T top() const {
        return maxY();
    }
    virtual inline T bottom() const {
        return minY();
    }
    virtual inline T left() const {
        return minX();
    }
    virtual inline T right() const {
        return maxX();
    }
    virtual inline T width()  const {
        return m_width;
    }
    virtual inline T height() const {
        return m_height;
    }
    virtual inline void width(T w){
        m_width = w;
    }
    virtual inline void height(T h){
        m_height = h;
    }
    virtual inline void addWidth(T v){
        m_width += v;
    }
    virtual inline void addHeight(T v){
        m_height += v;
    }
    virtual T minX() const {
        return m_x;
    }
    virtual inline void minX(T x){
        m_x = x;
    }
    virtual T minY() const {
        return m_y;
    }
    virtual inline void minY(T y){
        m_y = y;
    }
    virtual inline void addMinX(T dis){
        m_x += dis;
    }
    virtual inline void addMinY(T dis){
        m_y += dis;
    }
    NBRect(){
        NBRect_();
    }
    virtual ~NBRect(){
    }
    void clear(){
        NBRect_();
    }
protected:
    T m_x; // x方向最小值
    T m_y; // y方向最小值
    T m_width;
    T m_height;
    void NBRect_(){
        m_x = m_y = m_width = m_height = -1;
    }
}; // class NBRect
template <class T>
class NBUdRect : public NBRect<T> // upside down rect , for screen coordinate on Windows OS
{
public:
    virtual inline T top(){
        return minY();
    }
    virtual inline T bottom(){
        return maxY();
    }
};
//NBRect<int>::NBRect(int minX, int minY, int w, int h){
//    minX(minX);
//    minY(minY);
//    width(w);
//    height(h);
//}
typedef NBRect<int> NBRectI;
typedef NBRect<float> NBRectF;
typedef NBRect<double> NBRectD;
typedef NBUdRect<int> NBUdRectI;
typedef NBUdRect<float> NBUdRectF;
typedef NBUdRect<double> NBUdRectD;
}
#endif //NEWBEE_UTILS_RECT_H
