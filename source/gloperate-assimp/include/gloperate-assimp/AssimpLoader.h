
#pragma once


#include <gloperate/resources/Loader.h>

#include <gloperate-assimp/gloperate-assimp_api.h>


struct aiScene;


namespace gloperate_assimp
{


/**
*  @brief
*    Resource loader for ASSIMP scenes
*/
class GLOPERATE_ASSIMP_API AssimpLoader : public gloperate::Loader<aiScene>
{
public:
    bool canLoad(const std::string & ext) const override;

    std::vector<std::string> loadingTypes() const override;

    std::string allLoadingTypes() const override;

    /**
     *  @remarks
     *    Scene must be deleted with `aiReleaseImport(scene)`
     */
    aiScene * load(const std::string & filename, std::function<void(int, int)> progress) const override;
};


} // namespace gloperate_assimp
