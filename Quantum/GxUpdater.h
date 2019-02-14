#ifndef GXUPDATER_H
#define GXUPDATER_H

#include <glm/glm.hpp>

namespace Quantum {

	class GxUpdater {
	public:
		GxUpdater() {};
		~GxUpdater() {};
		virtual void update(glm::vec3) = 0;

	protected:
		void* gxObject;
	};

}

#endif