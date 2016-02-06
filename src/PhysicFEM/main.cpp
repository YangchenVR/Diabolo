#if 1

#include "appMacro.h"
#include "vrBase.h"
#include "vrGL.h"
#include "FEM_Simulator.h"
#include "glutFramework.h"
#include "iglFramework.h"
#include "vrGlobalConf.h"
#include "vrLog.h"
#include <boost/lexical_cast.hpp>
using namespace VR;

INITIALIZE_EASYLOGGINGPP

namespace VR
{

	void init_global()
	{
		VR::ConfigureParser::vrPropertyMap propertyMap;
		vrString inifile = FileSystem::get_currentpath() + vrString("/conf/param.conf");

		propertyMap[GlobalConf::g_const_Obj_hasCoord];
		propertyMap[GlobalConf::g_const_Obj_hasVerticeNormal];
		propertyMap[GlobalConf::g_const_Obj_normalizeMesh];
		propertyMap[GlobalConf::g_const_Obj_meshName];
		propertyMap[GlobalConf::g_const_Obj_textureName];
		propertyMap[GlobalConf::g_const_Obj_meshId];
		propertyMap[GlobalConf::g_const_Obj_octreeFineLevel];
		propertyMap[GlobalConf::g_const_Simulation_doSimulation];
		propertyMap[GlobalConf::g_const_Simulation_MinCount];
		propertyMap[GlobalConf::g_const_Simulation_MaxCount];
		propertyMap[GlobalConf::g_const_Simulation_YoungModulus];
		propertyMap[GlobalConf::g_const_Simulation_externalForceFactor];
		propertyMap[GlobalConf::g_const_Simulation_scriptForceFactor];
		propertyMap[GlobalConf::g_const_Simulation_animation_max_fps];
		propertyMap[GlobalConf::g_const_Simulation_camera_zoom];

		propertyMap[GlobalConf::g_const_Scene_windowWidth];
		propertyMap[GlobalConf::g_const_Scene_windowHeight];
		propertyMap[GlobalConf::g_const_Scene_bkgColor];
		propertyMap[GlobalConf::g_const_Scene_planeXsize];
		propertyMap[GlobalConf::g_const_Scene_planeZsize];
		propertyMap[GlobalConf::g_const_Scene_planeXdivs];
		propertyMap[GlobalConf::g_const_Scene_planeZdivs];
		propertyMap[GlobalConf::g_const_Scene_planeColor];
		propertyMap[GlobalConf::g_const_Scene_modelColor];
		propertyMap[GlobalConf::g_const_Scene_modelColor4Ka];
		propertyMap[GlobalConf::g_const_Scene_CamerPos];
		propertyMap[GlobalConf::g_const_Scene_LightPos];
		propertyMap[GlobalConf::g_const_Scene_EyeCenter];
		propertyMap[GlobalConf::g_const_Scene_ZUp];
		propertyMap[GlobalConf::g_const_Scene_ShowFPS];
		propertyMap[GlobalConf::g_const_Scene_FOV];
		propertyMap[GlobalConf::g_const_Scene_Near];
		propertyMap[GlobalConf::g_const_Scene_Far];
		propertyMap[GlobalConf::g_const_Scene_ShadowMapWidth];
		propertyMap[GlobalConf::g_const_Scene_ShadowMapHeight];


		VR::ConfigureParser::parser_configurefile(inifile, propertyMap);


		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Obj_hasCoord, GlobalConf::g_Obj_hasCoord);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Obj_hasVerticeNormal, GlobalConf::g_Obj_hasVerticeNormal);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Obj_normalizeMesh, GlobalConf::g_Obj_normalizeMesh);

		VR::ConfigureParser::getConfPropertyValueStr(propertyMap, GlobalConf::g_const_Obj_meshName, GlobalConf::g_str_Obj_meshName);
		VR::ConfigureParser::getConfPropertyValueStr(propertyMap, GlobalConf::g_const_Obj_textureName, GlobalConf::g_str_Obj_textureName);
		VR::ConfigureParser::getConfPropertyValueStr(propertyMap, GlobalConf::g_const_Obj_meshId, GlobalConf::g_str_Obj_meshId);

		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Obj_octreeFineLevel, GlobalConf::g_n_Obj_octreeFineLevel);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_doSimulation, GlobalConf::g_Simulation_doSimulation);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_MinCount, GlobalConf::g_n_Simulation_MinCount);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_MaxCount, GlobalConf::g_n_Simulation_MaxCount);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_YoungModulus, GlobalConf::g_db_Simulation_YoungModulus);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_externalForceFactor, GlobalConf::g_db_Simulation_externalForceFactor);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_scriptForceFactor, GlobalConf::g_db_Simulation_scriptForceFactor);
		
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_animation_max_fps, GlobalConf::g_db_Simulation_animation_max_fps);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Simulation_camera_zoom, GlobalConf::g_db_Simulation_camera_zoom);
		
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_windowWidth, GlobalConf::g_n_Scene_windowWidth);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_windowHeight, GlobalConf::g_n_Scene_windowHeight);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_bkgColor, GlobalConf::g_vec4_Scene_bkgColor);

		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_planeXsize, GlobalConf::g_db_Scene_planeXsize);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_planeZsize, GlobalConf::g_db_Scene_planeZsize);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_planeXdivs, GlobalConf::g_n_Scene_planeXdivs);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_planeZdivs, GlobalConf::g_n_Scene_planeZdivs);

		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_planeColor, GlobalConf::g_vec3_Scene_planeColor);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_modelColor, GlobalConf::g_vec3_Scene_modelColor);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_modelColor4Ka, GlobalConf::g_vec3_Scene_modelColor4Ka);

		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_CamerPos, GlobalConf::g_vec3_Scene_CamerPos);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_LightPos, GlobalConf::g_vec3_Scene_LightPos);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_EyeCenter, GlobalConf::g_vec3_Scene_EyeCenter);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_ZUp, GlobalConf::g_vec3_Scene_ZUp);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_ShowFPS, GlobalConf::g_Scene_ShowFPS);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_FOV, GlobalConf::g_db_Scene_FOV);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_Near, GlobalConf::g_db_Scene_Near);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_Far, GlobalConf::g_db_Scene_Far);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_ShadowMapWidth, GlobalConf::g_n_Scene_ShadowMapWidth);
		VR::ConfigureParser::getConfPropertyValue(propertyMap, GlobalConf::g_const_Scene_ShadowMapHeight, GlobalConf::g_n_Scene_ShadowMapHeight);

		GlobalConf::printConf();
	}
}


int main(int argc, char *argv[])
{
	using namespace VR;
	init_global();
#if USE_IGL
	VR::App::FEM_Simulator< 3, VR::App::OpenGL::iglFramework > app(argc, argv);
	app.createIGLScene();
	app.init();
	
	app.run();
#endif

#if USE_GLUT
	VR::App::FEM_Simulator< 3, VR::App::OpenGL::glutFramework > app(argc, argv);
	app.init();
	app.createVBOScene();
	app.run();
#endif//USE_GLUT
	
	return 0;
}

#else
#define USE_IGL (1)
#define USE_GLUT (0)

#include <igl/colon.h>
#include <igl/harmonic.h>
#include <igl/readOBJ.h>
#include <igl/readDMAT.h>
#include <igl/viewer/Viewer.h>
#include <algorithm>
#include <iostream>

#include "vrBase.h"
using namespace VR;




namespace VR
{

	void init_global()
	{
		std::map< vrString, vrString > propertyMap;
		vrString inifile = FileSystem::get_currentpath() + vrString("/conf/param.conf");
		propertyMap[vrString("Obj.hasCoord")];
		propertyMap[vrString("Obj.hasVerticeNormal")];
		propertyMap[vrString("Obj.normalizeMesh")];
		propertyMap[vrString("Obj.meshName")];
		propertyMap[vrString("Obj.textureName")];
		propertyMap[vrString("Obj.meshId")];
		propertyMap[vrString("Obj.octreeFineLevel")];
		propertyMap[vrString("Simulation.doSimulation")];
		propertyMap[vrString("Simulation.MinCount")];
		propertyMap[vrString("Simulation.MaxCount")];
		propertyMap[vrString("Simulation.YoungModulus")];
		propertyMap[vrString("Simulation.externalForceFactor")];
		propertyMap[vrString("Simulation.scriptForceFactor")];
		VR::ConfigureParser::parser_configurefile(inifile, propertyMap);
	}
}

#define MyMeshPath "D:/MyWorkspace/MyMesh/OBJ/"
vrVec3 purple(80.0 / 255.0, 64.0 / 255.0, 255.0 / 255.0);
vrVec3 gold(255.0 / 255.0, 228.0 / 255.0, 58.0 / 255.0);


bool deformation_field = false;

vrMat V, U, V_disp;
vrVec Z;
vrMatI F;
vrVecI b;
vrInt g_iVtxSize = 0;

void computeVertexDisp(const vrVec& dispVec, vrMat& dispMat, const vrInt nVtxSize)
{
}

bool pre_draw(igl::viewer::Viewer & viewer)
{
	using namespace Eigen;
	// Determine boundary conditions
	if (viewer.core.is_animating)
	{

	}

	viewer.data.set_vertices(U);
	viewer.data.compute_normals();
	return false;
}

bool key_down(igl::viewer::Viewer &viewer, unsigned char key, int mods)
{
	switch (key)
	{
	case ' ':
		viewer.core.is_animating = !viewer.core.is_animating;
		return true;
	case 'D':
	case 'd':
		deformation_field = !deformation_field;
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	using namespace Eigen;
	using namespace std;

#if vrLoggingPP
	START_EASYLOGGINGPP(argc, argv);
	el::Configurations conf("./conf/log.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	infoLog << string_format("Start using loggingPP %s", "abc");
#endif//#if vrLoggingPP
	{
		//infoLog << "current path :" << VR::FileSystem::get_currentpath();
		VR::init_global();

		VR::App::FEM_Simulator< 3 > app;
		//VR::FileSystem::printPathParts("E:\\DATA\\books.txt");
	}


	igl::readOBJ(MyMeshPath "Bunny_3w.obj", V, F);
	U = V;

	{
		using namespace VR;

		std::vector<int> v(50);
		std::generate(v.begin(), v.end(), std::rand); // Using the C function rand()
		vrPrintContainer(v);
	}
	// Pseudo-color based on selection
	vrMat C(F.rows(), 3);
	for (int f = 0; f < F.rows(); f++)
	{
		C.row(f) = gold;
	}

	// Plot the mesh with pseudocolors
	igl::viewer::Viewer viewer;
	viewer.data.set_mesh(U, F);
	viewer.core.show_lines = true;
	viewer.core.orthographic = true;
	viewer.data.set_colors(C);
	viewer.core.trackball_angle = Eigen::Quaternionf(sqrt(2.0), 0, sqrt(2.0), 0);
	viewer.core.trackball_angle.normalize();
	viewer.callback_pre_draw = &pre_draw;
	viewer.callback_key_down = &key_down;
	//viewer.core.is_animating = true;
	viewer.core.animation_max_fps = 30.;
	cout <<
		"Press [space] to toggle deformation." << endl <<
		"Press 'd' to toggle between biharmonic surface or displacements." << endl;
	viewer.launch();
}

#endif
