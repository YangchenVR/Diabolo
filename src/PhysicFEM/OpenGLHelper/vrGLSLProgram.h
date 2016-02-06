#ifndef _vrGLSLProgram_h_
#define _vrGLSLProgram_h_
#include "../appMacro.h"
#include "vrBase.h"
#include "../vrGL.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			namespace GLSLShader {
				enum GLSLShaderType {
					VERTEX, FRAGMENT, GEOMETRY,
					TESS_CONTROL, TESS_EVALUATION
				};
			};

			class vrGLSLProgram
			{
			private:
				int  handle;
				bool linked;
				vrString logString;

				int  getUniformLocation(const char * name);
				bool fileExists(const vrString & fileName);

			public:
				vrGLSLProgram() : handle(0), linked(false) { }

				bool   compileShaderFromFile(const char * fileName, GLSLShader::GLSLShaderType type);
				bool   compileShaderFromString(const vrString & source, GLSLShader::GLSLShaderType type);
				bool   link();
				bool   validate();
				void   use();

				vrString log();

				int    getHandle();
				bool   isLinked();

				void   bindAttribLocation(GLuint location, const char * name);
				void   bindFragDataLocation(GLuint location, const char * name);

				void   setUniform(const char *name, vrFloat x, vrFloat y, vrFloat z);
				void   setUniform(const char *name, const vrGLMVec2 & v);
				void   setUniform(const char *name, const vrGLMVec3 & v);
				void   setUniform(const char *name, const vrGLMVec4 & v);
				void   setUniform(const char *name, const vrGLMMat4 & m);
				void   setUniform(const char *name, const vrGLMMat3 & m);
				void   setUniform(const char *name, vrFloat val);
				void   setUniform(const char *name, vrInt val);
				void   setUniform(const char *name, bool val);

				void   printActiveUniforms();
				void   printActiveAttribs();
			};
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR
#endif//_vrGLSLProgram_h_
