#ifndef _DrawObjectBase_H_
#define _DrawObjectBase_H_

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
			class DrawObjectBase
			{
			public:
				DrawObjectBase(){}
				virtual ~DrawObjectBase(){}
			public:
				virtual void render() const = 0;
			};
		}//OpenGL
	}//App
}//VR
#endif//_DrawObjectBase_H_