#include "shaders.h"
extern s_globals globals;

void make_shader(std::string vertex_shader, std::string fragment_shader, GLuint* shader) {
    GLuint VS_h, FS_h, prog_h;

    //Nacteni shaderu
    VS_h = glCreateShader(GL_VERTEX_SHADER);
    FS_h = glCreateShader(GL_FRAGMENT_SHADER);
    std::string VSsrc = textFileRead(vertex_shader);
    const char* VS_string = VSsrc.c_str();
    std::string FSsrc = textFileRead(fragment_shader);
    const char* FS_string = FSsrc.c_str();
    glShaderSource(VS_h, 1, &VS_string, NULL);
    glShaderSource(FS_h, 1, &FS_string, NULL);
    glCompileShader(VS_h);
    logErrorShader(VS_h);
    glCompileShader(FS_h);
    logErrorShader(FS_h);

    //Tvorba programu
    prog_h = glCreateProgram();
    glAttachShader(prog_h, VS_h);
    glAttachShader(prog_h, FS_h);
    glLinkProgram(prog_h);
    logErrorProgram(prog_h);
    *shader = prog_h;       //nahraje program do promenne v mainu
    glUseProgram(prog_h);

}

void make_shaders() {
    GLuint shader_mesh;
    make_shader("resources/basic.vert", "resources/basic.frag", &shader_mesh);
    GLuint shader_light;
    make_shader("resources/light2.vert", "resources/light.frag", &shader_light);
    GLuint shader_texture;
    make_shader("resources/texture.vert", "resources/texture.frag", &shader_texture);
    globals.shader["mesh"] = shader_mesh;
    globals.shader["light"] = shader_light;
    globals.shader["texture"] = shader_texture;
}

std::string textFileRead(const std::string fn) {
    std::ifstream file;
    std::stringstream ss;
    file.open(fn);
    if (file.is_open()) {
        ss << file.rdbuf();
    }
    else {
        std::cerr << "Error opening file: " << fn << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();
    return std::move(ss.str());
}
void logErrorShader(const GLuint shader) {
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::make_shaders::COULD_NOT_COMPILE_SHADER" << "\n";
        std::cout << infoLog << "\n";

    }
}

void logErrorProgram(const GLuint program) {
    GLint success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::make_shaders::COULD_NOT_LINK_PROGRAM" << "\n";
        std::cout << infoLog << "\n";

    }
}