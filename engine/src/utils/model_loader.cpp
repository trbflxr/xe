//
// Created by trbflxr on 3/30/2020.
//

#include "xepch.hpp"
#include "model_loader.hpp"

#include "assimp_io.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <xe/graphics/material.hpp>

namespace xe {

  Assimp::Importer *ModelLoader::importer() {
    static std::shared_ptr<Assimp::Importer> importer = nullptr;
    if (!importer) {
      importer = std::make_shared<Assimp::Importer>();
      importer->SetIOHandler(ioSystem());
    }
    return importer.get();
  }

  Assimp::XEAI_IOSystem *ModelLoader::ioSystem() {
    static Assimp::XEAI_IOSystem *io = nullptr;
    if (!io) { //assimp delete it automatically
      io = new Assimp::XEAI_IOSystem();
    }
    return io;
  }

  bool ModelLoader::load(std::string_view file, Model *model) {
#if XE_DEBUG_TRACING
    const std::string name = std::string("ModelLoader lead model ") + file.data();
#else
    static const std::string name("VFS Read file");
#endif

    XE_CORE_TRACE("[ModelLoader / Begin] Loading '{}'", file);
    XE_TRACE_BEGIN("XE", name.c_str());

    const aiScene *scene = importer()->ReadFile(file.data(),
                                                aiProcessPreset_TargetRealtime_MaxQuality |
                                                aiProcess_JoinIdenticalVertices |
                                                aiProcess_Triangulate |
                                                aiProcess_GenSmoothNormals |
                                                aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
      XE_CORE_ERROR("[ModelLoader]: Unable to load scene from file: '{}'. Error: '{}'", file, importer()->GetErrorString());
      return false;
    }

    const std::string dir = string::getDirectoryName(file) + "/";

    //precess nodes in scene
    processNode(model, scene->mRootNode, scene, dir);

    XE_TRACE_END("XE", name.c_str());
    XE_CORE_TRACE("[ModelLoader/ End] Loading '{}'. {} meshes processed", file, model->meshes_.size());
    return true;
  }

  void ModelLoader::processNode(Model *model, aiNode *node, const aiScene *scene, const std::string &dir) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      processMesh(model, mesh, scene, dir);
    }

    //process all the node children recursively
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
      processNode(model, node->mChildren[i], scene, dir);
    }
  }

  void ModelLoader::processMesh(Model *model, aiMesh *mesh, const aiScene *scene, const std::string &dir) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    //material
    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    const int32_t matIndex = processMaterial(model, material, dir, mesh->mMaterialIndex);

    //vertices
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;

      vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
      vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
      vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
      vertex.biTangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};

      if (mesh->HasTextureCoords(0)) {
        vertex.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
      }

      vertices.emplace_back(vertex);
    }

    //indices
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
      aiFace face = mesh->mFaces[i];
      for (uint32_t j = 0; j < face.mNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }

    //todo: usage
    Mesh m = Mesh(std::move(vertices), std::move(indices), matIndex, Usage::Static);
    model->meshes_.emplace_back(m);
  }

  int32_t ModelLoader::processMaterial(Model *model, const aiMaterial *material, const std::string &dir, uint32_t id) {
    aiString aiTex{ };

    for (size_t i = 0; i < model->materials_.size(); ++i) {
      if (model->materials_[i]->id_ == static_cast<int32_t>(id)) {
        return static_cast<int32_t>(i);
      }
    }

    auto mt = std::make_shared<Material>();
    mt->id_ = id;

    for (uint32_t tex = aiTextureType_NONE; tex <= aiTextureType_UNKNOWN; tex++) {
      aiTextureType type = static_cast<aiTextureType>(tex);

      if (material->GetTextureCount(type) > 0) {
        material->GetTexture(type, 0, &aiTex);
        const std::string texName = dir + aiTex.C_Str();

        setTexture(*mt, tex, texName);
      }
    }
    model->materials_.emplace_back(mt);

    return static_cast<int32_t>(model->materials_.size()) - 1;
  }

  void ModelLoader::setTexture(Material &material, uint32_t type, std::string_view texture) {
    //fixme:
    gpu::Texture::Info info;
    info.minFilter = TextureMinFilter::LinearMipmapLinear;
    info.magFilter = TextureMagFilter::Linear;
    info.wrapS = TextureWrap::Repeat;
    info.wrapT = TextureWrap::Repeat;
    info.bindless = true;

    const aiTextureType t = static_cast<aiTextureType>(type);
    switch (t) {
      case aiTextureType_DIFFUSE: {
        material.gpu_.diffuse = std::make_shared<Texture>();
        material.gpu_.diffuse->setInfo(info);
        material.gpu_.diffuse->setBuildMipmap(true);
        material.gpu_.diffuse->loadFromFile(texture, true);
        break;
      }
      case aiTextureType_NORMALS: {
        material.gpu_.normals = std::make_shared<Texture>();
        material.gpu_.normals->setInfo(info);
        material.gpu_.normals->setBuildMipmap(true);
        material.gpu_.normals->loadFromFile(texture, true);
        break;
      }

      case aiTextureType_SPECULAR:
      case aiTextureType_LIGHTMAP:
      case aiTextureType_EMISSIVE:
      case aiTextureType_AMBIENT:
      case aiTextureType_HEIGHT:
      case aiTextureType_SHININESS:
      case aiTextureType_OPACITY:
      case aiTextureType_DISPLACEMENT:
      case aiTextureType_REFLECTION:
      case aiTextureType_BASE_COLOR:
      case aiTextureType_NORMAL_CAMERA:
      case aiTextureType_EMISSION_COLOR:
      case aiTextureType_METALNESS:
      case aiTextureType_DIFFUSE_ROUGHNESS:
      case aiTextureType_AMBIENT_OCCLUSION:
      default: break;
    }
  }

}
