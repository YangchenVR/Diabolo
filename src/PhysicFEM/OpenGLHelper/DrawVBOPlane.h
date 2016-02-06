#ifndef _DrawVBOPlane_H_
#define _DrawVBOPlane_H_

#include "../appMacro.h"
#include "vrBase.h"
#include "DrawObjectBase.h"
#include <boost/shared_ptr.hpp>
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			class DrawVBOPlane : public DrawObjectBase
			{
			public:
				DrawVBOPlane(vrFloat, vrFloat, vrInt, vrInt);
				virtual ~DrawVBOPlane(){}
			public:
				virtual void render() const;
			private:
				unsigned int vaoHandle;
				int faces;
			};

			typedef boost::shared_ptr< DrawVBOPlane > DrawVBOPlanePtr;
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR
#endif//_DrawVBOPlane_H_