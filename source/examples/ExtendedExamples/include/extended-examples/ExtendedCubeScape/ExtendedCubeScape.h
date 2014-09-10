#pragma once

#include <basic-examples/CubeScape/CubeScape.h>

#include <extended-examples/extended_examples_api.h>

class EXTENDED_EXAMPLES_API ExtendedCubeScape : public CubeScape
{
public:
    ExtendedCubeScape(gloperate::ResourceManager * resourceManager = nullptr);
    virtual ~ExtendedCubeScape();
};
