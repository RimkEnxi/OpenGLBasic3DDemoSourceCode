#pragma once
#include "init.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShadererror(const char* mstring, unsigned int shader);
void initTriangles();
void checkMaximumVertexAttribute();
void initTextureCoords();
void mouse_callback(GLFWwindow* window, double xposln, double yposln);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void initImgui(GLFWwindow* window);
bool S(bool bools);
string stringToutf8(const string& str);
//void initTransformMatrix();

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int program;
unsigned int VAO, VBO, EBO;
unsigned int texture1, texture2;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0f;
float fov = 45.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

static float up = 0.0f;
static float cameraSpeed;
static float leftAndright;
static float cameraSpeedst;

bool firstMouse = true;
bool checkCameraSpeed;
bool show_demo_window;
bool show_another_window;

bool checkbox1 = true;
bool checkbox1_1 = true;


const char* glsl_version = "#version 330";

int MouseHiddel = 1;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

ImVec4 clear_color;
