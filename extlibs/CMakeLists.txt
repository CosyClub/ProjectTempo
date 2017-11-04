##########################################
# Ogre
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf ogre-1.10.9.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Configure Build
execute_process(COMMAND cmake
	"-DCMAKE_INSTALL_PREFIX=${TEMPO_LIB_DIR}"
	"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
	"-DOGRE_LIB_DIRECTORY=${TEMPO_LIB_DIR}"
	-DOGRE_CONFIG_THREAD_PROVIDER=std
	-DOGRE_USE_STD11=ON
	-DOGRE_BUILD_DEPENDENCIES=ON
	-DOGRE_BUILD_COMPONENT_PYTHON=OFF
	-DOGRE_BUILD_COMPONENT_JAVA=OFF
	-DOGRE_BUILD_COMPONENT_BITES=OFF
	-DOGRE_BUILD_COMPONENT_TERRAIN=OFF
	-DOGRE_BUILD_COMPONENT_PAGING=OFF
	-DOGRE_BUILD_RENDERSYSTEM_GLES2=OFF
	-DOGRE_BUILD_RENDERSYSTEM_Direct3D9=OFF
	-DOGRE_BUILD_RENDERSYSTEM_Direct3D11=OFF
	-DOGRE_BUILD_SAMPLES=OFF
	-DOGRE_INSTALL_SAMPLES=OFF
	-DOGRE_BUILD_TOOLS=OFF
	-DOGRE_INSTALL_TOOLS=OFF
	-DOGRE_UNITY_BUILD=ON
	-DBUILD_SHARED_LIBS=OFF
	-DOGRE_STATIC=ON
	-G ${CMAKE_GENERATOR}
	"${TEMPO_EXTLIBS_DIR}/ogre-1.10.9/"
	WORKING_DIRECTORY "${TEMPO_EXTLIBS_DIR}/ogre-1.10.9/"
)
# Build Ogre
execute_process(COMMAND cmake --build
	"${TEMPO_EXTLIBS_DIR}/ogre-1.10.9/"
)

##########################################
# FreeImage
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf FreeImage-3.17.0.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
add_subdirectory(FreeImage-3.17.0)

##########################################
# SFML
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf SFML-2.4.2.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
add_subdirectory(SFML-2.4.2)
