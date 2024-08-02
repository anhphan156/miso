#include "math.h"
#include <math.h>
#include <string.h>

void matrixMul(float *A, float *B, float *result) {
    int   i, j;
    float r[16];
    for (i = 0; i < 4; i++) {
        float ax = A[i + 0];
        float ay = A[i + 4];
        float az = A[i + 8];
        float aw = A[i + 12];

        for (j = 0; j < 4; j++) {
            float bx = B[j * 4 + 0];
            float by = B[j * 4 + 1];
            float bz = B[j * 4 + 2];
            float bw = B[j * 4 + 3];

            r[i + j * 4] = ax * bx + ay * by + az * bz + aw * bw;
        }
    }
    memcpy(result, r, 16 * sizeof(float));
}

void mkLookAt4x4(float *up, float *forward, float *position, float *result) {

    float right[3];
    cross(up, forward, right);

    cross(forward, right, up);

    float n[16];
    float lookAt[16] = {
        right[0], up[0], forward[0], 0.0f, // 1
        right[1], up[1], forward[1], 0.0f, // 2
        right[2], up[2], forward[2], 0.0f, // 3
        0.0f,     0.0f,  0.0f,       1.0f  // 4
    };
    float pos[3] = {position[0] * -1.0f, position[1] * -1.0f, position[2] * -1.0f};
    float translation[16];
    mkTranslation4x4(pos, translation);
    matrixMul(lookAt, translation, n);
    memcpy(result, n, sizeof(float) * 16);
}

void mkPerspective4x4(float fov, float aspect, float znear, float zfar, float *result) {
    float lambda = zfar / (zfar - znear);
    float f      = 1 / tan(fov / 2.0);

    float n[16] = {
        aspect * f, 0.0f, 0.0f,
        0.0f, // 1
        0.0f,       f,    0.0f,
        0.0f, // 2
        0.0f,       0.0f, lambda,
        1.0f, // 3
        0.0f,       0.0f, lambda * znear * -1.0f,
        1.0f // 4
    };

    memcpy(result, n, 16 * sizeof(float));
}

void mkScale4x4(float *scale, float *result) {
    float n[16] = {
        scale[0], 0.0f,     0.0f,     0.0f, // 1
        0.0f,     scale[1], 0.0f,     0.0f, // 2
        0.0f,     0.0f,     scale[2], 0.0f, // 3
        0.0f,     0.0f,     0.0f,     1.0f, // 4
    };
    memcpy(result, n, 16 * sizeof(float));
}

void mkTranslation4x4(float *translation, float *result) {
    float n[16] = {
        1.0f,           0.0f,           0.0f,           0.0f, // 1
        0.0f,           1.0f,           0.0f,           0.0f, // 2
        0.0f,           0.0f,           1.0f,           0.0f, // 3
        translation[0], translation[1], translation[2], 1.0f, // 4
    };
    memcpy(result, n, 16 * sizeof(float));
}

void mkRotationZ4x4(float a, float *result) {
    float s     = sin(a);
    float c     = cos(a);
    float n[16] = {c, s, 0.0f, 0.0f, -s, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    memcpy(result, n, 16 * sizeof(float));
}

void mkRotationY4x4(float a, float *result) {
    float s     = sin(a);
    float c     = cos(a);
    float n[16] = {
        c,    0.0f, s,    0.0f, // 1
        0.0f, 1.0f, 0.0f, 0.0f, // 2
        -s,   0.0f, c,    0.0f, // 3
        0.0f, 0.0f, 0.0f, 1.0f  // 4
    };
    memcpy(result, n, 16 * sizeof(float));
}

void mkRotation2x2(float a, float *result) {
    float s    = sin(a);
    float c    = cos(a);
    float n[4] = {c, s, -s, c};
    memcpy(result, n, 4 * sizeof(float));
}

void cross(float *a, float *b, float *result) {
    float n[3] = {
        a[1] * b[2] - a[2] * b[1], // 1
        a[2] * b[0] - a[0] * b[2], // 2
        a[0] * b[1] - a[1] * b[0]  // 3
    };
    memcpy(result, n, sizeof(float) * 3);
}

void normalize(float *v, float *result) {
    float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    float n[3]   = {v[0] / length, v[1] / length, v[2] / length};
    memcpy(result, n, sizeof(float) * 3);
}

float lengthSquare(float *v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
