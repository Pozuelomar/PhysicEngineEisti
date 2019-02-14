#ifndef MYGXUPDATER_H
#define MYGXUPDATER_H

#include "Quantum.h"
#include "GxParticle.h"

class MyGxUpdater : public Quantum::GxUpdater {
public:
	MyGxUpdater();
	MyGxUpdater(GxParticle*);
	void update(glm::vec3);

private:
};

#endif


