#ifndef _RenderFrameworkInterface_h_
#define _RenderFrameworkInterface_h_

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
			class RenderFrameworkInterface
			{
			public:
				RenderFrameworkInterface(){}
				virtual ~RenderFrameworkInterface(){}

			public:
				virtual void virtual_resize(int width, int height) = 0;
				virtual void virtual_display(void) = 0;
				virtual void virtual_key(unsigned char key, int x, int y) = 0;
				virtual void virtual_idle(void) = 0;
				virtual void virtual_mouse(int button, int state, int x, int y) = 0;
				virtual void virtual_motion(int x, int y) = 0;
				virtual void virtual_shutdown() = 0;

			};
		}//OpenGL
	}//App
}//VR
//
#endif//_RenderFrameworkInterface_h_