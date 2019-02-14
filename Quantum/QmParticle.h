#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>
#include "GxUpdater.h"
#include <iostream>

namespace Quantum {

	class Force {
	public:
		virtual glm::vec3 apply() = 0;
	};

	class QmParticle  {
	public:
		QmParticle();
		QmParticle(glm::vec3 acc, glm::vec3 vel, glm::vec3 pos, float cha=1.0f);
		~QmParticle();
		virtual void integrate(float);
		
		glm::vec3 getAcc();
		glm::vec3 getVel();
		glm::vec3 getPos();
		float getCharge();

		GxUpdater* gxUpdater;
		void addForce(Force*);

	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		float charge;

		float damping;
		std::vector<Force *> forces;
	};



	class Drag : public Force {
	public:
		Drag(QmParticle *p, float k1, float k2) {
			particle = p;
			K1 = k1;
			K2 = k2;
		};
		glm::vec3 apply() {
			if (glm::length(particle->getVel()) == 0.0f)
				return glm::vec3(0.0f, 0.0f, 0.0f);

			float coef = K1 * glm::length(particle->getVel()) + K2 * glm::length(particle->getVel()) * glm::length(particle->getVel());

			return glm::normalize(particle->getVel()) * -coef;
		}
	private:
		QmParticle* particle;
		float K1;
		float K2;
	};

	class Spring : public Force {
	public:
		Spring(QmParticle *p1, QmParticle *p2, float rl, float c) {
			particle1 = p1;
			particle2 = p2;
			restlength = rl;
			springconstant = c;
		};
		glm::vec3 apply() {
			glm::vec3 d = particle1->getPos() - particle2->getPos();

			if (d == glm::vec3(0.0f, 0.0f, 0.0f))
				return glm::vec3(0.0f, 0.0f, 0.0f);

			float coef = -(glm::length(d) - restlength)*springconstant;
			return glm::normalize(d) * coef;
		}
	private:
		QmParticle* particle1;
		QmParticle* particle2;
		float restlength;
		float springconstant;
	};

	class Magnetism : public Force {
	public:
		Magnetism(QmParticle *p1, QmParticle *p2, float k = 1.0f) {
			particle1 = p1;
			particle2 = p2;
			K = k;
		};
		glm::vec3 apply() {
			glm::vec3 d = particle1->getPos() - particle2->getPos();
			if (glm::length(d) == 0.0f)
				return glm::vec3(0.0f, 0.0f, 0.0f);
			float l = glm::length(d);
			if (glm::length(d) <= 0.4f)
				l = 0.4f/l; // limiter attraction en cas de colision
			float coef = K * particle1->getCharge() * particle2->getCharge();
			return glm::normalize(d) * coef / l / l;
		}
	private:
		QmParticle* particle1;
		QmParticle* particle2;
		float K;
	};
}

#endif