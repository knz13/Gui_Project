
#setting cmake version

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
project("kv")

#creating variables for ease of adding libraries
set(${PROJECT_NAME}_DEPS_TO_BUILD )
set(${PROJECT_NAME}_SOURCE_FILES )
set(${PROJECT_NAME}_INCLUDES )
set(${PROJECT_NAME}_LIBRARIES )

        
        
#------------ additional lines ---------------


    # additional lines for alias "copy_file"...

    file(COPY ${PROJECT_SOURCE_DIR}/default_shaders/  DESTINATION ${PROJECT_BINARY_DIR}/default_shaders)



# repository download and settings for glm...

    dir_exists(glm_exists ${PROJECT_SOURCE_DIR}/vendor/glm)


    if(NOT ${glm_exists})
        FetchContent_Declare(GLM
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm
        )

        FetchContent_MakeAvailable(GLM)

    endif()
                    


# repository download and settings for glew...

    dir_exists(glew_exists ${PROJECT_SOURCE_DIR}/vendor/glew)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/glew/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${glew_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(GLEW
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
        GIT_TAG glew-cmake-2.2.0
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/glew
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -Dglew-cmake_BUILD_SHARED=OFF
                        
                    -DONLY_LIBS=ON
                        
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/glew/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD GLEW)

    endif()




# repository download and settings for thread_pool...

    dir_exists(thread_pool_exists ${PROJECT_SOURCE_DIR}/vendor/thread_pool)


    if(NOT ${thread_pool_exists})
        FetchContent_Declare(THREAD_POOL
        GIT_REPOSITORY https://github.com/bshoshany/thread-pool
        GIT_TAG v1.9
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/thread_pool
        )

        FetchContent_MakeAvailable(THREAD_POOL)

    endif()
                    


# repository download and settings for stb...

    dir_exists(stb_exists ${PROJECT_SOURCE_DIR}/vendor/stb)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/stb)


    if(NOT ${stb_exists})
        FetchContent_Declare(STB
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_TAG origin/master
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/stb
        )

        FetchContent_MakeAvailable(STB)

    endif()
                    


# repository download and settings for assimp...

    dir_exists(assimp_exists ${PROJECT_SOURCE_DIR}/vendor/assimp)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/assimp/include)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}assimp-vc143-mt$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})

        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${assimp_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(ASSIMP
        GIT_REPOSITORY https://github.com/assimp/assimp
        GIT_TAG v5.2.2
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/assimp
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DBUILD_SHARED_LIBS=OFF
                        
                    -DASSIMP_BUILD_TESTS=OFF
                        
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}assimp-vc143-mt$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX} ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD ASSIMP)

    endif()




# repository download and settings for glfw...

    dir_exists(glfw_exists ${PROJECT_SOURCE_DIR}/vendor/glfw)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/glfw/include)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${glfw_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.6
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/glfw
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD GLFW)

    endif()




# repository download and settings for entt...

    dir_exists(entt_exists ${PROJECT_SOURCE_DIR}/vendor/entt)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/entt/single_include)


    if(NOT ${entt_exists})
        FetchContent_Declare(ENTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG origin/wip
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/entt
        )

        FetchContent_MakeAvailable(ENTT)

    endif()
                    


# repository download and settings for imgui...

    dir_exists(imgui_exists ${PROJECT_SOURCE_DIR}/vendor/imgui)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/imgui)

    
    # adding sources


        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp)


    if(NOT ${imgui_exists})
        FetchContent_Declare(IMGUI
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG origin/docking
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui
        )

        FetchContent_MakeAvailable(IMGUI)

    endif()
                    


# repository download and settings for imguizmo...

    dir_exists(imguizmo_exists ${PROJECT_SOURCE_DIR}/vendor/imguizmo)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/imguizmo)

    
    # adding sources


        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imguizmo/ImGuizmo.cpp)


    if(NOT ${imguizmo_exists})
        FetchContent_Declare(IMGUIZMO
        GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo
        GIT_TAG 1.83
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imguizmo
        )

        FetchContent_MakeAvailable(IMGUIZMO)

    endif()
                    


# finding package and adding settings for alias OpenGL...


    find_package(OpenGL REQUIRED)


    # adding libraries:


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${OpenGL_LIBRARIES})

        list(APPEND ${PROJECT_NAME}_LIBRARIES OpenGL::GLU)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/window/window.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/opengl_wrappers/index_buffer.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/opengl_wrappers/shader.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_array.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/opengl_wrappers/vertex_buffer.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/general/color.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/camera/camera.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/drawable/drawable.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/general/structures.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/general/movable.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/drawable/drawing_modes.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/model_loader/model_loader.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/model_loader/texture_loader.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/gui_layer/gui_layer.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/opengl_wrappers/framebuffer.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/opengl_wrappers/texture.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/object.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/registry.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/object_properties_component.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/general/math.cpp)


#creating executable
add_executable(${PROJECT_NAME}

        ${${PROJECT_NAME}_SOURCE_FILES}
)
        
#setting c/cpp standard

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

        
#adding dependencies

foreach(X ${${PROJECT_NAME}_DEPS_TO_BUILD})

    add_dependencies(${PROJECT_NAME} ${X})

endforeach()


            
# ------------- linking libraries -------------

    target_link_libraries(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_LIBRARIES})



#------------ include directories -------------


    list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor)


    list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/source)


    target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDES})



# cacheing the build type

set(${PROJECT_NAME}_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE INTERNAL "")
        
