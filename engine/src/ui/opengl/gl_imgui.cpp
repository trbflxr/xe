//
// Created by FLXR on 9/12/2019.
//

#include "xepch.hpp"
#include "gl_imgui.hpp"

#include "embedded/embedded.hpp"

#include <xe/core/engine.hpp>
#include <xe/utils/logger.hpp>

namespace xe::ui::impl {

  // OpenGL Data
  static uint32_t gFontTexture = 0;
  static uint32_t gShaderHandle = 0;
  static uint32_t gVertHandle = 0;
  static uint32_t gFragHandle = 0;
  static int32_t gTex2d0Loc = 0;
  static int32_t gProjLoc = 0;
  static int32_t gAttrPosition = 0;
  static int32_t gAttrTexCoord = 0;
  static int32_t gAttrColor = 0;
  static uint32_t gVboHandle = 0;
  static uint32_t gElementsHandle = 0;
  static bool windowHasFocus = false;
  static bool mousePressed[3] = {false, false, false};

  // Callbacks
  static const char *getClipboardText(void *userData) {
    return glfwGetClipboardString((GLFWwindow *) userData);
  }

  static void setClipboardText(void *userData, const char *text) {
    glfwSetClipboardString((GLFWwindow *) userData, text);
  }

  static bool checkShader(uint32_t handle, const char *desc) {
    int32_t status = 0;
    int32_t logLength = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
    if (status == GL_FALSE) {
      XE_CORE_CRITICAL("[GL / ImGui] failed to compile {}", desc);
    }
    if (logLength > 0) {
      ImVector<char> buff;
      buff.resize((int32_t) (logLength + 1));
      glGetShaderInfoLog(handle, logLength, nullptr, (char *) buff.begin());
      XE_CORE_CRITICAL("[GL / ImGui] {}", buff.begin());
    }
    return status == GL_TRUE;
  }

  static bool checkProgram(uint32_t handle, const char *desc) {
    int32_t status = 0;
    int32_t logLength = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
    if (status == GL_FALSE) {
      XE_CORE_CRITICAL("[GL / ImGui] failed to link {}", desc);
    }
    if (logLength > 0) {
      ImVector<char> buff;
      buff.resize((int32_t) (logLength + 1));
      glGetProgramInfoLog(handle, logLength, nullptr, (char *) buff.begin());
      XE_CORE_CRITICAL("[GL / ImGui] {}", buff.begin());
    }
    return status == GL_TRUE;
  }

  bool init(Window::Data *data) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    // Setup back-end capabilities flags
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = Keyboard::Tab;
    io.KeyMap[ImGuiKey_LeftArrow] = Keyboard::Left;
    io.KeyMap[ImGuiKey_RightArrow] = Keyboard::Right;
    io.KeyMap[ImGuiKey_UpArrow] = Keyboard::Up;
    io.KeyMap[ImGuiKey_DownArrow] = Keyboard::Down;
    io.KeyMap[ImGuiKey_PageUp] = Keyboard::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = Keyboard::PageDown;
    io.KeyMap[ImGuiKey_Home] = Keyboard::Home;
    io.KeyMap[ImGuiKey_End] = Keyboard::End;
    io.KeyMap[ImGuiKey_Insert] = Keyboard::Insert;
    io.KeyMap[ImGuiKey_Delete] = Keyboard::Delete;
    io.KeyMap[ImGuiKey_Backspace] = Keyboard::BackSpace;
    io.KeyMap[ImGuiKey_Space] = Keyboard::Space;
    io.KeyMap[ImGuiKey_Enter] = Keyboard::Return;
    io.KeyMap[ImGuiKey_Escape] = Keyboard::Escape;
    io.KeyMap[ImGuiKey_A] = Keyboard::A;
    io.KeyMap[ImGuiKey_C] = Keyboard::C;
    io.KeyMap[ImGuiKey_V] = Keyboard::V;
    io.KeyMap[ImGuiKey_X] = Keyboard::X;
    io.KeyMap[ImGuiKey_Y] = Keyboard::Y;
    io.KeyMap[ImGuiKey_Z] = Keyboard::Z;

    io.SetClipboardTextFn = setClipboardText;
    io.GetClipboardTextFn = getClipboardText;
    io.ClipboardUserData = data->window;

    //todo: get charset form config
    ImFontConfig cfg = ImFontConfig();
    cfg.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
    cfg.FontDataOwnedByAtlas = true;

    auto f = Embedded::defaultFount();

    uint8_t *fd = new uint8_t[f.size()];
    std::memcpy(fd, f.data(), f.size());
    io.Fonts->AddFontFromMemoryTTF(fd, static_cast<int32_t>(f.size()), 13.0f, &cfg);

    return true;
  }

  void update(Window &window, Window::Data *data) {
    if (!gFontTexture) {
      createDeviceObjects();
    }

    static float lastTime = 0.0f;

    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt());

    float time = window.uptime();
    const float delta = time - lastTime;
    lastTime = time;

    io.DeltaTime = delta;

    const vec2u &windowSize = window.size();
    const vec2u &fbSize = window.framebufferSize();

    io.DisplaySize.x = static_cast<float>(windowSize.x);
    io.DisplaySize.y = static_cast<float>(windowSize.y);

    io.DisplayFramebufferScale = ImVec2(windowSize.x > 0.0f ? (static_cast<float>(fbSize.x) / windowSize.x) : 0.0f,
                                        windowSize.y > 0.0f ? (static_cast<float>(fbSize.y) / windowSize.y) : 0.0f);

    const vec2 mousePos = Engine::getMousePosition();
    if (windowHasFocus) {
      if (io.WantSetMousePos) {
        Engine::setMousePosition(io.MousePos);
      } else {
        io.MousePos = mousePos;
      }
      for (uint32_t i = 0; i < 3; i++) {
        io.MouseDown[i] = mousePressed[i] || Engine::isMouseButtonPressed((Mouse::Button) i);
        mousePressed[i] = false;
      }
    }

    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
      return;
    }

    if (io.WantCaptureMouse) {
      const ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
      if (cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
        glfwSetInputMode(data->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      } else {
        Cursor::Enum c;
        switch (cursor) {
          case ImGuiMouseCursor_TextInput: c = Cursor::IBeam;
            break;
          case ImGuiMouseCursor_ResizeNS: c = Cursor::ResizeY;
            break;
          case ImGuiMouseCursor_ResizeEW: c = Cursor::ResizeX;
            break;
          case ImGuiMouseCursor_Hand: c = Cursor::Hand;
            break;
          case ImGuiMouseCursor_ResizeAll:
          case ImGuiMouseCursor_ResizeNESW:
          case ImGuiMouseCursor_ResizeNWSE:
          case ImGuiMouseCursor_Arrow:
          default: c = Cursor::Arrow;
            break;
        }
        glfwSetCursor(data->window, data->defaultCursors[c]);
        glfwSetInputMode(data->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    } else {
      window.setCursor(window.activeCursor());
      window.setCursorVisible(window.isCursorVisible());
    }
  }

  bool processEvent(const Event &e) {
    if (e.type == Event::LostFocus || e.type == Event::MouseLeft) {
      windowHasFocus = false;
    } else if (e.type == Event::GainedFocus || e.type == Event::MouseEntered) {
      windowHasFocus = true;
    }

    if (windowHasFocus) {
      ImGuiIO &io = ImGui::GetIO();

      switch (e.type) {
        case Event::MouseButtonPressed: // fall-through
        case Event::MouseButtonReleased: {
          const int32_t button = e.mouseButton.button;
          if (e.type == Event::MouseButtonPressed && button >= 0 && button < 3) {
            mousePressed[e.mouseButton.button] = true;
            return io.WantCaptureMouse;
          }
          break;
        }

        case Event::MouseScrolled: {
          io.MouseWheel += e.mouseScroll.y;
          io.MouseWheelH += e.mouseScroll.x;
          return io.WantCaptureMouse;
        }

        case Event::KeyPressed: // fall-through
        case Event::KeyReleased: {
          io.KeysDown[e.key.code] = (e.type == Event::KeyPressed);
          io.KeyCtrl = e.key.control;
          io.KeyShift = e.key.shift;
          io.KeyAlt = e.key.alt;
          return io.WantCaptureKeyboard;
        }

        case Event::TextEntered: {
          io.AddInputCharacter(static_cast<uint16_t>(e.text.unicode));
          return io.WantCaptureKeyboard;
        }
        default: break;
      }
    }
    return false;
  }

  void draw(ImDrawData *drawData) {
    ImGuiIO &io = ImGui::GetIO();
    const int32_t fbWidth = (int32_t) (drawData->DisplaySize.x * io.DisplayFramebufferScale.x);
    const int32_t fbHeight = (int32_t) (drawData->DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fbWidth <= 0 || fbHeight <= 0) {
      return;
    }
    drawData->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    uint32_t lastActiveTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (int32_t *) &lastActiveTexture);
    glActiveTexture(GL_TEXTURE0);
    int32_t lastProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
    int32_t lastTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
#ifdef GL_SAMPLER_BINDING
    int32_t lastSampler;
    glGetIntegerv(GL_SAMPLER_BINDING, &lastSampler);
#endif
    int32_t lastArrayBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
    int32_t lastVertexArray;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);
#ifdef GL_POLYGON_MODE
    int32_t lastPolygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, lastPolygonMode);
#endif
    int32_t lastViewport[4];
    glGetIntegerv(GL_VIEWPORT, lastViewport);
    int32_t lastScissorBox[4];
    glGetIntegerv(GL_SCISSOR_BOX, lastScissorBox);
    uint32_t lastBlendSrcRgb;
    glGetIntegerv(GL_BLEND_SRC_RGB, (int32_t *) &lastBlendSrcRgb);
    uint32_t lastBlendDstRgb;
    glGetIntegerv(GL_BLEND_DST_RGB, (int32_t *) &lastBlendDstRgb);
    uint32_t lastBlendSrcAlpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (int32_t *) &lastBlendSrcAlpha);
    uint32_t lastBlendDstAlpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, (int32_t *) &lastBlendDstAlpha);
    uint32_t lastBlendEquationRgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (int32_t *) &lastBlendEquationRgb);
    uint32_t lastBlendEquationAlpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (int32_t *) &lastBlendEquationAlpha);
    GLboolean lastEnableBlend = glIsEnabled(GL_BLEND);
    GLboolean lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
    GLboolean lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
    GLboolean lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    glViewport(0, 0, (int32_t) fbWidth, (int32_t) fbHeight);
    const float L = drawData->DisplayPos.x;
    const float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    const float T = drawData->DisplayPos.y;
    const float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
    const float ortho[4][4] = {
        {2.0f / (R - L),    0.0f,              0.0f,  0.0f},
        {0.0f,              2.0f / (T - B),    0.0f,  0.0f},
        {0.0f,              0.0f,              -1.0f, 0.0f},
        {(R + L) / (L - R), (T + B) / (B - T), 0.0f,  1.0f},
    };
    glUseProgram(gShaderHandle);
    glUniform1i(gTex2d0Loc, 0);
    glUniformMatrix4fv(gProjLoc, 1, GL_FALSE, &ortho[0][0]);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, 0);
#endif

    uint32_t vaoHandle = 0;
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, gVboHandle);
    glEnableVertexAttribArray(gAttrPosition);
    glEnableVertexAttribArray(gAttrTexCoord);
    glEnableVertexAttribArray(gAttrColor);
    glVertexAttribPointer(gAttrPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (void *) IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(gAttrTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (void *) IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(gAttrColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert),
                          (void *) IM_OFFSETOF(ImDrawVert, col));

    // Draw
    ImVec2 pos = drawData->DisplayPos;
    for (int32_t n = 0; n < drawData->CmdListsCount; n++) {
      const ImDrawList *cmdList = drawData->CmdLists[n];
      const ImDrawIdx *idxBufferOffset = nullptr;

      glBindBuffer(GL_ARRAY_BUFFER, gVboHandle);
      glBufferData(GL_ARRAY_BUFFER, (int32_t) cmdList->VtxBuffer.Size * sizeof(ImDrawVert),
                   (const void *) cmdList->VtxBuffer.Data, GL_STREAM_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gElementsHandle);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int32_t) cmdList->IdxBuffer.Size * sizeof(ImDrawIdx),
                   (const void *) cmdList->IdxBuffer.Data, GL_STREAM_DRAW);

      for (int32_t cmd_i = 0; cmd_i < cmdList->CmdBuffer.Size; cmd_i++) {
        const ImDrawCmd *pcmd = &cmdList->CmdBuffer[cmd_i];
        if (pcmd->UserCallback) {
          pcmd->UserCallback(cmdList, pcmd);
        } else {
          ImVec4 clipRect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y,
                                   pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
          if (clipRect.x < fbWidth && clipRect.y < fbHeight && clipRect.z >= 0.0f && clipRect.w >= 0.0f) {
            glScissor((int32_t) clipRect.x, (int32_t) (fbHeight - clipRect.w),
                      (int32_t) (clipRect.z - clipRect.x), (int32_t) (clipRect.w - clipRect.y));

            // Bind texture, Draw
            glBindTexture(GL_TEXTURE_2D, *reinterpret_cast<const uint32_t *>(&pcmd->TextureId));
            glDrawElements(GL_TRIANGLES, static_cast<int32_t>(pcmd->ElemCount), GL_UNSIGNED_SHORT, idxBufferOffset);
          }
        }
        idxBufferOffset += pcmd->ElemCount;
      }
    }
    glDeleteVertexArrays(1, &vaoHandle);

    // Restore modified GL state
    glUseProgram(lastProgram);
    glBindTexture(GL_TEXTURE_2D, lastTexture);
#ifdef GL_SAMPLER_BINDING
    glBindSampler(0, lastSampler);
#endif
    glActiveTexture(lastActiveTexture);
    glBindVertexArray(lastVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
    glBlendEquationSeparate(lastBlendEquationRgb, lastBlendEquationAlpha);
    glBlendFuncSeparate(lastBlendSrcRgb, lastBlendDstRgb, lastBlendSrcAlpha, lastBlendDstAlpha);
    if (lastEnableBlend) {
      glEnable(GL_BLEND);
    } else {
      glDisable(GL_BLEND);
    }
    if (lastEnableCullFace) {
      glEnable(GL_CULL_FACE);
    } else {
      glDisable(GL_CULL_FACE);
    }
    if (lastEnableDepthTest) {
      glEnable(GL_DEPTH_TEST);
    } else {
      glDisable(GL_DEPTH_TEST);
    }
    if (lastEnableScissorTest) {
      glEnable(GL_SCISSOR_TEST);
    } else {
      glDisable(GL_SCISSOR_TEST);
    }
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (uint32_t) lastPolygonMode[0]);
#endif
    glViewport(lastViewport[0], lastViewport[1], (int32_t) lastViewport[2], (int32_t) lastViewport[3]);
    glScissor(lastScissorBox[0], lastScissorBox[1], (int32_t) lastScissorBox[2], (int32_t) lastScissorBox[3]);
  }

  void stop(Window::Data * /*data*/) {
    destroyDeviceObjects();
  }

  bool createFontTexture() {
    // Build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    uint8_t *pixels;
    int32_t width;
    int32_t height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    // Upload texture to graphics system
    int32_t lastTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
    glGenTextures(1, &gFontTexture);
    glBindTexture(GL_TEXTURE_2D, gFontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = *(ImTextureID *) &gFontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, lastTexture);

    return true;
  }

  void destroyFontTexture() {
    if (gFontTexture) {
      ImGuiIO &io = ImGui::GetIO();
      glDeleteTextures(1, &gFontTexture);
      io.Fonts->TexID = nullptr;
      gFontTexture = 0;
    }
  }

  bool createDeviceObjects() {
    // Backup GL state
    int32_t lastTexture;
    int32_t lastArrayBuffer;
    int32_t lastVertexArray;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);

    const char *vert130 = R"(
      #version 130
      in vec2 position;
      in vec2 texCoord;
      in vec4 color;
      out vec2 v_texCoord;
      out vec4 v_color;
      uniform mat4 proj;
      void main() {
         v_texCoord = texCoord;
         v_color = color;
         gl_Position = proj * vec4(position.xy,0,1);
      }
    )";

    const char *frag130 = R"(
      #version 130
      in vec2 v_texCoord;
      in vec4 v_color;
      out vec4 color;
      uniform sampler2D u_tex2d0;
      void main() {
          color = v_color * texture(u_tex2d0, v_texCoord.st);
      }
    )";

    // Create shaders
    gVertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gVertHandle, 1, &vert130, nullptr);
    glCompileShader(gVertHandle);
    checkShader(gVertHandle, "vertex shader");

    gFragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gFragHandle, 1, &frag130, nullptr);
    glCompileShader(gFragHandle);
    checkShader(gFragHandle, "fragment shader");

    gShaderHandle = glCreateProgram();
    glAttachShader(gShaderHandle, gVertHandle);
    glAttachShader(gShaderHandle, gFragHandle);
    glLinkProgram(gShaderHandle);
    checkProgram(gShaderHandle, "shader program");

    gTex2d0Loc = glGetUniformLocation(gShaderHandle, "u_tex2d0");
    gProjLoc = glGetUniformLocation(gShaderHandle, "proj");
    gAttrPosition = glGetAttribLocation(gShaderHandle, "position");
    gAttrTexCoord = glGetAttribLocation(gShaderHandle, "texCoord");
    gAttrColor = glGetAttribLocation(gShaderHandle, "color");

    // Create buffers
    glGenBuffers(1, &gVboHandle);
    glGenBuffers(1, &gElementsHandle);

    createFontTexture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, lastTexture);
    glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
    glBindVertexArray(lastVertexArray);

    return true;
  }

  void destroyDeviceObjects() {
    if (gVboHandle) {
      glDeleteBuffers(1, &gVboHandle);
    }
    if (gElementsHandle) {
      glDeleteBuffers(1, &gElementsHandle);
      gVboHandle = 0;
      gElementsHandle = 0;
    }

    if (gShaderHandle && gVertHandle) {
      glDetachShader(gShaderHandle, gVertHandle);
    }
    if (gVertHandle) {
      glDeleteShader(gVertHandle);
      gVertHandle = 0;
    }

    if (gShaderHandle && gFragHandle) {
      glDetachShader(gShaderHandle, gFragHandle);
    }
    if (gFragHandle) {
      glDeleteShader(gFragHandle);
      gFragHandle = 0;
    }

    if (gShaderHandle) {
      glDeleteProgram(gShaderHandle);
      gShaderHandle = 0;
    }

    destroyFontTexture();
  }

}
