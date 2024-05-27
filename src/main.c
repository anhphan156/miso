#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_GLFW
#include "cimgui.h"
#include "cimgui_impl.h"
#include "math/math.h"
#include "renderer/mesh.h"
#include "renderer/model_loader.h"
#include "renderer/stdinc.h"
#include "renderer/window.h"

struct ImGuiContext *ctx;
struct ImGuiIO *io;

void gui_init(GLFWwindow *win) {
  // IMGUI_CHECKVERSION();
  ctx = igCreateContext(NULL);
  io = igGetIO();

  const char *glsl_version = "#version 460 core";
  ImGui_ImplGlfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup style
  igStyleColorsDark(NULL);
}

int main() {
  GLFWwindow *window = GetWindow();
  if (window == NULL) {
    return 1;
  }
  glewInit();
  gui_init(window);

  const char model_path[50] =
      "/home/backspace/data/dev/miso/resources/cube.obj";
  float *positions = NULL;
  unsigned int *indices = NULL;
  unsigned int positions_count = 0;
  unsigned int indices_count = 0;
  load_model(model_path, &positions, &positions_count, &indices,
             &indices_count);

  unsigned int meshes_count = 200;
  struct Mesh *mesh =
      CreateMesh(positions, positions_count, indices, indices_count);

  free(positions);
  free(indices);

  unsigned int modelMatricesBufferDataSize = sizeof(float) * 16 * meshes_count;
  unsigned int modelMatricesBuffer;
  GLCall(glCreateBuffers(1, &modelMatricesBuffer));
  GLCall(glNamedBufferStorage(modelMatricesBuffer, modelMatricesBufferDataSize,
                              NULL, GL_DYNAMIC_STORAGE_BIT));

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  float translation[16];
  float scale[16];
  float rotation[16];
  float view[16];
  float perspective[16];
  float view_proj[16];
  float r[16];

  float *modelMatrices = (float *)alloca(modelMatricesBufferDataSize);

  mkPerspective4x4(3.14f / 4.0f, GetAspectRatio(), 1.0f, 100.0f, perspective);

  float up[3] = {0.0f, 1.0f, 0.0f};
  float forward[3] = {0.0f, .0f, 1.0f};
  float camera_pos[3] = {0.0f, 0.0f, 0.0f};
  mkLookAt4x4(up, forward, camera_pos, view);
  matrixMul(perspective, view, view_proj);

  float step = 2.0f / meshes_count;
  float ls = step * 2.f;
  float lscale[3] = {ls, ls, ls};

  bool showAnotherWindow = true;
  unsigned long fr = 0;
  while (!glfwWindowShouldClose(window)) {
    printf("%f\n", fr++ / glfwGetTime());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    if (showAnotherWindow) {
      igBegin("imgui Another Window", &showAnotherWindow, 0);
      igText("Hello from imgui");
      ImVec2 buttonSize;
      buttonSize.x = 0;
      buttonSize.y = 0;
      if (igButton("Close me", buttonSize)) {
        showAnotherWindow = false;
      }
      igEnd();
    }

    igRender();
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    for (int i = 0; i < meshes_count; i++) {
      double t = glfwGetTime();

      float px = ((float)i + 0.5f) * step - 1.0f;

      float py;
      if (i % 2 == 0) {
        py = sin(px * 3.14f + t);
      } else {
        py = sin(px * 3.14f + t - 3.14f);
      }

      float p[3] = {px, py, 2.0f};
      memcpy(mesh->position, p, sizeof(float) * 3);
      memcpy(mesh->scale, lscale, sizeof(float) * 3);

      mkTranslation4x4(mesh->position, translation);
      mkRotationY4x4(0.0f, rotation);
      mkScale4x4(mesh->scale, scale);
      matrixMul(rotation, scale, r);
      matrixMul(translation, r, r);

      memcpy(modelMatrices + i * 16, r, sizeof(float) * 16);

      /*GLCall(glDrawElements(GL_TRIANGLES, mesh->indices_count,
       * GL_UNSIGNED_INT,*/
      /*                      NULL));*/
    }
    GLCall(glNamedBufferSubData(modelMatricesBuffer, 0,
                                modelMatricesBufferDataSize, modelMatrices));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, modelMatricesBuffer));
    GLCall(glBindVertexArray(mesh->vao));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));
    GLCall(glUseProgram(mesh->shader_program));
    GLCall(int loc = glGetUniformLocation(mesh->shader_program, "u_vp"));
    if (loc != -1) {
      GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, view_proj));
    }
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count,
                                   GL_UNSIGNED_INT, NULL, meshes_count));

    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ClearMesh(mesh);
  glfwTerminate();

  return 0;
}
