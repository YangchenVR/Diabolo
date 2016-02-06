#include "DrawVBOMesh.h"
#include "../vrGL.h"
#include "vrLog.h"
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT
			DrawVBOMesh::DrawVBOMesh(bool center, bool loadTc, bool genTangents)
				:reCenterMesh(center), loadTex(loadTc), genTang(genTangents)
			{}

			void DrawVBOMesh::loadOBJ(Geometry::MeshDataStructPtr obj)
			{
				m_objInfo = obj;
				Geometry::MeshDataStruct& refObjInfo = *m_objInfo;
				std::vector< vrGLMVec3 > & points = refObjInfo.points;
				std::vector< vrGLMVec3 > & normals = refObjInfo.normals;
				std::vector< vrGLMVec2 > & texCoords = refObjInfo.texCoords;
				std::vector<vrInt>& faces = refObjInfo.faces;

				if (normals.size() == 0) {
					generateAveragedNormals(points, normals, faces);
				}

				std::vector< vrGLMVec4 > tangents;
				if (genTang && texCoords.size() > 0) {
					generateTangents(points, normals, faces, texCoords, tangents);
				}

				if (reCenterMesh) {
					center(points);
				}

				storeVBO(points, normals, texCoords, tangents, faces);
			}

			void DrawVBOMesh::loadDisplacementOBJ(Geometry::MeshDataStructPtr obj)
			{
				//m_objInfo = obj;
				Geometry::MeshDataStruct& refObj = *obj;
				std::vector< vrGLMVec3 > & points = refObj.displacedVertices;
				std::vector< vrGLMVec3 > & normals = refObj.normals;
				std::vector< vrGLMVec2 > & texCoords = refObj.texCoords;
				std::vector< vrInt > & faces = refObj.faces;

				std::vector< vrGLMVec4 >  tangents;
				updateVBO(points, normals, texCoords, tangents, faces);
			}

			void DrawVBOMesh::generateAveragedNormals(
				const std::vector< vrGLMVec3 > & points,
				std::vector< vrGLMVec3 > & normals,
				const std::vector<vrInt> & faces)
			{
				for (vrUnsigned i = 0; i < points.size(); i++) {
					normals.push_back(vrGLMVec3(0.0));
				}

				for (vrUnsigned i = 0; i < faces.size(); i += 3) {
					const vrGLMVec3 & p1 = points[faces[i]];
					const vrGLMVec3 & p2 = points[faces[i + 1]];
					const vrGLMVec3 & p3 = points[faces[i + 2]];

					vrGLMVec3 a = p2 - p1;
					vrGLMVec3 b = p3 - p1;
					vrGLMVec3 n = glm::normalize(glm::cross(a, b));

					normals[faces[i]] += n;
					normals[faces[i + 1]] += n;
					normals[faces[i + 2]] += n;
				}

				for (vrUnsigned i = 0; i < normals.size(); i++) {
					normals[i] = glm::normalize(normals[i]);
				}
			}

			void DrawVBOMesh::generateTangents(
				const std::vector< vrGLMVec3 > & points,
				const std::vector< vrGLMVec3 > & normals,
				const std::vector< vrInt > & faces,
				const std::vector< vrGLMVec2 > & texCoords,
				std::vector< vrGLMVec4 > & tangents)
			{
				std::vector< vrGLMVec3 > tan1Accum;
				std::vector< vrGLMVec3 > tan2Accum;

				for (vrUnsigned i = 0; i < points.size(); i++) {
					tan1Accum.push_back(vrGLMVec3(0.0));
					tan2Accum.push_back(vrGLMVec3(0.0));
					tangents.push_back(vrGLMVec4(0.0));
				}

				// Compute the tangent vector
				for (vrUnsigned i = 0; i < faces.size(); i += 3)
				{
					const vrGLMVec3 &p1 = points[faces[i]];
					const vrGLMVec3 &p2 = points[faces[i + 1]];
					const vrGLMVec3 &p3 = points[faces[i + 2]];

					const vrGLMVec2 &tc1 = texCoords[faces[i]];
					const vrGLMVec2 &tc2 = texCoords[faces[i + 1]];
					const vrGLMVec2 &tc3 = texCoords[faces[i + 2]];

					vrGLMVec3 q1 = p2 - p1;
					vrGLMVec3 q2 = p3 - p1;
					vrFloat s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
					vrFloat t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
					vrFloat r = 1.0f / (s1 * t2 - s2 * t1);
					vrGLMVec3 tan1((t2*q1.x - t1*q2.x) * r,
						(t2*q1.y - t1*q2.y) * r,
						(t2*q1.z - t1*q2.z) * r);
					vrGLMVec3 tan2((s1*q2.x - s2*q1.x) * r,
						(s1*q2.y - s2*q1.y) * r,
						(s1*q2.z - s2*q1.z) * r);
					tan1Accum[faces[i]] += tan1;
					tan1Accum[faces[i + 1]] += tan1;
					tan1Accum[faces[i + 2]] += tan1;
					tan2Accum[faces[i]] += tan2;
					tan2Accum[faces[i + 1]] += tan2;
					tan2Accum[faces[i + 2]] += tan2;
				}

				for (vrUnsigned i = 0; i < points.size(); ++i)
				{
					const vrGLMVec3 &n = normals[i];
					vrGLMVec3 &t1 = tan1Accum[i];
					vrGLMVec3 &t2 = tan2Accum[i];

					// Gram-Schmidt orthogonalize
					tangents[i] = vrGLMVec4(glm::normalize(t1 - (glm::dot(n, t1) * n)), 0.0);
					// Store handedness in w
					tangents[i].w = (glm::dot(glm::cross(n, t1), t2) < 0.0) ? -1.0 : 1.0;
				}
				tan1Accum.clear();
				tan2Accum.clear();
			}

			void DrawVBOMesh::center(std::vector< vrGLMVec3 > & points) 
			{
				if (points.size() < 1) return;

				vrGLMVec3 maxPoint = points[0];
				vrGLMVec3 minPoint = points[0];

				// Find the AABB
				for (vrUnsigned i = 0; i < points.size(); ++i) {
					vrGLMVec3 & point = points[i];
					if (point.x > maxPoint.x) maxPoint.x = point.x;
					if (point.y > maxPoint.y) maxPoint.y = point.y;
					if (point.z > maxPoint.z) maxPoint.z = point.z;
					if (point.x < minPoint.x) minPoint.x = point.x;
					if (point.y < minPoint.y) minPoint.y = point.y;
					if (point.z < minPoint.z) minPoint.z = point.z;
				}

				// Center of the AABB
				vrGLMVec3 center = vrGLMVec3((maxPoint.x + minPoint.x) / 2.0,
					(maxPoint.y + minPoint.y) / 2.0,
					(maxPoint.z + minPoint.z) / 2.0);

				// Translate center of the AABB to the origin
				for (vrUnsigned i = 0; i < points.size(); ++i) {
					vrGLMVec3 & point = points[i];
					point = point - center;
				}
			}

			void DrawVBOMesh::updateVBO(const std::vector< vrGLMVec3 > & points,
				const std::vector< vrGLMVec3 > & normals,
				const std::vector< vrGLMVec2 > &texCoords,
				const std::vector< vrGLMVec4 > &tangents,
				const std::vector< vrInt > &elements)
			{
				vrSizt_t nVerts = points.size();
				faces = elements.size() / 3;
				nElementCount = elements.size();

				infoLog << string_format("faces (%d)  element (%d)\n", faces, elements.size());


				const vrInt nVtxVnCount = nElementCount /** _nExternalMemory*/;
				vrFloat3 * v = new vrFloat3[nVtxVnCount];
				memset(v, '\0', nVtxVnCount*sizeof(vrFloat3));

				vrFloat3 * n = new vrFloat3[nVtxVnCount];
				memset(n, '\0', nVtxVnCount*sizeof(vrFloat3));

				vrFloat2 * tc = NULL;
				vrFloat4 * tang = NULL;

				for (int i = 0; i<nElementCount; ++i)
				{
					const int idx = elements[i];
					const vrGLMVec3& refPoint = points[idx];
					const vrGLMVec3& refNormal = normals[idx];
					v[i] = make_vrFloat3(refPoint.x, refPoint.y, refPoint.z);
					n[i] = make_vrFloat3(refNormal.x, refNormal.y, refNormal.z);
				}
				/*float3 * ptr_vertexes;
				float3 * ptr_normals;
				cudaGLMapBufferObject((void**)&ptr_vertexes, handle[0]);
				cudaGLMapBufferObject((void**)&ptr_normals, handle[1]);



				cudaGLUnmapBufferObject(handle[0]);
				cudaGLUnmapBufferObject(handle[1]);*/

				//glGenVertexArrays( 1, &vaoHandle );
				glBindVertexArray(vaoHandle);

				vrInt nBuffers = 2;
				vrUnsigned bufIdx = 0;
				//glGenBuffers(nBuffers, handle);

				glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
				glBufferData(GL_ARRAY_BUFFER, nVtxVnCount * sizeof(vrFloat3), v, GL_STREAM_DRAW/*GL_STATIC_DRAW*/);
				glVertexAttribPointer((GLuint)0, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(0);  // Vertex position

				glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
				glBufferData(GL_ARRAY_BUFFER, nVtxVnCount * sizeof(vrFloat3), n, GL_STREAM_DRAW/*GL_STATIC_DRAW*/);
				glVertexAttribPointer((GLuint)1, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(1);  // Vertex normal

				glBindVertexArray(0);

				delete[] v;
				delete[] n;
			}

			void DrawVBOMesh::storeVBO(const std::vector< vrGLMVec3 > & points,
				const std::vector< vrGLMVec3 > & normals,
				const std::vector< vrGLMVec2 > &texCoords,
				const std::vector< vrGLMVec4 > &tangents,
				const std::vector< vrInt > &elements)
			{
				vrInt nVerts = points.size();
				faces = elements.size() / 3;
				nElementCount = elements.size();
				infoLog << string_format("faces (%d)  element (%d)\n", faces, elements.size());
				//LogInfo("faces (%d)  element (%d)\n", faces, elements.size());

				const int nVtxVnCount = nElementCount * vrExternalMemory;
				vrFloat3 * v = new vrFloat3[nVtxVnCount];
				memset(v, '\0', nVtxVnCount*sizeof(vrFloat3));

				vrFloat3 * n = new vrFloat3[nVtxVnCount];
				memset(n, '\0', nVtxVnCount*sizeof(vrFloat3));

				vrFloat2 * tc = NULL;
				vrFloat4 * tang = NULL;

				for (vrInt i = 0; i<nElementCount; ++i)
				{
					const vrInt idx = elements[i];
					const vrGLMVec3& refPoint = points[idx];
					const vrGLMVec3& refNormal = normals[idx];
					v[i] = make_vrFloat3(refPoint.x, refPoint.y, refPoint.z);
					n[i] = make_vrFloat3(refNormal.x, refNormal.y, refNormal.z);
				}

				glGenVertexArrays(1, &vaoHandle);
				glBindVertexArray(vaoHandle);

				vrInt nBuffers = 2;
				vrUnsigned bufIdx = 0;
				glGenBuffers(nBuffers, handle);

				glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
				glBufferData(GL_ARRAY_BUFFER, nVtxVnCount * sizeof(vrFloat3), v, GL_STREAM_DRAW/*GL_STATIC_DRAW*/);
				glVertexAttribPointer((GLuint)0, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(0);  // Vertex position

				glBindBuffer(GL_ARRAY_BUFFER, handle[bufIdx++]);
				glBufferData(GL_ARRAY_BUFFER, nVtxVnCount * sizeof(vrFloat3), n, GL_STREAM_DRAW/*GL_STATIC_DRAW*/);
				glVertexAttribPointer((GLuint)1, 3, VR_GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
				glEnableVertexAttribArray(1);  // Vertex normal

				glBindVertexArray(0);

				delete[] v;
				delete[] n;

			}

			void DrawVBOMesh::render() const 
			{
				glBindVertexArray(vaoHandle);
				glDrawArrays(GL_TRIANGLES, 0, nElementCount);
				//glDrawElements(GL_TRIANGLES, 3 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
			}
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR