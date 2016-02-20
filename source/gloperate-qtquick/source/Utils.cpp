
#include <gloperate-qtquick/Utils.h>

#include <glbinding/Binding.h>
#include <globjects/globjects.h>

    
namespace gloperate_qtquick
{


void Utils::initContext()
{
    glbinding::Binding::initialize(false);
    globjects::init();
}


} // namespace gloperate_qtquick
