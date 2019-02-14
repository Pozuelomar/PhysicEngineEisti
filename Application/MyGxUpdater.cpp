#include "stdafx.h"
#include "MyGxUpdater.h"

MyGxUpdater::MyGxUpdater()
{
}

MyGxUpdater::MyGxUpdater(GxParticle* object)
{
	this->gxObject = object;
}


void MyGxUpdater::update(glm::vec3 pos)
{
	((GxParticle*)gxObject)->setPos(pos);
}


