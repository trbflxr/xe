//
// Created by FLXR on 9/3/2019.
//

#include "xepch.hpp"
#include <xe/core/object.hpp>

#include <xe/ui/ui.hpp>

namespace xe {

  Object::Object() {
    static uint32_t id;
    id_ = ++id;
    name_ = "Object";
  }

  void Object::onUi() {
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll;
    ImGui::InputText(uiText("##Name").c_str(), name_.data(), flags);
    ImGui::SameLine();
    ImGui::Text("(%i)", id_);
  }

  std::string Object::uiText(std::string_view label) {
    return std::string(label) + "##" + std::to_string(id_);
  }

}
