/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    ShaderProgram.cpp                              oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <Lums/ShaderProgram.hpp>
#include <Lums/Shader.hpp>
#include <iostream>

using namespace lm;

ShaderProgram::ShaderProgram()
{
	_program = glCreateProgram();
}

void
ShaderProgram::attach(const Shader& shader)
{
	const GLuint s = shader.get();

	glAttachShader(_program, s);
	_shaders.push_back(s);
}

void
ShaderProgram::link()
{
    GLint linked = 0;

	glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &linked);
    if (linked)
    {
	   for (auto s : _shaders)
		  glDetachShader(_program, s);
	   _shaders.clear();
    }
    else
    {
    	char* msg;
		GLint logSize;

		std::cout << "Link failed!" << std::endl;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logSize);
		msg = new char[logSize];
		glGetProgramInfoLog(_program, logSize, nullptr, msg);
		puts(msg);
		delete [] msg;
    }
}

void
ShaderProgram::use() const
{
	glUseProgram(_program);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_program);
}
