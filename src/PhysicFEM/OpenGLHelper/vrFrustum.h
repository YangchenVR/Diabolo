#ifndef _vrFrustum_H_
#define _vrFrustum_H_

#include "../appMacro.h"
#include "vrBase.h"
#include "../vrGL.h"
#include <boost/shared_ptr.hpp>

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			namespace Projection {
				enum ProjType {
					PERSPECTIVE, ORTHO
				};
			}

			
			class vrFrustum
			{
			private:
				Projection::ProjType type;

				vrGLMVec3 origin;
				vrGLMVec3 at;
				vrGLMVec3 up;

				vrGLMVec3::value_type mNear;
				vrGLMVec3::value_type mFar;
				vrGLMVec3::value_type xmin, xmax, ymin, ymax;
				vrGLMVec3::value_type fovy, ar;

				vrGLMMat4 view, proj;
				GLuint handle[2];

			public:
				vrFrustum(Projection::ProjType type);

				void orient(const vrGLMVec3 &pos, const vrGLMVec3& a, const vrGLMVec3& u);
				void setOrthoBounds(vrFloat xmin, vrFloat xmax, vrFloat ymin, vrFloat ymax,
					vrFloat, vrFloat);
				void setPerspective(vrFloat, vrFloat, vrFloat, vrFloat);
				void enclose(const vrFrustum &);

				vrGLMMat4 getViewMatrix() const;
				vrGLMMat4 getProjectionMatrix() const;
				vrGLMVec3 getOrigin() const;
				vrGLMVec3 getCenter() const;

				void printInfo() const;
				void render() const;
			};

			typedef boost::shared_ptr< vrFrustum > vrFrustumPtr;
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR
#endif//_vrFrustum_H_