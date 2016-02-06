#ifndef _DrawVBOLineSet_H_
#define _DrawVBOLineSet_H_

#include "../appMacro.h"
#include "vrBase.h"
#include "DrawObjectBase.h"
#include <boost/shared_ptr.hpp>
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			class DrawVBOLineSet : public DrawObjectBase
			{
			public:
				DrawVBOLineSet(){ m_nLinesCount = 0; }
				~DrawVBOLineSet(void){}
			public:
				void initialize(const vrInt nLinesCount, vrFloat * pos, vrInt * idx);
				void updateLineSet(const int nLinesCount, vrFloat3 * pos, vrInt2 * idx);
				virtual void render() const;

				vrInt getLineCount()const{ return m_nLinesCount; }
				void setLineCount(const vrInt nLineSize){ m_nLinesCount = nLineSize; }
				vrUnsigned getVBOHandle(vrInt idx){ return handle[idx]; }
			private:
				vrUnsigned vaoHandle;
				vrInt m_nLinesCount;
				vrUnsigned handle[2];
			};

			typedef boost::shared_ptr< DrawVBOLineSet > DrawVBOLineSetPtr;
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR
#endif//_DrawVBOLineSet_H_