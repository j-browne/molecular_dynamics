#ifndef VEC3
#define VEC3

class vec3 {
private:
	double val[3];
public:
	vec3(double x, double y, double z);

	void setX(double x);
	void setY(double y);
	void setZ(double z);
	double getX() const;
	double getY() const;
	double getZ() const;

	double mag();
	vec3 unit();
	static double dot(const vec3& v1, const vec3& v2);
	static vec3 cross(const vec3& v1, const vec3& v2);
};

vec3 operator*(const vec3& v, double s);
vec3 operator*(double s, const vec3& v);
vec3 operator/(const vec3& v, double s);
vec3 operator+(const vec3& v1, const vec3& v2);
vec3 operator-(const vec3& v1, const vec3& v2);
vec3 operator-(const vec3& v);

#endif // VEC3
