#include <extended-examples/ExtendedCubeScape/ExtendedCubeScape.h>

#include <reflectionzeug/Property.h>

ExtendedCubeScape::ExtendedCubeScape(gloperate::ResourceManager * /*resourceManager*/)
{
    addProperty<int> (std::string("numcubes"),  dynamic_cast<CubeScape*>(this), &CubeScape::numberOfCubes, &CubeScape::setNumberOfCubes);
    addProperty<bool>(std::string("animation"), dynamic_cast<CubeScape*>(this), &CubeScape::animation,     &CubeScape::setAnimation);
}

ExtendedCubeScape::~ExtendedCubeScape()
{
}
