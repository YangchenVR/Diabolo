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
  if(viewer.core.is_animating)
  {
    
  }
  
  viewer.data.set_vertices(U);
  viewer.data.compute_normals();
  return false;
}

bool key_down(igl::viewer::Viewer &viewer, unsigned char key, int mods)
{
  switch(key)
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
  infoLog << string_format("Start using loggingPP %s","abc");
#endif//#if vrLoggingPP
  {
	  //infoLog << "current path :" << VR::FileSystem::get_currentpath();
	  VR::init_global();
	  
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
  for(int f = 0;f<F.rows();f++)
  {
	  C.row(f) = gold;
  }

  // Plot the mesh with pseudocolors
  igl::viewer::Viewer viewer;
  viewer.data.set_mesh(U, F);
  viewer.core.show_lines = true;
  viewer.core.orthographic = true;
  viewer.data.set_colors(C);
  viewer.core.trackball_angle = Eigen::Quaternionf(sqrt(2.0),0,sqrt(2.0),0);
  viewer.core.trackball_angle.normalize();
  viewer.callback_pre_draw = &pre_draw;
  viewer.callback_key_down = &key_down;
  //viewer.core.is_animating = true;
  viewer.core.animation_max_fps = 30.;
  cout<<
    "Press [space] to toggle deformation."<<endl<<
    "Press 'd' to toggle between biharmonic surface or displacements."<<endl;
  viewer.launch();
}
