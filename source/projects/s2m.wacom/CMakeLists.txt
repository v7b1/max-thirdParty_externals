
include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

set(MAX_OBJ_PATH ${CMAKE_CURRENT_SOURCE_DIR}/MaxObject)
set(WACOM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Wacom)


set(WAC_SOURCES
	${WACOM_PATH}/NSAppleEventDescriptorHelperCategory.h
	${WACOM_PATH}/NSAppleEventDescriptorHelperCategory.m
	${WACOM_PATH}/TabletAEDictionary.h
	${WACOM_PATH}/WacomTabletDriver.h
	${WACOM_PATH}/WacomTabletDriver.m
)

set(MAX_OBJ_SOURCES
	${MAX_OBJ_PATH}/MaxAudioSignal.h
	${MAX_OBJ_PATH}/MaxAudioSignal.m
	${MAX_OBJ_PATH}/MaxObject_Prefix.pch
	${MAX_OBJ_PATH}/MaxObject.h
	${MAX_OBJ_PATH}/MaxObject.m
	${MAX_OBJ_PATH}/maxobjectbridge.h
	${MAX_OBJ_PATH}/maxobjectbridge.m

)

include_directories( 
	"${C74_INCLUDES}"
	"${MAX_OBJ_PATH}"
	"${WACOM_PATH}"
)

add_library( 
	${PROJECT_NAME} 
	MODULE
	${PROJECT_NAME}.m
 #out of some reason we also have to add the following file...
    ${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/c74support/max-includes/common/commonsyms.c
	${WAC_SOURCES}
	${MAX_OBJ_SOURCES}

)

if(APPLE)
	# create groups in our project
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${WAC_SOURCES} ${MAX_OBJ_SOURCES})
	target_link_libraries(${PROJECT_NAME} PUBLIC "-framework Foundation")
	target_link_libraries(${PROJECT_NAME} PUBLIC "-framework Cocoa")
endif()


include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)
