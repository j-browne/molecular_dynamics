#include "vec3.h"
#include "particle.h"

particle::particle(): mass(1), pos(0,0,0), vel(0,0,0), acc(0,0,0) {
}

void particle::setMass(double m) {
	mass = m;
}

void particle::setPos(const vec3& p) {
	pos = p;
}

void particle::setVel(const vec3& v) {
	vel = v;
}

void particle::setAcc(const vec3& a) {
	acc = a;
}

double particle::getMass() const {
	return mass;
}

vec3 particle::getPos() const {
	return pos;
}

vec3 particle::getVel() const {
	return vel;
}

vec3 particle::getAcc() const {
	return acc;
}
