#ifndef SHADER_H
#define SHADER_H

/*
	MAKE FUNCTIONS INTO INLINES
	CUT GLM DEPENDENCIES
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "util/vec.h"
#include "util/glad/glad.h"
class Shader 
{
public:
	GLuint ID; //ID de cada shader program en particular

	Shader() : ID(0) {};

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		compile(vertexPath, fragmentPath);
	};

	void compile(const GLchar* vertexPath, const GLchar* fragmentPath) //el constructor pide las rutas de los dos shaders a compilar y linkear
	{
		//variables para almacenar el codigo y leer desde archivos
		std::string vShaderCode, fShaderCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//permitir que los objetos de archivo puedan tirar exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try{
			//abrir archivos
			vShaderFile.open(vertexPath);
			//lee los buffer de los archivos en streams
			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			//cerrar los archivos
			vShaderFile.close();
			//convertir los streams en strings
			vShaderCode = vShaderStream.str();
		}
		catch (std::ifstream::failure e){
			std::cout << "ERROR. NO SE LEYO EL CODIGO VS CORRECTAMENTE.\n";
			std::cerr << e.what() << "\n";
		}
		
		try {
			fShaderFile.open(fragmentPath);
			std::stringstream fShaderStream;
			fShaderStream << fShaderFile.rdbuf();
			fShaderFile.close();
			fShaderCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR. NO SE LEYO EL CODIGO FS CORRECTAMENTE.\n";
			std::cerr << e.what() << "\n";
		}
		//obtener las funciones como cadenas de texto de C
		const char* vCodeInC = vShaderCode.c_str();
		const char* fCodeInC = fShaderCode.c_str();
		//compilar los shaders
		unsigned int vBuild, fBuild;
		///compilar vertex shader
		vBuild = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vBuild, 1, &vCodeInC, NULL);
		glCompileShader(vBuild);
		std::cout << "El archivo que se leyó fue: " << vertexPath << ".\n";
		//std::cout << "El código que contenía era:\n" << vCodeInC << "\n";
		checkCompileErrors(vBuild, "VERTEX");
		///compilar fragment shader
		fBuild = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fBuild, 1, &fCodeInC, NULL);
		glCompileShader(fBuild);
		std::cout << "El archivo que se leyó fue: " << fragmentPath << ".\n";
		//std::cout << "El código que contenía era:\n" << fCodeInC << "\n";
		checkCompileErrors(fBuild, "FRAGMENT");
		///crear y compilar el shader program
		ID = glCreateProgram();
		glAttachShader(ID, vBuild);
		glAttachShader(ID, fBuild);
		glLinkProgram(ID);
		checkCompileErrors(ID,"PROGRAM");
		///borrar los shaders porque ya no son necesarios
		glDeleteShader(vBuild);
		glDeleteShader(fBuild);
	}
	inline void use() const //simplemente llama a usar el shaderProgram
	{
		glUseProgram(ID);
	}
	//funciones de utilidad
	//NOTA IMPORTANTE: SOLO DEBEN USARSE CON UNIFORMS QUE ESTÉN DECLARADOS. NO FUNCIONAN CON UNIFORMS NO DECLARADOS NI DECLARAN LOS UNIFORMS QUE NO EXISTAN!!!!!!
	void setBool(const std::string &name, bool value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec4f(const std::string &name, float x, float y, float z, float w){
		glUniform4f(glGetUniformLocation(ID, name.c_str()),x,y,z,w);
	}

	void setVec3f(const std::string &name, float x, float y, float z) const{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec3f(const std::string &name, const vec3 &vec) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()),vec.x(),vec.y(),vec.z());
	}
	
	//meter coordtype para shaders

	void setMat4f(const std::string &name, const float (&matrix)[4][4]) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]); //se señala el puntero de donde inicia la matriz. también funciona value_ptr(matrix)
	}

	void setMat4f(const std::string &name, const mat4x4 &matrix) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix.e[0]); //se señala el puntero de donde inicia la matriz. también funciona value_ptr(matrix)
	}
private:
	void checkCompileErrors(unsigned int programID, const char* argumentType)
	{
		GLint success;
		char infoLog[1024];
		///checar si el programa se linkeó bien si lo que preguntan es por el programa
		if (argumentType == "PROGRAM")
		{
			glGetProgramiv(programID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(programID, 1024, NULL, infoLog);
				std::cout << "ERROR AL COMPILAR EL SHADER PROGRAM.\n" << infoLog << "\n";
			}
		}
		else
		{
			///Checar el log de compilación de los shaders
			glGetShaderiv(programID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(programID, 1024, NULL, infoLog);
				std::cout << "ERROR AL COMPILAR EL " << argumentType << " SHADER.\n" << infoLog << "\n";
			}
		}
	}
	
};

#endif // !SHADER_H
