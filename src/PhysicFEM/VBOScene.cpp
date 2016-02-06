#include "VBOScene.h"
#include "vrGlobalConf.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "OpenGLHelper/vrglutils.h"
#include "VR_Geometry_MeshDataStruct.h"
#include "vrLog.h"
#include <gl/freeglut.h>
namespace VR
{
	namespace App
	{
		namespace OpenGL
		{
#if USE_GLUT	

			VBOScene::VBOScene(bool showFPS, vrFloat g_FOV, vrFloat g_near, vrFloat g_far
				, const vrGLMVec3& _lightPos
				, const vrGLMVec3& _camerPos
				, const vrGLMVec3& _eyeCenter
				, const vrGLMVec3& _zUp
#if SHOW_SHADOWMAP
				, int _shadowMapWidth
				, int _shadowMapHeight
#endif
				)
				:m_trackball(0.5f, VR::Interactive::unitquaternion(VR::Interactive::vrAngleAxis(VR::Cgmath::DegToRad(0), VR::vrVec3(0, 1, 0))))
				, FVOY(g_FOV), MyFar(g_far), MyNear(g_near)
				, lightPos(_lightPos)//(-0.8f, 0.825f, 0.75f),
				, eyeCenter(_eyeCenter)//(0.0f,0.1f,0.0f),
				, zUp(_zUp)//(0.0f,1.f,0.0f),
				, camerPos(_camerPos)//(0.0f,0.05f,0.85f)
#if SHOW_SHADOWMAP
				, shadowMapWidth(_shadowMapWidth), shadowMapHeight(_shadowMapHeight)
#endif
			{
				angle = 0.f;
				m_TexRender.BuildFont();
			}

			VBOScene::~VBOScene(void)
			{
			}

			void VBOScene::initScene()
			{
				

				compileAndLinkShader();

				
				glClearColor(GlobalConf::g_vec4_Scene_bkgColor.x(), 
							 GlobalConf::g_vec4_Scene_bkgColor.y(),
							 GlobalConf::g_vec4_Scene_bkgColor.z(), 
							 GlobalConf::g_vec4_Scene_bkgColor.w());
				glEnable(GL_DEPTH_TEST);

				m_planeColor = make_glm_vec3(GlobalConf::g_vec3_Scene_planeColor); 
				m_modelColor = make_glm_vec3(GlobalConf::g_vec3_Scene_modelColor); 
				m_modelColor4Ka = make_glm_vec3(GlobalConf::g_vec3_Scene_modelColor4Ka);

				
				m_trackball.SetColor(RGB(130, 80, 30));
				m_trackball.SetDrawConstraints();

				m_scene_ObjMesh = DrawVBOMeshPtr(new DrawVBOMesh(true, false,false));
				Geometry::MeshDataStructPtr tmp_objInfo = Geometry::MeshDataStructPtr(new Geometry::MeshDataStruct);
				tmp_objInfo->loadOBJ(vrCStr(GlobalConf::g_str_Obj_meshName), false, true);
				m_scene_ObjMesh->loadOBJ(tmp_objInfo);
				m_scene_OctreeGrid = DrawVBOLineSetPtr(new DrawVBOLineSet);
#if USE_FEM

				YC::Geometry::MeshDataStruct tmp_objInfo;
				//tmp_objInfo.loadPLY(_PLY_NAME);
				tmp_objInfo.loadOBJ(GlobalVariable::g_strMeshPath.c_str(), false);
				m_scene_ObjMesh->loadOBJ(tmp_objInfo);

				//obj_grid_armadillo_unify_grid_level6_classic
#if CellLevel < 5
				const int nCellCount = sizeof(armadillo_level4) / sizeof(armadillo_level4[0]);
				m_physicalSimulation.initialize(armadillo_level4, nCellCount, tmp_objInfo);
#else
				const int nCellCount = sizeof(obj_grid_armadillo_unify_grid_level6_classic) / sizeof(obj_grid_armadillo_unify_grid_level6_classic[0]);
				m_physicalSimulation.initialize(obj_grid_armadillo_unify_grid_level6_classic, nCellCount, tmp_objInfo);
#endif



				{
					const int nLineCount = nCellCount * 12 * _nExternalMemory;
					float * pos = new float[nLineCount * 2 * 3];
					int * idx = new int[nLineCount * 2];
					memset(pos, '\0', nLineCount * 2 * 3 * sizeof(float));
					memset(idx, '\0', nLineCount * 2 * sizeof(int));
					m_scene_OctreeGrid->initialize(nLineCount, pos, idx);
					m_scene_OctreeGrid->setLineCount(0);
					delete[] pos;
					delete[] idx;
				}
				glLineWidth(0.5f);
#endif

#if SHOW_SHADOWMAP

				
				m_scene_plane = DrawVBOPlanePtr(new DrawVBOPlane(GlobalConf::g_db_Scene_planeXsize, GlobalConf::g_db_Scene_planeZsize, GlobalConf::g_n_Scene_planeXdivs, GlobalConf::g_n_Scene_planeZdivs));
				vrFloat scale = 2.0;

				setupFBO();

				
				GLuint programHandle = prog.getHandle();
				pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
				pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");
				
				shadowBias = vrGLMMat4(vrGLMVec4(0.5, 0.0, 0.0, 0.0),
					vrGLMVec4(0.0, 0.5, 0.0, 0.0),
					vrGLMVec4(0.0, 0.0, 0.5, 0.0),
					vrGLMVec4(0.5, 0.5, 0.5, 1.0)
					);

				lightFrustum = vrFrustumPtr(new vrFrustum(Projection::PERSPECTIVE));

				
				lightFrustum->orient(lightPos, eyeCenter, zUp);
				lightFrustum->setPerspective(FVOY, (vrFloat)1.0, (vrFloat)1.0, (vrFloat)25.0);
				lightPV = shadowBias * lightFrustum->getProjectionMatrix() * lightFrustum->getViewMatrix();
				
				prog.setUniform("Light.Intensity", vrGLMVec3(0.5));
				
				prog.setUniform("ShadowMap", 0);
#else
#endif
				
#if SHOWFPS
				timer.sdkCreateTimer();
				//cutilCheckError(sdkCreateTimer(&timer));
#endif
			}
			void VBOScene::update(vrFloat t){}

			void VBOScene::render()
			{
				
				OpenGL::vrGLUtils::checkForOpenGLError(__FILE__, __LINE__);
				prog.use();
#if SHOWFPS
				timer.sdkStartTimer();
				//cutilCheckError(sdkStartTimer(&timer));
				glBindTexture(GL_TEXTURE_2D, m_depthTex);
#endif

#if USE_FEM

#if USE_ModalWrap
				m_physicalSimulation.simulationOnCPU_Global_WithModalWrap(m_nTimeStep++);
#else
				m_physicalSimulation.simulationOnCPU(m_nTimeStep++);
#endif


				m_scene_ObjMesh->loadDisplacementOBJ(m_physicalSimulation.getObjMesh());

				const int nCellCount = m_physicalSimulation.getCellSize();
				const int nLineCount = nCellCount * 12;
				float3* pos = new float3[nLineCount * 2];
				int2* index_Lines = new int2[nLineCount];
				memset(pos, '\0', sizeof(float3)*nLineCount * 2);
				memset(index_Lines, '\0', sizeof(int2)*nLineCount);

				m_physicalSimulation.generateDisplaceLineSet(&pos, &index_Lines);
				m_scene_OctreeGrid->updateLineSet(nLineCount, pos, index_Lines);
				m_nTimeStep = m_nTimeStep & (128 - 1);

#endif
				// Pass 1 (shadow map generation)
				view = lightFrustum->getViewMatrix();
				projection = lightFrustum->getProjectionMatrix();
				glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowMapWidth, shadowMapHeight);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(2.5f, 10.0f);


				drawScene();


				glCullFace(GL_BACK);
				glDisable(GL_POLYGON_OFFSET_FILL);

				// Pass 2 (render)
				view = glm::lookAt(camerPos, eyeCenter, zUp);
				
				//view = glm::lookAt(cameraPos,vec3(0.0f),vec3(0.0f,1.0f,0.0f));
				prog.setUniform("Light.Position", view * vrGLMVec4(lightFrustum->getOrigin(), 1.0));
				projection = glm::perspective(FVOY, (vrFloat)aspect, MyNear, MyFar);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, width, height);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
				glDisable(GL_CULL_FACE);
				drawScene();

#if SHOWFPS
				timer.sdkStopTimer();// cutilCheckError(sdkStopTimer(&timer));
				computeFPS();
				//glutSwapBuffers();
#endif
				
				glutSwapBuffers();
			}

			void VBOScene::drawScene()
			{
				showMesh();
				showPlane();
				model = vrGLMMat4(1.0);
			}

			void VBOScene::showMesh()
			{
				prog.setUniform(PREFIX"Ka", m_modelColor4Ka);
				prog.setUniform(PREFIX"Kd", m_modelColor);
				prog.setUniform(PREFIX"Ks", vrGLMVec3(0.35, 0.35, 0.35));
				prog.setUniform(PREFIX"Shininess", 180.0f);

				

				{
					vrMat3 rotMat = m_trackball.getRotation();
					model = vrGLMMat4(
						rotMat.coeff(0, 0), rotMat.coeff(1, 0), rotMat.coeff(2, 0), 0.,
						rotMat.coeff(0, 1), rotMat.coeff(1, 1), rotMat.coeff(2, 1), 0.0,
						rotMat.coeff(0, 2), rotMat.coeff(1, 2), rotMat.coeff(2, 2), 0.0,
						0.0, 0.0, 0.0, 1.0);
					model *= glm::scale((vrGLMMat4::value_type)0.5f, (vrGLMMat4::value_type)0.5f, (vrGLMMat4::value_type)0.5f);
				}
				setMatrices();
				//m_scene_test_teapot->render();
				m_scene_ObjMesh->render();
				//m_scene_OctreeGrid->render();
			}

			void VBOScene::showPlane()
			{
				//prog.setUniform("Kd", 0.7f, 0.5f, 0.3f);
				prog.setUniform(PREFIX"Kd", m_planeColor);
				prog.setUniform(PREFIX"Ks", 0.1, 0.1, 0.1);
				prog.setUniform(PREFIX"Ka", m_planeColor);
				prog.setUniform(PREFIX"Shininess", 1.0f);

				model = vrGLMMat4(1.0);
				model *= glm::translate(vrGLMVec3(0.0, -0.25, 0.0));
				//model *= glm::translate(vec3(0.0f,-0.45f,0.0f));
				setMatrices();
				m_scene_plane->render();
			}

			void VBOScene::resize(int w, int h)
			{
				glViewport(0, 0, w, h);
				width = w;
				height = h;

				aspect = ((vrFloat)width) / height;
				projection = glm::perspective(FVOY, (vrFloat)aspect, MyNear, MyFar);

				m_trackball.ClientAreaResize(VR::Interactive::ERect(0, 0, width, height));
#if SHOWFPS
				m_MyGLSLFont.resize(width, height);
#endif
			}

			void VBOScene::mouse(int button, int state, int x, int y)
			{

				if (state == GLUT_DOWN)
				{
					if (((GetKeyState(VK_CONTROL) & 0x80) > 0))
					{
						m_trackball.MouseDown(VR::Interactive::EPoint(x, y));
					}
				}
				else if (state == GLUT_UP)
				{
					m_trackball.MouseUp(VR::Interactive::EPoint(x, y));
				}
			}

			void VBOScene::motion(int x, int y)
			{
				if (GLUT_ACTIVE_CTRL == glutGetModifiers())
				{
					m_trackball.MouseMove(VR::Interactive::EPoint(x, y));
				}
				
			}

			void printMat(glm::mat4  mat){
				int i, j;
				for (j = 0; j < 4; j++){
					for (i = 0; i < 4; i++){
						printf("%f ", mat[i][j]);
					}
					printf("\n");
				}
			}

			void VBOScene::setMatrices()
			{
				vrGLMMat4 mv = view * model;
				prog.setUniform("ModelViewMatrix", mv);
				prog.setUniform("NormalMatrix",
					vrGLMMat3(vrGLMVec3(mv[0]), vrGLMVec3(mv[1]), vrGLMVec3(mv[2])));
				prog.setUniform("MVP", projection * mv);

#if SHOW_SHADOWMAP
				prog.setUniform("ShadowMatrix", lightPV * model);
#endif
				
			}

#define VBOSceneStableFunction (1)
#if VBOSceneStableFunction
			void VBOScene::compileAndLinkShader()
			{
#if SHOW_SHADOWMAP
				if (!prog.compileShaderFromFile("./shaders/pcf.vs", GLSLShader::VERTEX))
				{
					errorLog << string_format("Vertex shader failed to compile!\n%s", prog.log().c_str());
					vrExit;
				}
				if (!prog.compileShaderFromFile("./shaders/pcf.fs", GLSLShader::FRAGMENT))
				{
					errorLog << string_format("Fragment shader failed to compile!\n%s", prog.log().c_str());
					vrExit;
				}
#endif//SHOW_SHADOWMAP

				if (!prog.link())
				{
					printf("Shader program failed to link!\n%s",
						prog.log().c_str());
					exit(1);
				}

				if (!prog.validate())
				{
					printf("Program failed to validate!\n%s",
						prog.log().c_str());
					exit(1);
				}

				prog.use();
			}

			void VBOScene::setupFBO()
			{
				GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
				// The depth buffer texture

				glGenTextures(1, &m_depthTex);
				glBindTexture(GL_TEXTURE_2D, m_depthTex);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth,
					shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

				// Assign the depth buffer texture to texture channel 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_depthTex);

				// Create and set up the FBO
				glGenFramebuffers(1, &shadowFBO);
				glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_TEXTURE_2D, m_depthTex, 0);

				GLenum drawBuffers[] = { GL_NONE };
				glDrawBuffers(1, drawBuffers);

				GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (result == GL_FRAMEBUFFER_COMPLETE) {
					printf("Framebuffer is complete.\n");
				}
				else {
					printf("Framebuffer is not complete.\n");
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			void VBOScene::computeFPS()
			{
				static int fpsCount = 0;
				static int fpsLimit = 50;
				fpsCount++;

				if (fpsCount == fpsLimit) {

					double ifps = 1.0 / (timer.sdkGetAverageTimerValue() / 1000000.0);
					//sprintf(VR_FEM::TexRender::fps, "Cuda GL Interop Wrapper: %3.1f fps ", ifps);  
					sprintf(vrTexRender::fps, "%3.1f fps ", ifps);

					//glutSetWindowTitle(TexRender::fps);
					fpsCount = 0;

					//cutilCheckError(sdkResetTimer(&timer));
				}

				m_MyGLSLFont.printTextOnGLSL(vrTexRender::fps);
			}
#endif//VBOSceneStableFunction

#endif//USE_GLUT
		}//OpenGL
	}//App
}//VR