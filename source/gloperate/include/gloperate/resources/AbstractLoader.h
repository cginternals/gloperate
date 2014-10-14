#pragma once


#include <string>
#include <vector>
#include <gloperate/gloperate_api.h>


namespace globjects {
    class Referenced;
}
namespace gloperate
{


class GLOPERATE_API AbstractLoader {


    public:
        AbstractLoader();
        virtual ~AbstractLoader();

        virtual bool canLoad(const std::string & ext) const = 0;
        virtual std::vector<std::string> loadingTypes() const = 0;
        virtual std::string allLoadingTypes() const = 0;


};


} // namespace gloperate
