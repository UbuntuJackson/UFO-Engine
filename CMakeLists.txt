cmake_minimum_required(VERSION 3.10)

project(UFO)

option(HAS_TERMINAL "Show a terminal window for STDOUT/STDERR" ON)
option(USE_ALSA       "Force using ALSA as audio backend (Linux-only)")
option(USE_PULSEAUDIO "Force using PulseAudio as audio backend (Linux-only)")
option(USE_SDL2_MIXER "Force using SDL2_mixer as audio backend")

# Set C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_C_COMPILER "/usr/bin/gcc")

if(NOT WIN32)

    # Find required libraries
    find_package(CURL REQUIRED)
    find_package(PNG REQUIRED)
    find_package(ZLIB REQUIRED)

endif()

# Find require
set(
    SRC
    src/asset_manager/asset_manager.cpp
    #src/animation/animation_bundle.cpp
    #src/animation/animation_manager.cpp
    #src/background/background_system.cpp
    src/camera/camera.cpp
    src/drawing_system/drawing_system.cpp
    src/external/olcPixelGameEngine.cpp
    src/external/olcSoundWaveEngine.cpp
    src/external/cJSON.c
    src/file/file.cpp
    src/file/file_menu.cpp
    #src/graphics_provider/ufo_pge.cpp
    #src/graphics_provider/graphics_provider.cpp
    src/json/json.cpp
    src/json/json_variant.cpp
    #src/widgets/menu_system.cpp
    src/graphics/graphics.cpp
    src/graphics/graphics_engine_pge.cpp
    src/widgets/widget.cpp
    src/widgets/theme.cpp
    src/widgets/colour_rectangle_theme.cpp
    src/widgets/nine_patch_theme.cpp
    src/widgets/button.cpp
    src/widgets/file_dialogue.cpp
    src/widgets/file_dialogue_folder_button.cpp
    src/widgets/file_dialogue_file_button.cpp
    src/widgets/file_dialogue_button.cpp
    #src/widget/new_widget.cpp
    #src/widget/menu_maker.cpp
    #src/player_body/player_body.cpp
    #src/player_body/top_down_body.cpp
    #src/scene_system/scene_system.cpp
    #src/scene/scene.cpp
    src/shapes/ray2.cpp
    src/tilemap/tilemap.cpp
    src/tilemap/tile_map.cpp
    #src/ufo_constants/ufo_constants.cpp
    #src/ufo_collision/ufo_collision.cpp
    #src/ufo_engine/ufo_engine_main.cpp

    src/graphics_provider/pixel_game_engine.cpp
    src/ufo_input/ufo_input.cpp
    src/keyboard/single_keyboard.cpp
    src/mouse/mouse.cpp
    src/ufo_engine/ufo_engine.cpp
    src/level/level.cpp
    src/actor/actor.cpp
    src/sprite_reference/abstract_sprite_reference.cpp
    src/sprite_reference/sprite_reference.cpp
    src/sprite_reference/widget_sprite_reference.cpp
    src/sprite_reference/level_sprite_reference.cpp
    src/sprite_reference/level_widget_sprite_reference.cpp
    src/player_body/player_body.cpp
    src/level/tcollision_system.cpp
    src/animation/animation.cpp
    src/background/background_image.cpp
    src/animation/animated_sprite_reference.cpp
    src/level/spawn_point.cpp
    src/axis_aligned_collision/axis_aligned_collision_system.cpp
    src/axis_aligned_collision/axis_aligned_collision_body.cpp
    src/level/level_transition_effect.cpp
    src/random/random_number_generator.cpp
    src/shapes/rectangle.cpp
    src/shapes/circle.cpp
    src/shapes/raw_shape_base.cpp
    src/shapes/pill.cpp
    src/shapes/collision_pill.cpp
    src/ufo_clock/timer.cpp

    src/profile/profile.cpp

    # Alternative collision system
    src/collision/collision_system.cpp
    src/shapes/collision_circle.cpp
    src/shapes/collision_rectangle.cpp

    # Bitmaplevel
    src/bitmap_level/bitmap_collision_body.cpp
    src/bitmap_level/bitmap_dynamic_solid.cpp
    src/bitmap_level/colour_utils.cpp
    src/bitmap_level/ufo_global.cpp

    #src/custom/button_actor_config.cpp
    #src/custom/button_sprite_reference_config.cpp

    #src/ufo_lang/ufo_lang.cpp
    src/ufo_maths/ufo_maths.cpp
    src/ufo_maths/is_polygon_clockwise.cpp
    #src/shapes/new_shape.cpp
    #src/ufo_sprite/ufo_sprite.cpp
    #src/ufo_clock/ufo_time.cpp
    #src/ufo_vector_utils/range.cpp
)

add_library(UFO ${SRC})
target_include_directories(UFO PUBLIC src/actor)
target_include_directories(UFO PUBLIC src/animation)
target_include_directories(UFO PUBLIC src/asset_manager)
target_include_directories(UFO PUBLIC src/background)
target_include_directories(UFO PUBLIC src/bitmap_level)
target_include_directories(UFO PUBLIC src/camera)
target_include_directories(UFO PUBLIC src/collision)
target_include_directories(UFO PUBLIC src/console)
target_include_directories(UFO PUBLIC src/drawing_system)
target_include_directories(UFO PUBLIC src/editor)
target_include_directories(UFO PUBLIC src/external)
target_include_directories(UFO PUBLIC src/file)
target_include_directories(UFO PUBLIC src/graphics)
target_include_directories(UFO PUBLIC src/json)
target_include_directories(UFO PUBLIC src/keyboard)
target_include_directories(UFO PUBLIC src/level)
target_include_directories(UFO PUBLIC src/mouse)
target_include_directories(UFO PUBLIC src/profile)
target_include_directories(UFO PUBLIC src/random)
target_include_directories(UFO PUBLIC src/shapes)
target_include_directories(UFO PUBLIC src/sprite_reference)
target_include_directories(UFO PUBLIC src/tilemap)
target_include_directories(UFO PUBLIC src/ufo_benchmarker)
target_include_directories(UFO PUBLIC src/ufo_clock)
target_include_directories(UFO PUBLIC src/ufo_engine)
target_include_directories(UFO PUBLIC src/ufo_maths)
target_include_directories(UFO PUBLIC src/widgets)

#target_compile_options(${UFO} -Wall -Wextra)

if(NOT WIN32)

    # Link libraries
    target_link_libraries(UFO PUBLIC
        CURL::libcurl
        PNG::PNG
        ZLIB::ZLIB
    )

endif()

# OpenGL
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
target_include_directories(UFO PUBLIC ${OpenGL_INCLUDE_DIRS})
target_link_libraries(UFO PUBLIC OpenGL::GL)

# X11
if(NOT WIN32)
    find_package(X11 REQUIRED)
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 not found. Please install X11 development libraries.")
    endif()
    target_include_directories(UFO PUBLIC ${X11_INCLUDE_DIR})
    target_link_libraries(UFO PUBLIC X11::X11)
endif()

# Threads
find_package(Threads REQUIRED)
target_link_libraries(UFO PUBLIC Threads::Threads)

# stdc++fs
target_compile_features(UFO PUBLIC cxx_std_17)

# Platform-specific audio backends
if(NOT WIN32)  # Only consider ALSA or PulseAudio on Linux
    if(USE_ALSA)
        
        # ALSA
        find_package(ALSA REQUIRED)
        target_link_libraries(UFO PUBLIC ALSA::ALSA)
        include_directories(${ALSA_INCLUDE_DIRS})
        add_compile_definitions(SOUNDWAVE_USING_ALSA=1)

    else()

        # PulseAudio
        find_package(PulseAudio REQUIRED)
        target_link_libraries(UFO PUBLIC ${PULSEAUDIO_LIBRARY} pulse-simple)
        include_directories(${PULSEAUDIO_INCLUDE_DIR})
        add_compile_definitions(SOUNDWAVE_USING_PULSE=1)

    endif()
endif()

if(USE_SDL2_MIXER AND NOT EMSCRIPTEN)

    # SDL2_mixer
    find_package(SDL2_mixer REQUIRED)
    target_link_libraries(UFO PUBLIC SDL2_mixer::SDL2_mixer)

    add_compile_definitions(SOUNDWAVE_USING_SDLMIXER=1)

endif()
