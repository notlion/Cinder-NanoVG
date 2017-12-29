if( NOT TARGET Cinder-OpenCV3 )
	get_filename_component( NanoVG_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../deps/nanovg/src" ABSOLUTE )
	get_filename_component( Cinder-NanoVG_INC_PATH "${CMAKE_CURRENT_LIST_DIR}/../../include" ABSOLUTE )
	get_filename_component( Cinder-NanoVG_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )
	
	set( NanoVG_SOURCES
		${NanoVG_SOURCE_PATH}/fontstash.h
		${NanoVG_SOURCE_PATH}/nanovg_gl_utils.h
		${NanoVG_SOURCE_PATH}/nanovg_gl.h
		${NanoVG_SOURCE_PATH}/nanovg.h
		${NanoVG_SOURCE_PATH}/nanovg.c
		${NanoVG_SOURCE_PATH}/stb_image.h
		${NanoVG_SOURCE_PATH}/stb_truetype.h
	)

	source_group(NanoVG FILES ${NanoVG_SOURCES})

	set( Cinder-NanoVG_INCLUDES
		${Cinder-NanoVG_INC_PATH}/ci_nanovg.hpp
		${Cinder-NanoVG_INC_PATH}/ci_nanovg_gl.hpp
		${Cinder-NanoVG_INC_PATH}/SvgRenderer.hpp
	)

	set( Cinder-NanoVG_SOURCES
		${Cinder-NanoVG_SOURCE_PATH}/ci_nanovg.cpp
		${Cinder-NanoVG_SOURCE_PATH}/ci_nanovg_gl.cpp
		${Cinder-NanoVG_SOURCE_PATH}/SvgRenderer.cpp
	)

	list( APPEND Cinder-NanoVG_LIBRARIES
		${NanoVG_SOURCES}
		${Cinder-NanoVG_INCLUDES}
		${Cinder-NanoVG_SOURCES}
	)

	add_library( Cinder-NanoVG ${Cinder-NanoVG_LIBRARIES} )
	target_include_directories( Cinder-NanoVG PUBLIC "${CINDER_PATH}/include" ${NanoVG_SOURCE_PATH} ${Cinder-NanoVG_INC_PATH} )
	
endif()