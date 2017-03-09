// Local Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"          // to print vectors and matrices

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

#define RED 1.0f, 0.0f, 0.0f            // define macros for convenience
#define BLUE 0.0f, 0.0f, 1.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define ORANGE 1.0F, 0.6F, 0.0F
#define PURPLE 1.0f, 0.0f, 1.0f
#define YELLOW 1.0f, 1.0f, 0.0f

// device coordinates
#define LEFT -0.5f
#define TOP 0.5f
#define RIGHT 0.5f
#define BOTTOM -0.5f
#define MIDDLE 0.0f
#define FRONT 0.5f
#define BACK -0.5f

const GLchar* vertexSource =
"#version 330 core\n"             // glsl version
"in vec3 position;"
"in vec3 normal;"
"out vec3 Normal;"
"out vec3 FragPos;"
"uniform mat4 model;"
"uniform mat4 view;"
"void main()"
"{"
"    Normal = mat3(transpose(inverse(model))) * normal;"
"    FragPos = vec3(model * vec4(position, 1.0f));"
"    gl_Position = view * model * vec4(position, 1.0);"   // gl_Position is special variable for final position
"}";                                                    // must be in homogeneous coordinates -- put in 0 for z and 1 for w
// multiply by model matrix to transform
const GLchar* fragmentSource =
"#version 330 core\n"
"in vec3 Normal;"
"in vec3 FragPos;"

"uniform vec3 lightPos;"
"uniform vec3 viewPos;"
"uniform vec3 lightColor;"
"uniform vec3 objectColor;"

"out vec4 outColor;"
"void main()"
"{"
"    vec3 norm = normalize(Normal);"
"    vec3 lightDir = normalize(lightPos - FragPos);"
"    float diff = max(dot(norm, lightDir), 0.0);"
"    vec3 diffuse = diff * lightColor;"

"    vec3 result = (diffuse) * objectColor;"
"    outColor = vec4(result, 1.0);"
"}";

// vertex data
GLfloat vertices[] = {
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
  
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// callback for keyboard input
// move camera when arrow keys are pressed, rotate it when arrow keys are pressed with control
void key_callback(GLFWwindow* mWindow, int key, int scancode, int action, int mods)
{
  std::cout << (mods == GLFW_MOD_SHIFT) << std::endl;
  if (key == GLFW_KEY_X && action == GLFW_PRESS) glfwSetWindowShouldClose(mWindow, true);
  if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
    if (mods == GLFW_MOD_SHIFT) std::cout << "rotating camera left\n"; // rotate camera
    else std::cout << "moving camera left\n";  // move camera
    // etc.
  }
    
}

int main(int argc, char * argv[]) {
  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  auto mWindow = glfwCreateWindow(200, 200, "CISC3620", nullptr, nullptr);
  // Check for Valid Context
  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }
  
  // callbacks
  glfwSetKeyCallback(mWindow, key_callback);
  
  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
  
  glEnable(GL_DEPTH_TEST);
  
  // Create Vertex Array Object: this will store all the information about the vertex data that we are about to specify
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Create a Vertex Buffer Object and copy the vertex data to it
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  // Create and compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  // Create and compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  // Link the vertex and fragment shader into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  // Specify the layout of the vertex data
  // position
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);  // attribute location, # values, value type, normalize?, stride, offset
  // normal
  GLint normAttrib = glGetAttribLocation(shaderProgram, "normal");
  glEnableVertexAttribArray(normAttrib);
  glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  
  // lighting uniforms
  glm::vec3 eyePos(0.0f, 0.0f, 3.0f);
  glm::vec3 lightPos(-0.5f, 0.5f, 3.0f);
  GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
  GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
  GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
  GLint eyePosLoc = glGetUniformLocation(shaderProgram, "eyePos");
  glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.3f);
  glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
  glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
  glUniform3f(eyePosLoc, eyePos.x, eyePos.y, eyePos.z);
  
  // model matrix
  GLint modelTransform = glGetUniformLocation(shaderProgram, "model");
  glm::mat4 rotate_model = glm::rotate(glm::mat4(1.0f), 15.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  
  // view matrix
  glm::mat4 ortho_model = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  GLint viewTransform = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewTransform, 1, GL_FALSE, glm::value_ptr(ortho_model));
	
  // Rendering Loop
  float r = 0.1f;
  while (glfwWindowShouldClose(mWindow) == false) {
    
    // Background Fill Color
    glClearColor(0.9f, 0.9f, 0.9f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 model = glm::rotate(rotate_model, r+=0.01, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(model));

    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));
    
    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }   glfwTerminate();
  
  // clean up
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  
  return EXIT_SUCCESS;
}
