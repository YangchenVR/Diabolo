#ifndef _vrglutils_h_
#define _vrglutils_h_
#include "../appMacro.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			class vrGLUtils
			{
			public:
				vrGLUtils(){}

				static int checkForOpenGLError(const char *, int);
				static void dumpGLInfo(bool dumpExtensions = false);
			};
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR
#endif//_vrglutils_h_