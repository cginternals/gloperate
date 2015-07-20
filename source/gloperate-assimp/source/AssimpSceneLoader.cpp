
#include <gloperate-assimp/AssimpSceneLoader.h>

#include <algorithm>
#include <iostream>
#include <string>

#include <gloperate/ext-includes-begin.h>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/matrix4x4.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>

#include <gloperate/ext-includes-end.h>

#include <reflectionzeug/variant/Variant.h>

#include <gloperate/primitives/PolygonalGeometry.h>
#include <gloperate/primitives/Scene.h>
#include <gloperate/primitives/Light.h>


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

Scene * AssimpSceneLoader::load(const std::string & filename, const reflectionzeug::Variant & options, std::function<void(int, int)> /*progress*/) const
{
    bool smoothNormals = false;

    // Get options
    const reflectionzeug::VariantMap * map = options.asMap();
    if (map) {
        if (map->count("smoothNormals") > 0) smoothNormals = map->at("smoothNormals").value<bool>();
    }

    // Import scene
    auto assimpScene = aiImportFile(
        filename.c_str(),
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        (smoothNormals ? aiProcess_GenSmoothNormals : aiProcess_GenNormals));

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
        auto mesh = scene->mMeshes[i];        
        auto meshNode = scene->mRootNode->FindNode(mesh->mName);
        auto transformation = determineTransformation(meshNode);
        sceneOut->meshes().push_back(convertGeometry(mesh, transformation));
    }

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        aiString filename;
        // only fetch texture with index 0
        if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &filename) == aiReturn_SUCCESS)
        {
            sceneOut->materials()[i] = std::string(filename.C_Str());
        }
    }

    for (size_t i = 0; i < scene->mNumLights; ++i)
    {
        auto light = scene->mLights[i];
        auto lightNode = scene->mRootNode->FindNode(light->mName);
        auto transformation = determineTransformation(lightNode);
        sceneOut->lights().push_back(convertLight(light, transformation));
    }

    // Return scene
    return sceneOut;
}

glm::mat4 AssimpSceneLoader::determineTransformation(const aiNode* node) const
{
    aiMatrix4x4 transformation;
    auto currentNode = node;
    while (currentNode != nullptr)
    {
        transformation *= currentNode->mTransformation;
        currentNode = currentNode->mParent;
    }

    return glm::mat4{
        transformation.a1, transformation.b1, transformation.c1, transformation.d1,
        transformation.a2, transformation.b2, transformation.c2, transformation.d2,
        transformation.a3, transformation.b3, transformation.c3, transformation.d3,
        transformation.a4, transformation.b4, transformation.c4, transformation.d4,
    };
}

PolygonalGeometry * AssimpSceneLoader::convertGeometry(const aiMesh * mesh, const glm::mat4 & transformation) const
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
        vertices.push_back(glm::vec3(transformation * glm::vec4(vertex.x, vertex.y, vertex.z, 1)));
    }
    geometry->setVertices(std::move(vertices));

    // Does the mesh contain normal vectors?
    if (mesh->HasNormals())
    {
        // Copy normal array
        std::vector<glm::vec3> normals;
        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            auto vertexAndNormal = mesh->mNormals[i] + mesh->mVertices[i];
            auto transformedVertexAndNormal = glm::vec3(transformation * glm::vec4(vertexAndNormal.x, vertexAndNormal.y, vertexAndNormal.z, 1.f));
            normals.push_back(glm::normalize(transformedVertexAndNormal - geometry->vertices()[i]));
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


Light * AssimpSceneLoader::convertLight(const aiLight * light, const glm::mat4 & transformation) const
{
    auto lightOut = new Light;
    lightOut->setType(static_cast<LightSourceType>(light->mType));
    lightOut->setName(std::string(light->mName.data));

    auto position = light->mPosition;
    auto transformedPosition = glm::vec3(transformation * glm::vec4(position.x, position.y, position.z, 1));
    lightOut->setPosition(transformedPosition);

    auto direction = light->mDirection;
    auto positionAndDirection = position + direction;
    auto transformedPositionAndDirection = glm::vec3(transformation * glm::vec4(positionAndDirection.x, positionAndDirection.y, positionAndDirection.z, 1.f));
    lightOut->setDirection(glm::normalize(transformedPositionAndDirection - transformedPosition));

    auto colorDiffuse = light->mColorDiffuse;
    lightOut->setColorDiffuse(glm::vec3(colorDiffuse.r, colorDiffuse.g, colorDiffuse.b) / 255.f);

    auto colorSpecular = light->mColorSpecular;
    lightOut->setColorSpecular(glm::vec3(colorSpecular.r, colorSpecular.g, colorSpecular.b) / 255.f);

    lightOut->setAttenuationConstant(light->mAttenuationConstant);
    lightOut->setAttenuationLinear(light->mAttenuationLinear);
    lightOut->setAttenuationQuadratic(light->mAttenuationQuadratic);
    lightOut->setCosineCutoff(abs(cos(light->mAngleInnerCone)));

    return lightOut;
}

} // namespace gloperate_assimp
