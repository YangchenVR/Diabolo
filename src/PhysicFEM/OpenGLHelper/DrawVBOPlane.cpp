#include "DrawVBOPlane.h"
#include "../vrGL.h"
#include "vrglutils.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			DrawVBOPlane::DrawVBOPlane(vrFloat xsize, vrFloat zsize, vrInt xdivs, vrInt zdivs)
			{
				
				faces = xdivs * zdivs;
				GLfloat * v = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
				GLfloat * n = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
				GLfloat * tex = new GLfloat[2 * (xdivs + 1) * (zdivs + 1)];
				unsigned int * el = new unsigned int[6 * xdivs * zdivs];

				GLfloat x2 = xsize / GLfloat(2.0);
				GLfloat z2 = zsize / GLfloat(2.0);
				GLfloat iFactor = (GLfloat)zsize / zdivs;
				GLfloat jFactor = (GLfloat)xsize / xdivs;
				GLfloat texi = GLfloat(1.0) / zdivs;
				GLfloat texj = GLfloat(1.0) / xdivs;
				GLfloat x, z;
				vrInt vidx = 0, tidx = 0;
				for (vrInt i = 0; i <= zdivs; i++) {
					z = iFactor * i - z2;
					for (vrInt j = 0; j <= xdivs; j++) {
						x = jFactor * j - x2;
						v[vidx] = x;
						v[vidx + 1] = 0.0f;
						v[vidx + 2] = z;
						n[vidx] = 0.0f;
						n[vidx + 1] = 1.0f;
						n[vidx + 2] = 0.0f;
						vidx += 3;
						tex[tidx] = j * texi;
						tex[tidx + 1] = i * texj;
						tidx += 2;
					}
				}

				unsigned int rowStart, nextRowStart;
				vrInt idx = 0;
				for (vrInt i = 0; i < zdivs; i++) {
					rowStart = i * (xdivs + 1);
					nextRowStart = (i + 1) * (xdivs + 1);
					for (vrInt j = 0; j < xdivs; j++) {
						el[idx] = rowStart + j;
						el[idx + 1] = nextRowStart + j;
						el[idx + 2] = nextRowStart + j + 1;
						el[idx + 3] = rowStart + j;
						el[idx + 4] = nextRowStart + j + 1;
						el[idx + 5] = rowStart + j + 1;
						idx += 6;
					}
				}

				unsigned int handle[4];
				glGenBuffers(4, handle);

				glGenVertexArrays(1, &vaoHandle);
				glBindVertexArray(vaoHandle);
				
				glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
				glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs + 1) * (zdivs + 1) * sizeof(GLfloat), v, GL_STATIC_DRAW);
				glVertexAttribPointer((GLuint)0, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(0);  // Vertex position

				glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
				glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs + 1) * (zdivs + 1) * sizeof(GLfloat), n, GL_STATIC_DRAW);
				glVertexAttribPointer((GLuint)1, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(1);  // Vertex normal

				glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
				glBufferData(GL_ARRAY_BUFFER, 2 * (xdivs + 1) * (zdivs + 1) * sizeof(GLfloat), tex, GL_STATIC_DRAW);
				glVertexAttribPointer((GLuint)2, 2, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(2);  // Texture coords

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * xdivs * zdivs * sizeof(unsigned int), el, GL_STATIC_DRAW);

				glBindVertexArray(0);

				delete[] v;
				delete[] n;
				delete[] tex;
				delete[] el;
			}

			void DrawVBOPlane::render() const {
				vrGLUtils::checkForOpenGLError(__FILE__, __LINE__);
				glBindVertexArray(vaoHandle);
				glDrawElements(GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
				vrGLUtils::checkForOpenGLError(__FILE__, __LINE__);
			}
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR