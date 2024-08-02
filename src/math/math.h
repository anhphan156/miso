#ifndef MATH_H
#define MATH_H
#include <math.h>
#include <string.h>

void matrixMul(float *A, float *B, float *result);

void mkLookAt4x4(float *up, float *forward, float *position, float *result);

void mkPerspective4x4(float fov, float aspect, float znear, float zfar, float *result);
void mkScale4x4(float *scale, float *result);

void mkTranslation4x4(float *translation, float *result);

void mkRotationZ4x4(float a, float *result);

void mkRotationY4x4(float a, float *result);

void mkRotation2x2(float a, float *result);

void cross(float *a, float *b, float *result);

void normalize(float *v, float *result);

float lengthSquare(float *v);

#endif
