#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <vector>
#include "QmParticle.h"


namespace Quantum {

	class QmWorld {
	public:
		QmWorld();
		~QmWorld();
		void integrate(float);
		void addParticle(QmParticle*);
		std::vector<QmParticle*> getParticles();
		void update(float dt);

		void clear();
		std::vector<QmParticle*> particles;

	private:
		float time;
		float tick(float t);
	};

}

#endif