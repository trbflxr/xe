//
// Created by trbflxr on 3/30/2020.
//

#ifndef XE_MODEL_LOADER_HPP
#define XE_MODEL_LOADER_HPP

#include <xe/graphics/model.hpp>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace Assimp {
  class Importer;
  class XEAI_IOSystem;
}

namespace xe {

  class ModelLoader {
  public:
    static bool load(std::string_view file, Model *model);

  private:
    static Assimp::Importer *importer();
    static Assimp::XEAI_IOSystem *ioSystem();

    static void processNode(Model *model, aiNode *node, const aiScene *scene, const std::string &dir);
    static void processMesh(Model *model, aiMesh *mesh, const aiScene *scene, const std::string &dir);
    static int32_t processMaterial(Model *model, const aiMaterial *material, const std::string &dir, uint32_t id);

    static void setTexture(Material &material, uint32_t type, std::string_view texture);
  };

}

#endif //XE_MODEL_LOADER_HPP
