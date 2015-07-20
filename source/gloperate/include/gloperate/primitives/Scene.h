
#pragma once


#include <vector>
#include <memory>
#include <map>
#include <string>
#include <glm/common.hpp>
#include <glm/gtx/compatibility.hpp>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class PolygonalGeometry;


struct BoneNode
{
    std::string boneName;
    glm::mat4 bindTransform;
    std::vector<BoneNode> children;
};

struct TranslationKey
{
    double time;
    glm::vec3 translation;
};

struct RotationKey
{
    double time;
    glm::quat rotation;
};

struct ScaleKey
{
    double time;
    glm::vec3 scale;
};

struct Channel
{
    std::string boneName;
    std::vector<TranslationKey> translation;
    std::vector<RotationKey> rotation;
    std::vector<ScaleKey> scale;
};


struct RigAnimationTrack
{
    double duration; //in Ticks
    double ticksPerSecond;
    std::vector<Channel> boneChannels;
};

/**
*  @brief
*    3D scene representation
*
*  @remarks
*    A 3D scene is a container that contains the components a scene is composed of, i.e.,
*    a list of meshes, textures, etc. Not that this class does not include a scene in
*    the sense of a scene graph (yet), it merely contains a list of individual components.
*/
class GLOPERATE_API Scene
{
public:
    /**
    *  @brief
    *    Constructor
    */
    Scene();

    /**
    *  @brief
    *    Destructor
    */
    ~Scene();

    /**
    *  @brief
    *    Get meshes
    *
    *  @return
    *    List of meshes
    */
    const std::vector<PolygonalGeometry *> & meshes() const;

    /**
    *  @brief
    *    Get meshes
    *
    *  @return
    *    List of meshes
    */
    std::vector<PolygonalGeometry *> & meshes();

	/**
	*  @brief
	*    Get materials by material id
	*
	*  @return
	*    Map of materials
	*/
	const std::map<unsigned int, std::string> & materials() const;

	/**
	*  @brief
	*    Get materials by material id
	*
	*  @return
	*    Map of materials
	*/
    std::map<unsigned int, std::string> & materials();


    std::vector<RigAnimationTrack *> & animations();
    const std::vector<RigAnimationTrack *> & animations() const;

    std::shared_ptr<BoneNode> boneHierarchy();
    std::shared_ptr<BoneNode> boneHierarchy() const;
    void setBoneHierarchy(std::shared_ptr<BoneNode> boneHierarchy);

protected:
    std::vector<PolygonalGeometry *> m_meshes;        /**< Mesh array */
    std::map<unsigned int, std::string> m_materials;  /**< Materials map */
    std::vector<RigAnimationTrack*> m_animations; /**< Animation Array */
    std::shared_ptr<BoneNode> m_boneHierarchy;
};


} // namespace gloperate
