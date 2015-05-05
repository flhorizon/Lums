/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Shader.cpp                                     oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>

#include <string>
#include <fstream>
#include <Lums/Shader.hpp>
#include <Lums/OperatingSystem.hpp>

using namespace lm;

Shader::Shader(const char* path, Type type, bool resource)
{
	GLenum shaderType;
	const std::string filename = resource ? resourcePath() + '/' + path : std::string(path);
	std::ifstream stream(filename.c_str());

	if (!stream.good())
		std::cout << "Not found: " << filename << std::endl;

	if (type == Type::Vertex)
		shaderType = GL_VERTEX_SHADER;
	else
		shaderType = GL_FRAGMENT_SHADER;
	stream.seekg(0, stream.end);
	size_t size = stream.tellg();
	stream.seekg(0, stream.beg);
	char* f = new char[size + 1];
	stream.read(f, size);
	f[size] = 0;
	load(f, shaderType);
	delete [] f;
}

const Shader&
Shader::fragment()
{
	static Shader s;

	if (!s._shader)
	{
		static const char* str = ""
            "#version 150\n"
            "\n"
            "in vec2 lmTexCoord;\n"
			"uniform sampler2D lmSampler;\n"
            "out vec4 lmFragColor;\n"
            "\n"
			"void main() {\n"
			"  //lmFragColor = texture(lmSampler, lmTexCoord.xy);\n"
            "  lmFragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
			"}\n"
		"";
		s.load(str, GL_FRAGMENT_SHADER);
	}
	return s;
}

const Shader&
Shader::vertex()
{
	static Shader s;

	if (!s._shader)
	{
		static const char* str = ""
            "#version 150\n"
            "\n"
            "in vec3 vert;\n"
            "\n"
            "void main() {\n"
            "  gl_Position = vec4(vert, 1.0);\n"
			"}\n"
		"";
		s.load(str, GL_VERTEX_SHADER);
	}
	return s;
}

Shader::~Shader()
{
	glDeleteShader(_shader);
}

// Private

Shader::Shader()
: _shader(0)
{

}

void
Shader::load(const char* str, GLenum type)
{
	_shader = glCreateShader(type);
	glShaderSource(_shader, 1, &str, nullptr);
	glCompileShader(_shader);
	GLint success;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char* msg;
		GLint logSize;

		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &logSize);
		msg = new char[logSize];
		glGetShaderInfoLog(_shader, logSize, nullptr, msg);
		puts(str);
		puts(msg);
		delete [] msg;
	}
}