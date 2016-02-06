#ifndef _AbstractRenderFramework_H_
#define _AbstractRenderFramework_H_

#include "vrConfigureParser.h"
#include "RenderFrameworkInterface.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
			class AbstractRenderFramework
			{
			public:
				AbstractRenderFramework(int argc, char *argv[]){}
				virtual ~AbstractRenderFramework(){}

			public:
				virtual bool initFramework(RenderFrameworkInterface * app_ptr) = 0;
				virtual void runMainLoop() = 0;
			};
		}//OpenGL
	}//App
}//VR
#endif//_AbstractRenderFramework_H_