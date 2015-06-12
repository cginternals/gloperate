
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

enum class MeshLoadOptions : unsigned int
{
    /**
    *  @brief
    *    use default options
    */
    None            = 0x00,
    /**
    *  @brief
    *    generate smooth normals
    */
    SmoothNormals   = 0x01
};

MeshLoadOptions operator&(MeshLoadOptions a, MeshLoadOptions b);
MeshLoadOptions operator|(MeshLoadOptions a, MeshLoadOptions b);
MeshLoadOptions operator^(MeshLoadOptions a, MeshLoadOptions b);

/**
*  @brief
*    Loader for meshes (PolygonalGeometry) that uses ASSIMP for import
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
    /**
    *  @brief
    *    Load mesh from file
    *
    *  @param[in] filename
    *    File name
    *  @param[in] options
    *    Options for loading mesh (can be empty),
    *    a Variant holding a value from the enum MeshLoadOptions 
    *  @param[in] progress
    *    Callback function that is invoked on progress (can be empty)
    *
    *  @return
    *    Loaded resource (can be null)
    */
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
