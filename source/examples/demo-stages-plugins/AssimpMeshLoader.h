
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Loader.h>


struct aiMesh;
struct aiScene;

namespace gloperate
{
    class Drawable;
}


/**
*  @brief
*    Loader for meshes (PolygonalGeometry) that uses ASSIMP for import
*
*  Supported options:
*    "smoothNormals" <bool>: Generate smooth normals
*/
class AssimpMeshLoader : public gloperate::Loader<gloperate::Drawable>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        AssimpMeshLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load a shader from a source code file"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    /**
    *  @brief
    *    Constructor
    */
    AssimpMeshLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AssimpMeshLoader();

    // Virtual loader methods
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;
    virtual gloperate::Drawable * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


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
    gloperate::Drawable * convertGeometry(const aiMesh * mesh) const;
};
