#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "GL\glew.h"

ModuleProgram::ModuleProgram()
{}

// Destructor
ModuleProgram::~ModuleProgram()
{}

// Called before render is available
bool ModuleProgram::Init()
{
    bool ret = true;
    /*
    program = glCreateProgram();
    */
    char* vertexShader = LoadShaderSource("vertexShader.txt");
    char* fragmentShader = LoadShaderSource("fragmentShader.txt");

    //glCreateShader(GL_VERTEX_SHADER);
    //glCreateShader(GL_FRAGMENT_SHADER);

    unsigned compiledVertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned compiledFragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glCompileShader(compiledVertex);
    glCompileShader(compiledFragment);
    /*
    glAttachShader(program,compiledVertex);
    glAttachShader(program, compiledFragment);
    glLinkProgram(program);*/
    CreateProgram(compiledVertex, compiledFragment);
    return ret;

}

// Called every draw update
update_status ModuleProgram::Update()
{


    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleProgram::CleanUp()
{

    glDeleteProgram(program_id);
    return true;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
    program_id = glCreateProgram();
    glAttachShader(program_id, vtx_shader);
    glAttachShader(program_id, frg_shader);
    glLinkProgram(program_id);
    int res;
    glGetProgramiv(program_id, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetProgramInfoLog(program_id, len, &written, info);
            LOG("Program Log Info: %s", info);
            free(info);
        }
    }
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    return program_id;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{

    char* dataShader = nullptr;
    FILE* file = nullptr;
    fopen_s(&file, shader_file_name, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        dataShader = (char*)malloc(size + 1);
        fseek(file, 0, SEEK_SET);
        fread(dataShader, 1, size, file);
        dataShader[size] = 0;
        fclose(file);
    }
    return dataShader;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
    unsigned shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, 0);
    glCompileShader(shader_id);
    int res = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetShaderInfoLog(shader_id, len, &written, info);
            LOG("Log Info: %s", info);
            free(info);
        }
    }
    return shader_id;
}
