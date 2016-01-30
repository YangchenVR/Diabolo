#include "vrString.h"
#include <boost/format.hpp>
namespace VR
{
	vrString awesome_printf_helper(boost::format& f){
		return boost::str(f);
	}
}//namespace VR