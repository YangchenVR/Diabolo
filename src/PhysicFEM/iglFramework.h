#ifndef _iglFramework_h_
#define _iglFramework_h_


#include "vrIGL.h"
//#include "AbstractApplication.h"
#include "AbstractRenderFramework.h"
#include "RenderFrameworkInterface.h"

#include "vrGlobalConf.h"
#include "IGL_Mesh_Struct.h"

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_IGL

			class iglFramework : public AbstractRenderFramework
			{
			public://configure data

			public:
				iglFramework(int argc, char *argv[])
					:m_argc(argc), m_argv((char**)argv), AbstractRenderFramework(argc, argv)
				{}
				virtual ~iglFramework(){}

				void setViewerPtr(iglViewerPtr ptr)
				{
					m_iglViewerPtr = ptr;
				}

			protected://framework interface

				bool initFramework(RenderFrameworkInterface * app_ptr)
				{
					vrASSERT(m_iglViewerPtr);
					m_app_ptr = app_ptr;
					const int windowWidth = GlobalConf::g_n_Scene_windowWidth;
					const int windowHeight = GlobalConf::g_n_Scene_windowHeight;
					//init glut
					
					VR::Geometry::IGL_Mesh_Struct refMesh;


					//InitProcess(refMesh.m_original_vtx_pos, refMesh.m_original_face_struct, refMesh.m_original_vtx_dofs, refMesh.m_current_face_color_id);

					igl::readOBJ(vrCStr(GlobalConf::g_str_Obj_meshName), refMesh.m_original_vtx_pos, refMesh.m_original_face_struct);
					refMesh.m_current_vtx_pos = refMesh.m_original_vtx_pos;

					refMesh.m_nVtxSize = refMesh.m_original_vtx_pos.rows();
					refMesh.m_nFaceSize = refMesh.m_original_face_struct.rows();
					// Pseudo-color based on selection
					refMesh.m_current_face_color.resize(refMesh.m_nFaceSize, vrDim);

#if USE_TBB
					parallel_for(blocked_range<size_t>(0, refMesh.m_nFaceSize), ApplyColorMultiDomain(&refMesh.m_current_face_color, &refMesh.m_current_face_color_id), auto_partitioner());
					//parallel_for(blocked_range<size_t>(0, refMesh.m_nFaceSize), ApplyColor(&refMesh.m_current_face_color), auto_partitioner());
#else
					for (int f = 0; f < refMesh.m_nFaceSize; f++)
					{
						refMesh.m_current_face_color.row(f) = Colors::gold;
					}
#endif
					iglViewer& viewer = *m_iglViewerPtr;
					viewer.data.set_mesh(refMesh.m_current_vtx_pos, refMesh.m_original_face_struct);
					viewer.core.show_lines = true;
					viewer.core.orthographic = true;
					viewer.data.set_colors(refMesh.m_current_face_color);
					//viewer.core.trackball_angle = Eigen::Quaternionf(sqrt(2.0), 0, sqrt(2.0), 0);
					viewer.core.trackball_angle.normalize();
					viewer.callback_pre_draw = &pre_draw;
					viewer.callback_key_down = &key_down;
					viewer.core.is_animating = true;					
					viewer.core.animation_max_fps = GlobalConf::g_db_Simulation_animation_max_fps;
					viewer.core.camera_zoom = GlobalConf::g_db_Simulation_camera_zoom;

					
					viewer.core.background_color = Eigen::Vector3f(GlobalConf::g_vec4_Scene_bkgColor.x(),
						GlobalConf::g_vec4_Scene_bkgColor.y(), 
						GlobalConf::g_vec4_Scene_bkgColor.z());

					return true;
				}

				void runMainLoop()
				{
					vrASSERT(m_iglViewerPtr);
					m_iglViewerPtr->launch();
				}

			public://method for glut
				static bool pre_draw(igl::viewer::Viewer & viewer)
				{
					m_app_ptr->virtual_display();
					return false;
				}

				static bool key_down(igl::viewer::Viewer &viewer, unsigned char key, int mods)
				{
					m_app_ptr->virtual_key(key, 0, 0);
					return false;
				}

			private:
				int m_argc;
				char** m_argv;
				iglViewerPtr m_iglViewerPtr;
				static RenderFrameworkInterface * m_app_ptr;
			};
#endif//USE_IGL
		}//OpenGL
	}//namespace App
}//namespace VR

#endif//_iglFramework_h_