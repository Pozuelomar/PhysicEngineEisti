#include "stdafx.h"
#include <iostream>
#include "QmWorld.h"

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	time = 0.f;
}

QmWorld::~QmWorld()
{

}

void QmWorld::integrate(float t)
{
	time += t;
	for each (QmParticle* p in particles)
		p->integrate(t);
}

void QmWorld::addParticle(QmParticle* p)
{
	particles.push_back(p);
}

std::vector<QmParticle*> QmWorld::getParticles()
{
	return particles;
}

void QmWorld::clear()
{

	for each (QmParticle* p in particles)
	{
		delete p->gxUpdater;
		delete p;
	}
	particles.clear();
}

