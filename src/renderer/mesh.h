#ifndef MESH_H
#define MESH_H

struct Mesh {
    unsigned int vao;
    unsigned int ibo;
    unsigned int vbo;
    unsigned int shader_program;

    unsigned int indices_count;

    float position[3];
    float scale[3];
};

struct Mesh *CreateMesh(float *positions, unsigned int position_count, unsigned int *indices, unsigned int indices_count);

void ClearMesh(struct Mesh *);

#endif
