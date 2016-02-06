#ifndef _VBOScene_H_
#define _VBOScene_H_
#include "appMacro.h"
#include "vrBase.h"
#include "SceneBase.h"
#include "OpenGLHelper/vrTexRender.h"
#include "OpenGLHelper/vrGLSLProgram.h"
#include "OpenGLHelper/vrFrustum.h"

#include "OpenGLHelper/DrawVBOPlane.h"
#include "OpenGLHelper/DrawVBOMesh.h"
#include "OpenGLHelper/DrawVBOLineSet.h"
#include "vrRotation.h"
#include "OpenGLHelper/vrFPS.h"
#include "OpenGLHelper/vrGLSLFont.h"


#define SHOW_SHADOWMAP (1)
#if SHOW_SHADOWMAP
#define PREFIX "Material."
#else
#define PREFIX ""
#endif
#define SHOWFPS (1)
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			class VBOScene : public SceneBase
			{
			public:
				VBOScene(bool showFPS, vrFloat g_FOV, vrFloat g_near, vrFloat g_far
					, const vrGLMVec3& _lightPos
					, const vrGLMVec3& _camerPos
					, const vrGLMVec3& _eyeCenter
					, const vrGLMVec3& _zUp
#if SHOW_SHADOWMAP
					, int _shadowMapWidth
					, int _shadowMapHeight
#endif
					);
				virtual ~VBOScene(void);

			public:
				void initScene();
				void update(vrFloat t);
				void render();
				void resize(int, int);
				virtual void mouse(int button, int state, int x, int y);
				void motion(int x, int y);

			private:
				VR::ConfigureParser::vrPropertyMap m_confMap;

				vrVec4 m_bkg_color;
				bool m_showFPS;
				vrTexRender m_TexRender;
				vrGLSLProgram prog;
				vrGLMMat4 model;
				vrGLMMat4 view;
				vrGLMMat4 projection;

				const vrGLMVec3 lightPos;
				const vrGLMVec3 eyeCenter;
				const vrGLMVec3 zUp;
				const vrGLMVec3 camerPos;

				vrGLMVec3 m_planeColor;
				vrGLMVec3 m_modelColor;
				vrGLMVec3 m_modelColor4Ka;

				void setMatrices();
				void compileAndLinkShader();

				DrawVBOPlanePtr m_scene_plane;
				DrawVBOMeshPtr m_scene_ObjMesh;
				DrawVBOLineSetPtr m_scene_OctreeGrid;

				vrInt width, height;
				vrFloat angle;
				vrFloat aspect;

				void showPlane();
				void showMesh();

				const vrFloat FVOY;// (60.f)
				const vrFloat MyNear;// (0.3f)
				const vrFloat MyFar;//  (100.f)

				//For mouse interactive
				VR::Interactive::vrBallController m_trackball;

				GLuint m_depthTex;

#if SHOW_SHADOWMAP
				//For ShadowMap + PCF
				GLuint shadowFBO, pass1Index, pass2Index;
				const vrInt shadowMapWidth;
				const vrInt shadowMapHeight;
				vrGLMMat4 lightPV;
				vrGLMMat4 shadowBias;
				vrFrustumPtr lightFrustum;

				void setupFBO();
				void drawScene();
				void spitOutDepthBuffer();
#endif

#if SHOWFPS
				OpenGL::vrFPS timer;
				void computeFPS();
				vrGLSLFont m_MyGLSLFont;
#endif
			};
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR
#endif//_VBOScene_H_