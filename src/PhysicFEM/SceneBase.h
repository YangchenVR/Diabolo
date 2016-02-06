#ifndef _vrSceneBase_h_
#define _vrSceneBase_h_
#include "vrBase.h"
#include <boost/shared_ptr.hpp>
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
			class SceneBase
			{
			public:
				/**
				Load textures, initialize shaders, etc.
				*/
				virtual void initScene() = 0;

				/**
				This is called prior to every frame.  Use this
				to update your animation.
				*/
				virtual void update(vrFloat ) = 0;

				/**
				Draw your scene.
				*/
				virtual void render() = 0;

				/**
				Called when screen is resized
				*/
				virtual void resize(int, int) = 0;

				virtual void mouse(int button, int state, int x, int y) = 0;

				virtual void motion(int x, int y) = 0;
			};

			typedef boost::shared_ptr< SceneBase > SceneBasePtr;
		}//OpenGL
	}//App
}//VR
#endif//_vrSceneBase_h_