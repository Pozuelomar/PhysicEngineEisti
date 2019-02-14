#include "stdafx.h"
#include "QmParticle.h"
#include <iostream>

using namespace Quantum;

QmParticle::QmParticle() : acceleration(0, 0, 0), velocity(0, 0, 0), position(0, 0, 0)
{
}

QmParticle::QmParticle(glm::vec3 acc, glm::vec3 vel, glm::vec3 pos, float cha) : QmParticle()
{
	acceleration = acc;
	velocity = vel;
	position = pos;
	damping = 1.f;
	charge = cha;
	forces = {};
}

QmParticle::~QmParticle()
{

}

void QmParticle::integrate(float t)
{
	glm::vec3 sumForces = glm::vec3(0.0f, 0.0f, 0.0f);
	for each (Force* f in forces)
		sumForces += f->apply();

	position = position + t*velocity + 0.5f * (acceleration+sumForces) * t * t;
	velocity = velocity* (pow(damping, t)) + t*(acceleration+sumForces);

	if (gxUpdater != NULL)
		gxUpdater->update(position);
}

glm::vec3 QmParticle::getAcc()
{
	return acceleration;
}

glm::vec3 QmParticle::getVel()
{
	return velocity;
}

glm::vec3 QmParticle::getPos()
{
	return position;
}

float QmParticle::getCharge()
{
	return charge;
}

void QmParticle::addForce(Force* f) {
	forces.push_back(f);
}
