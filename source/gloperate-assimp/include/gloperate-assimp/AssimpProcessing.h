
#pragma once


#include <vector>

#include <gloperate-assimp/gloperate-assimp_api.h>


struct aiMesh;
struct aiScene;


namespace gloperate_assimp
{


class PolygonalGeometry;


class GLOPERATE_ASSIMP_API AssimpProcessing
{
public:
    static std::vector<PolygonalGeometry> convertToGeometries(const aiScene * scene);
    static PolygonalGeometry convertToGeometry(const aiMesh * mesh);
};


} // namespace gloperate_assimp
