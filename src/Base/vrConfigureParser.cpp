#include "vrConfigureParser.h"
#include "vrLog.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
namespace VR
{
	namespace ConfigureParser
	{
		vrString makeKey(const vrLpsz lpszSection, const vrLpsz lpszName)
		{
			return string_format("%s.%s", lpszSection, lpszName);
		}

		vrString getConfPropertyValue(const vrPropertyMap& propertyMap, const vrString& strKey)
		{
			return propertyMap.at(strKey);
		}

		void parser_configurefile(const vrString& strConf, vrPropertyMap& propertyMap)
		{
			boost::property_tree::ptree pt;
			boost::property_tree::ini_parser::read_ini(strConf, pt);
			for (auto& itr : propertyMap)
			{
				itr.second = pt.get<vrString>(itr.first);
				//infoLog << (itr.first) << " " << (itr.second);
			}
		}
	}//namespace ConfigureParser
}//namespace VR