
#include <gloperate-assimp/AssimpSceneLoader.h>

#include <algorithm>
#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>

#include <gloperate/primitives/PolygonalGeometry.h>
#include <gloperate/primitives/Scene.h>


using namespace gloperate;


namespace gloperate_assimp
{


AssimpSceneLoader::AssimpSceneLoader()
{
}

AssimpSceneLoader::~AssimpSceneLoader()
{
}

bool AssimpSceneLoader::canLoad(const std::string & ext) const
{
    if (ext.empty())
        return false;

    const auto extension = (ext[0] == '.') ? ext : '.' + ext;
    const auto supported = aiIsExtensionSupported(extension.c_str());

    return supported == AI_TRUE;
}

std::vector<std::string> AssimpSceneLoader::loadingTypes() const
{
    return
    {
        "Collada (*.dae, *.xml)",
        "Blender (*.blend)",
        "Biovision BVH (*.bvh)",
        "3D Studio Max 3DS (*.3ds)",
        "3D Studio Max ASE (*.ase)",
        "Wavefront Object (*.obj)",
        "Stanford Polygon Library (*.ply)",
        "AutoCAD DXF (*.dxf)",
        "IFC-STEP, Industry Foundation Classes (*.ifc)",
        "Neutral File Format (*.nff)",
        "Sense8 WorldToolkit (*.nff)",
        "Valve Model (*.smd,*.vta)",
        "Quake I (*.mdl)",
        "Quake II (*.md2)",
        "Quake III (*.md3)",
        "Quake 3 BSP (*.pk3)",
        "RtCW (*.mdc)",
        "Doom 3 (*.md5mesh, *.md5anim, *.md5camera)",
        "DirectX X (*.x)",
        "Quick3D (*.q3o, q3s)",
        "Raw Triangles (.raw)",
        "AC3D (*.ac)",
        "Stereolithography (*.stl)",
        "Autodesk DXF (*.dxf)",
        "Irrlicht Mesh (*.irrmesh, *.xml)",
        "Irrlicht Scene (*.irr, *.xml)",
        "Object File Format (*.off)",
        "Terragen Terrain (*.ter)",
        "3D GameStudio Model (*.mdl)",
        "3D GameStudio Terrain (*.hmp)",
        "Ogre (*.mesh.xml, *.skeleton.xml, *.material)",
        "Milkshape 3D (*.ms3d)",
        "LightWave Model (*.lwo)",
        "LightWave Scene (*.lws)",
        "Modo Model (*.lxo)",
        "CharacterStudio Motion (*.csm)",
        "Stanford Ply (*.ply)",
        "TrueSpace (*.cob, *.scn)",
        "XGL (*.xgl, *.zgl)"
    };
}

std::string AssimpSceneLoader::allLoadingTypes() const
{
    auto assimp_string = aiString{};
    aiGetExtensionList(&assimp_string);

    auto string = std::string{assimp_string.C_Str()};
    std::replace(string.begin(), string.end(), ';', ' ');
    return string;
}

Scene * AssimpSceneLoader::load(const std::string & filename, std::function<void(int, int)> /*progress*/) const
{
    // Import scene
    auto assimpScene = aiImportFile(
        filename.c_str(),
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_GenNormals);

    // Check for errors
    if (!assimpScene)
    {
        std::cout << aiGetErrorString();
        return nullptr;
    }

    // Convert scene into gloperate scene
    Scene * scene = convertScene(assimpScene);

    // Release scene
    aiReleaseImport(assimpScene);

    // Return loaded scene
    return scene;
}

Scene * AssimpSceneLoader::convertScene(const aiScene * scene) const
{
    // Create new scene
    Scene * sceneOut = new Scene;

    // Convert meshes from the scene
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        sceneOut->meshes().push_back(convertGeometry(scene->mMeshes[i]));
    }

    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        aiString filename;
        // only fetch texture with index 0
        if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &filename) == aiReturn_SUCCESS)
        {
            sceneOut->materials()[i] = std::string(filename.C_Str());
        }
    }

    // Return scene
    return sceneOut;
}

PolygonalGeometry * AssimpSceneLoader::convertGeometry(const aiMesh * mesh) const
{
    // Create geometry
    PolygonalGeometry * geometry = new PolygonalGeometry;

    // Copy index array
    std::vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        const auto & face = mesh->mFaces[i];
        for (auto j = 0u; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    geometry->setIndices(std::move(indices));

    // Copy vertex array
    std::vector<glm::vec3> vertices;
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        const auto & vertex = mesh->mVertices[i];
        vertices.push_back({ vertex.x, vertex.y, vertex.z });
    }
    geometry->setVertices(std::move(vertices));

    // Does the mesh contain normal vectors?
    if (mesh->HasNormals())
    {
        // Copy normal array
        std::vector<glm::vec3> normals;
        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            const auto & normal = mesh->mNormals[i];
            normals.push_back({ normal.x, normal.y, normal.z });
        }
        geometry->setNormals(std::move(normals));
    }

    // Does the mesh contain texture coordinates?
    if (mesh->HasTextureCoords(0))
    {
        // Copy texture cooridinate array
        std::vector<glm::vec3> textureCoordinates;
        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            const auto & textureCoordinate = mesh->mTextureCoords[0][i];
            textureCoordinates.push_back({ textureCoordinate.x, textureCoordinate.y, textureCoordinate.z });
        }
        geometry->setTextureCoordinates(std::move(textureCoordinates));
    }

    // Materials
    geometry->setMaterialIndex(mesh->mMaterialIndex);

    // Return geometry
    return geometry;
}


} // namespace gloperate_assimp
