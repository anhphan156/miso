#include "model_loader.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>
#include <stdlib.h>

void load_model(const char *model_path, float **positions, unsigned int *positions_count, unsigned int **indices, unsigned int *indices_count) {

    const struct aiScene *scene = aiImportFile(model_path, aiProcess_CalcTangentSpace | aiProcess_Triangulate);
    if (NULL == scene) {
        printf("%s\n", aiGetErrorString());
        *positions       = NULL;
        *indices         = NULL;
        *positions_count = 0;
        *indices_count   = 0;
        return;
    }

    *positions_count = 8 * scene->mMeshes[0]->mNumVertices;
    *positions       = (float *)malloc(sizeof(float) * *positions_count);

    int i;
    for (i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
        float pos_x    = scene->mMeshes[0]->mVertices[i].x;
        float pos_y    = scene->mMeshes[0]->mVertices[i].y;
        float pos_z    = scene->mMeshes[0]->mVertices[i].z;
        float normal_x = scene->mMeshes[0]->mNormals[i].x;
        float normal_y = scene->mMeshes[0]->mNormals[i].y;
        float normal_z = scene->mMeshes[0]->mNormals[i].z;

        float uv_x;
        float uv_y;
        if (scene->mMeshes[0]->mTextureCoords[0]) {
            uv_x = scene->mMeshes[0]->mTextureCoords[0][i].x;
            uv_y = scene->mMeshes[0]->mTextureCoords[0][i].y;
        } else {
            uv_x = 0.0f;
            uv_y = 0.0f;
        }

        (*positions)[i * 8 + 0] = pos_x;
        (*positions)[i * 8 + 1] = pos_y;
        (*positions)[i * 8 + 2] = pos_z;
        (*positions)[i * 8 + 3] = normal_x;
        (*positions)[i * 8 + 4] = normal_y;
        (*positions)[i * 8 + 5] = normal_z;
        (*positions)[i * 8 + 6] = uv_x;
        (*positions)[i * 8 + 7] = uv_y;
    }

    *indices_count = scene->mMeshes[0]->mNumFaces * 3;
    *indices       = (unsigned int *)malloc(sizeof(unsigned int) * *indices_count);
    for (i = 0; i < scene->mMeshes[0]->mNumFaces; i++) {
        struct aiFace face = scene->mMeshes[0]->mFaces[i];
        for (unsigned int j = 0; j < 3; j++) {
            (*indices)[i * 3 + j] = face.mIndices[j];
        }
    }

    aiReleaseImport(scene);
}
