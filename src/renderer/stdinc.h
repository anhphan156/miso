#ifndef STCINC_H
#define STCINC_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE

#ifndef DEBUG_MODE
#define GLCall(x) x;
#else

#define BREAKPOINT asm volatile("int3;")
#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    BREAKPOINT;

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLCheckError(#x, __FILE__, __LINE__));

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

static int GLCheckError(const char *function, const char *file, int line) {
  GLenum e;
  if ((e = glGetError())) {
    printf("\nOpenGL error: 0x%x in file %s on line %d at function %s\n\n", e,
           file, line, function);
    return 0;
  }
  return 1;
}
#endif

#endif
