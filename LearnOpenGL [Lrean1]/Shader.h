///	本源码由Enxi进行编写
///	提供于广大人员学习基础的OpenGL3D知识
/// QQ：3598434596

#ifndef  SHADER_H
#define SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
#include <glm/glm/glm.hpp>

//#include<glew/glew.h>

using namespace std;

class Shader
{
public:
	unsigned int ID;
	const char* xvShaderSourceCode =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexCoord;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(aPos,1.0f);\n"
		"	TexCoord = vec2(aTexCoord.x,aTexCoord.y);\n"
		"}\0";

	const char* xfShaderSourceCode =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"uniform sampler2D texture2;\n"
		"void main()\n"
		"{\n"
		"	FragColor = mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.2);\n" //texture(texture1,TexCoord) * vec4(ourColor,1.0)
		"}\n\0";
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		vShaderFile.exceptions(ifstream::failbit || ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit || ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderstream, fShaderstream;

			fShaderstream << vShaderFile.rdbuf();
			vShaderstream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderstream.str();
			fragmentCode = fShaderstream.str();

		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//Debug the output of the text;
		//cout << "---------------------\n\nVertex Shader Code : \n" << vShaderCode << "\n\n---------------------" << endl << endl;;
		//cout << "---------------------\n\nfragment Shader Code: \n" << fShaderCode << "\n\n---------------------"  << endl << endl;;

		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &xvShaderSourceCode, NULL);
		glCompileShader(vertex);
		checkShadererrors("vertex", vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &xfShaderSourceCode, NULL);
		glCompileShader(fragment);
		checkShadererrors("fragment", fragment);

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkShadererrors("program", ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Use()
	{
		glUseProgram(ID);
	}
	void setBool(const string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	void checkShadererrors(const char* mstring, unsigned int shader)
	{
		int success;
		GLchar infoLog[1024];

		if (mstring != "program")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER::" << mstring << "::COMPILATION_FAILED\n" << infoLog << endl << endl;
			}
			else
			{
				cout << "SUCCESSFULLY::SHADER::" << mstring << "::COMPILATION_SUCCESSFULLY!\n" << endl << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << endl << endl;
			}
			else
			{
				cout << "SUCCESSFULLY::PROGRAM::LINK_SUCCESSFULLY!!\n" << endl << endl;
			}
		}
	}
};

#endif

