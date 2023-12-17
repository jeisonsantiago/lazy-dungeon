#ifndef VEC2_H
#define VEC2_H

namespace lazyDungeon::math {

template<typename T>
struct vec2{
    T x = 0;
    T y = 0;

    inline vec2(T x, T y):x(x),y(y){};
    inline vec2(T v):x(v),y(v){};
    inline vec2() = default;

    // operator+
inline vec2<T> operator+(const vec2<T>&v2) const{
        return vec2<T>(this->x + v2.x,this->y + v2.y);
    }

    // operator+=
    inline vec2<T> &operator+=(const vec2<T>&v2){
        this->x += v2.x;
        this->y += v2.y;
        return *this;
    }

    // operator+=
    inline vec2<T> &operator*=(const vec2<T>&v2){
        this->x *= v2.x;
        this->y *= v2.y;
        return *this;
    }

    // operator-
    inline vec2<T> operator-(const vec2<T>&v2) const{
        return vec2<T>(this->x - v2.x,this->y - v2.y);
    }

    // operator-=
    inline vec2<T> &operator-=(const vec2<T>&v2){
        this->x -= v2.x;
        this->y -= v2.y;
        return *this;
    }

    // operator*
    inline vec2<T> operator*(const vec2<T> v2) const{
        return vec2<T>(this->x * v2.x,this->y * v2.y);
    }

    // operator*
    inline vec2<T> operator*(const T& s) const{
        return vec2<T>(this->x * s,this->y * s);
    }

    // operator*=
    inline vec2<T> operator*=(const T& s){
        this->x *= s;
        this->y *= s;
        return *this;
    }

    inline vec2<T> normalized(){
        T length = std::sqrt(x*x + y*y);

        if(length == 0){
            return {0,0};
        }

        vec2<T> res(x/length,y/length);
        return res;
    }

    inline T length(){
        T length = std::sqrt(x*x + y*y);

        return length;
    }

    void print(){
        //JENGINE_INFO("x:%f y:%f", x,y);
    }

};

}




#endif //VEC2_H
