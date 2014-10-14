#pragma once


#include <string>
#include <vector>
#include <gloperate/gloperate_api.h>


namespace globjects {
    class Referenced;
}
namespace gloperate
{


class GLOPERATE_API AbstractStorer {


    public:
        AbstractStorer();
        virtual ~AbstractStorer();

        virtual bool canStore(const std::string & ext) const = 0;
        virtual std::vector<std::string> storingTypes() const = 0;
        virtual std::string allStoringTypes() const = 0;


};


} // namespace gloperate
