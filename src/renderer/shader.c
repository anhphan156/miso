#include "shader.h"
#include "stdinc.h"
#include <alloca.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *load_shader_source(char *file_name, size_t *len) {
    struct stat sb;
    char       *buf = NULL;

    int fd = open(file_name, O_RDONLY);
    if (-1 == fd) {
        perror("open");
        return NULL;
    }

    if (-1 == fstat(fd, &sb)) {
        perror("fstat");
        return NULL;
    }

    buf  = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    *len = sb.st_size;

    close(fd);

    return buf;
}

unsigned int compile_shader(unsigned int type, const char *const source) {
    GLCall(unsigned int id = glCreateShader(type));
    GLCall(glShaderSource(id, 1, &source, NULL));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        if (message) {
            printf("Shader compilation failed: %s", message);
        }
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int create_shader_program(char *vert, char *frag) {
    GLCall(unsigned int program = glCreateProgram());

    size_t vert_len, frag_len;
    char  *vert_source = load_shader_source(vert, &vert_len);
    char  *frag_source = load_shader_source(frag, &frag_len);
    if (vert_source == NULL || frag_source == NULL) {
        printf("shader source is NULL");
    }

    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vert_source);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, frag_source);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    munmap(vert_source, vert_len);
    munmap(frag_source, frag_len);
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int create_compute_shader_program(char *file_path) {
    GLCall(unsigned int program = glCreateProgram());

    size_t source_len;
    char  *source = load_shader_source(file_path, &source_len);
    if (NULL == source)
        printf("shader source is NULL");

    unsigned int cs = compile_shader(GL_COMPUTE_SHADER, source);

    GLCall(glAttachShader(program, cs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    munmap(source, source_len);
    GLCall(glDeleteShader(cs));

    return program;
}

void uniform1f(unsigned int program, char *name, float value) {
    GLCall(int loc = glGetUniformLocation(program, name));
    if (loc != -1)
        GLCall(glUniform1f(loc, value));
}
void uniform1i(unsigned int program, char *name, int value) {
    GLCall(int loc = glGetUniformLocation(program, name));
    if (loc != -1)
        GLCall(glUniform1i(loc, value));
}
void uniformm4f(unsigned int program, char *name, float *value) {
    GLCall(int loc = glGetUniformLocation(program, name));
    if (loc != -1)
        GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, value));
}
void uniform3fv(unsigned int program, char *name, float *value) {
    GLCall(int loc = glGetUniformLocation(program, name));
    if (loc != -1)
        GLCall(glUniform3fv(loc, 1, value));
}
