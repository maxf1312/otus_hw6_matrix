cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)
option(WITH_GTEST "Whether to build Google test" ON)

configure_file(version.h.in version.h)

add_definitions(-D USE_PRETTY)

add_executable(matrix main.cpp)
add_library(libmatrix vers.cpp)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(matrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(libmatrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(matrix PRIVATE
    libmatrix
)

if(WITH_BOOST_TEST)
    
    #if(WIN32)
        set (Boost_ROOT "C:/local/boost_1_87_0/") # Путь к библиотеке Boost
    #endif()

    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )

    target_link_libraries(test_version
        ${Boost_LIBRARIES}
        libmatrix
    )
endif()

if(WITH_GTEST)
    find_package(GTest  REQUIRED)
    add_executable(test_versiong test_versiong.cpp)
    add_executable(test_matrix test_matrix.cpp)

    target_compile_definitions(test_matrix PUBLIC -DUSE_DBG_TRACE)

    set_target_properties(test_versiong PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_matrix PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(test_versiong
        gtest
        libmatrix
    )

    target_link_libraries(test_matrix
        gtest
        libmatrix
    )
endif()

if (MSVC)
    target_compile_options(matrix PRIVATE
        /W4
    )
    target_compile_options(libmatrix PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
    if(WITH_GTEST)
        target_compile_options(test_versiong PRIVATE
            /W4
        )
        target_compile_options(test_matrix PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(libmatrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
    if(WITH_GTEST)
        target_compile_options(test_versiong PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
        target_compile_options(test_matrix PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT maxf1312@yandex.ru)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_version test_version)
endif()

if(WITH_GTEST)
    #include(GoogleTest)
    enable_testing()
    add_test(test_versiong test_versiong)
    add_test(test_matrix test_matrix)
endif()
