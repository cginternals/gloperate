
#include <gloperate-assimp/AssimpSceneLoader.h>

#include <algorithm>
#include <iostream>
#include <string>

#include <gloperate/ext-includes-begin.h>

#include <glm/glm.hpp>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>

#include <gloperate/ext-includes-end.h>

#include <reflectionzeug/variant/Variant.h>

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
        sceneOut->meshes().push_back(convertGeometry(scene->mMeshes[i]));
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

    if (scene->HasAnimations())
    {
        //Get the Bone hierarchy
        BoneNode* hierarchy = convertBoneHierarchy(scene->mRootNode);
        sceneOut->setBoneHierarchy(std::shared_ptr<BoneNode>{hierarchy});
        //Get the animations out of the file
        sceneOut->animations().resize(scene->mNumAnimations);
        for (size_t i = 0; i< scene->mNumAnimations; ++i)
        {
            sceneOut->animations().at(i) = convertRigAnimation(scene->mAnimations[i]);
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

    // Is the mesh rigged?
    if (mesh->HasBones())
    {
        //TODO: Load the Bones, the mapping and the bind-matrices into the polgeom
        constexpr int noneValue = -1;
        std::map<std::string,size_t> boneMapping;
        std::vector<glm::mat4> bindTransforms;
        std::vector<glm::ivec4> vertexBoneIndices(mesh->mNumVertices,glm::ivec4(noneValue));
        std::vector<glm::vec4> vertexBoneWeights(mesh->mNumVertices);
        auto insertWeight = [&](int BoneId, int vertId, float weight)
        {

            //Only the 4 weights are stored,empty entries are marked with -1 in BoneIndex
            for(size_t i = 0; i < 4; i++)
            {
                if(vertexBoneIndices[vertId][i] == noneValue)
                {
                    vertexBoneIndices[vertId][i] = BoneId;
                    vertexBoneWeights[vertId][i] = weight;
                    break;
                }
            }
        };

        auto CopyaiMat = [](const aiMatrix4x4 &from, glm::mat4 &to) {
            to[0][0] = from.a1; to[1][0] = from.a2;
            to[2][0] = from.a3; to[3][0] = from.a4;
            to[0][1] = from.b1; to[1][1] = from.b2;
            to[2][1] = from.b3; to[3][1] = from.b4;
            to[0][2] = from.c1; to[1][2] = from.c2;
            to[2][2] = from.c3; to[3][2] = from.c4;
            to[0][3] = from.d1; to[1][3] = from.d2;
            to[2][3] = from.d3; to[3][3] = from.d4;
        };

        int numBones = 0;

        for (size_t i = 0; i < mesh->mNumBones; i++)
        {
            std::string boneName(mesh->mBones[i]->mName.C_Str());
            //Check whether this Bone was already there
            if(boneMapping.count(boneName) == 1)
            {
                continue; //No need to process bones double so just save the trouble
            }

            //insert a new bone
            size_t boneIndex = numBones;
            numBones++;
            boneMapping[boneName] = boneIndex;
            bindTransforms.push_back(glm::mat4());

            CopyaiMat(mesh->mBones[i]->mOffsetMatrix, bindTransforms[boneIndex]);

            for(size_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
            {
                auto curWeight = mesh->mBones[i]->mWeights[j];
                insertWeight(boneIndex, curWeight.mVertexId, curWeight.mWeight);
            }

        }
        geometry->setBoneMapping(std::move(boneMapping));
        geometry->setBindTransforms(std::move(bindTransforms));
        geometry->setVertexBoneIndices(std::move(vertexBoneIndices));
        geometry->setVertexBoneWeights(std::move(vertexBoneWeights));

    }


    // Materials
    geometry->setMaterialIndex(mesh->mMaterialIndex);

    // Return geometry
    return geometry;
}

BoneNode* AssimpSceneLoader::convertBoneHierarchy(const aiNode * root) const
{
    auto CopyaiMat = [](const aiMatrix4x4 &from, glm::mat4 &to) {
        to[0][0] = from.a1; to[1][0] = from.a2;
        to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2;
        to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2;
        to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2;
        to[2][3] = from.d3; to[3][3] = from.d4;
    };

    BoneNode* hierarchy = new BoneNode;
    hierarchy->boneName = std::string{root->mName.C_Str()};
    CopyaiMat(root->mTransformation,hierarchy->bindTransform);
    hierarchy->children.resize(root->mNumChildren);

    for(size_t i = 0; i < root->mNumChildren; i++)
    {
        BoneNode* child = convertBoneHierarchy(root->mChildren[i]);
        hierarchy->children[i] = std::move(*child);
        delete child;
    }
    return hierarchy;
}

RigAnimationTrack* AssimpSceneLoader::convertRigAnimation(const aiAnimation * anim) const
{
    RigAnimationTrack* animTrack = new RigAnimationTrack;
    animTrack->duration = anim->mDuration;
    animTrack->ticksPerSecond = anim->mTicksPerSecond;
    animTrack->boneChannels.resize(anim->mNumChannels);

    for(size_t i = 0; i < anim->mNumChannels; i++)
    {
        Channel& curChannel = animTrack->boneChannels[i];

        curChannel.translation.resize(anim->mChannels[i]->mNumPositionKeys);
        for(size_t j = 0; j< anim->mChannels[i]->mNumPositionKeys; j++)
        {
            TranslationKey posKey;
            auto& curKey = anim->mChannels[i]->mPositionKeys[j];
            posKey.time = curKey.mTime;
            posKey.translation.x = curKey.mValue.x;
            posKey.translation.y = curKey.mValue.y;
            posKey.translation.z = curKey.mValue.z;
            curChannel.translation[j] = posKey;
        }

        curChannel.rotation.resize(anim->mChannels[i]->mNumRotationKeys);
        for(size_t j = 0; j< anim->mChannels[i]->mNumRotationKeys; j++)
        {
            RotationKey rotKey;
            auto& curKey = anim->mChannels[i]->mRotationKeys[j];
            rotKey.time = curKey.mTime;
            rotKey.rotation.x = curKey.mValue.x;
            rotKey.rotation.y = curKey.mValue.y;
            rotKey.rotation.z = curKey.mValue.z;
            rotKey.rotation.w = curKey.mValue.w;
            curChannel.rotation[j] = rotKey;
        }

        curChannel.scale.resize(anim->mChannels[i]->mNumScalingKeys);
        for(size_t j = 0; j< anim->mChannels[i]->mNumScalingKeys; j++)
        {
            ScaleKey scaleKey;
            auto& curKey = anim->mChannels[i]->mScalingKeys[j];
            scaleKey.time = curKey.mTime;
            scaleKey.scale.x = curKey.mValue.x;
            scaleKey.scale.y = curKey.mValue.y;
            scaleKey.scale.z = curKey.mValue.z;
            curChannel.scale[j] = scaleKey;
        }

        animTrack->boneChannels[i].boneName = std::string(anim->mChannels[i]->mNodeName.C_Str());

    }
    return animTrack;

}


} // namespace gloperate_assimp
