#include "ray.h"

ray::ray()
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	this->m_vertices.resize(601); //TODO: Window width problem fix later...
}

ray::~ray()
{
}

const bool ray::castRay(const player* player, const world* world)
{
	return true;
}