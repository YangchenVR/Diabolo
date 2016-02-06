#ifndef _IGL_Mesh_Struct_H_
#define _IGL_Mesh_Struct_H_
#include "vrBase.h"

namespace VR
{
	namespace Geometry
	{
		struct IGL_Mesh_Struct
		{
			vrMat m_original_vtx_pos;
			vrMatI m_original_vtx_dofs;
			vrMat m_current_vtx_pos;
			vrMat m_current_vtx_disp;
			vrMatI m_original_face_struct;
			vrVecI m_current_face_color_id;
			vrMat m_current_face_color;
			vrMat m_current_face_color_subspace;
			vrInt m_nVtxSize;
			vrInt m_nFaceSize;
		};
	}//Geometry
}//VR
#endif//_IGL_Mesh_Struct_H_