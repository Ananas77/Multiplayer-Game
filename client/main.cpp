#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <string>

#include "shader.cpp"
#include "camera.cpp"
#include "entities.cpp"
#include "defines.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
string readFile(const char *filePath);
unsigned int loadTexture(const char *filePath, int wrap, int magFilter, GLenum pixelFormat);
void glfw_error_callback(int error, const char *description);
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam);

// settings
unsigned int windowWidth = 1000;
unsigned int windowHeight = 1000;
bool wireframe = false;

int main()
{
    /*
     * Window init
     */

    cout << "Creating Window..." << endl;

    // glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#ifdef _DEBUG
    cout << "Enabled Debug mode." << endl;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    // window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Multiplayer Game", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

#ifdef _DEBUG
    // error logging
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    glEnable(GL_DEPTH_TEST);

    // Wireframe mode
    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    /*
     * shaders, cameras, textures
     */

    cout << "Done!" << endl
         << "Compiling shaders..." << endl;
    // shader
    Shader basicShader("shaders/main.vert", "shaders/main.frag");

    cout << "Done!" << endl
         << "Loading assets..." << endl;

    // camera
    Camera mainCamera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, windowWidth / windowHeight, 45.0f, 0.1f, 100.0f);

    // textures
    unsigned int testTexture = loadTexture("graphics/texture.png", GL_MIRRORED_REPEAT, GL_LINEAR, GL_RGBA);
    unsigned int testSpecular = loadTexture("graphics/specular.png", GL_MIRRORED_REPEAT, GL_LINEAR, GL_RGBA);

    /*
     * Create Entities
     */

    // vertices
    float vertices[] = 
    {
        // positions            //Normals           //texture coords

        //front
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right front
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left front
        0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right front
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // top left front

        //back
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  1.0f, 0.0f,   // bottom right back
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,  0.0f, 0.0f,   // bottom left back
        0.5f, 0.5f, -0.5f,      0.0f, 0.0f, -1.0f,  1.0f, 1.0f,   // top right back
        -0.5f, 0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  0.0f, 1.0f,   // top left back

        //top
        0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // top right front
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // top left front
        0.5f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right back
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left back

        //bottom
        0.5f, -0.5f, 0.5f,      0.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right front
        -0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left front
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f,  1.0f, 1.0f,   // bottom right back
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,  0.0f, 1.0f,   // bottom left back

        //right
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom right front
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // top right front
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // bottom right back
        0.5f, 0.5f, -0.5f,      1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top right back

        //left
        -0.5f, -0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,   // bottom right front
        -0.5f, 0.5f, 0.5f,      -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,   // top right front
        -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // bottom right back
        -0.5f, 0.5f, -0.5f,     -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,   // top right back
    };
    unsigned int numVertices = sizeof(vertices) / sizeof(float);

    // indices
    unsigned int indices[] = 
    {
        // front
        0, 1, 2,
        1, 2, 3,

        // back
        4, 5, 6,
        5, 6, 7,

        // top
        8, 9, 10,
        9, 10, 11,

        // bottom
        12, 13, 14,
        13, 14, 15,

        // right
        16, 17, 18,
        17, 18, 19,

        // left
        20, 21, 22,
        21, 22, 23
    };
    unsigned int numIndices = sizeof(indices) / sizeof(unsigned int);

    Material testMaterial;
    testMaterial.ambient = 0.35f;
    testMaterial.diffuse = 1.0f;
    testMaterial.shininess = 32.0f;
    testMaterial.texture = testTexture;
    testMaterial.specular = testSpecular;

    Spotlight mainLight;
    mainLight.position = glm::vec3(0.0f, 0.0f, -1.0f);
    mainLight.direction = normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    mainLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    mainLight.ambient = 1.0f;
    mainLight.diffuse = 1.0f;
    mainLight.specular = 1.0f;
    mainLight.constant = 1.0f;
    mainLight.linear = 0.09f;
    mainLight.quadratic = 0.032f;
    mainLight.cutOff = glm::cos(glm::radians(12.5f));
    mainLight.outerCutOff = glm::cos(glm::radians(17.5f));

    basicShader.use();
    basicShader.setVec3("light.position", mainLight.position);
    basicShader.setVec3("light.direction", mainLight.direction);
    basicShader.setVec3("light.ambient", mainLight.color * mainLight.ambient);
    basicShader.setVec3("light.diffuse", mainLight.color * mainLight.diffuse);
    basicShader.setVec3("light.specular", mainLight.color * mainLight.specular);
    basicShader.setFloat("light.constant", mainLight.constant);
    basicShader.setFloat("light.linear", mainLight.linear);
    basicShader.setFloat("light.quadratic", mainLight.quadratic);
    basicShader.setFloat("light.cutOff", mainLight.cutOff);
    basicShader.setFloat("light.outerCutOff", mainLight.outerCutOff);

    Entity cube(vertices, indices, numVertices, numIndices, testMaterial, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

    cout << "Done!" << endl;

    /*
     * render loop
     */
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // Clear Window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        cube.render(basicShader, mainCamera);
        cube.rotate(glm::vec3(1.0f, 1.0f, 0.0f) / 3.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // close window
    glfwTerminate();
    return 0;
}

// Input processing
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// reset GL Viewport when window size changes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

// reads file
string readFile(const char *filePath)
{
    string content;
    ifstream fileStream(filePath, ios::in);

    if (!fileStream.is_open())
    {
        cerr << "Could not read file " << filePath << ". File does not exist." << endl;
        return "";
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

// loads texture, so that it is usable for GL
unsigned int loadTexture(const char *filePath, int wrap, int magFilter, GLenum pixelFormat)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);

    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture " << filePath << endl;
    }

    stbi_image_free(data);

    return texture;
}

// GL debug context

void glfw_error_callback(int error, const char *description)
{
    std::cout << "Error code: " << error << std::endl
              << "Description: " << description << std::endl;
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
