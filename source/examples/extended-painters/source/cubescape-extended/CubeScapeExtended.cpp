#include <extended-painters/cubescape-extended/CubeScapeExtended.h>

#include <reflectionzeug/Property.h>

CubeScapeExtended::CubeScapeExtended(gloperate::ResourceManager & resourceManager)
: CubeScape(resourceManager)
{
    addProperty<int> (std::string("numcubes"),  dynamic_cast<CubeScape*>(this), &CubeScape::numberOfCubes, &CubeScape::setNumberOfCubes);
    addProperty<bool>(std::string("animation"), dynamic_cast<CubeScape*>(this), &CubeScape::animation,     &CubeScape::setAnimation);
}

CubeScapeExtended::~CubeScapeExtended()
{
}
