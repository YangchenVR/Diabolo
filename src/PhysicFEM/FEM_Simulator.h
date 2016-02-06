#ifndef _FEM_Simulator_H_
#define _FEM_Simulator_H_

#include "appMacro.h"
#include "AbstractApplication.h"
#include "glutFramework.h"
#include "iglFramework.h"
#include "RenderFrameworkInterface.h"
#include "vrConfigureParser.h"
#include "SceneBase.h"


namespace VR
{
	namespace App
	{
		template< int dimension , class RenderFramework>
		class FEM_Simulator : public RenderFramework, public OpenGL::RenderFrameworkInterface, public AbstractApplication<dimension>
		{
		public:
			FEM_Simulator(int argc, char *argv[])
				:RenderFramework(argc, argv), AbstractApplication<dimension>()
			{}
			virtual ~FEM_Simulator(){}

		public:
			virtual bool init();
			virtual bool pre_progress();
			virtual bool run();
			virtual bool post_progress();
		public:
			void createVBOScene(/*bool showFPS, vrFloat g_FOV, vrFloat g_near, vrFloat g_far
				, const vrGLMVec3& _lightPos
				, const vrGLMVec3& _camerPos
				, const vrGLMVec3& _eyeCenter
				, const vrGLMVec3& _zUp
				, int _shadowMapWidth
				, int _shadowMapHeight*/);
			void createIGLScene();
		protected:
			virtual void virtual_resize(int width, int height);
			virtual void virtual_display(void);
			virtual void virtual_key(unsigned char key, int x, int y);
			virtual void virtual_idle(void);
			virtual void virtual_mouse(int button, int state, int x, int y);
			virtual void virtual_motion(int x, int y);
			virtual void virtual_shutdown();

		private:
			OpenGL::SceneBasePtr m_ScenePtr;
		};
#if USE_GLUT
		template class FEM_Simulator<3/*dimension*/, OpenGL::glutFramework >;
#else
		template class FEM_Simulator<3/*dimension*/, OpenGL::iglFramework >;
#endif // USE_GLUT
	}//App
}//VR


#endif//_FEM_Simulator_H_