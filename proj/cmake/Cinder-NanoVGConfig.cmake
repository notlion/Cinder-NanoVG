if( NOT TARGET Cinder-NanoVG )
	get_filename_component( Cinder_NanoVG_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE )

	list( APPEND Cinder_NanoVG_SOURCES
		${Cinder_NanoVG_PATH}/src/ci_nanovg.cpp
		${Cinder_NanoVG_PATH}/src/ci_nanovg_gl.cpp
		${Cinder_NanoVG_PATH}/src/SvgRenderer.cpp
	)
	add_library( Cinder-NanoVG ${Cinder_NanoVG_SOURCES} )

	if( NOT TARGET cinder )
	    include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
	    find_package( cinder REQUIRED PATHS
	        "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
	        "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()

	target_include_directories( Cinder-NanoVG PUBLIC "${Cinder_NanoVG_PATH}/include" )

	get_filename_component( NanoVG_PATH "${Cinder_NanoVG_PATH}/deps/nanovg/src" ABSOLUTE )
	add_library( NanoVG ${NanoVG_PATH}/nanovg.c )
	target_include_directories( NanoVG PUBLIC "${NanoVG_PATH}" )
	add_dependencies( Cinder-NanoVG NanoVG )

	target_link_libraries( Cinder-NanoVG PUBLIC NanoVG PRIVATE cinder)

endif()
