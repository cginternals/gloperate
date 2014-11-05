#pragma once

#include <basic-painters/cubescape/CubeScape.h>

#include <reflectionzeug/PropertyGroup.h>

#include <extended-painters/extended_painters_api.h>

class EXTENDED_PAINTERS_API CubeScapeExtended : public CubeScape, public reflectionzeug::PropertyGroup
{
public:
	CubeScapeExtended(gloperate::ResourceManager & resourceManager);
	virtual ~CubeScapeExtended();
};
