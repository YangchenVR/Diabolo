#ifndef _vrConfigureParser_h_
#define _vrConfigureParser_h_
#include "vrTypes.h"

namespace VR
{
	namespace ConfigureParser
	{
		typedef std::map< vrString, vrString > vrPropertyMap;
		extern vrString makeKey(const vrLpsz strSection, const vrLpsz strName);
		extern vrString getConfPropertyValue(const vrPropertyMap& propertyMap, const vrString& strKey);
		extern void parser_configurefile(const vrString& strConf, vrPropertyMap& propertyMap);
		
	}//namespace ConfigureParser
}//namespace VR

#endif//_vrConfigureParser_h_