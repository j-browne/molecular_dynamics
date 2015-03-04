#include "vec3.h"
#include <cmath>

vec3::vec3(double x, double y, double z) {
	setX(x);
	setY(y);
	setZ(z);
}

void vec3::set(int i, double x) {
	val[i] = x;
}

void vec3::setX(double x) {
	val[0] = x;
}

void vec3::setY(double y) {
	val[1] = y;
}

void vec3::setZ(double z) {
	val[2] = z;
}

double vec3::get(int i) const {
	return val[i];
}

double vec3::getX() const {
	return val[0];
}

double vec3::getY() const {
	return val[1];
}

double vec3::getZ() const {
	return val[2];
}

double vec3::mag() const {
	return sqrt(vec3::dot(*this, *this));
}

vec3 vec3::unit() const {
	return *this/mag();
}

double vec3::dot(const vec3& v1, const vec3& v2) {
	double tot = 0;
	for (int i = 0; i < 3; ++i) {
		tot = tot + v1.get(i)*v2.get(i);
	}
	return tot;
}

vec3 vec3::cross(const vec3& v1, const vec3& v2) {
	return vec3(v1.getY()*v2.getZ()-v1.getZ()*v2.getY(),
	            v1.getZ()*v2.getX()-v1.getX()*v2.getZ(),
	            v1.getX()*v2.getY()-v1.getY()*v2.getX());
}

vec3 operator*(const vec3& v, double s) {
	return vec3(s*v.getX(), s*v.getY(), s*v.getZ());
}

vec3 operator*(double s, const vec3& v) {
	return vec3(s*v.getX(), s*v.getY(), s*v.getZ());
}

vec3 operator/(const vec3& v, double s) {
	return vec3(v.getX()/s, v.getY()/s, v.getZ()/s);
}

vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.getX()+v2.getX(),
	            v1.getY()+v2.getY(),
	            v1.getZ()+v2.getZ());
}

vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.getX()-v2.getX(),
	            v1.getY()-v2.getY(),
	            v1.getZ()-v2.getZ());
}

vec3 operator-(const vec3& v) {
	return vec3(-v.getX(),
	            -v.getY(),
	            -v.getZ());
}
