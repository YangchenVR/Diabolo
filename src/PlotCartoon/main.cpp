/* ============================================================================
 * Freetype GL - A C OpenGL Freetype engine
 * Platform:    Any
 * WWW:         http://code.google.com/p/freetype-gl/
 * ----------------------------------------------------------------------------
 * Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials proided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Nicolas P. Rougier.
 * ============================================================================
 */
#include <stdio.h>
#include <time.h>
#include "freetype-gl.h"
#include "shader.h"
#include "vertex-buffer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#ifndef _DEBUG
#include "soil.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "vrRotation.h"
#include "vrString.h"

//VR_FEM::CBallController g_trackball_2(0.5f, unitquaternion(DegToRad(1 * 180), myVector(0, 1, 0))*unitquaternion(DegToRad(-30 * 0), myVector(1, 0, 0)));
VR::Interactive::vrBallController g_trackball_1(0.5f, 
	VR::Interactive::unitquaternion(Eigen::AngleAxisf(VR::Cgmath::DegToRad(1 * 180), VR::vrVec3(0, 1, 0)))*
	VR::Interactive::unitquaternion(Eigen::AngleAxisf(VR::Cgmath::DegToRad(-30 * 0), VR::vrVec3(1, 0, 0))));

#if defined(__APPLE__)
    #include <Glut/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    //#include <GLUT/glut.h>
	#include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#define MY_DEFING (1)
#if MY_DEFING

#define ObjMeshPath "D:/PG2015MESH-okok/frame_"
//armadillo_surface_hex_mesh_level7_quads.obj
//"bunny_surface_hex_mesh_level7_quads.obj"
//#define QuadsMeshpath "D:/MyWorkspace/MyMesh/OBJ/For_Plot/armadillo_surface_hex_mesh_level7.obj"
VR::vrString QuadsMeshpath;
//#define MY_SHAPE GL_TRIANGLES
#define MY_SHAPE GL_QUADS
#include <vector>
#include <string>
using std::vector;
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;
using glm::vec4;
using namespace glm;

typedef struct { float x,y,z;} xyz;
typedef struct { float r,g,b,a;} rgba;
typedef struct { xyz position, normal; rgba color;} vertex;

const int nMaxFrameCount = 6140;

#define windowWidth (1024)
#define windowHeight (1024)


namespace YC
{
	
	namespace Geometry
	{
		struct MeshDataStruct
		{
			std::string fileName;
			float m_maxDiameter,m_translation_x,m_translation_y,m_translation_z;
			vector<glm::vec3>  points;
			vector<glm::vec3>  normals;
			vector<glm::vec2> texCoords;
			vector<glm::vec4> tangents;
			vector<int> faces;
			glm::vec3 handle;
			glm::vec3 tip;

			void trimString( std::string & str ) 
			{
				const char * whiteSpace = " \t\n\r";
				size_t location;
				location = str.find_first_not_of(whiteSpace);
				str.erase(0,location);
				location = str.find_last_not_of(whiteSpace);
				str.erase(location + 1);
			}

			float minIn3(float a, float b, float c)
			{
				return (a < b) ? 
					( a < c ? a : c ) :
					( b < c ? b : c);
			}

			float maxIn3(float a, float b, float c)
			{
				return (a > b) ? 
					(a > c ? a : c) : 
					(b > c ? b : c);
			}

			void MeshDataStruct::loadOBJ(const char* lpszFileName, bool loadTex)
			{
				//reCenterMesh(center), loadTex(loadTc), genTang(genTangents)
				using namespace std;

				vector <glm::vec3> normalizePoints;

				fileName = std::string(lpszFileName);
				int nFaces = 0;

				std::ifstream objStream( fileName, std::ios::in );

				if( !objStream ) {
					cerr << "Unable to open OBJ file: " << fileName << endl;
					exit(1);
				}

				string line, token;
				vector<int> face;

				getline( objStream, line );
				while( !objStream.eof() ) {
					trimString(line);
					if( line.length( ) > 0 && line.at(0) != '#' ) {
						istringstream lineStream( line );

						lineStream >> token;
						
						if (token == "v" ) {
							float x, y, z;
							lineStream >> x >> y >> z;
							points.push_back( glm::vec3(x,y,z) );
						}else if (token == "vt" && loadTex) {
							// Process texture coordinate
							float s,t;
							lineStream >> s >> t;
							texCoords.push_back( glm::vec2(s,t) );
						} else if (token == "vn" ) {
							float x, y, z;
							lineStream >> x >> y >> z;
							normals.push_back( glm::vec3(x,y,z) );
						} else if (token == "f" ) {
							nFaces++;

							// Process face
							face.clear();
							size_t slash1, slash2;
							//int point, texCoord, normal;
							while( lineStream.good() ) {
								string vertString;
								lineStream >> vertString;
								int pIndex = -1, nIndex = -1 , tcIndex = -1;

								slash1 = vertString.find("/");
								if( slash1 == string::npos ){
									pIndex = atoi( vertString.c_str() ) - 1;
								} else {
									slash2 = vertString.find("/", slash1 + 1 );
									pIndex = atoi( vertString.substr(0,slash1).c_str() )
										- 1;
									if( slash2 > slash1 + 1 ) {
										tcIndex =
											atoi( vertString.substr(slash1 + 1, slash2).c_str() )
											- 1;
									}
									nIndex =
										atoi( vertString.substr(slash2 + 1,vertString.length()).c_str() )
										- 1;
								}
								if( pIndex == -1 ) {
									printf("Missing point index!!!");
								} else {
									face.push_back(pIndex);
								}

								if( loadTex && tcIndex != -1 && pIndex != tcIndex ) {
									printf("Texture and point indices are not consistent.\n");
								}
								if ( nIndex != -1 && nIndex != pIndex ) {
									printf("Normal and point indices are not consistent.\n");
								}
							}
							// If number of edges in face is greater than 3,
							// decompose into triangles as a triangle fan.
							if (3 == face.size())
							{
								faces.push_back(face[0]);
								faces.push_back(face[1]);
								faces.push_back(face[2]);
							}
							else if (4 == face.size())
							{
								faces.push_back(face[0]);
								faces.push_back(face[1]);
								faces.push_back(face[2]);
								faces.push_back(face[3]);
							}
							else if( face.size() > 4 ) 
							{
								int v0 = face[0];
								int v1 = face[1];
								int v2 = face[2];
								// First face
								faces.push_back(v0);
								faces.push_back(v1);
								faces.push_back(v2);
								for( unsigned i = 3; i < face.size(); i++ ) {
									v1 = v2;
									v2 = face[i];
									faces.push_back(v0);
									faces.push_back(v1);
									faces.push_back(v2);
								}
							} else {
								/*faces.push_back(face[0]);
								faces.push_back(face[1]);
								faces.push_back(face[2]);*/
								printf("Error face.size()\n");
								exit(66);
							}
						}
					}
					else if (line.substr(0,8) == "#handle#"){
						string vertString = line.substr(8);
						//std::cout << vertString << std::endl;
						int sharp1 = vertString.find("#");
						handle[0] = atof(vertString.substr(0,sharp1).c_str());
						vertString = vertString.substr(sharp1+1);
						//std::cout << vertString << std::endl;
						int sharp2 = vertString.find("#");
						handle[1] = atof(vertString.substr(0,sharp2).c_str());
						vertString = vertString.substr(sharp2+1);
						//std::cout << vertString << std::endl;
						handle[2] = atof(vertString.c_str());
						//printf("handle {%f,%f,%f}\n",handle[0],handle[1],handle[2]);
						//system("pause");
					}else if (line.substr(0,5) == "#tip#"){
						string vertString = line.substr(5);
						//std::cout << vertString << std::endl;
						int sharp1 = vertString.find("#");
						tip[0] = atof(vertString.substr(0,sharp1).c_str());
						vertString = vertString.substr(sharp1+1);
						//std::cout << vertString << std::endl;
						int sharp2 = vertString.find("#");
						tip[1] = atof(vertString.substr(0,sharp2).c_str());
						vertString = vertString.substr(sharp2+1);
						//std::cout << vertString << std::endl;
						tip[2] = atof(vertString.c_str());
						//printf("handle {%f,%f,%f}\n",tip[0],tip[1],tip[2]);
						//system("pause");
					}
					getline( objStream, line );
				}

				objStream.close();

				if (false)
				{
					//normalize
					float rx,ry,rz;
					float xmin = FLT_MAX ,xmax = -FLT_MAX ,ymin = FLT_MAX ,ymax = -FLT_MAX ,zmin = FLT_MAX ,zmax = -FLT_MAX ;

					vector<glm::vec3> &vertices = points;
					vector<glm::vec3> &normalizeVertices = normalizePoints;
					const long verticesNum = vertices.size();
					for (int i = 0; i < verticesNum; ++i)
					{
						rx = vertices[i][0];
						if (xmin > rx)
						{
							xmin = rx;
						}
						if (xmax < rx)
						{
							xmax = rx;
						}

						ry = vertices[i][1];
						if (ymin > ry)
						{
							ymin = ry;
						}
						if (ymax < ry)
						{
							ymax = ry;
						}

						rz = vertices[i][2];
						if (zmin > rz)
						{
							zmin = rz;
						}
						if (zmax < rz)
						{
							zmax = rz;
						}

					}

					//printf("x : %f,%f \t  y : %f,%f \t  z : %f,%f \t",xmax,xmin,ymax,ymin,zmax,zmin);
					//MyPause;

					float maxDiameter,minDiameter,maxRadius,minRadius,X_diameter(xmax - xmin),Y_diameter(ymax - ymin),Z_diameter(zmax - zmin);
					float X_radius(X_diameter/2.f),Y_radius(Y_diameter/2.f),Z_radius(Z_diameter/2.f);
					float massCenter_x(xmin + X_diameter / 2.f),massCenter_y(ymin + Y_diameter/2.f),massCenter_z(zmin + Z_diameter/2.f);

					maxDiameter = maxIn3(X_diameter	, Y_diameter, Z_diameter);
					minDiameter = minIn3(X_diameter	, Y_diameter, Z_diameter);
					maxRadius   = maxIn3(X_radius	, Y_radius	, Z_radius);
					minRadius   = minIn3(X_radius	, Y_radius	, Z_radius);

					float translation_x(maxRadius-massCenter_x), translation_y(maxRadius-massCenter_y), translation_z(maxRadius-massCenter_z);


					float x_scale = 1.0 / (maxDiameter) ;
					float y_scale = 1.0 / (maxDiameter) ;
					float z_scale = 1.0 / (maxDiameter) ;
					//printf("maxDiameter is %f translation_x(%f,%f,%f) \n",maxDiameter,translation_x, translation_y, translation_z);
					m_maxDiameter = maxDiameter;
					m_translation_x = translation_x; 
					m_translation_y = translation_y;
					m_translation_z = translation_z;

					normalizeVertices.clear();
					for (int j=0;j< vertices.size() ; ++j)
					{
						glm::vec3& refPoint = vertices[j];
						normalizeVertices.push_back(glm::vec3( x_scale * (refPoint[0]+ translation_x) -0.5f,
							y_scale * (refPoint[1] + translation_y) -0.5f,
							z_scale * (refPoint[2] + translation_z) -0.5f));
					}

					//Q_ASSERT(g_strMeshId == std::string("armadillo"));
					points = normalizePoints;
				}

				/*if( normals.size() == 0 ) {
				generateAveragedNormals(points,normals,faces);
				}

				vector<vec4> tangents;
				if( genTang && texCoords.size() > 0 ) {
				generateTangents(points,normals,faces,texCoords,tangents);
				}

				if( reCenterMesh ) {
				center(points);
				}*/

				//storeVBO(points, normals, texCoords, tangents, faces);

				printMeshDataStructInfo(std::cout);
			}
			void printMeshDataStructInfo( std::ostream& out)
			{
				out << "points " << points.size() << "  faces " << faces.size() << std::endl;
			}
		};
	}
}

void loadPG2015ObjMesh(const char* lpszMeshPath, vertex_buffer_t ** cubePtr, glm::vec3& handle, glm::vec3& tip);
void loadPG2015ObjMesh_Quads(const char* lpszMeshPath, vertex_buffer_t ** cubePtr);
void unloadPG2015ObjMesh(vertex_buffer_t ** cubePtr);

#endif // MY_DEFINE

// ------------------------------------------------------- global variables ---
GLuint shader;
vertex_buffer_t * cube;
vertex_buffer_t * blade;



// ------------------------------------------------------------------- init ---
void init( void )
{
    glPolygonOffset( 1, 1 );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glEnable( GL_DEPTH_TEST ); 
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_LINE_SMOOTH );

	g_trackball_1.SetColor(RGB(130,80,30));
	g_trackball_1.SetDrawConstraints();

	//g_trackball_2.SetColor(RGB(130, 80, 30));
	//g_trackball_2.SetDrawConstraints();
}


// ---------------------------------------------------------------- display ---
void display( void )
{
	
    static GLuint Color = 0;
    float seconds_elapsed = (float)clock() / CLOCKS_PER_SEC;

    if( !Color )
    {
        Color = glGetUniformLocation( shader, "Color" );
    }
	
   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
	/*glRotatef(200,0,1,0);
	glRotatef(0,1,0,0); for armadillo*/
	/*glRotatef(20,0,1,0);
	glRotatef(25,1,0,0); // for bunny*/
	
	g_trackball_1.IssueGLrotation();
	//g_trackball_2.IssueGLrotation();
	glScalef(3.,3.,3.);
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_POLYGON_OFFSET_FILL );
    glUseProgram( shader );
    glUniform4f( Color, 1, 1, 1, 1 );
    vertex_buffer_render( cube, MY_SHAPE );
    glDisable( GL_POLYGON_OFFSET_FILL );
    glEnable( GL_BLEND );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDepthMask( GL_FALSE );
    glUniform4f( Color, 0, 0, 0, .5 );
    vertex_buffer_render( cube, MY_SHAPE );
	//vertex_buffer_render( blade, GL_LINES );
    glUseProgram( 0 );
    glDepthMask( GL_TRUE );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    glPopMatrix();

	

	
    glutSwapBuffers( );

	
}


// ---------------------------------------------------------------- reshape ---
void reshape( int width, int height )
{
    glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45.0, width/(float) height, 2.0, 10.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glTranslatef( 0.0, 0.0, -5.0 );

	g_trackball_1.ClientAreaResize(VR::Interactive::ERect(0,0,width,height));
	//g_trackball_2.ClientAreaResize(VR_FEM::ERect(0, 0, width, height));
	glutPostRedisplay();
}

void motion(int x, int y)
{
	if (((GetKeyState( VK_CONTROL ) & 0x80) > 0)  /*|| GLUT_ACTIVE_ALT == glutGetModifiers()*/)
	{
		g_trackball_1.MouseMove(VR::Interactive::EPoint(x, y));
		//g_trackball_2.MouseMove(VR_FEM::EPoint(x, y));
		//printf("motion (%d,%d)\n",x,y);
	}
	
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) 
	{
		if (((GetKeyState( VK_CONTROL ) & 0x80) > 0) )
		{
			//g_trackball.setCuttingTrack(false);
			g_trackball_1.MouseDown(VR::Interactive::EPoint(x, y));
			//g_trackball_2.MouseDown(VR_FEM::EPoint(x, y));
			//printf("mouse down(%d,%d)\n",x,y);
		}
	} 
	else if (state == GLUT_UP) 
	{
		g_trackball_1.MouseUp(VR::Interactive::EPoint(x, y));
		//g_trackball_2.MouseUp(VR_FEM::EPoint(x, y));
		//printf("mouse up(%d,%d)\n",x,y);
	}
	glutPostRedisplay();
}

void Specialkeyboard(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_LEFT:
		{
			g_trackball_1.Key(39);
			//g_trackball_2.Key(39);
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			g_trackball_1.Key(37);
			//g_trackball_2.Key(37);
			break;
		}
	case GLUT_KEY_UP:
		{
			g_trackball_1.Key(38);
			//g_trackball_2.Key(38);
			break;
		}
	case GLUT_KEY_DOWN:
		{
			g_trackball_1.Key(40);
			//g_trackball_2.Key(40);
			break;
		}
	default:
		{
			printf("Special key %d\n",key);
		}
	}
	glutPostRedisplay();
}

// --------------------------------------------------------------- keyboard ---
void keyboard( unsigned char key, int x, int y )
{
    if ( key == 27 )
    {
        exit( EXIT_SUCCESS );
    }
	else if ('p' == key || 'P' == key)
	{
#ifndef _DEBUG
		std::stringstream ss;
		VR::Interactive::myReal x, y, z, w;
		g_trackball_1.getRotationInfo(x,y,z,w);
		//g_trackball_2.getRotationInfo(x, y, z, w);
		ss << QuadsMeshpath << "_"<<x<< "_"<<y<< "_"<<z<< "_"<<w << ".bmp";
		SOIL_save_screenshot(ss.str().c_str(),	SOIL_SAVE_TYPE_BMP,	0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
#endif
		
	}

	
}


// ------------------------------------------------------------------ timer ---
void timer( int dt )
{
    glutPostRedisplay();
    glutTimerFunc( dt, timer, dt );
}

glm::vec3 plane_A[3]={glm::vec3(-0.002100,0.160000,1.000000), glm::vec3(0.270000,0.4500000,1.000000), glm::vec3(0.270000,0.4500000,-1.000000)};
glm::vec3 plane_B[3]={glm::vec3(-0.002100,0.160000,1.000000), glm::vec3(-0.2650000,0.500000,1.000000), glm::vec3(-0.2650000,0.500000,-1.000000)};
glm::vec3 planeA_normal, planeB_normal;
glm::vec3 computeNormal(const glm::vec3& x, const glm::vec3& y, const glm::vec3& z)
{
	return glm::cross(y - x, z-y);	
}
int ComputeDomainId(const glm::vec3 loc)
{
	if (loc[1] > 0.f)
	{
		//上半身
		if (loc[0] < -0.17f)
		{
			return 1;
		}
		else if (loc[0] > 0.18f)
		{
			return 5;
		}
		else 
		{
			bool flagA, flagB;
			if ( planeA_normal[0] * ( loc[0] - plane_A[0][0]) + planeA_normal[1] * (loc[1] - plane_A[0][1]) + planeA_normal[2] * (loc[2] - plane_A[0][2]) > 0.f )
			{
				flagA = true;
			}
			else
			{
				flagA = false;
			}

			if ( planeB_normal[0] * ( loc[0] - plane_B[0][0]) + planeB_normal[1] * (loc[1] - plane_B[0][1]) + planeB_normal[2] * (loc[2] - plane_B[0][2]) > 0.f )
			{
				flagB = true;
			}
			else
			{
				flagB = false;
			}

			if (flagA && !flagB)
			{
				return 3;
			}
			else
			{
				if (loc[0] < -0.0021f)
				{
					return 2;
				}
				else
				{
					return 4;
				}
			}
		}
	}
	else if (loc[1] <= 0.f)
	{
		//下半身
		if (-0.0021f <= loc[0])
		{
			return 6;
		}
		else if (-0.0021f > loc[0])
		{
			return 0;
		}
		else
		{
			printf("ERROR 0 %f \n", loc[0]); vrPause;
		}
	}
	else
	{
		printf("ERROR 1 %f \n", loc[1]); vrPause;
	}
}

class DefaultValue
{
public:
	DefaultValue():nCount(0){}
	int nCount;
};


void loadPG2015ObjMesh_Quads(const char* lpszMeshPath, vertex_buffer_t ** cubePtr)
{
	planeA_normal = computeNormal(plane_A[0], plane_A[1], plane_A[2]);
	planeB_normal = computeNormal(plane_B[0], plane_B[1], plane_B[2]);
	YC::Geometry::MeshDataStruct objMesh;
	objMesh.loadOBJ(lpszMeshPath,false);

	const int nVerticeSize = objMesh.points.size();
	const int nFaceSize = objMesh.faces.size();

	std::vector< xyz > v(nVerticeSize)/*,n(nVerticeSize)*/;
	std::vector< int  > color(nVerticeSize);
	xyz n[] = { { 0, 0, 1}};
	rgba c[]={{247/255.f, 233/255.f, 207/255.f, 1},//{49/255.f,57/255.f,174/255.f, 1},
	{108/255.f,112/255.f,184/255.f,1},//{138/255.f,166/255.f,120/255.f, 1},
	{221/255.f,42/255.f,43/255.f, 1},
	{218/255.f,207/255.f,47/255.f, 1},
	{147/255.f,180/255.f,125/255.f, 1},
	{223/255.f,143/255.f,135/255.f, 1},
	{205.f/255.f,205.f/255.f,205.f/255.f, 1}}; //肉粉 
	//rgba c[]={{1.f, 0.f, 0.f, 1},{0.f, 1.f, 0.f, 1},{0.f, 0.f, 1.f, 1},{1.f, 1.f, 0.f, 1},{0.f, 1.f, 1.f, 1},{1.f, 0.f, 1.f, 1},{1.f, 0.4f, 0.f, 1}}; //肉粉 
	//rgba c[]={{1.f, 0.4f, 0.f, 1}};
	std::vector< vertex > vertices(nFaceSize);
	std::vector< GLuint > indices(nFaceSize);


	for (int i=0;i<nVerticeSize;++i)
	{		
		v[i].x = objMesh.points[i][0];
		v[i].y = objMesh.points[i][1];
		v[i].z = objMesh.points[i][2];

		color[i] = ComputeDomainId(glm::vec3(v[i].x,v[i].y,v[i].z));

		color[i] = 0;
	}

	std::map< int,std::vector< int > > mapColorGroup;

	for (int i=0;i<nFaceSize;)
	{
		std::map< int, DefaultValue >  mapmap; 
		for (int j=0;j<24;++j)
		{
			mapmap[ color[objMesh.faces[i+j]] ].nCount++;
		}

		std::map< int, DefaultValue >::const_iterator ci = mapmap.begin(); 
		int nMaxCount = -1;
		int nColorId;
		for (;ci != mapmap.end();++ci)
		{
			if (ci->second.nCount > nMaxCount)
			{
				nColorId = ci->first;
				nMaxCount = ci->second.nCount;
			}
		}

		for (int j=0;j<24;++j,++i)
		{
			vertices[i].position = v[ objMesh.faces[i] ];
			vertices[i].normal = n[0];		
			vertices[i].color = c[nColorId];
			indices[i] = i;

			mapColorGroup[nColorId].push_back(objMesh.faces[i]);
		}
	}

	(*cubePtr) = vertex_buffer_new( "vertex:3f,normal:3f,color:4f" );
	vertex_buffer_push_back( *cubePtr, &vertices[0], vertices.size(), &indices[0], indices.size() );

	{
		//output group Quads obj mesh
		std::stringstream ss; 
		ss << lpszMeshPath << ".group.obj";
		std::ofstream outfile(ss.str().c_str());

		const int nVerticeSize = objMesh.points.size();
		const int nFaceSize = objMesh.faces.size();

		for (int v=0;v<nVerticeSize;++v)
		{
			const glm::vec3& ref_pos = objMesh.points[v];
			outfile << "v " << ref_pos[0] << " " << ref_pos[1] << " " << ref_pos[2] << std::endl;
		}
		outfile << "# vertex count is " << nVerticeSize << std::endl << std::endl;

		printf("#Group Count [%d]#\n",mapColorGroup.size());
		int nQuadCount = 0;
		std::map< int,std::vector< int > >::const_iterator ci_map = mapColorGroup.begin();
		std::map< int,std::vector< int > >::const_iterator endc_map = mapColorGroup.end();
		for (;ci_map != endc_map; ++ci_map)
		{
			const int nGroupId = ci_map->first;
			outfile << "g sub_" << nGroupId << std::endl;
			const std::vector< int >& refVec = ci_map->second;

			for (int i=0;i<refVec.size();)
			{
				for (int j=0;j<6;++j)
				{
					outfile << "f " << refVec[i+0]+1 << " " << refVec[i+1]+1 << " " << refVec[i+2]+1 << " " << refVec[i+3]+1 << std::endl;
					i += 4;
				}
			}

			nQuadCount += (refVec.size() / 4);
		}
		outfile.close();
	}
}

void loadPG2015ObjMesh(const char* lpszMeshPath, vertex_buffer_t ** cubePtr, glm::vec3& handle, glm::vec3& tip)
{
	YC::Geometry::MeshDataStruct objMesh;
	objMesh.loadOBJ(lpszMeshPath,false);

	handle = objMesh.handle;
	tip = objMesh.tip;

	const int nVerticeSize = objMesh.points.size();
	const int nFaceSize = objMesh.faces.size();
	
	std::vector< xyz > v(nVerticeSize)/*,n(nVerticeSize)*/;
	xyz n[] = { { 0, 0, 1}};
	rgba c[]={{1.f, 0.6666f, 0.6666f, 1}};

	std::vector< vertex > vertices(nFaceSize);
	std::vector< GLuint > indices(nFaceSize);


	for (int i=0;i<nVerticeSize;++i)
	{		
		v[i].x = objMesh.points[i][0];
		v[i].y = objMesh.points[i][1];
		v[i].z = objMesh.points[i][2];
	}

	for (int i=0;i<nFaceSize;++i)
	{
		vertices[i].position = v[ objMesh.faces[i] ];
		vertices[i].normal = n[0];
		vertices[i].color = c[0];
		indices[i] = i;
	}

	(*cubePtr) = vertex_buffer_new( "vertex:3f,normal:3f,color:4f" );
	vertex_buffer_push_back( *cubePtr, &vertices[0], vertices.size(), &indices[0], indices.size() );
	/*xyz v[] = { { 1, 1, 1},  {-1, 1, 1},  {-1,-1, 1}, { 1,-1, 1},
	{ 1,-1,-1},  { 1, 1,-1},  {-1, 1,-1}, {-1,-1,-1} };
	xyz n[] = { { 0, 0, 1},  { 1, 0, 0},  { 0, 1, 0} ,
	{-1, 0, 1},  { 0,-1, 0},  { 0, 0,-1} };
	rgba c[] = { {1, 1, 1, 1},  {1, 1, 0, 1},  {1, 0, 1, 1},  {0, 1, 1, 1},
	{1, 0, 0, 1},  {0, 0, 1, 1},  {0, 1, 0, 1},  {0, 0, 0, 1} };
	vertex vertices[24] =  {
		{v[0],n[0],c[0]}, {v[1],n[0],c[1]}, {v[2],n[0],c[2]}, {v[3],n[0],c[3]},
		{v[0],n[1],c[0]}, {v[3],n[1],c[3]}, {v[4],n[1],c[4]}, {v[5],n[1],c[5]},
		{v[0],n[2],c[0]}, {v[5],n[2],c[5]}, {v[6],n[2],c[6]}, {v[1],n[2],c[1]},
		{v[1],n[3],c[1]}, {v[6],n[3],c[6]}, {v[7],n[3],c[7]}, {v[2],n[3],c[2]},
		{v[7],n[4],c[7]}, {v[4],n[4],c[4]}, {v[3],n[4],c[3]}, {v[2],n[4],c[2]},
		{v[4],n[5],c[4]}, {v[7],n[5],c[7]}, {v[6],n[5],c[6]}, {v[5],n[5],c[5]} };
		GLuint indices[24] = { 0, 1, 2, 3,    4, 5, 6, 7,   8, 9,10,11,
			12,13,14,15,  16,17,18,19,  20,21,22,23 };

		cube = vertex_buffer_new( "vertex:3f,normal:3f,color:4f" );
		vertex_buffer_push_back( cube, vertices, 24, indices, 24 );*/
}

void unloadPG2015ObjMesh(vertex_buffer_t ** cubePtr)
{
	vertex_buffer_delete(*cubePtr);
}
// ------------------------------------------------------------------- main ---
int main( int argc, char **argv )
{
	using namespace VR;
	{
		vrString inifile = FileSystem::get_currentpath() + vrString("/conf/plotCartoon.conf");
		ConfigureParser::vrPropertyMap pm;
		pm[ConfigureParser::makeKey("Mesh","Type")];
		pm[ConfigureParser::makeKey("Mesh", "Path")];
		ConfigureParser::parser_configurefile(inifile, pm);
		ConfigureParser::getConfPropertyValueStr(pm, ConfigureParser::makeKey("Mesh", "Path"), QuadsMeshpath);

		int type; 
		ConfigureParser::getConfPropertyValue(pm, ConfigureParser::makeKey("Mesh", "Type"), type);
		std::cout << type << std::endl
			<< QuadsMeshpath << std::endl;
		
	}

    glutInit( &argc, argv );
    glutInitWindowSize( windowWidth,windowHeight );
	
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutCreateWindow( argv[0] );
    glutReshapeFunc( reshape );
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutSpecialFunc(Specialkeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
    //glutTimerFunc( 1000/60, timer, 1000/60 ); // not working on some systems (bug in GLUT)
    glutIdleFunc(display);

	//glutFullScreen();

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf( stderr, "Error: %s\n", glewGetErrorString(err) );
        exit( EXIT_FAILURE );
    }
    fprintf( stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION) );


    
	loadPG2015ObjMesh_Quads(vrCStr(QuadsMeshpath), &cube);
    shader = shader_load("shaders/cube.vert","shaders/cube.frag");

    init( );
    glutMainLoop( );
    return EXIT_SUCCESS;
}
