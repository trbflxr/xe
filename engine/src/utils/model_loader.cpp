//
// Created by trbflxr on 3/30/2020.
//

#include "xepch.hpp"
#include "model_loader.hpp"

#include "assimp_io.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
                                                aiProcess_CalcTangentSpace |
                                                aiProcess_FlipUVs);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
      XE_CORE_ERROR("[ModelLoader]: Unable to load scene from file: '{}'. Error: '{}'", file, importer()->GetErrorString());
      return false;
    }

    //precess nodes in scene
    processNode(model, scene->mRootNode, scene);

    XE_TRACE_END("XE", name.c_str());
    XE_CORE_TRACE("[ModelLoader/ End] Loading '{}'", file);
    return true;
  }

  void ModelLoader::processNode(Model *model, aiNode *node, const aiScene *scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      processMesh(model, mesh, scene);
    }

    //process all the node children recursively
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
      processNode(model, node->mChildren[i], scene);
    }
  }

  void ModelLoader::processMesh(Model *model, aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

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

    model->meshes_.emplace_back(std::move(vertices), std::move(indices));

    //material
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    processMaterial(model, material);
  }

  void ModelLoader::processMaterial(Model *model, const aiMaterial *material) {
    //todo: impl
  }

}
