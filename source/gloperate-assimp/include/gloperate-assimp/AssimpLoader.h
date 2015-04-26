
#pragma once


#include <gloperate/resources/Loader.h>

#include <gloperate-assimp/gloperate-assimp_api.h>


struct aiMesh;
struct aiScene;


namespace gloperate
{
    class PolygonalGeometry;
}


namespace gloperate_assimp
{


/**
*  @brief
*    Loader for meshes (PolygonalGeometry) that uses ASSIMP for import
*/
class GLOPERATE_ASSIMP_API AssimpLoader : public gloperate::Loader<gloperate::PolygonalGeometry>
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
    virtual gloperate::PolygonalGeometry * load(const std::string & filename, std::function<void(int, int)> progress) const override;


protected:
    /**
    *  @brief
    *    Convert all meshes from an ASSIMP scene into gloperate meshes
    *
    *  @param[in] scene
    *    ASSIMP scene (must be valid!)
    *  @param[in] numMeshes
    *    Maximum number of meshes that are imported (if 0, all found meshes are imported)
    *
    *  @return
    *    List of meshes, which must be destroyed by the caller
    */
    std::vector<gloperate::PolygonalGeometry *> convertGeometries(const aiScene * scene, size_t numMeshes = 0) const;

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
