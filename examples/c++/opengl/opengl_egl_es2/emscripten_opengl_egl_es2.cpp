#ifndef __EMSCRIPTEN__
#define USE_GLEW 0
#endif

#if USE_GLEW
#include "GL/glew.h"
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>

#if !USE_GLEW
#include <GLES2/gl2.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

std::string vert_shader = R"(
#version 100
//precision mediump float;

attribute vec3 a_pos;
attribute float a_id;
varying float v_id;

void main()
{
    v_id = a_id;
    gl_Position = vec4(a_pos, 1.0);
}
)";

std::string frag_shader = R"(
#version 100
precision mediump float;

varying float v_id;
uniform float u_time;

vec3 HUEtoRGB(in float H)
{
    float R = abs(H * 6.0 - 3.0) - 1.0;
    float G = 2.0 - abs(H * 6.0 - 2.0);
    float B = 2.0 - abs(H * 6.0 - 4.0);
    return clamp( vec3(R,G,B), 0.0, 1.0 );
}

void main()
{
    float hue = fract(u_time * 0.3 + v_id * 0.5);
    gl_FragColor = vec4(HUEtoRGB(hue), 1.0);
}
)";

bool CompileStatus(GLuint shader)
{
    GLint status = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char>log( logLen );
        GLsizei written;
        glGetShaderInfoLog(shader, logLen, &written, log.data());
        std::cout << "compile error:" << std::endl << log.data() << std::endl;
    }
    return status != GL_FALSE;
}

bool LinkStatus(GLuint program)
{
    GLint status = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char>log(logLen);
        GLsizei written;
        glGetProgramInfoLog( program, logLen, &written, log.data() );
        std::cout << "link error:" << std::endl << log.data() << std::endl;
    }
    return status != GL_FALSE;
}

GLuint shader_program_object = 0;
GLuint triangle_vbo;
GLint pos_attribute_index;
GLint id_attribute_index;
GLint time_uniform_location;
std::chrono::high_resolution_clock::time_point start_time;

void gl_init()
{
    auto vert_sh_obj = glCreateShader(GL_VERTEX_SHADER);
    const char *vert_shader_ptr = vert_shader.c_str();
    glShaderSource(vert_sh_obj, 1, &vert_shader_ptr, nullptr);
    glCompileShader(vert_sh_obj);
    CompileStatus(vert_sh_obj);

    auto frag_sh_obj = glCreateShader(GL_FRAGMENT_SHADER);
    const char *frag_shader_ptr = frag_shader.c_str();
    glShaderSource(frag_sh_obj, 1, &frag_shader_ptr, nullptr);
    glCompileShader(frag_sh_obj);
    CompileStatus(frag_sh_obj);

    shader_program_object = glCreateProgram();
    glAttachShader(shader_program_object, vert_sh_obj);
    glAttachShader(shader_program_object, frag_sh_obj);
    glLinkProgram(shader_program_object);
    LinkStatus(shader_program_object);

    glDeleteShader(vert_sh_obj);
    glDeleteShader(frag_sh_obj);

    pos_attribute_index = glGetAttribLocation(shader_program_object, "a_pos");
    std::cout << "`a_pos` attribute index: " << pos_attribute_index << std::endl;
    id_attribute_index = glGetAttribLocation(shader_program_object, "a_id");
    std::cout << "`a_id` attribute index: " << id_attribute_index << std::endl;
    time_uniform_location = glGetUniformLocation(shader_program_object, "u_time");
    std::cout << "`u_time` uniform location: " << time_uniform_location << std::endl;

    static const std::vector<GLfloat> varray{ -0.866f, -0.75f, 0.0f, 0.0f, 0.866f, -0.75f, 0.0f, 1.0f, 0.0f, 0.75f, 0.0f, 2.0f };
    glGenBuffers(1, &triangle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
    glBufferData(GL_ARRAY_BUFFER, varray.size()*sizeof(*varray.data()), varray.data(), GL_STATIC_DRAW );

    start_time = std::chrono::high_resolution_clock::now();
}

struct Environment
{
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLSurface surface;
};

static struct Environment g_env = {};

void gl_draw()
{
    std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
    auto delta_time = current_time - start_time;
    double time_s = (double)std::chrono::duration_cast<std::chrono::milliseconds>(delta_time).count() / 1000;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_object);
    glUniform1f(time_uniform_location, static_cast<float>(time_s));

    glEnableVertexAttribArray(pos_attribute_index);
    glEnableVertexAttribArray(id_attribute_index);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
    glVertexAttribPointer(pos_attribute_index, 3, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), nullptr);
    glVertexAttribPointer(id_attribute_index, 1, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, 3);

    eglSwapBuffers(g_env.display, g_env.surface);
}

EM_BOOL gl_animation_frame(double time, void* userData) 
{
    gl_draw();
    return EM_TRUE;
}

int main(int argc, char *argv[])
{
    std::cout << "EGL, OpenGL ES 2.0" << std::endl;
    std::cout << "initialize view ..." << std::endl;

    g_env.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_env.display, &g_env.majorVersion, &g_env.minorVersion);
    EGLint numConfigs = 0;
    EGLint attribList[] = 
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };
    EGLConfig config;
    eglChooseConfig(g_env.display, attribList, &config, 1, &numConfigs);
    
    g_env.surface = eglCreateWindowSurface(g_env.display, config, 0, NULL);
    static const EGLint contextAttribList[] = 
    {
        EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    EGLContext context = eglCreateContext(g_env.display, config, EGL_NO_CONTEXT, contextAttribList);
    eglMakeCurrent(g_env.display, g_env.surface, g_env.surface, context);

    std::cout << glGetString( GL_VENDOR ) << std::endl;
    std::cout << glGetString( GL_RENDERER ) << std::endl;
    std::cout << glGetString( GL_VERSION ) << std::endl;
    std::cout << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    std::cout << "initialize OpenGL/WebGL ..." << std::endl;
    gl_init();
     
    std::cout << "run application ..." << std::endl; 
    //emscripten_request_animation_frame_loop(gl_animation_frame, 0);
    emscripten_set_main_loop(gl_draw, 0, false);
    
    return 0;
}
