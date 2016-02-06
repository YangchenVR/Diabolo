#ifndef _DrawVBOMesh_H_
#define _DrawVBOMesh_H_

#include "../appMacro.h"
#include "vrBase.h"
#include "DrawObjectBase.h"
#include <boost/shared_ptr.hpp>
#include "../VR_Geometry_MeshDataStruct.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			class DrawVBOMesh : public DrawObjectBase
			{
			public:
				DrawVBOMesh(bool reCenterMesh = false, bool loadTc = false, bool genTangents = false);
				~DrawVBOMesh(void){}
			public:
				void loadOBJ(Geometry::MeshDataStructPtr );
				void loadDisplacementOBJ(Geometry::MeshDataStructPtr );
				virtual void render()const;
			protected:
				void generateAveragedNormals(const std::vector< vrGLMVec3 > &, std::vector< vrGLMVec3 > &, const std::vector<vrInt> &);
				void generateTangents( const std::vector< vrGLMVec3 > & , const std::vector< vrGLMVec3 > & , const std::vector< vrInt > & , const std::vector< vrGLMVec2 > & , std::vector< vrGLMVec4 > & tangents);
				void center(std::vector< vrGLMVec3 > & points);
				void updateVBO(const std::vector< vrGLMVec3 > &, const std::vector< vrGLMVec3 > & , const std::vector< vrGLMVec2 > &, const std::vector< vrGLMVec4 > &, const std::vector< vrInt > &);
				void storeVBO(const std::vector< vrGLMVec3 > & ,const std::vector< vrGLMVec3 > & ,const std::vector< vrGLMVec2 > &,	const std::vector< vrGLMVec4 > &,const std::vector< vrInt > &);
			protected:
				vrUnsigned faces;
				vrUnsigned vaoHandle;
				vrUnsigned handle[5];
				bool reCenterMesh, loadTex, genTang;
			private:
				Geometry::MeshDataStructPtr m_objInfo;
				vrInt nElementCount;//3*faces
			};

			typedef boost::shared_ptr< DrawVBOMesh > DrawVBOMeshPtr;
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR

#endif//_DrawVBOMesh_H_

