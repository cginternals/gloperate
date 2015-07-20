
#pragma once

#include <glm/glm.hpp>

#include <gloperate/resources/Loader.h>

#include <gloperate-assimp/gloperate-assimp_api.h>

struct aiMesh;
struct aiScene;
struct aiLight;
struct aiNode;

namespace gloperate
{
    class Scene;
    class PolygonalGeometry;
    class Light;
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
    virtual gloperate::Scene * load(const std::string & filename, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const override;


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
    *    Determine Object-To-Scene-Space transformation matrix
    *
    *  @param[in] node
    *    ASSIMP node (can be a nullptr)
    *
    *  @return
    *    Object-To-Scene-Space transformation matrix
    */
    glm::mat4 determineTransformation(const aiNode * node) const;

    /**
    *  @brief
    *    Convert ASSIMP mesh into gloperate mesh
    *
    *  @param[in] mesh
    *    ASSIMP mesh (must be valid!)
    *
    *  @param[in] transformation
    *    Object-To-Scene-Space transformation matrix
    *
    *  @return
    *    Mesh, must be destroyed by the caller
    */
    gloperate::PolygonalGeometry * convertGeometry(const aiMesh * mesh, const glm::mat4 & transformation) const;

    /**
    *  @brief
    *    Convert ASSIMP light into gloperate light
    *
    *  @param[in] light
    *    ASSIMP light (must be valid!)
    *
    *  @param[in] transformation
    *    Object-To-Scene-Space transformation matrix
    *
    *  @return
    *    Light, must be destroyed by the caller
    */
    gloperate::Light * convertLight(const aiLight * light, const glm::mat4 & transformation) const;
};


} // namespace gloperate_assimp
