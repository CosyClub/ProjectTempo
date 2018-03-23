message ("START ==========================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")

cmake_minimum_required(VERSION 2.8.12)
project("ProjectTempo")
set(GLOBAL PROPERTY USE_FOLDERS ON)

set(EXTLIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/extlibs")

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
)

#Support YCM completion for Raef and Anthony
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# :TODO: have some way of configuring this
set(CMAKE_BUILD_TYPE Debug)

cmake_host_system_information(RESULT TEMPO_MACHINE_HOSTNAME QUERY HOSTNAME)
if(${TEMPO_MACHINE_HOSTNAME} MATCHES "it[0-9]+.users.bris.ac.uk")
	message("*** WE ARE BUILDING ON A LAB MACHINE ***")
	set(TEMPO_BUILD_LAB ON)
endif()

set(TEMPO_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")
set(TEMPO_BIN_DIR "${CMAKE_CURRENT_LIST_DIR}/bin")
set(TEMPO_EXTLIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/extlibs")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY         "${TEMPO_LIB_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_LIB_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${TEMPO_LIB_DIR}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY         "${TEMPO_LIB_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_LIB_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${TEMPO_LIB_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${TEMPO_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TEMPO_BIN_DIR}")

set(TEMPO_SFML_DIR
	"${TEMPO_EXTLIBS_DIR}/SFML-2.4.2"
)
set(TEMPO_SFML_INCLUDE_DIR
	"${TEMPO_SFML_DIR}/include"
)

set(TEMPO_STB_INCLUDE_DIR
	"${TEMPO_EXTLIBS_DIR}/stb/"
)

set(TEMPO_LIB_TEMPO_DIR
	"${CMAKE_CURRENT_LIST_DIR}/src/lib-tempo"
	)
set(TEMPO_LIB_TEMPO_INCLUDE_DIR
	"${TEMPO_LIB_TEMPO_DIR}/include"
	)

set(TEMPO_ANAX_DIR
	"${CMAKE_CURRENT_LIST_DIR}/extlibs/anax-1.1.1"
	)
set(TEMPO_ANAX_INCLUDE_DIR
	"${TEMPO_ANAX_DIR}/include"
	)

set(TEMPO_GLM_DIR
	"${CMAKE_CURRENT_LIST_DIR}/extlibs/glm"
	)
set(TEMPO_GLM_INCLUDE_DIR
	"${TEMPO_GLM_DIR}"
)

set(TEMPO_IRRLICHT_DIR
	"${CMAKE_CURRENT_LIST_DIR}/extlibs/irrlicht-1.8.4"
)
set(TEMPO_IRRLICHT_INCLUDE_DIR
	"${TEMPO_IRRLICHT_DIR}/include"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# irrlicht _requires_ gnu++11 instead of c++11 so there goes msvc compatibility
	set(CMAKE_CXX_FLAGS "-std=gnu++11 -Wall -pipe -fstrict-aliasing") # reset polluted flags
elseif (MSVC)
	# GR for rtti
	# weXXX for warnings to errors
	# woXXX for report first warning only
	# Gm for mimimal rebuild
	# MP for multi process build
	# INCREMENTAL for enable incremental compilation
	set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc") # reset polluted flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /INCREMENTAL /ZI")
endif ()
message ("OPTIONS BEFORE EXTLIBS =========================================================")
message ("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message ("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message ("EXTLIBS ========================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
add_subdirectory(${TEMPO_EXTLIBS_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# GNU Flags for the entire project added here
	set(SRC_FLAGS "-Werror")
elseif (MSVC)
	string(REGEX REPLACE "/Wall" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REGEX REPLACE "/EHs-c-" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REGEX REPLACE "/GR-" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()
message ("OPTIONS BEFORE SRC =============================================================")
message ("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message ("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message ("SRC ============================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
add_subdirectory(src)


if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" OR CMAKE_CL_64)
	file(COPY "${TEMPO_EXTLIBS_DIR}/SFML-2.4.2/extlibs/bin/x64/openal32.dll" DESTINATION "${TEMPO_BIN_DIR}")
else ()
	file(COPY "${TEMPO_EXTLIBS_DIR}/SFML-2.4.2/extlibs/bin/x86/openal32.dll" DESTINATION "${TEMPO_BIN_DIR}")
endif ()

add_custom_target(copy_compile_commands ALL)
add_custom_command(
	TARGET copy_compile_commands POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_BINARY_DIR}/compile_commands.json
		${CMAKE_CURRENT_LIST_DIR}/compile_commands.json)

message ("END ============================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
