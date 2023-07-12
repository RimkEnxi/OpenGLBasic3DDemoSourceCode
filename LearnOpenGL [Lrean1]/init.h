#pragma once
///	本源码由Enxi进行编写
///	提供于广大人员学习基础的OpenGL3D知识
/// QQ：3598434596
#include<glad/glad.h>
#include<glfw32/glfw3.h>
#include<windows.h>
#include<iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <conio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include "Shader.h"
#include "headinclude.h"

#pragma comment(lib,"glfw3.lib")

using namespace std;

bool hiddenBUtton = true;

void testprintf(const char* mstring)
{
	printf(mstring);
}

void initSpeed()
{
	cameraSpeed = 0.05f; //speed
	leftAndright = 4.5f;
}

void initImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_window = true;
	bool checkbox1 = false;
	bool checkbox2 = false;
	clear_color = ImVec4(255.0f, 255.0f, 255.0f, 1.00f);
}

string stringToutf8(const string& str)
{
	int newLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[newLen + 1];
	memset(pwBuf, 0, newLen * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, newLen);

	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, pwBuf, newLen, pBuf, nLen, NULL, NULL);

	string ret = pBuf;
	delete[] pwBuf;
	delete[] pBuf;

	return ret;
}

int initOpengl(int width, int height, const char* title,bool DrawTriangles,bool outMaximumVertexAttribute)
{
	system("title OpenGL后台输出");
	initSpeed();
	if (!glfwInit())
	{
		cout << "Failed to initialize OpenGL..." << endl << endl;
	}
	else {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		cout << "Succeeded to initialize OpenGL!" << endl << endl;
	}
	
	if (!width)
	{
		width = 800;
		cout << "Info::Default numeric value for the window width: [ " << width << "]" << endl << endl;
	}
	if (!height) {
		height = 600;
		cout << "Info::Default numeric value for the window height: [" << height << "]" << endl << endl;
	}
	if (!title)
	{
		title = "Default Window Title";
		cout << "Info::Default name for the window name: [" << title << "]" << endl << endl;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		cout << "Failed to create window!" << endl << endl;;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl << endl;;
		return -1;
	}

	Shader ourShader("texture.vs", "texture.fs");

	if (outMaximumVertexAttribute)
	{
		checkMaximumVertexAttribute();
	}
	if (DrawTriangles)
	{
		initTriangles();
	}

	ourShader.Use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	/*
	glm::mat4 trans;
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	unsigned int transformLocation = glGetUniformLocation(ourShader.ID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
	*/

	//initMatrixTransform();

	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glEnable(GL_DEPTH_TEST);
	cout << "Current number of VAO:   " << VAO << "\n" << endl;
	cout << "Current number of VBO:   " << VBO << "\n" << endl;
	cout << "Current number of EBO:   " << EBO << "\n" << endl;

	initImgui(window);
	
	int blood;
	int hunger;

	while (!glfwWindowShouldClose(window))
	{
		//ImGui::drawtext
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame; //Time interval
		lastFrame = currentFrame; //The last frame of the game

		processInput(window);

		glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		{
			static int counter = 0;

			ImGui::Begin(u8"欢迎使用Window console");

			ImGui::SliderFloat(u8"(前后移动速度)", &cameraSpeed, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat(u8"(左右移动速度)", &leftAndright, 0.0f,10.0f);
			ImGui::ColorEdit3(u8"窗口背景颜色", (float*)&clear_color); // Edit 3 floats representing a color

			/*
			if (ImGui::Checkbox(u8"锁定人物移速", &checkCameraSpeed))
			{
				if (checkbox1 == true)
				{

				}
			}
			*/

			if (ImGui::Button(u8"一键恢复原本背景颜色"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				clear_color = ImVec4(255.0f, 255.0f, 255.0f, 1.00f);
			ImGui::SameLine();

			ImGui::Text(u8"窗口刷新平均值：%.3f ms/帧(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin(u8"[额外窗口(测试按钮)]", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button(u8"关闭Window console"))
				show_another_window = false;
			ImGui::End();
		}

		ImGui::Render();
		int display_w, display_h;
		//glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x* clear_color.w, clear_color.y* clear_color.w, clear_color.z* clear_color.w, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (DrawTriangles)
		{
			//GLfloat timeNumericvalue = glfwGetTime();
			//GLfloat greenNumericvalue = (sin(timeNumericvalue) / 2) + 0.5;
			//GLint vertexColorlocation = glGetUniformLocation(program, "ourColor");
			//glUniform4f(vertexColorlocation, greenNumericvalue, greenNumericvalue, greenNumericvalue, 1.0f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			ourShader.Use();

			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
			ourShader.setMat4("projection", projection);

			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			ourShader.setMat4("view", view);

			glBindVertexArray(VAO);
			for (unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void initTriangles()
///GLSL数据类型：int、float、double、uint、bool
///GLSL向量类型：
///	vecn(包含n个float分量的默认向量)
///	bvecn(包含n个bool分量的向量)
/// ivecn(包含n个int分量的向量)
/// uvecn(包含n个unsigned int分量的向量)
/// dvecn(包含n个double分量的向量)
/// 
/// 对颜色使用rgba，对纹理使用stpq
/// 
/// Uniform(从CPU中的应用向GPU中的着色器发送数据的方式)(Uniform是全局变量)
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	/*
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	*/

	/*
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShadererror("vertex", vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShadererror("fragment", fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	checkShadererror("program", program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	*/

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	initTextureCoords();
}
void initTextureCoords()
///	top left:(0,1)
///	bottom left:(0,0)
/// middle:(0.5,1)
///	top right:(1,1)
/// bottom right:(1,0)
/// 
/// How textures wrap：
/// GL_REAPEAT(对纹理的默认行为，重复纹理图像)
/// GL_MIRRORED(和GL_REAPEAT一样，但每次重复图片都是镜像放置)
/// GL_CLAMP_TO_EDGE(纹理坐标会被约束在0-1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果)
/// GL_CLAMP_TO_BORDER(超出的部分为用户指定的边缘颜色)
/// 
/// Texture filtering：
///	GL_NEAREST(临近过滤，默认过滤方式。当设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素)
/// GL_LINEAR(线性过滤。它会鲫鱼纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色)
/// 
/// Multi-level faded textures
/// GL_NEAREST_MIPMAP_NEAREST(使用最邻近的多级渐变纹理来匹配像素大小，并使用邻近插值进行纹理采样)
/// GL_LINEAR_MIPMAP_NEAREST(使用最邻近的多级渐变纹理级别，并使用线性插值来进行纹理采样)
/// GL_NEAREST_MIPMAP_LINEAR(在两个最匹配像素大小的多级渐变纹理之间进行线性插值，使用邻近插值进行采样)
/// GL_LINEAR_MIPMAP_LINEEAR(在两个邻近的多级渐变纹理之间使用线性插值，并使用线性插值进行采样)
{
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannles; //Width,Height,ColorChannles
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannles, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	/*
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannles, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	*/
}

void checkShadererror(const char* mstring, unsigned int shader)
{
	int success;
	GLchar infoLog[512];

	if (mstring != "program")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::" << mstring << "::COMPILATION_FAILED\n" << infoLog << endl << endl;
		}
		else
		{
			cout << "SUCCESSFULLY::SHADER::" << mstring << "::COMPILATION_SUCCESSFULLY!\n" << endl << endl;
		}
	}else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog <<endl << endl;
		}
		else
		{
			cout << "SUCCESSFULLY::PROGRAM::LINK_SUCCESSFULLY!!\n" << endl << endl;
		}
	}
}
bool takebool(bool value)
{
	return value == true ? false : true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	int escape = glfwGetKey(window, GLFW_KEY_ESCAPE);
	int keyW = glfwGetKey(window, GLFW_KEY_W);
	int keyS = glfwGetKey(window, GLFW_KEY_S);
	int keyA = glfwGetKey(window, GLFW_KEY_A);
	int keyD = glfwGetKey(window, GLFW_KEY_D);
	int keyQ = glfwGetKey(window, GLFW_KEY_Q);
	int ch;

	if (escape == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		cout << "Set the hotkey successfully! Hotkey::[Escape]  " << endl << endl;
		cout << "The destruction window is complete... " << endl << endl;
	}
	if (keyW == GLFW_PRESS)
	{
		cameraPos -= cameraPos * (float)cameraSpeed;
		cout << "Current key: W"  << endl << endl;
		
	}
	if (keyS == GLFW_PRESS)
	{
		cameraPos += cameraPos * (float)cameraSpeed;
		cout << "Current key: S" << endl << endl;
	}
	if (keyA == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * (float)leftAndright);
		cout << "Current key: A" << endl << endl;
	}
	if (keyD == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * (float)leftAndright);
		cout << "Current key: D" << endl << endl;
	}
	if (keyQ == GLFW_PRESS)
	{
		if (hiddenBUtton == false) hiddenBUtton = takebool(hiddenBUtton);
		else hiddenBUtton = takebool(hiddenBUtton);
		if (hiddenBUtton)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Sleep(250);
	}
	
}

void mouse_callback(GLFWwindow* window, double xposln, double yposln)
{
	float xpos = static_cast<float>(xposln);
	float ypos = static_cast<float>(yposln);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	firstMouse = false;

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void checkMaximumVertexAttribute()
{
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported:  " << nrAttributes << endl << endl;
}

bool S(bool bools)
{
	if (bools == true)
	{
		bools = false;
	}
	else
	{
		bools = true;
	}
	return bools;
}