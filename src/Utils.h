#ifndef UTILS_H
#define UTILS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/euler_angles.hpp>
#include "Utils.h"
using namespace std;

class Utils
{
private:
    static std::string readShaderFile(const char *filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static GLuint prepareShader(int shaderTYPE, const char *shaderPath);
    static int finalizeShaderProgram(GLuint sprogram);
    void setupDebugPrint();
    void printOpenGLVersion();

public:
    Utils();
    static bool checkOpenGLError();
    static GLuint createShaderProgram(const char *vertPath, const char *fragPath);
    static GLuint createShaderProgram(const char *vertPath, const char *geomPath, const char *fragPath);
    static GLuint createShaderProgram(const char *vertPath, const char *tescPath, const char *tesePath, const char *fragPath);
    static GLuint createShaderProgram(const char *vertPath, const char *tescPath, const char *tesePath, const char *geomPath, const char *fragPath);
};
#endif // UTILS_H