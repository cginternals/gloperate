
#pragma once


#include <vector>
#include <map>
#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class PolygonalGeometry;
class Light;


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

    /**
    *  @brief
    *    Get lights
    *
    *  @return
    *    vector of lights
    */
    const std::vector<Light *> & lights() const;

    /**
    *  @brief
    *    Get lights
    *
    *  @return
    *    vector of lights
    */
    std::vector<Light *> & lights();

protected:
    std::vector<PolygonalGeometry *> m_meshes;        /**< Mesh array */
	std::map<unsigned int, std::string> m_materials;  /**< Materials map */
    std::vector<Light *> m_lights; /**< Light array */
};


} // namespace gloperate
