#ifndef _glutFramework_h_
#define _glutFramework_h_

#include "appMacro.h"
#include "AbstractRenderFramework.h"
#include "RenderFrameworkInterface.h"
#include "vrGL.h"
#include "vrGlobalConf.h"

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			class glutFramework : public AbstractRenderFramework
			{
			public://configure data

			public:
				glutFramework(int argc, char *argv[])
					:m_argc(argc), m_argv((char**)argv), AbstractRenderFramework(argc, argv)
				{}
				virtual ~glutFramework(){}

			protected://framework interface
				bool initFramework(RenderFrameworkInterface * app_ptr)
				{
					m_app_ptr = app_ptr;
					const int windowWidth = GlobalConf::g_n_Scene_windowWidth;
					const int windowHeight = GlobalConf::g_n_Scene_windowHeight;
					//init glut
					glutInit(&m_argc, m_argv);

					glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
					glutInitContextVersion(4, 0);
					glutInitContextProfile(GLUT_CORE_PROFILE);

					glutInitWindowSize(windowWidth, windowHeight);
					glutInitWindowPosition(0, 0);
					glutCreateWindow(vrAppTitle);

					ogl_LoadFunctions();

					glutReshapeFunc(static_resize);
					glutDisplayFunc(static_display);
					glutKeyboardFunc(static_key);
					glutIdleFunc(static_idle);
					glutMouseFunc(static_mouse);
					glutMotionFunc(static_motion);
					glutCloseFunc(static_shutdown);

					return true;
				}

				void runMainLoop()
				{
					glutMainLoop();
				}

			public://method for glut
				static void static_resize(int width, int height)
				{
					m_app_ptr->virtual_resize(width, height);
				}

				static void static_display(void)
				{
					m_app_ptr->virtual_display();
				}

				static void static_key(unsigned char key, int x, int y)
				{
					m_app_ptr->virtual_key(key, x, y);
				}

				static void static_idle(void)
				{
					m_app_ptr->virtual_idle();
				}

				static void static_mouse(int button, int state, int x, int y)
				{
					m_app_ptr->virtual_mouse(button, state, x, y);
				}

				static void static_motion(int x, int y)
				{
					m_app_ptr->virtual_motion(x, y);
				}

				static void static_shutdown()
				{
					m_app_ptr->virtual_shutdown();
				}


			private:
				int m_argc;
				char** m_argv;
				static RenderFrameworkInterface * m_app_ptr;
			};
#endif // use_glut
		}//OpenGL
	}//namespace App
}//namespace VR

#endif//_glutFramework_h_