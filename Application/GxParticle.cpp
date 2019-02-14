#include "stdafx.h"
#include "GxParticle.h"
#include <iostream>

GxParticle::GxParticle()
{

}

GxParticle::GxParticle(glm::vec3 c, float rad)
{
	color = c;
	radius = rad;
}

GxParticle::~GxParticle()
{

}

void GxParticle::setPos(glm::vec3 newPos)
{
	position = newPos;
}

void GxParticle::setCol(glm::vec3 newColor)
{
	color = newColor;
}

void GxParticle::setRad(float newRadius)
{
	radius = newRadius;
}

glm::vec3 GxParticle::getPos()
{
	return position;
}

glm::vec3 GxParticle::getColor()
{
	return color;
}

float GxParticle::getRadius()
{
	return radius;
}
