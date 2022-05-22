

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


	dir_exists(stb_exists ${PROJECT_SOURCE_DIR}/vendor/stb)


	if(NOT ${stb_exists})

		FetchContent_Declare(stb

			GIT_REPOSITORY https://github.com/nothings/stb

			GIT_TAG origin/master

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/stb

		)
		FetchContent_MakeAvailable(stb)
	endif()


#repository download and settings for alias assimp...


	dir_exists(assimp_exists ${PROJECT_SOURCE_DIR}/vendor/assimp)


	if(NOT ${assimp_exists})

		ExternalProject_Add(assimp

			GIT_REPOSITORY https://github.com/assimp/assimp

			GIT_TAG v5.2.2

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/assimp
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

				       -DASSIMP_BUILD_TESTS=OFF

				       -DBUILD_SHARED_LIBS=OFF

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}assimp-vc143-mt$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

	endif()


#repository download and settings for alias imguizmo...


	dir_exists(imguizmo_exists ${PROJECT_SOURCE_DIR}/vendor/imguizmo)


	if(NOT ${imguizmo_exists})

		FetchContent_Declare(imguizmo

			GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo

			GIT_TAG 1.83

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imguizmo

		)
		FetchContent_MakeAvailable(imguizmo)
	endif()


#repository download and settings for alias react_physics...


	dir_exists(react_physics_exists ${PROJECT_SOURCE_DIR}/vendor/react_physics)


	if(NOT ${react_physics_exists})

		ExternalProject_Add(react_physics

			GIT_REPOSITORY https://github.com/DanielChappuis/reactphysics3d

			GIT_TAG v0.9.0

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/react_physics
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/react_physics/lib/${CMAKE_STATIC_LIBRARY_PREFIX}reactphysics3d$<$<CONFIG:Debug>:>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

	endif()


#repository download and settings for alias imgui...


	dir_exists(imgui_exists ${PROJECT_SOURCE_DIR}/vendor/imgui)


	if(NOT ${imgui_exists})

		FetchContent_Declare(imgui

			GIT_REPOSITORY https://github.com/ocornut/imgui

			GIT_TAG origin/docking

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui

		)
		FetchContent_MakeAvailable(imgui)
	endif()


#repository download and settings for alias entt...


	dir_exists(entt_exists ${PROJECT_SOURCE_DIR}/vendor/entt)


	if(NOT ${entt_exists})

		FetchContent_Declare(entt

			GIT_REPOSITORY https://github.com/skypjack/entt

			GIT_TAG origin/master

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/entt

		)
		FetchContent_MakeAvailable(entt)
	endif()


#repository download and settings for alias yaml...


	dir_exists(yaml_exists ${PROJECT_SOURCE_DIR}/vendor/yaml)


	if(NOT ${yaml_exists})

		ExternalProject_Add(yaml

			GIT_REPOSITORY https://github.com/jbeder/yaml-cpp

			GIT_TAG yaml-cpp-0.7.0

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/yaml
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

				       -DYAML_BUILD_TESTS=OFF

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/yaml/lib/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

	endif()


#repository download and settings for alias dialog...


	dir_exists(dialog_exists ${PROJECT_SOURCE_DIR}/vendor/dialog)


	if(NOT ${dialog_exists})

		ExternalProject_Add(dialog

			GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended

			GIT_TAG origin/master

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/dialog
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/dialog/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nfd$<$<CONFIG:Debug>:>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

	endif()


#repository download and settings for alias glew...


	dir_exists(glew_exists ${PROJECT_SOURCE_DIR}/vendor/glew)


	if(NOT ${glew_exists})

		ExternalProject_Add(glew

			GIT_REPOSITORY https://github.com/Perlmint/glew-cmake

			GIT_TAG glew-cmake-2.2.0

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/glew
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/glew/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

	endif()


#repository download and settings for alias glfw...


	dir_exists(glfw_exists ${PROJECT_SOURCE_DIR}/vendor/glfw)


	if(NOT ${glfw_exists})

		ExternalProject_Add(glfw

			GIT_REPOSITORY https://github.com/glfw/glfw

			GIT_TAG 3.3.7

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/glfw
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3$<$<CONFIG:Debug>:>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

	endif()


#repository download and settings for alias glm...


	dir_exists(glm_exists ${PROJECT_SOURCE_DIR}/vendor/glm)


	if(NOT ${glm_exists})

		FetchContent_Declare(glm

			GIT_REPOSITORY https://github.com/g-truc/glm

			GIT_TAG 0.9.9.8

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm

		)
		FetchContent_MakeAvailable(glm)
	endif()


#find package settings for alias opengl

	find_package(OpenGL REQUIRED)

#creating executable
add_executable(kv


	${PROJECT_SOURCE_DIR}/source/assets/asset_object.cpp

	${PROJECT_SOURCE_DIR}/source/assets/folder_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/material_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/scene_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/shader_asset.cpp

	${PROJECT_SOURCE_DIR}/source/assets/text_asset.cpp

	${PROJECT_SOURCE_DIR}/source/camera/camera.cpp

	${PROJECT_SOURCE_DIR}/source/components/component.cpp

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

	${PROJECT_SOURCE_DIR}/source/object/object.cpp

	${PROJECT_SOURCE_DIR}/source/object/object_properties.cpp

	${PROJECT_SOURCE_DIR}/source/object/object_property_register.cpp

	${PROJECT_SOURCE_DIR}/source/object/registry.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/framebuffer.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/index_buffer.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/shader.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/texture.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_array.cpp

	${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_buffer.cpp

	${PROJECT_SOURCE_DIR}/source/window/window.cpp

	${PROJECT_SOURCE_DIR}/main.cpp

	${PROJECT_SOURCE_DIR}/vendor/imguizmo/ImGuizmo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp

)

set_property(TARGET kv PROPERTY CXX_STANDARD 20)


#setting dependencies...


if(NOT ${assimp_exists})
	add_dependencies(kv assimp)
endif()

if(NOT ${react_physics_exists})
	add_dependencies(kv react_physics)
endif()

if(NOT ${yaml_exists})
	add_dependencies(kv yaml)
endif()

if(NOT ${dialog_exists})
	add_dependencies(kv dialog)
endif()

if(NOT ${glew_exists})
	add_dependencies(kv glew)
endif()

if(NOT ${glfw_exists})
	add_dependencies(kv glfw)
endif()

#adding libraries...


target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}assimp-vc143-mt$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/react_physics/lib/${CMAKE_STATIC_LIBRARY_PREFIX}reactphysics3d$<$<CONFIG:Debug>:>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yaml/lib/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/dialog/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nfd$<$<CONFIG:Debug>:>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glew/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3$<$<CONFIG:Debug>:>${CMAKE_STATIC_LIBRARY_SUFFIX})

target_link_libraries(kv PUBLIC ${OpenGL_LIBRARIES})

target_link_libraries(kv PUBLIC OpenGL::GLU)


#adding includes...


target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/source)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/stb/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/assimp/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imguizmo/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/react_physics/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imgui/)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/entt/single_include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yaml/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/dialog/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glew/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glfw/include)

target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glm/)
