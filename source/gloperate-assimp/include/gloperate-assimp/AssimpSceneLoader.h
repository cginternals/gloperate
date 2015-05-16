
#pragma once


#include <gloperate/resources/Loader.h>

#include <gloperate-assimp/gloperate-assimp_api.h>


struct aiMesh;
struct aiScene;

namespace gloperate
{
    class Scene;
    class PolygonalGeometry;
}


namespace gloperate_assimp
{


/**
*  @brief
*    Loader for meshes (PolygonalGeometry) that uses ASSIMP for import
*/
class GLOPERATE_ASSIMP_API AssimpSceneLoader : public gloperate::Loader<gloperate::Scene>
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AssimpSceneLoader();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AssimpSceneLoader();

    // Virtual loader methods
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;
    virtual gloperate::Scene * load(const std::string & filename, std::function<void(int, int)> progress) const override;


protected:
    /**
    *  @brief
    *    Convert an ASSIMP scene into a gloperate scene
    *
    *  @param[in] scene
    *    ASSIMP scene (must be valid!)
    *
    *  @return
    *    Scene
    */
    gloperate::Scene * convertScene(const aiScene * scene) const;

    /**
    *  @brief
    *    Convert ASSIMP mesh into gloperate mesh
    *
    *  @param[in] mesh
    *    ASSIMP mesh (must be valid!)
    *
    *  @return
    *    Mesh, must be destroyed by the caller
    */
    gloperate::PolygonalGeometry * convertGeometry(const aiMesh * mesh) const;
};


} // namespace gloperate_assimp
