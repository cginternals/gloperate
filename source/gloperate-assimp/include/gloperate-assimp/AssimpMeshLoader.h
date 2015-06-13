
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
*
*  Supported options:
*    "smoothNormals" <bool>: Generate smooth normals
*/
class GLOPERATE_ASSIMP_API AssimpMeshLoader : public gloperate::Loader<gloperate::PolygonalGeometry>
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AssimpMeshLoader();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AssimpMeshLoader();

    // Virtual loader methods
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;
    virtual gloperate::PolygonalGeometry * load(const std::string & filename, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const override;


protected:
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
