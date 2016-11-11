//Primitiveなオブジェクトの構造体を入れ込んでおく
#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#define _OX_EPSILON_ 0.000001f

struct Float3 : D3DXVECTOR3{
	float x, y, z;
	Float3(){}
	Float3(float x, float y, float z) : x(x),y(y),z(z){  }
	Float3(const D3DXVECTOR3& f) : D3DXVECTOR3(f){}
	~Float3(){}

	Float3 operator +(const Float3 &r) const {
		return Float3(x + r.x, y + r.y, z + r.z);
	}

	Float3 operator-(const Float3 &r) const {
		return Float3(x - r.x, y - r.y, z - r.z);

	}
	Float3 operator-(){
		return Float3(x - 1.0f, y - 1.0f, z - 1.0f);

	}

	Float3 operator *(const Float3 &r) const {
		return Float3(x*r.x, y*r.y, z*r.z);
	}
	Float3 operator /(const Float3 &r) const {
		return Float3(x/r.x, y/r.y, z/r.z);
	}
	Float3 operator *(float r) const {
		return Float3(x*r, y*r, z*r);
	}

	Float3 operator /(float r)const {
		return Float3(x / r, y / r, z / r);
	}
	friend Float3 operator *(float l, const Float3 &r){
		return Float3(r.x*l, r.y*l, r.z*l);
	}

	friend Float3 operator /(float l, const Float3 &r){
		return Float3(r.x/l, r.y/l, r.z/l);
	}
	float  dot(const Float3 &r) const {
		return x*r.x + y * r.y + z*r.z;
	}

	Float3 cross(const Float3 &r) const{
		return Float3(x*r.z - z*r.y, z*r.x - x*r.z, x* r.y - y*r.x);
	}

	float length() const{
		return sqrtf(lengthSq());
	}

	float lengthSq() const{
		return x*x + y*y + z*z;
	}

	void norm(){
		const float len = length();
		if (len > 0.0f){
			x /= len;
			y /= len;
			z /= len;
		}
	}
	Float3 normalized() const{
		const float len = length();
		if (len > 0.0f){
			return Float3(x / len, y / len, z / len);
		}
		return Float3(0.0f, 0.0f, 0.0f);

	}

	float distance(const Float3 &r) const{ //二点間距離
		Float3 dist = Float3(x,y,z) - r;
		return dist.length();
	}

	Float3& operator =(const D3DXVECTOR3 &r){
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}
	

	const float Get(int i) const {
			return *((&x) + i);
	}


};

typedef Float3 Point;

struct Vector3 : public Float3{ //原点を通る無限遠の直線と考えたベクトル?
	Vector3() {}
	Vector3(float x, float y, float z) : Float3(x, y, z){}
	Vector3(const Float3 &r) : Float3(r){}
	~Vector3(){}

	Vector3& operator =(const Float3 &r){
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}

	Vector3& operator =(const D3DXVECTOR3 &r){
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}
	void norm(){
		const float len = length();
		if (len > 0.0f){
			x /= len;
			y /= len;
			z /= len;
		}
	}

	bool isVertical(const Vector3 &r) const{
		float d = dot(r);
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);//誤差範囲(十分0と認められるなら)true
	}
	bool isParallel(const Vector3 &r) const{
		float d = cross(r).lengthSq();
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);
	}
	bool isSharpAngle(const Vector3 &r) const{
		return (dot(r) >= 0.0f);
	}


};
struct Line{
	Point p; //ベクトル開始位置
	Vector3 v; //方向ベクトル
	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f){}
	Line(const Point &p, const Vector3 &v) : p(p), v(v){}
	~Line(){}
	//線上の座標を得る
	Point getPoint(float t) const{
		return p + t*v;
	}
};
//点pからv分の長さをもつ線分
struct Segment : public Line{
	Segment(){}
	Segment(const Point &p, const Vector3 &v) : Line(p, v){}
	Segment(const Point &p1, const Point &p2) : Line(p1, p2 - p1){}

	Float3 getEndPoint() const {
		return p + v;
	}
};

//球
struct Sphere{
	Point p; //中心点
	float r; //半径
	Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f){}
	Sphere(const Point & p, float r) : p(p), r(r){}
	~Sphere(){}

};

//Align-Axis-BoundingBox
struct AABB{
	Point p; //中心点
	Float3 hl; //各軸の辺の長さの半分
	AABB(){}
	AABB(const Point & p, const Float3 &hl) : p(p), hl(hl){}

	float lenX() const { return hl.x * 2.0f; };
	float lenY() const { return hl.y * 2.0f; };
	float lenZ() const { return hl.z * 2.0f; };
	float len(int i){ //iによってXYZの順番に取り出す
		return *((&hl.x) + i)*2.0f;

	}

	float GetMin(int i){ //最小の場所を返す
		return p.Get(i)-len(i)/2.0f;
	}
	float GetMax(int i){ //最小の場所を返す
		return p.Get(i) + len(i) / 2.0f;
	}
};

#endif