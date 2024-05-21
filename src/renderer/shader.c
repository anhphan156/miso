#include <alloca.h>
#include "stdinc.h"
#include "shader.h"

char* load_shader_source(char* file_name){
    char* buffer = 0;    
    long length;
    FILE* f = fopen(file_name, "r");

    if(f){
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char*)malloc(length + 1);
        if(buffer){
            fread(buffer, 1, length, f); 
        }
        *(buffer + length) = '\0';
        fclose(f);
    }

    return buffer;
}

unsigned int compile_shader(unsigned int type, const char* const source){
    GLCall(unsigned int id = glCreateShader(type));
    GLCall(glShaderSource(id, 1, &source, NULL));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(!result){
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        if(message){
            printf("Shader compilation failed: %s", message);
        }
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int create_shader_program(char* vert, char* frag){
    GLCall(unsigned int program = glCreateProgram());

    char* vert_source = load_shader_source(vert);
    char* frag_source = load_shader_source(frag);

    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vert_source);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, frag_source);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    free(vert_source);
    free(frag_source);
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}