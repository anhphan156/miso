#include "mesh.h"
#include "shader.h"
#include "stdinc.h"

struct Mesh *CreateMesh(float *positions, unsigned int position_count, unsigned int *indices, unsigned int indices_count) {
    struct Mesh *m = (struct Mesh *)malloc(sizeof(struct Mesh));
    m->indices_count = indices_count;

    GLCall(glGenVertexArrays(1, &m->vao));
    GLCall(glBindVertexArray(m->vao));

    GLCall(glGenBuffers(1, &m->vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m->vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, position_count * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glGenBuffers(1, &m->ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // attributes
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)0));

    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(3 * sizeof(float))));

    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(6 * sizeof(float))));

    char vert[61] = "/home/backspace/data/dev/miso/resources/shaders/color.vert";
    char frag[61] = "/home/backspace/data/dev/miso/resources/shaders/color.frag";
    m->shader_program = create_shader_program(vert, frag);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

    return m;
}

void ClearMesh(struct Mesh *m) {

    GLCall(GLCall(glDeleteBuffers(1, &m->vbo)));
    GLCall(glDeleteBuffers(1, &m->ibo));
    GLCall(glDeleteProgram(m->shader_program));
    GLCall(glDeleteVertexArrays(1, &m->vao));

    free(m);
}
