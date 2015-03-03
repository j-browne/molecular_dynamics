#ifndef PARTICLE
#define PARTICLE

#include "vec3.h"

class particle {
private:
	double mass;
	vec3 pos;
	vec3 vel;
	vec3 acc;
public:
	particle();

	void setMass(double m);
	void setPos(const vec3& p);
	void setVel(const vec3& v);
	void setAcc(const vec3& a);
	double getMass() const;
	vec3 getPos() const;
	vec3 getVel() const;
	vec3 getAcc() const;
};

#endif // PARTICLE
