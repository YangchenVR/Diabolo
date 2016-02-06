#include "glutFramework.h"
#include "RenderFrameworkInterface.h"
//#include "vrBase.h"

#if USE_GLUT
VR::App::OpenGL::RenderFrameworkInterface * VR::App::OpenGL::glutFramework::m_app_ptr = vrNull;
#endif