#ifndef _IGLScene_H_
#define _IGLScene_H_
#include "vrBase.h"
#include "SceneBase.h"
#include "vrIGL.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_IGL
			class IGLScene : public SceneBase
			{
			public:
				virtual void initScene()
				{
					m_iglViewerPtr = iglViewerPtr(new iglViewer);
				}

				virtual void update(vrFloat){}
				virtual void render(){}
				virtual void resize(int, int){}
				virtual void mouse(int button, int state, int x, int y){}
				virtual void motion(int x, int y){}

				iglViewerPtr getViewerPtr()
				{
					return m_iglViewerPtr;
				}
			private:
				iglViewerPtr m_iglViewerPtr;
				
			};
			typedef boost::shared_ptr<IGLScene> IGLScenePtr;
#endif//USE_IGL
		}//OpenGL
	}//App
}//VR
#endif//_IGLScene_H_