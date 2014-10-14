#pragma once


#include <string>
#include <vector>
#include <gloperate/gloperate_api.h>


namespace globjects {
    class Texture;
}
namespace gloperate
{


class AbstractLoader;
class AbstractStorer;


class ResourceManager {


    public:
        GLOPERATE_API ResourceManager();
        GLOPERATE_API virtual ~ResourceManager();

        GLOPERATE_API const std::vector<AbstractLoader *> & loaders() const;
        GLOPERATE_API const std::vector<AbstractStorer *> & storers() const;
        GLOPERATE_API void addLoader(AbstractLoader * loader);
        GLOPERATE_API void addStorer(AbstractStorer * storer);

        template <typename T>
        T * load(const std::string & filename) const;

        template <typename T>
        bool store(const std::string & filename, T * resource) const;


    protected:
        GLOPERATE_API std::string getFileExtension(const std::string & filename) const;


    protected:
        std::vector<AbstractLoader *> m_loaders;
        std::vector<AbstractStorer *> m_storers;


};


} // namespace gloperate


#include <gloperate/resources/ResourceManager.hpp>
