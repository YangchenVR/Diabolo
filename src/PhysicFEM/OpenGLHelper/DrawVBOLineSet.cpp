#include "DrawVBOLineSet.h"
#include "../vrGL.h"
#include "vrglutils.h"

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			void DrawVBOLineSet::render() const 
			{
				glBindVertexArray(vaoHandle);
				glDrawElements(GL_LINES, m_nLinesCount * 2, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
			}

			void DrawVBOLineSet::initialize(const vrInt nLinesCount, vrFloat * pos, vrInt * idx)
			{
				m_nLinesCount = nLinesCount;
				glGenVertexArrays(1, &vaoHandle);
				glBindVertexArray(vaoHandle);


				glGenBuffers(2, handle);
				
				glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
				glBufferData(GL_ARRAY_BUFFER, m_nLinesCount * 2 * 3 * sizeof(vrFloat), pos, GL_STATIC_DRAW);
				glVertexAttribPointer((GLuint)0, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(0);  // Vertex position

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nLinesCount * 2 * sizeof(GLuint), idx, GL_STATIC_DRAW);

				glBindVertexArray(0);
			}

			void DrawVBOLineSet::updateLineSet(const int nLinesCount, vrFloat3 * pos, vrInt2 * idx)
			{
				m_nLinesCount = nLinesCount;
				//glGenVertexArrays( 1, &vaoHandle );
				glBindVertexArray(vaoHandle);

				glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
				glBufferData(GL_ARRAY_BUFFER, m_nLinesCount * 2 * sizeof(vrFloat3), pos, GL_STATIC_DRAW);
				glVertexAttribPointer((GLuint)0, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(0);  // Vertex position

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nLinesCount  * sizeof(vrInt2), idx, GL_STATIC_DRAW);

				glBindVertexArray(0);
			}
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR