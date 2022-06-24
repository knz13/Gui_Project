

#this CMakeLists was created with EasyCmake - V2 
#the repository can be found at https://github.com/knz13/EasyCmake_Cpp


cmake_minimum_required(VERSION 3.18)



#adding useful functions

function(DIR_EXISTS variable dir_path)

file(GLOB ${variable}_check ${dir_path}/*)

list(LENGTH ${variable}_check ${variable}_len)

if(${${variable}_len} EQUAL 0)

set(${variable} FALSE PARENT_SCOPE)

else()

set(${variable} TRUE PARENT_SCOPE)

endif()

endfunction()

#adding extra cmake libs
include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)


#project name
project("kv_project")

# --------------------- Repository declarations ------------------------


#repository download and settings for alias stb...


	FetchContent_Declare(stb_project

		GIT_REPOSITORY https://github.com/nothings/stb

		GIT_TAG origin/master

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/stb

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES stb_project)


#repository download and settings for alias assimp...


	#adding options

	set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")

	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

	FetchContent_Declare(assimp_project

		GIT_REPOSITORY https://github.com/assimp/assimp

		GIT_TAG v5.2.2

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/assimp

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES assimp_project)


#repository download and settings for alias imguizmo...


	FetchContent_Declare(imguizmo_project

		GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo

		GIT_TAG 1.83

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imguizmo

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES imguizmo_project)


#repository download and settings for alias imgui...


	FetchContent_Declare(imgui_project

		GIT_REPOSITORY https://github.com/ocornut/imgui

		GIT_TAG origin/docking

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES imgui_project)


#repository download and settings for alias yaml...


	#adding options

	set(YAML_BUILD_TESTS OFF CACHE INTERNAL "")

	FetchContent_Declare(yaml_project

		GIT_REPOSITORY https://github.com/jbeder/yaml-cpp

		GIT_TAG yaml-cpp-0.7.0

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/yaml

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES yaml_project)


#repository download and settings for alias dialog...


	FetchContent_Declare(dialog_project

		GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended

		GIT_TAG origin/master

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/dialog

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES dialog_project)


#repository download and settings for alias glew...


	FetchContent_Declare(glew_project

		GIT_REPOSITORY https://github.com/Perlmint/glew-cmake

		GIT_TAG glew-cmake-2.2.0

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glew

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES glew_project)


#repository download and settings for alias glm...


	FetchContent_Declare(glm_project

		GIT_REPOSITORY https://github.com/g-truc/glm

		GIT_TAG 0.9.9.8

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES glm_project)


#repository download and settings for alias sdl_repo...


	FetchContent_Declare(sdl_repo_project

		GIT_REPOSITORY https://github.com/libsdl-org/SDL

		GIT_TAG release-2.0.22

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/sdl_repo

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES sdl_repo_project)


#repository download and settings for alias ecspp...


	FetchContent_Declare(ecspp_project

		GIT_REPOSITORY https://github.com/knz13/ecspp

		GIT_TAG origin/main

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/ecspp

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES ecspp_project)


#repository download and settings for alias yael...


	FetchContent_Declare(yael_project

		GIT_REPOSITORY https://github.com/knz13/YAEL

		GIT_TAG origin/main

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/yael

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES yael_project)


#repository download and settings for alias async...


	FetchContent_Declare(async_project

		GIT_REPOSITORY https://github.com/Amanieu/asyncplusplus

		GIT_TAG v1.1

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/async

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES async_project)


#repository download and settings for alias pybind11...


	FetchContent_Declare(pybind11_project

		GIT_REPOSITORY https://github.com/pybind/pybind11

		GIT_TAG v2.9.2

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/pybind11

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES pybind11_project)


#repository download and settings for alias date...


	#adding options

	set(BUILD_TZ_LIB ON CACHE INTERNAL "")

	FetchContent_Declare(date_project

		GIT_REPOSITORY https://github.com/HowardHinnant/date

		GIT_TAG v3.0.1

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/date

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES date_project)


	FetchContent_MakeAvailable(${${PROJECT_NAME}_DEPENDENCIES})

	foreach(X IN LISTS ${PROJECT_NAME}_DEPENDENCIES)

		FetchContent_GetProperties(${X}
			SOURCE_DIR ${X}_SOURCE_DIR
			BINARY_DIR ${X}_BINARY_DIR
		)

	endforeach() 

#creating executable
add_executable(kv


	${PROJECT_SOURCE_DIR}/main.cpp

	${PROJECT_SOURCE_DIR}/source/assets/asset_object.cpp

	${PROJECT_SOURCE_DIR}/source/assets/folder_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/material_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/scene_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/shader_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/text_asset.cpp

	${PROJECT_SOURCE_DIR}/source/camera/camera.cpp

	${PROJECT_SOURCE_DIR}/source/general/color.cpp

	${PROJECT_SOURCE_DIR}/source/general/helpers.cpp

	${PROJECT_SOURCE_DIR}/source/general/math.cpp

	${PROJECT_SOURCE_DIR}/source/general/structures.cpp

	${PROJECT_SOURCE_DIR}/source/general/transform.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/explorer_view.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/game_view.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/gui_layer.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/gui_useful_implementations.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/gui_view.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/properties_view.cpp

	${PROJECT_SOURCE_DIR}/source/gui_layer/scene_hierarchy_view.cpp

	${PROJECT_SOURCE_DIR}/source/mesh/mesh.cpp

	${PROJECT_SOURCE_DIR}/source/model_loader/model_loader.cpp

	${PROJECT_SOURCE_DIR}/source/model_loader/texture_loader.cpp

	${PROJECT_SOURCE_DIR}/source/object/game_object.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/framebuffer.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/index_buffer.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/shader.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/texture.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_array.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_buffer.cpp

	${PROJECT_SOURCE_DIR}/source/window/window.cpp

	${PROJECT_SOURCE_DIR}/source/assets/texture2D_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/render_texture_asset.cpp

	${PROJECT_SOURCE_DIR}/source/panels/lighting_panel.cpp

	${PROJECT_SOURCE_DIR}/vendor/imguizmo/ImGuizmo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp

)

set_property(TARGET kv PROPERTY CXX_STANDARD 17)


#adding libraries...


target_link_libraries(kv PUBLIC assimp)

target_link_libraries(kv PUBLIC yaml-cpp)

target_link_libraries(kv PUBLIC nfd)

target_link_libraries(kv PUBLIC libglew_static)

target_link_libraries(kv PUBLIC SDL2::SDL2-static)

target_link_libraries(kv PUBLIC SDL2::SDL2main)

target_link_libraries(kv PUBLIC Async++)

target_link_libraries(kv PUBLIC pybind11::embed)

target_link_libraries(kv PUBLIC date::date)


#adding includes...


target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/source)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/stb/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/assimp/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imguizmo/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imgui/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yaml/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/dialog/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glew/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/sdl_repo/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glm/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/ecspp/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yael/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/async/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/pybind11/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/date/include)

#------------------- Extra Lines ----------------

	# Extra Lines for alias copy ...
add_custom_command(
TARGET kv POST_BUILD
COMMAND ${CMAKE_COMMAND}
-E copy_directory ${CMAKE_SOURCE_DIR}/defaults $<TARGET_FILE_DIR:kv>/defaults)
