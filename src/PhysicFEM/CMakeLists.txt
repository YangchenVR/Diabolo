#Diabolo/src/Base/PhysicFEM
FILE (GLOB_RECURSE PhysicFEM_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)

ADD_SUBDIRECTORY(OpenGLHelper)
#string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_lower )
#if (CMAKE_BUILD_TYPE_lower STREQUAL "debug")
#    SET (module_BIN ${PROJECT_NAME}_Base_Debug)
#else (CMAKE_BUILD_TYPE_lower STREQUAL "debug")
#	SET (module_BIN ${PROJECT_NAME}_Base_Release)
#endif (CMAKE_BUILD_TYPE_lower STREQUAL "debug") 
SET (PhysicFEM_NAME ${PROJECT_NAME}_PhysicFEM)
ADD_EXECUTABLE(${PhysicFEM_NAME} ${PhysicFEM_SRCS})
SET (PhysicFEM_LIBS ${Boost_LIBRARIES} ${PROJECT_NAME}_Base ${GLSDK_LIBRARIES})
TARGET_LINK_LIBRARIES(${PhysicFEM_NAME} ${PhysicFEM_LIBS})
add_dependencies(${PhysicFEM_NAME} ${PROJECT_NAME}_Base)

SET_TARGET_PROPERTIES(${PhysicFEM_NAME} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${PhysicFEM_NAME} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${PhysicFEM_NAME} DESTINATION lib)
