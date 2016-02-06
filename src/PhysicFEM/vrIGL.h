#ifndef _vrIGL_H_
#define _vrIGL_H_

#include "appMacro.h"
#if USE_IGL 

#include <igl/colon.h>
#include <igl/harmonic.h>
#include <igl/readOBJ.h>
#include <igl/readDMAT.h>
#include <igl/viewer/Viewer.h>
#include <boost/shared_ptr.hpp>

namespace VR
{
	typedef igl::viewer::Viewer iglViewer;
	typedef boost::shared_ptr< igl::viewer::Viewer > iglViewerPtr;
}
#endif//USE_IGL

#endif//_vrIGL_H_