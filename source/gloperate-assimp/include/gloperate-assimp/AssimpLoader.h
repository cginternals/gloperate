
#pragma once


#include <gloperate/resources/Loader.h>

#include <gloperate-assimp/gloperate-assimp_api.h>


namespace gloperate_assimp
{


class PolygonalGeometry;


/**
*  @brief
*    Loader for meshes (PolygonalGeometry) that uses ASSIMP for import
*/
class GLOPERATE_ASSIMP_API AssimpLoader : public gloperate::Loader<PolygonalGeometry>
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AssimpLoader();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AssimpLoader();

    // Virtual loader methods
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;
    virtual PolygonalGeometry * load(const std::string & filename, std::function<void(int, int)> progress) const override;
};


} // namespace gloperate_assimp
