#include "vrFrustum.h"
#include <glm/gtx/transform2.hpp>
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT

#define PI 3.141592653589793
#define TWOPI 6.2831853071795862
#define TWOPI_F 6.2831853f
#define TO_RADIANS(x) (x * 0.017453292519943295)
#define TO_DEGREES(x) (x * 57.29577951308232)
			vrFrustum::vrFrustum(Projection::ProjType t) : type(t)
			{
				GLuint elem[12 * 2];

				this->orient(vrGLMVec3(0.0f, 0.0f, 1.0f), vrGLMVec3(0.0f, 0.0f, 0.0f), vrGLMVec3(0.0f, 1.0f, 0.0f));
				if (type == Projection::ORTHO) {
					this->setOrthoBounds(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
				}
				else {
					this->setPerspective(50.0f, 1.0f, 0.5f, 100.0f);

					GLuint idx = 0;
					// The outer edges
					elem[idx++] = 0; elem[idx++] = 5;
					elem[idx++] = 0; elem[idx++] = 6;
					elem[idx++] = 0; elem[idx++] = 7;
					elem[idx++] = 0; elem[idx++] = 8;
					// The near plane
					elem[idx++] = 1; elem[idx++] = 2;
					elem[idx++] = 2; elem[idx++] = 3;
					elem[idx++] = 3; elem[idx++] = 4;
					elem[idx++] = 4; elem[idx++] = 1;
					// The far plane
					elem[idx++] = 5; elem[idx++] = 6;
					elem[idx++] = 6; elem[idx++] = 7;
					elem[idx++] = 7; elem[idx++] = 8;
					elem[idx++] = 8; elem[idx++] = 5;
				}

				glBindVertexArray(0);
				glGenBuffers(2, handle);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLuint), elem, GL_STATIC_DRAW);
			}

			void vrFrustum::orient(const vrGLMVec3 &pos, const vrGLMVec3& a, const vrGLMVec3& u)
			{
				this->origin = pos;
				this->at = a;
				this->up = u;
			}

			void vrFrustum::setOrthoBounds(vrFloat xmin, vrFloat xmax, vrFloat ymin, vrFloat ymax,
				vrFloat nearDist, vrFloat farDist)
			{
				this->xmin = xmin;
				this->xmax = xmax;
				this->ymin = ymin;
				this->ymax = ymax;
				this->mNear = nearDist;
				this->mFar = farDist;
			}

			void vrFrustum::setPerspective(vrFloat fovy, vrFloat ar, vrFloat nearDist, vrFloat farDist)
			{
				this->fovy = fovy;
				this->ar = ar;
				this->mNear = nearDist;
				this->mFar = farDist;
			}

			void vrFrustum::enclose(const vrFrustum & other)
			{
				vrGLMVec3 n = glm::normalize(other.origin - other.at);
				vrGLMVec3 u = glm::normalize(glm::cross(other.up, n));
				vrGLMVec3 v = glm::normalize(glm::cross(n, u));
				if (type == Projection::PERSPECTIVE)
					this->orient(origin, other.getCenter(), up);
				vrGLMMat4 m = this->getViewMatrix();

				vrGLMVec3 p[8];

				// Get 8 points that define the frustum
				if (other.type == Projection::PERSPECTIVE) {
					vrGLMVec3::value_type dy = other.mNear * std::tan((vrGLMVec3::value_type)TO_RADIANS(other.fovy) / 2.0f);
					vrGLMVec3::value_type dx = other.ar * dy;
					vrGLMVec3 c = other.origin - n * vrGLMVec3::value_type( other.mNear);
					p[0] = c + u * dx + v * dy;
					p[1] = c - u * dx + v * dy;
					p[2] = c - u * dx - v * dy;
					p[3] = c + u * dx - v * dy;
					dy = other.mFar * std::tan((vrGLMVec3::value_type)TO_RADIANS(other.fovy) / 2.0f);
					dx = other.ar * dy;
					c = other.origin - n * vrGLMVec3::value_type(other.mFar);
					p[4] = c + u * dx + v * dy;
					p[5] = c - u * dx + v * dy;
					p[6] = c - u * dx - v * dy;
					p[7] = c + u * dx - v * dy;
				}
				else {
					vrGLMVec3 c = other.origin - n * other.mNear;
					p[0] = c + u * vrGLMVec3::value_type(other.xmax) + v * vrGLMVec3::value_type(other.ymax);
					p[1] = c + u * other.xmax + v * other.ymin;
					p[2] = c + u * other.xmin + v * other.ymax;
					p[3] = c + u * other.xmin + v * other.ymin;
					c = other.origin - n * other.mFar;
					p[4] = c + u * other.xmax + v * other.ymax;
					p[5] = c + u * other.xmax + v * other.ymin;
					p[6] = c + u * other.xmin + v * other.ymax;
					p[7] = c + u * other.xmin + v * other.ymin;
				}

				// Adjust frustum to contain
				if (type == Projection::PERSPECTIVE) {
					fovy = 0.0f;
					mFar = 0.0f;
					mNear = std::numeric_limits<vrGLMVec3::value_type>::max();
					vrGLMVec3::value_type maxHorizAngle = 0.0f;
					for (int i = 0; i < 8; i++) {
						// Convert to local space
						vrGLMVec4 pt = m * vrGLMVec4(p[i], 1.0f);

						if (pt.z < 0.0f) {
							vrGLMVec3::value_type d = -pt.z;
							vrGLMVec3::value_type angle = std::atan(std::abs(pt.x) / d);
							if (angle > maxHorizAngle) maxHorizAngle = angle;
							angle = (vrGLMVec3::value_type)TO_DEGREES(std::atan(std::abs(pt.y) / d));
							if (angle * 2.0f > fovy) fovy = angle * 2.0f;
							if (mNear > d) mNear = d;
							if (mFar < d) mFar = d;
						}
					}
					vrGLMVec3::value_type h = (mNear * std::tan((vrGLMVec3::value_type)TO_RADIANS(fovy) / 2.0f)) * 2.0f;
					vrGLMVec3::value_type w = (mNear * std::tan(maxHorizAngle)) * 2.0f;
					ar = w / h;
				}
				else {
					xmin = ymin = mNear = std::numeric_limits<vrGLMVec3::value_type>::max();
					xmax = ymax = mFar = std::numeric_limits<vrGLMVec3::value_type>::min();
					for (int i = 0; i < 8; i++) {
						// Convert to local space
						vrGLMVec4 pt = m * vrGLMVec4(p[i], 1.0);
						if (xmin > pt.x) xmin = pt.x;
						if (xmax < pt.x) xmax = pt.x;
						if (ymin > pt.y) ymin = pt.y;
						if (ymax < pt.y) ymax = pt.y;
						if (mNear > -pt.z) mNear = -pt.z;
						if (mFar < -pt.z) mFar = -pt.z;
					}
				}

			}

			vrGLMMat4 vrFrustum::getViewMatrix() const
			{
				return glm::lookAt(origin, at, up);
			}

			vrGLMMat4 vrFrustum::getProjectionMatrix() const
			{
				if (type == Projection::PERSPECTIVE)
					return glm::perspective(fovy, ar, mNear, mFar);
				else
					return glm::ortho(xmin, xmax, ymin, ymax, mNear, mFar);
			}

			vrGLMVec3 vrFrustum::getOrigin() const
			{
				return this->origin;
			}

			vrGLMVec3 vrFrustum::getCenter() const
			{
				vrGLMVec3::value_type dist = (mNear + mFar) / 2.0f;
				vrGLMVec3 r = glm::normalize(at - origin);

				return origin + (r * dist);
			}

			void vrFrustum::printInfo() const
			{
				if (type == Projection::PERSPECTIVE) {
					printf("Perspective:  fovy = %f  ar = %f  near = %f  far = %f\n",
						fovy, ar, mNear, mFar);
				}
				else {
					printf("Orthographic: x(%f,%f) y(%f,%f) near = %f far = %f\n",
						xmin, xmax, ymin, ymax, mNear, mFar);
				}
				printf("   Origin = (%f, %f, %f)  at = (%f, %f, %f) up = (%f, %f, %f)\n",
					origin.x, origin.y, origin.z, at.x, at.y, at.z, up.x, up.y, up.z);
			}

			void vrFrustum::render() const
			{
				if (type == Projection::PERSPECTIVE) {
					static vrGLMVec3::value_type vert[9 * 3];
					static vrGLMVec3 p[8];

					vert[0] = origin.x;
					vert[1] = origin.y;
					vert[2] = origin.z;

					vrGLMVec3 n = glm::normalize(this->origin - this->at);
					vrGLMVec3 u = glm::normalize(glm::cross(this->up, n));
					vrGLMVec3 v = glm::normalize(glm::cross(n, u));

					vrGLMVec3::value_type dy = mNear * std::tan((vrGLMVec3::value_type)TO_RADIANS(fovy) / 2.0f);
					vrGLMVec3::value_type dx = ar * dy;
					vrGLMVec3 c = origin - n * mNear;  // Center of near plane
					p[0] = c + u * dx + v * dy;
					p[1] = c - u * dx + v * dy;
					p[2] = c - u * dx - v * dy;
					p[3] = c + u * dx - v * dy;
					dy = mFar * std::tan((vrGLMVec3::value_type)TO_RADIANS(fovy) / 2.0f);
					dx = ar * dy;
					c = origin - n * mFar;      // Center of far plane
					p[4] = c + u * dx + v * dy;
					p[5] = c - u * dx + v * dy;
					p[6] = c - u * dx - v * dy;
					p[7] = c + u * dx - v * dy;

					vrInt idx = 3;
					for (vrInt i = 0; i < 8; i++) {
						vert[idx++] = p[i].x;
						vert[idx++] = p[i].y;
						vert[idx++] = p[i].z;
					}

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
					glBufferData(GL_ARRAY_BUFFER, 9 * 3 * sizeof(vrGLMVec3::value_type), vert, GL_DYNAMIC_DRAW);
					glVertexAttribPointer(0, 3, VR_GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);  // Vertex position

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
					glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
				}
			}
#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR