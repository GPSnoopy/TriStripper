
set(exe_name TriStripper.Test)

set(src_files
	fileio.h
	gl_mesh.cpp
	gl_mesh.h
	gl_model.cpp
	gl_model.h
	gl_renderer.cpp
	gl_renderer.h
	high_res_timer.cpp
	high_res_timer.h
	main.cpp
	varrays_normalizer.cpp
	varrays_normalizer.h
)

source_group("Source" FILES ${src_files})

include_directories(../include)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR})

add_executable(${exe_name} ${src_files})
set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_link_libraries(${exe_name} PRIVATE "TriStripper" ${GLUT_LIBRARY} ${OPENGL_LIBRARIES})
add_definitions(-D FREEGLUT_STATIC)

set_target_properties(${exe_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
