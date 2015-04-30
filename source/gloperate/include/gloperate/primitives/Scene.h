
#pragma once


#include <vector>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class PolygonalGeometry;


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


protected:
    std::vector<PolygonalGeometry *> m_meshes;    /**< Mesh array */
};


} // namespace gloperate
