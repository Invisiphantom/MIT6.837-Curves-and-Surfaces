#include "Utils.h"

Utils::Utils() {}

string Utils::readShaderFile(const char *filePath)
{
    string content;

    ifstream fileStream(filePath, ios::in);
    if (!fileStream.is_open())
    {
        cerr << "Unable to open file: " << filePath << endl;
    }
    string line = "";
    while (!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

bool Utils::checkOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        cerr << "glError: 0x" << hex << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

void Utils::printShaderLog(GLuint shader)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cerr << "Shader Info Log: " << log << endl;
        free(log);
    }
}

GLuint Utils::prepareShader(int shaderTYPE, const char *shaderPath)
{
    GLint shaderCompiled;
    string shaderStr = readShaderFile(shaderPath);
    const char *shaderSrc = shaderStr.c_str();
    GLuint shaderRef = glCreateShader(shaderTYPE);

    if (shaderRef == 0 || shaderRef == GL_INVALID_ENUM)
    {
        printf("Error: Could not create shader \"%s\" of type:%d\n", shaderPath, shaderTYPE);
        exit(1);
    }

    glShaderSource(shaderRef, 1, &shaderSrc, NULL);
    glCompileShader(shaderRef);
    checkOpenGLError();

    glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE)
    {
        cerr << "shader compilation error: " << shaderPath << endl;
        printShaderLog(shaderRef);
    }
    return shaderRef;
}

void Utils::printProgramLog(int prog)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = (char *)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cerr << "Program Info Log: " << log << endl;
        free(log);
    }
}

int Utils::finalizeShaderProgram(GLuint sprogram)
{
    GLint linked;
    glLinkProgram(sprogram);
    checkOpenGLError();
    glGetProgramiv(sprogram, GL_LINK_STATUS, &linked);
    if (linked != 1)
    {
        cerr << "linking failed" << endl;
        printProgramLog(sprogram);
    }
    return sprogram;
}

GLuint Utils::createShaderProgram(const char *vertPath, const char *fragPath)
{
    GLuint vShader = prepareShader(GL_VERTEX_SHADER, vertPath);
    GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fragPath);
    GLuint vfprogram = glCreateProgram();
    glAttachShader(vfprogram, vShader);
    glAttachShader(vfprogram, fShader);
    finalizeShaderProgram(vfprogram);
    return vfprogram;
}

GLuint Utils::createShaderProgram(const char *vertPath, const char *geomPath, const char *fragPath)
{
    GLuint vShader = prepareShader(GL_VERTEX_SHADER, vertPath);
    GLuint gShader = prepareShader(GL_GEOMETRY_SHADER, geomPath);
    GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fragPath);
    GLuint vgfprogram = glCreateProgram();
    glAttachShader(vgfprogram, vShader);
    glAttachShader(vgfprogram, gShader);
    glAttachShader(vgfprogram, fShader);
    finalizeShaderProgram(vgfprogram);
    return vgfprogram;
}

GLuint Utils::createShaderProgram(const char *vertPath, const char *tescPath, const char *tesePath, const char *fragPath)
{
    GLuint vShader = prepareShader(GL_VERTEX_SHADER, vertPath);
    GLuint tcShader = prepareShader(GL_TESS_CONTROL_SHADER, tescPath);
    GLuint teShader = prepareShader(GL_TESS_EVALUATION_SHADER, tesePath);
    GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fragPath);
    GLuint vtfprogram = glCreateProgram();
    glAttachShader(vtfprogram, vShader);
    glAttachShader(vtfprogram, tcShader);
    glAttachShader(vtfprogram, teShader);
    glAttachShader(vtfprogram, fShader);
    finalizeShaderProgram(vtfprogram);
    return vtfprogram;
}

GLuint Utils::createShaderProgram(const char *vertPath, const char *tescPath, const char *tesePath, const char *geomPath, const char *fragPath)
{
    GLuint vShader = prepareShader(GL_VERTEX_SHADER, vertPath);
    GLuint tcShader = prepareShader(GL_TESS_CONTROL_SHADER, tescPath);
    GLuint teShader = prepareShader(GL_TESS_EVALUATION_SHADER, tesePath);
    GLuint gShader = prepareShader(GL_GEOMETRY_SHADER, geomPath);
    GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fragPath);
    GLuint vtgfprogram = glCreateProgram();
    glAttachShader(vtgfprogram, vShader);
    glAttachShader(vtgfprogram, tcShader);
    glAttachShader(vtgfprogram, teShader);
    glAttachShader(vtgfprogram, gShader);
    glAttachShader(vtgfprogram, fShader);
    finalizeShaderProgram(vtgfprogram);
    return vtgfprogram;
}
