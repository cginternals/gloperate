#include "VirtualTimeCapability.h"

#include <basic-examples/CubeScape/CubeScape.h>

VirtualTimeCapability::VirtualTimeCapability(CubeScape & cubescape)
: m_cubescape(cubescape)
{
}

VirtualTimeCapability::~VirtualTimeCapability()
{
}

void VirtualTimeCapability::update(float delta)
{
    m_cubescape.update(delta);
}
