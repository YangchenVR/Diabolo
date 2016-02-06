#include "vrGlobalConf.h"

#include "vrLog.h"
namespace VR
{
	namespace GlobalConf
	{
		bool g_Obj_hasCoord;
		vrString const g_const_Obj_hasCoord = vrString("Obj.hasCoord");

		bool g_Obj_hasVerticeNormal;
		vrString const g_const_Obj_hasVerticeNormal = vrString("Obj.hasVerticeNormal");

		bool g_Obj_normalizeMesh;
		vrString const g_const_Obj_normalizeMesh = vrString("Obj.normalizeMesh");

		vrString g_str_Obj_meshName;
		vrString const g_const_Obj_meshName = vrString("Obj.meshName");

		vrString g_str_Obj_textureName;
		vrString const g_const_Obj_textureName = vrString("Obj.textureName");

		vrString g_str_Obj_meshId;
		vrString const g_const_Obj_meshId = vrString("Obj.meshId");

		vrInt g_n_Obj_octreeFineLevel;
		vrString const g_const_Obj_octreeFineLevel = vrString("Obj.octreeFineLevel");

		bool g_Simulation_doSimulation;
		vrString const g_const_Simulation_doSimulation = vrString("Simulation.doSimulation");

		vrInt g_n_Simulation_MinCount;
		vrString const g_const_Simulation_MinCount = vrString("Simulation.MinCount");

		vrInt g_n_Simulation_MaxCount;
		vrString const g_const_Simulation_MaxCount = vrString("Simulation.MaxCount");

		vrFloat g_db_Simulation_YoungModulus;
		vrString const g_const_Simulation_YoungModulus = vrString("Simulation.YoungModulus");

		vrFloat g_db_Simulation_externalForceFactor;
		vrString const g_const_Simulation_externalForceFactor = vrString("Simulation.externalForceFactor");

		vrFloat g_db_Simulation_scriptForceFactor;
		vrString const g_const_Simulation_scriptForceFactor = vrString("Simulation.scriptForceFactor");

		vrInt g_n_Scene_windowWidth;
		vrString const g_const_Scene_windowWidth = vrString("Scene.windowWidth");

		vrInt g_n_Scene_windowHeight;
		vrString const g_const_Scene_windowHeight = vrString("Scene.windowHeight");

		vrVec4 g_vec4_Scene_bkgColor;
		vrString const g_const_Scene_bkgColor = vrString("Scene.bkgColor");

		vrFloat g_db_Scene_planeXsize;
		vrString const g_const_Scene_planeXsize = vrString("Scene.planeXsize");

		vrFloat g_db_Scene_planeZsize;
		vrString const g_const_Scene_planeZsize = vrString("Scene.planeZsize");

		vrInt g_n_Scene_planeXdivs;
		vrString const g_const_Scene_planeXdivs = vrString("Scene.planeXdivs");

		vrInt g_n_Scene_planeZdivs;
		vrString const g_const_Scene_planeZdivs = vrString("Scene.planeZdivs");

		vrVec3 g_vec3_Scene_planeColor;
		vrString const g_const_Scene_planeColor = vrString("Scene.planeColor");

		vrVec3 g_vec3_Scene_modelColor;
		vrString const g_const_Scene_modelColor = vrString("Scene.modelColor");

		vrVec3 g_vec3_Scene_modelColor4Ka;
		vrString const g_const_Scene_modelColor4Ka = vrString("Scene.modelColor4Ka");

		vrVec3 g_vec3_Scene_CamerPos;
		vrString const g_const_Scene_CamerPos = vrString("Scene.CamerPos");

		vrVec3 g_vec3_Scene_LightPos;
		vrString const g_const_Scene_LightPos = vrString("Scene.LightPos");

		vrVec3 g_vec3_Scene_EyeCenter;
		vrString const g_const_Scene_EyeCenter = vrString("Scene.EyeCenter");

		vrVec3 g_vec3_Scene_ZUp;
		vrString const g_const_Scene_ZUp = vrString("Scene.ZUp");

		bool g_Scene_ShowFPS;
		vrString const g_const_Scene_ShowFPS = vrString("Scene.ShowFPS");

		vrFloat g_db_Scene_FOV;
		vrString const g_const_Scene_FOV = vrString("Scene.FOV");

		vrFloat g_db_Scene_Near;
		vrString const g_const_Scene_Near = vrString("Scene.Near");

		vrFloat g_db_Scene_Far;
		vrString const g_const_Scene_Far = vrString("Scene.Far");

		vrInt g_n_Scene_ShadowMapWidth;
		vrString const g_const_Scene_ShadowMapWidth = vrString("Scene.ShadowMapWidth");

		vrInt g_n_Scene_ShadowMapHeight;
		vrString const g_const_Scene_ShadowMapHeight = vrString("Scene.ShadowMapHeight");

		vrFloat g_db_Simulation_animation_max_fps;
		vrString const g_const_Simulation_animation_max_fps = vrString("Simulation.animation_max_fps");

		vrFloat g_db_Simulation_camera_zoom;
		vrString const g_const_Simulation_camera_zoom = vrString("Simulation.camera_zoom");

		void printConf()
		{
			infoLog << g_const_Obj_hasCoord << " = " << g_Obj_hasCoord;
			infoLog << g_const_Obj_hasVerticeNormal << " = " << g_Obj_hasVerticeNormal;
			infoLog << g_const_Obj_normalizeMesh << " = " << g_Obj_normalizeMesh;
			infoLog << g_const_Obj_meshName << " = " << g_str_Obj_meshName;
			infoLog << g_const_Obj_textureName << " = " << g_str_Obj_textureName;
			infoLog << g_const_Obj_meshId << " = " << g_str_Obj_meshId;
			infoLog << g_const_Obj_octreeFineLevel << " = " << g_n_Obj_octreeFineLevel;
			infoLog << g_const_Simulation_doSimulation << " = " << g_Simulation_doSimulation;
			infoLog << g_const_Simulation_MinCount << " = " << g_n_Simulation_MinCount;
			infoLog << g_const_Simulation_MaxCount << " = " << g_n_Simulation_MaxCount;
			infoLog << g_const_Simulation_YoungModulus << " = " << g_db_Simulation_YoungModulus;
			infoLog << g_const_Simulation_externalForceFactor << " = " << g_db_Simulation_externalForceFactor;
			infoLog << g_const_Simulation_scriptForceFactor << " = " << g_db_Simulation_scriptForceFactor;
			infoLog << g_const_Simulation_animation_max_fps << " = " << g_db_Simulation_animation_max_fps;
			infoLog << g_const_Simulation_camera_zoom << " = " << g_db_Simulation_camera_zoom;

			infoLog << g_const_Scene_windowWidth << " = " << g_n_Scene_windowWidth;
			infoLog << g_const_Scene_windowHeight << " = " << g_n_Scene_windowHeight;
			infoLog << g_const_Scene_bkgColor << " = " << g_vec4_Scene_bkgColor.transpose();
			infoLog << g_const_Scene_planeXsize << " = " << g_db_Scene_planeXsize;
			infoLog << g_const_Scene_planeZsize << " = " << g_db_Scene_planeZsize;
			infoLog << g_const_Scene_planeXdivs << " = " << g_n_Scene_planeXdivs;
			infoLog << g_const_Scene_planeZdivs << " = " << g_n_Scene_planeZdivs;
			infoLog << g_const_Scene_planeColor << " = " << g_vec3_Scene_planeColor.transpose();
			infoLog << g_const_Scene_modelColor << " = " << g_vec3_Scene_modelColor.transpose();
			infoLog << g_const_Scene_modelColor4Ka << " = " << g_vec3_Scene_modelColor4Ka.transpose();
			infoLog << g_const_Scene_CamerPos << " = " << g_vec3_Scene_CamerPos.transpose();
			infoLog << g_const_Scene_LightPos << " = " << g_vec3_Scene_LightPos.transpose();
			infoLog << g_const_Scene_EyeCenter << " = " << g_vec3_Scene_EyeCenter.transpose();
			infoLog << g_const_Scene_ZUp << " = " << g_vec3_Scene_ZUp.transpose();
			infoLog << g_const_Scene_ShowFPS << " = " << g_Scene_ShowFPS;
			infoLog << g_const_Scene_FOV << " = " << g_db_Scene_FOV;
			infoLog << g_const_Scene_Near << " = " << g_db_Scene_Near;
			infoLog << g_const_Scene_Far << " = " << g_db_Scene_Far;
			infoLog << g_const_Scene_ShadowMapWidth << " = " << g_n_Scene_ShadowMapWidth;
			infoLog << g_const_Scene_ShadowMapHeight << " = " << g_n_Scene_ShadowMapHeight;
		}
	}//GlobalConf
}//VR