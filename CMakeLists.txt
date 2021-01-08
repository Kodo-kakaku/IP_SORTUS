cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
else()
    project(helloworld VERSION 0.0.42)
    set(BOOST_ROOT "C:\\boost\\include\\boost-1_75")
    set(BOOST_LIBRARYDIR "C:\\boost\\lib")
endif()

configure_file(version.h.in version.h)

add_executable(ip_sortus_cli main.cpp)
add_library(ip_sortus lib.cpp)
add_executable(test_version test_version.cpp)

set_target_properties(ip_sortus_cli ip_sortus test_version PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_include_directories(ip_sortus
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(test_version PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

target_link_libraries(ip_sortus_cli PRIVATE
        ip_sortus
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
        ip_sortus
)

if (MSVC)
    target_compile_options(ip_sortus_cli PRIVATE
        /W4
    )
    target_compile_options(ip_sortus PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(ip_sortus_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(ip_sortus PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS ip_sortus_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
