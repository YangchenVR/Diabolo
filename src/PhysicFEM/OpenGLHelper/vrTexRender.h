#ifndef _vrTexRender_h_
#define _vrTexRender_h_

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
			class vrTexRender
			{
			public:
				vrTexRender();
				~vrTexRender();
			public:
				void BuildFont(/*GLFWwindow * window*/);
				void KillFont();// Delete The Font List
				void glPrint(const char *text);
			private:
				unsigned	base;				// Base Display List For The Font Set
			public:
				static char fps[256];
			};
		}//OpenGL
	}//App
}//VR

#endif//_vrTexRender_h_