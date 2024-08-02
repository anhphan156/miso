#include "imguiLayers/includes.h"
#include "imguiLayers/init.h"
#include "math/math.h"
#include "renderer/mesh.h"
#include "renderer/model_loader.h"
#include "renderer/shader.h"
#include "renderer/stdinc.h"
#include "renderer/window.h"

struct ImGuiContext *ctx;
struct ImGuiIO      *io;

int main() {
    GLFWwindow *window = GetWindow();
    if (window == NULL) {
        return 1;
    }
    glewInit();
    gui_init(window, &ctx, &io);

    const char    model_path[50]  = "/home/backspace/data/dev/miso/resources/cube.obj";
    float        *positions       = NULL;
    unsigned int *indices         = NULL;
    unsigned int  positions_count = 0;
    unsigned int  indices_count   = 0;
    load_model(model_path, &positions, &positions_count, &indices, &indices_count);

    unsigned int meshes_count = 1000;
    struct Mesh *mesh         = CreateMesh(positions, positions_count, indices, indices_count);

    free(positions);
    free(indices);

    unsigned int positionBufferSize = sizeof(float) * 4 * meshes_count * meshes_count;
    unsigned int positionBuffer;
    GLCall(glCreateBuffers(1, &positionBuffer));
    GLCall(glNamedBufferData(positionBuffer, positionBufferSize, NULL, GL_DYNAMIC_DRAW));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    char         compute_path[66] = "/home/backspace/data/dev/miso/resources/shaders/position.compute";
    unsigned int cs_program       = create_compute_shader_program(compute_path);

    float view[16];
    float perspective[16];
    float view_proj[16];

    mkPerspective4x4(3.14f / 4.0f, GetAspectRatio(), 1.0f, 100.0f, perspective);

    float up[3]          = {0.0f, 1.0f, 0.0f};
    float forward_tmp[3] = {0.0f, -1.0f, 2.0f};
    float forward[3];
    normalize(forward_tmp, forward);
    float camera_pos[3] = {0.0f, 1.0f, 0.0f};
    mkLookAt4x4(up, forward, camera_pos, view);
    matrixMul(perspective, view, view_proj);

    float         ls         = 2.f;
    float         freq       = 1.0f;
    float         amp        = 1.0f;
    unsigned long fr         = 0;
    bool          showWindow = true;
    float         lPos[3]    = {0.0f, 10.0f, -3.0f};
    float         shape      = 1.0f;

    meshes_count = 200;
    while (!glfwWindowShouldClose(window)) {

        IMGUI_NEW_FRAME

        igBegin("imgui Window", &showWindow, 0);
        igDragFloat("Shape", &shape, .01f, 0.f, 1.0f, "%.2f", 0);
        igDragFloat("Freq", &freq, .1f, 0.0f, 20.0f, "%.3f", 0);
        igDragFloat("Amp", &amp, .05f, 0.0f, 5.0f, "%.3f", 0);
        igDragFloat("Scale", &ls, .1f, .1f, 20.0f, "%.3f", 0);
        igInputInt("Count", (int *)&meshes_count, 0, 0, 0);
        igSliderFloat3("Light Pos", lPos, -10.0f, 10.0f, "%.2f", 0);
        igText("FPS: %f", fr++ / glfwGetTime());
        igEnd();

        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glUseProgram(cs_program));
        uniform1i(cs_program, "_Resolution", meshes_count);
        uniform1f(cs_program, "_t", glfwGetTime());
        uniform1f(cs_program, "_Step", 2.0 / meshes_count);
        uniform1f(cs_program, "_Freq", freq);
        uniform1f(cs_program, "_Amp", amp);
        uniform1f(cs_program, "_Shape", shape);

        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, positionBuffer));
        int groups = meshes_count / 8 + 1;
        GLCall(glDispatchCompute(groups, groups, 1));
        GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        GLCall(glBindVertexArray(mesh->vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo));

        GLCall(glUseProgram(mesh->shader_program));
        uniformm4f(mesh->shader_program, "u_vp", view_proj);
        uniform1f(mesh->shader_program, "u_baseScale", ls);
        uniform1f(mesh->shader_program, "u_resolution", meshes_count);
        uniform1f(mesh->shader_program, "u_time", glfwGetTime());
        uniform3fv(mesh->shader_program, "u_lightPos", lPos);

        GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, NULL, meshes_count * meshes_count));

        IMGUI_RENDER
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ClearMesh(mesh);
    glDeleteBuffers(1, &positionBuffer);
    glfwTerminate();

    return 0;
}
