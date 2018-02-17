link_directories("${TEMPO_LIB_DIR}")

add_executable(irrlicht_test
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(irrlicht_test PRIVATE ${TEMPO_IRRLICHT_INCLUDE_DIR})
target_include_directories(irrlicht_test PRIVATE ${TEMPO_SFML_INCLUDE_DIR}    )


set_target_properties(irrlicht_test PROPERTIES OUTPUT_NAME "IrrlichtTest")

target_link_libraries(irrlicht_test "Irrlicht")

target_include_directories(irrlicht_test PRIVATE ${TEMPO_SDL2_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
target_link_libraries(irrlicht_test ${OPENGL_gl_LIBRARY} )
target_link_libraries(irrlicht_test ${OPENGL_glu_LIBRARY})

if(UNIX)
	target_link_libraries(irrlicht_test z)
	target_link_libraries(irrlicht_test X11)
	target_link_libraries(irrlicht_test Xxf86vm)
endif(UNIX)

target_link_libraries(irrlicht_test sfml-system)
