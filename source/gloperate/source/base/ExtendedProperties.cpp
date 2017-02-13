
#include <gloperate/base/ExtendedProperties.h>


namespace gloperate
{


const std::string & VectorPrefix<int>::getPrefix()
{
    static const auto prefix = std::string("i");

    return prefix;
}    
    
    
} // namespace gloperate
