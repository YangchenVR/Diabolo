#include "FEM_Simulator.h"
#include <boost/lexical_cast.hpp>
#include "VBOScene.h"
#include "vrLog.h"
#include "OpenGLHelper/vrglutils.h"
#include "IGLScene.h"
namespace VR
{
	namespace App
	{
#define TEMPLATE_FEM_Simulator template< int dimension, class RenderFramework>
#define TEMPLATE_FEM_Simulator_Class FEM_Simulator< dimension,RenderFramework >

		TEMPLATE_FEM_Simulator
		bool TEMPLATE_FEM_Simulator_Class::init()
		{
			initFramework(this);

			return true;
		}

		TEMPLATE_FEM_Simulator
			void TEMPLATE_FEM_Simulator_Class::createVBOScene(/*
			bool showFPS, vrFloat g_FOV, vrFloat g_near, vrFloat g_far
			, const vrGLMVec3& _lightPos
			, const vrGLMVec3& _camerPos
			, const vrGLMVec3& _eyeCenter
			, const vrGLMVec3& _zUp
			, int _shadowMapWidth
			, int _shadowMapHeight*/)
		{
#if USE_GLUT
				OpenGL::vrGLUtils::checkForOpenGLError(__FILE__, __LINE__);

				m_ScenePtr = OpenGL::SceneBasePtr(
					new OpenGL::VBOScene(
						  GlobalConf::g_Scene_ShowFPS
						, GlobalConf::g_db_Scene_FOV
						, GlobalConf::g_db_Scene_Near
						, GlobalConf::g_db_Scene_Far
						, make_glm_vec3(GlobalConf::g_vec3_Scene_LightPos)
						, make_glm_vec3(GlobalConf::g_vec3_Scene_CamerPos)
						, make_glm_vec3(GlobalConf::g_vec3_Scene_EyeCenter)
						, make_glm_vec3(GlobalConf::g_vec3_Scene_ZUp)
						, GlobalConf::g_n_Scene_ShadowMapWidth
						, GlobalConf::g_n_Scene_ShadowMapHeight
						)
					);
			////////////////////////////////////////////////////

			OpenGL::vrGLUtils::dumpGLInfo();
			m_ScenePtr->initScene();
#endif//USE_GLUT
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::createIGLScene()
		{
#if USE_IGL
			m_ScenePtr = OpenGL::SceneBasePtr(new OpenGL::IGLScene);
			m_ScenePtr->initScene();

			OpenGL::IGLScenePtr iglScenePtr = boost::dynamic_pointer_cast<OpenGL::IGLScene>(m_ScenePtr);
			RenderFramework* tmpPtr = dynamic_cast<RenderFramework*>(this);
			tmpPtr->setViewerPtr((iglScenePtr->getViewerPtr()));
#endif
		}
		
		TEMPLATE_FEM_Simulator
		bool TEMPLATE_FEM_Simulator_Class::pre_progress()
		{
			return true;
		}

		TEMPLATE_FEM_Simulator
		bool TEMPLATE_FEM_Simulator_Class::run()
		{
			runMainLoop();
			return true;
		}

		TEMPLATE_FEM_Simulator
		bool TEMPLATE_FEM_Simulator_Class::post_progress()
		{
			return true;
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_resize(int width, int height)
		{
			m_ScenePtr->resize(width, height);
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_display(void)
		{			
			m_ScenePtr->render();
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_key(unsigned char key, int x, int y)
		{
#if USE_GLUT
			switch (key)
			{
			case 27:
			{
					   glutLeaveMainLoop();
					   break;
			}
			}
			glutPostRedisplay();
#endif//USE_GLUT
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_idle(void)
		{
#if USE_GLUT
			glutPostRedisplay();
#endif//USE_GLUT
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_mouse(int button, int state, int x, int y)
		{
			m_ScenePtr->mouse(button, state, x, y);
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_motion(int x, int y)
		{
			m_ScenePtr->motion(x, y);
		}

		TEMPLATE_FEM_Simulator
		void TEMPLATE_FEM_Simulator_Class::virtual_shutdown()
		{
			debugLog << "App exit!";
		}

	}//App
}//VR