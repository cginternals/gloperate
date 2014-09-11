#pragma once

#include <gloperate/capabilities/VirtualTimeCapability.h>

class CubeScape;

class VirtualTimeCapability : public gloperate::VirtualTimeCapability
{
public:
    VirtualTimeCapability(CubeScape & cubescape);
    virtual ~VirtualTimeCapability();

    virtual void update(float delta) override;

protected:
    CubeScape & m_cubescape;
};
