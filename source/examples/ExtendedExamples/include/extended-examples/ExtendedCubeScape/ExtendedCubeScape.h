#pragma once

#include <basic-examples/CubeScape/CubeScape.h>

#include <reflectionzeug/PropertyGroup.h>

#include <extended-examples/extended_examples_api.h>

class EXTENDED_EXAMPLES_API ExtendedCubeScape : public CubeScape, public reflectionzeug::PropertyGroup
{
public:
    ExtendedCubeScape(gloperate::ResourceManager & resourceManager);
    virtual ~ExtendedCubeScape();
};
