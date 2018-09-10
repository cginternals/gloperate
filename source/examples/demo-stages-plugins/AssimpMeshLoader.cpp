
#include "AssimpMeshLoader.h"

#include <algorithm>
#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>

#include <cppassist/logging/logging.h>

#include <cppexpose/variant/Variant.h>

#include <glbinding/gl/enum.h>

#include <gloperate/rendering/Drawable.h>


using namespace gloperate;


CPPEXPOSE_COMPONENT(AssimpMeshLoader, gloperate::AbstractLoader)


AssimpMeshLoader::AssimpMeshLoader(gloperate::Environment * environment)
: gloperate::Loader<gloperate::Drawable>(environment)
{
}

AssimpMeshLoader::~AssimpMeshLoader()
{
}

bool AssimpMeshLoader::canLoad(const std::string & ext) const
{
    if (ext.empty())
        return false;

    const auto extension = (ext[0] == '.') ? ext : '.' + ext;
    const auto supported = aiIsExtensionSupported(extension.c_str());

    return supported == AI_TRUE;
}

std::vector<std::string> AssimpMeshLoader::loadingTypes() const
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

std::string AssimpMeshLoader::allLoadingTypes() const
{
    auto assimp_string = aiString{};
    aiGetExtensionList(&assimp_string);

    auto string = std::string{assimp_string.C_Str()};
    std::replace(string.begin(), string.end(), ';', ' ');
    return string;
}

Drawable * AssimpMeshLoader::load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> /*progress*/) const
{
    bool smoothNormals = false;

    // Get options
    const cppexpose::VariantMap * map = options.asMap();
    if (map) {
        if (map->count("smoothNormals") > 0) smoothNormals = map->at("smoothNormals").value<bool>();
    }

    // Import scene
    auto scene = aiImportFile(
        filename.c_str(),
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        (smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals));

    // Check for errors
    if (!scene)
    {
        cppassist::error("AssimpMeshLoader") << aiGetErrorString();
        return nullptr;
    }

    // Convert first mesh found in the scene
    Drawable * geometry = nullptr;
    if (scene->mNumMeshes > 0) {
        geometry = convertGeometry(scene->mMeshes[0]);
    }

    // Release scene
    aiReleaseImport(scene);

    // Return loaded mesh
    return geometry;
}

Drawable * AssimpMeshLoader::convertGeometry(const aiMesh * mesh) const
{
    // Create geometry
    Drawable * geometry = new Drawable;

    // Copy index array
    std::vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        const auto & face = mesh->mFaces[i];
        for (auto j = 0u; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    globjects::Buffer * indexBuffer = new globjects::Buffer;
    indexBuffer->setData(std::move(indices), gl::GL_STATIC_DRAW);
    geometry->setIndexBuffer(indexBuffer);
    geometry->setSize(indices.size());
    geometry->setDrawMode(gloperate::DrawMode::ElementsIndexBuffer);

    // Copy vertex array
    std::vector<glm::vec3> vertices;
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        const auto & vertex = mesh->mVertices[i];
        vertices.push_back({ vertex.x, vertex.y, vertex.z });
    }
    globjects::Buffer * vertexBuffer = new globjects::Buffer;
    geometry->setBuffer(0, vertexBuffer);
    geometry->setData(0, std::move(vertices), gl::GL_STATIC_DRAW);
    geometry->bindAttribute(0, 0);
    geometry->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec3));
    geometry->setAttributeBindingFormat(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    geometry->enableAttributeBinding(0);

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
        globjects::Buffer * normalBuffer = new globjects::Buffer;
        geometry->setBuffer(1, normalBuffer);
        geometry->setData(1, std::move(normals), gl::GL_STATIC_DRAW);
        geometry->bindAttribute(1, 1);
        geometry->setAttributeBindingBuffer(1, 1, 0, sizeof(glm::vec3));
        geometry->setAttributeBindingFormat(1, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
        geometry->enableAttributeBinding(1);
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
        globjects::Buffer * texCoordBuffer = new globjects::Buffer;
        geometry->setBuffer(2, texCoordBuffer);
        geometry->setData(2, std::move(textureCoordinates), gl::GL_STATIC_DRAW);
        geometry->bindAttribute(2, 2);
        geometry->setAttributeBindingBuffer(2, 2, 0, sizeof(glm::vec3));
        geometry->setAttributeBindingFormat(2, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
        geometry->enableAttributeBinding(2);
    }

    // Materials
    //geometry->setMaterialIndex(mesh->mMaterialIndex);

    // Return geometry
    return geometry;
}
