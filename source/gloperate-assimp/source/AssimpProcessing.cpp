
#include <gloperate-assimp/AssimpProcessing.h>

#include <cassert>

#include <glm/glm.hpp>

#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <gloperate-assimp/PolygonalGeometry.h>


namespace gloperate_assimp
{


std::vector<PolygonalGeometry> AssimpProcessing::convertToGeometries(const aiScene * scene)
{
    auto geometries = std::vector<PolygonalGeometry>{};

    for (auto i = 0u; i < scene->mNumMeshes; ++i)
        geometries.push_back(convertToGeometry(scene->mMeshes[i]));

    return geometries;
}

PolygonalGeometry AssimpProcessing::convertToGeometry(const aiMesh * mesh)
{
    auto geometry = PolygonalGeometry{};

    auto indices = std::vector<unsigned int>{};
    for (auto i = 0u; i < mesh->mNumFaces; ++i)
    {
        const auto & face = mesh->mFaces[i];
        for (auto j = 0u; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    geometry.setIndices(std::move(indices));

    auto vertices = std::vector<glm::vec3>{};
    for (auto i = 0u; i < mesh->mNumVertices; ++i)
    {
        const auto & vertex = mesh->mVertices[i];
        vertices.push_back({ vertex.x, vertex.y, vertex.z });
    }
    geometry.setVertices(std::move(vertices));

    if (mesh->HasNormals())
    {
        auto normals = std::vector<glm::vec3>{};
        for (auto i = 0u; i < mesh->mNumVertices; ++i)
        {
            const auto & normal = mesh->mNormals[i];
            normals.push_back({ normal.x, normal.y, normal.z });
        }
        geometry.setNormals(std::move(normals));
    }

    return geometry;
}


} // namespace gloperate_assimp
