#ifndef _vrString_h_
#define _vrString_h_


#include <string>
#include <boost/format.hpp>
namespace VR
{
	typedef std::string vrString;
	

	vrString awesome_printf_helper(boost::format& f);

	template<class T, class... Args>
	vrString awesome_printf_helper(boost::format& f, T&& t, Args&&... args){
		return awesome_printf_helper(f % std::forward<T>(t), std::forward<Args>(args)...);
	}

	template<typename... Arguments>
	vrString string_format(std::string const& fmt, Arguments&&... args)
	{
		boost::format f(fmt);
		return awesome_printf_helper(f, std::forward<Arguments>(args)...);
	}
}//namespace VR

#endif//_vrString_h_