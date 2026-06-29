//=============================================================================================
// OpenGL keretrendszer
//=============================================================================================
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#define FRAMEWORK GLFW
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

//---------------------------
class GPUProgram
{
	//--------------------------
	GLuint shaderProgramId = 0;
	bool waitError = true;

	bool checkShader(unsigned int shader, std::string message)
	{ // shader ford�t�si hib�k kezel�se
		GLint infoLogLength = 0, result = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (!result)
		{
			std::string errorMessage(infoLogLength, '\0');
			glGetShaderInfoLog(shader, infoLogLength, NULL, (GLchar *)errorMessage.data());
			printf("%s! \n Log: \n%s\n", message.c_str(), errorMessage.c_str());
			if (waitError)
				getchar();
			return false;
		}
		return true;
	}

	bool checkLinking(unsigned int program)
	{ // shader szerkeszt�si hib�k kezel�se
		GLint infoLogLength = 0, result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (!result)
		{
			std::string errorMessage(infoLogLength, '\0');
			glGetProgramInfoLog(program, infoLogLength, nullptr, (GLchar *)errorMessage.data());
			printf("Failed to link shader program! \n Log: \n%s\n", errorMessage.c_str());
			if (waitError)
				getchar();
			return false;
		}
		return true;
	}

	int getLocation(const std::string &name)
	{ // uniform v�ltoz� c�m�nek lek�rdez�se
		int location = glGetUniformLocation(shaderProgramId, name.c_str());
		if (location < 0)
			printf("uniform %s cannot be set\n", name.c_str());
		return location;
	}

public:
	GPUProgram() {}
	GPUProgram(const char *const vertexShaderSource, const char *const fragmentShaderSource, const char *const geometryShaderSource = nullptr)
	{
		create(vertexShaderSource, fragmentShaderSource, geometryShaderSource);
	}

	void create(const char *const vertexShaderSource, const char *const fragmentShaderSource, const char *const geometryShaderSource = nullptr)
	{
		// Program l�trehoz�sa a forr�s sztringb�l
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		if (!vertexShader)
		{
			printf("Error in vertex shader creation\n");
			exit(1);
		}
		glShaderSource(vertexShader, 1, (const GLchar **)&vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		if (!checkShader(vertexShader, "Vertex shader error"))
			return;

		// Program l�trehoz�sa a forr�s sztringb�l, ha van geometria �rnyal�
		GLuint geometryShader = 0;
		if (geometryShaderSource != nullptr)
		{
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			if (!geometryShader)
			{
				printf("Error in geometry shader creation\n");
				exit(1);
			}
			glShaderSource(geometryShader, 1, (const GLchar **)&geometryShaderSource, NULL);
			glCompileShader(geometryShader);
			if (!checkShader(geometryShader, "Geometry shader error"))
				return;
		}

		// Program l�trehoz�sa a forr�s sztringb�l
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		if (!fragmentShader)
		{
			printf("Error in fragment shader creation\n");
			exit(1);
		}

		glShaderSource(fragmentShader, 1, (const GLchar **)&fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		if (!checkShader(fragmentShader, "Fragment shader error"))
			return;

		shaderProgramId = glCreateProgram();
		if (!shaderProgramId)
		{
			printf("Error in shader program creation\n");
			exit(-1);
		}
		glAttachShader(shaderProgramId, vertexShader);
		glAttachShader(shaderProgramId, fragmentShader);
		if (geometryShader > 0)
			glAttachShader(shaderProgramId, geometryShader);

		// Connect the fragmentColor to the frame buffer memory
		glBindFragDataLocation(shaderProgramId, 0, "fragmentColor"); // this output goes to the frame buffer memory

		// Szerkeszt�s
		if (!link())
			return;

		// Ez fusson
		glUseProgram(shaderProgramId);
	}

	bool link()
	{
		glLinkProgram(shaderProgramId);
		return checkLinking(shaderProgramId);
	}

	void Use() { glUseProgram(shaderProgramId); } // make this program run

	void setUniform(int i, const std::string &name)
	{
		int location = getLocation(name);
		if (location >= 0)
			glUniform1i(location, i);
	}

	void setUniform(float f, const std::string &name)
	{
		int location = getLocation(name);
		if (location >= 0)
			glUniform1f(location, f);
	}

	void setUniform(const vec2 &v, const std::string &name)
	{
		int location = getLocation(name);
		if (location >= 0)
			glUniform2fv(location, 1, glm::value_ptr(v));
	}

	void setUniform(const vec3 &v, const std::string &name)
	{
		int location = getLocation(name);
		if (location >= 0)
			glUniform3fv(location, 1, glm::value_ptr(v));
	}

	void setUniform(const vec4 &v, const std::string &name)
	{
		int location = getLocation(name);
		if (location >= 0)
			glUniform4fv(location, 1, glm::value_ptr(v));
	}

	void setUniform(const mat4 &mat, const std::string &name)
	{
		int location = getLocation(name);
		if (location >= 0)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	~GPUProgram()
	{
		if (shaderProgramId > 0)
			glDeleteProgram(shaderProgramId);
	}
};

//---------------------------
template <class T>
class Geometry
{
	//---------------------------
	unsigned int vao, vbo; // GPU
protected:
	std::vector<T> vtx; // CPU
public:
	Geometry()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		int nf = (int)(sizeof(T) / sizeof(float));
		if (nf <= 4)
			glVertexAttribPointer(0, nf, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	std::vector<T> &Vtx() { return vtx; }
	void updateGPU()
	{ // CPU -> GPU
		if (vtx.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(T), &vtx[0], GL_DYNAMIC_DRAW);
		}
	}
	void Bind()
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	} // aktiv�l�s
	void Draw(GPUProgram *prog, int type, vec3 color)
	{
		if (vtx.size() > 0)
		{
			prog->setUniform(color, "color");
			glBindVertexArray(vao);
			glDrawArrays(type, 0, (int)vtx.size());
		}
	}
	virtual ~Geometry()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
};

enum MouseButton
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};
enum SpecialKeys
{
	KEY_RIGHT = 262,
	KEY_LEFT = 263,
	KEY_DOWN = 264,
	KEY_UP = 265
};

bool pollKey(int key);
float getElapsedTime();

//---------------------------
class glApp
{
	//---------------------------
public:
	glApp(const char *caption);
	glApp(unsigned int major, unsigned int minor,		 // K�rt OpenGL major.minor verzi�
		  unsigned int winWidth, unsigned int winHeight, // Alkalmaz�i ablak felbont�sa
		  const char *caption);							 // Megfog�cs�k sz�vege
	void refreshScreen();								 // Ablak �rv�nytelen�t�se
	// Esem�nykezel�k
	virtual void onInitialization() {}	  // Inicializ�ci�
	virtual void onDisplay() {}			  // Ablak �rv�nytelen
	virtual void onKeyboard(int key) {}	  // Klaviat�ra gomb lenyom�s
	virtual void onKeyboardUp(int key) {} // Klaviat�ra gomb elenged
	// Eg�r gomb lenyom�s/elenged�s
	virtual void onMousePressed(MouseButton but, int pX, int pY) {}
	virtual void onMouseReleased(MouseButton but, int pX, int pY) {}
	// Eg�r mozgat�s lenyomott gombbal
	virtual void onMouseMotion(int pX, int pY) {}
	// Telik az id�
	virtual void onTimeElapsed(float startTime, float endTime) {}
};
