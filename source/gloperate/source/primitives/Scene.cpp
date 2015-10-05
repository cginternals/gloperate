
#include <gloperate/primitives/Scene.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/primitives/PolygonalGeometry.h>


namespace gloperate
{


Scene::Scene()
{
}

Scene::~Scene()
{
    // Destroy meshes
    for (PolygonalGeometry * mesh : m_meshes)
    {
        delete mesh;
    }
}

const std::vector<PolygonalGeometry *> & Scene::meshes() const
{
    return m_meshes;
}

std::vector<PolygonalGeometry *> & Scene::meshes()
{
    return m_meshes;
}

const std::map<unsigned, std::string>& Scene::materials() const
{
	return m_materials;
}

std::map<unsigned, std::string>& Scene::materials()
{
	return m_materials;
}


} // namespace gloperate
