
if (APPLE) # exclude from windows build for now

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/fftw3")


include_directories( 
	"${C74_INCLUDES}"
	"${LIB_PATH}/include"
)


add_library( 
	${PROJECT_NAME} 
	MODULE
	${PROJECT_NAME}.c
)

if(APPLE)
	target_link_libraries(${PROJECT_NAME} PUBLIC "-framework Accelerate")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_PATH}/libfftw3.a)
else()
	# need to link windows version of fftw3
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)

endif()
