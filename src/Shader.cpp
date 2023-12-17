#include "Shader.hpp"

// Constructor
Shader::Shader() = default;

// Destructor
Shader::~Shader() {
	glDeleteProgram(m_shaderID); // deallocate Program
}

// Selects a shader
void Shader::bind() const {
	glUseProgram(m_shaderID);
}

// Deselects a shader
void Shader::unbind() {
	glUseProgram(0);
}

void Shader::log(const char* system, const char* message) {
    std::cout << "[" << system << "]" << message << std::endl;
}

// Loads a shader
std::string Shader::loadShader(const std::string &fileName) {
    std::string result;
    // Get every line of data
    std::string line;
    std::ifstream myFile(fileName.c_str());
    if (myFile.is_open()) {
        while (getline(myFile,line)) {
                result += line + '\n';
                // SDL_Log(line); // uncomment this if you want to see the shader code get printed out
        }
    } else {
        log("loadShader", "file not found. Try an absolute file path to see if the file exists");
    }
    myFile.close(); // close file
    return result;
}

// Creates a shader from a loaded vertex and fragment shaders
void Shader::createShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    unsigned int program = glCreateProgram(); // create a new program
    // Compile our shaders
    unsigned int myVertexShader   = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myFragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    // Link our program
    // These have been compiled already
    glAttachShader(program,myVertexShader);
    glAttachShader(program,myFragmentShader);
    // Link our programs that have been 'attached'
    glLinkProgram(program);
    glValidateProgram(program);
    // Once the shaders have been linked in, we can delete them
    glDetachShader(program,myVertexShader);
    glDetachShader(program,myFragmentShader);
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);
    if (!checkLinkStatus(program)) {
        log("createShader", "ERROR, shader did not link! Were there compile errors in the shader?");
    }
    m_shaderID = program;
}

// Compiles loaded shaders
unsigned int Shader::compileShader(unsigned int type, const std::string &source) {
    // Compile our shaders
    // id is the type of shader (vertex, fragment, etc.)
    unsigned int id;
    if (type == GL_VERTEX_SHADER) {
        id = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER) {
        id = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // the source of our shader
    glCompileShader(id); // now compile our shader
    // Retrieve the result of our compilation
    int result;
    // This code is returning any compilation errors that may have occurred
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(id, length, &length, errorMessages);
        if (type == GL_VERTEX_SHADER) {
            log("compileShader ERROR", "GL_VERTEX_SHADER compilation failed!");
            log("compileShader ERROR", (const char *)errorMessages);
        } else if (type == GL_FRAGMENT_SHADER) {
            log("compileShader ERROR", "GL_FRAGMENT_SHADER compilation failed!");
            log("compileShader ERROR", (const char *)errorMessages);
        }
        delete[] errorMessages; // reclaim our memory
        glDeleteShader(id); // delete our broken shader
        return 0;
    }
    return id;
}

// Checks if shaders 'linked' successfully
bool Shader::checkLinkStatus(GLuint programID) {
    // Retrieve the result of our compilation                                                                                           
    int result;                                                                                                                         
    // This code is returning any Linker errors that may have occurred
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetProgramInfoLog(programID, length, &length, errorMessages);
        SDL_Log("ERROR in linking process\n");
        SDL_Log("%s\n",errorMessages);
        delete[] errorMessages; // reclaim our memory
        return false;
    }
    return true;
}

// Set our uniforms for our shader.
void Shader::setUniformMatrix4fv(const GLchar* name, const GLfloat* value) const{
    // Note that we are now 'looking' inside the shader for a particular
    // variable. This means the name has to exactly match!
    GLint location = glGetUniformLocation(m_shaderID, name);
    if (location >= 0) {
        // Now update this information through our uniforms.
        // glUniformMatrix4v means a 4x4 matrix of floats
        glUniformMatrix4fv(location, 1, GL_FALSE, value);
    } else {
        std::cerr << "Could not find " << name << ", maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Sets 2 float values in a vec2 in our uniform.
void Shader::setUniform2fv(const GLchar* name, const GLfloat* value) const {
    GLint location = glGetUniformLocation(m_shaderID, name);
    if (location >= 0) {
        glUniform2fv(location, 1, value);
    } else {
        std::cerr << "Could not find " << name << ", maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Sets 1 int value in our uniform
void Shader::setUniform1i(const GLchar* name, int value) const{
    GLint location = glGetUniformLocation(m_shaderID,name);
    glUniform1i(location, value);
}

// Sets 1 float value in our uniform
void Shader::setUniform1f(const GLchar* name, float value) const {
    GLint location = glGetUniformLocation(m_shaderID,name);
    if (location >= 0) {
        glUniform1f(location, value);
    } else {
        std::cerr << "Could not find " << name << ", maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
}
