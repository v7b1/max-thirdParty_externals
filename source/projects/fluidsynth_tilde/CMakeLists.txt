include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

#############################################################
# MAX EXTERNAL
#############################################################

# exclude from windows build for now.
if (APPLE)

set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories( 
	"${MAX_SDK_INCLUDES}"
	"${MAX_SDK_MSP_INCLUDES}"
	"${MAX_SDK_JIT_INCLUDES}"
	"${CMAKE_CURRENT_SOURCE_DIR}/src"

)

set(FLUID_SOURCES
    "${SRC_PATH}/fluid_sys.h"
    "${SRC_PATH}/fluid_hash.c"
    "${SRC_PATH}/fluid_conv.c"
    "${SRC_PATH}/fluid_dsp_float.c"
    "${SRC_PATH}/fluid_defsfont.c"
    "${SRC_PATH}/fluid_chorus.c"
    "${SRC_PATH}/fluid_sfont.h"
    "${SRC_PATH}/fluid_rev.c"
    "${SRC_PATH}/fluidsynth_priv.h"
    "${SRC_PATH}/fluid_tuning.c"
    "${SRC_PATH}/fluidmaxversion.h"
    "${SRC_PATH}/fluid_voice.h"
    "${SRC_PATH}/fluid_mod.h"
    "${SRC_PATH}/fluid_phase.h"
    "${SRC_PATH}/fluid_synth.c"
    "${SRC_PATH}/fluid_chan.c"
    "${SRC_PATH}/fluid_settings.h"
    "${SRC_PATH}/fluid_gen.c"
    "${SRC_PATH}/fluid_list.c"
    "${SRC_PATH}/fluid_sys.c"
    "${SRC_PATH}/fluid_conv.h"
    "${SRC_PATH}/fluid_hash.h"
    "${SRC_PATH}/fluid_tuning.h"
    "${SRC_PATH}/fluid_rev.h"
    "${SRC_PATH}/fluid_chorus.h"
    "${SRC_PATH}/fluid_defsfont.h"
    "${SRC_PATH}/fluid_gen.h"
    "${SRC_PATH}/fluid_settings.c"
    "${SRC_PATH}/fluid_chan.h"
    "${SRC_PATH}/fluidsynth.h"
    "${SRC_PATH}/fluid_list.h"
    "${SRC_PATH}/fluid_synth.h"
    "${SRC_PATH}/fluid_mod.c"
    "${SRC_PATH}/fluid_voice.c"

    "${SRC_PATH}/fluidsynth/midi.h"
    "${SRC_PATH}/fluidsynth/settings.h"
    "${SRC_PATH}/fluidsynth/types.h"
    "${SRC_PATH}/fluidsynth/synth.h"
    "${SRC_PATH}/fluidsynth/gen.h"
    "${SRC_PATH}/fluidsynth/midi.h"
    "${SRC_PATH}/fluidsynth/log.h"
    "${SRC_PATH}/fluidsynth/voice.h"
    "${SRC_PATH}/fluidsynth/mod.h"
    "${SRC_PATH}/fluidsynth/sfont.h"

)

set(PROJECT_SRC
    "fluidmax.c"
)

add_library( 
	${PROJECT_NAME} 
	MODULE
	${PROJECT_SRC}
    ${FLUID_SOURCES}
)

# create groups in our project
source_group(TREE ${SRC_PATH} FILES ${FLUID_SOURCES})

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)

endif()
