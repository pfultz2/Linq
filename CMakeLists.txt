cmake_minimum_required(VERSION 3.0)
project(linq CXX)

find_package(Boost)

add_library(boost::boost INTERFACE IMPORTED)
set_target_properties(boost::boost PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

add_library(linq INTERFACE)
add_library(linq::linq ALIAS linq)
target_include_directories(linq INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(TARGETS linq EXPORT linq-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(EXPORT linq-targets
    FILE linq-targets.cmake
    NAMESPACE linq::
    DESTINATION lib/cmake/linq
)

install(FILES
    linq-config.cmake
    DESTINATION lib/cmake/linq
)

install(FILES linq.h DESTINATION include)
install(DIRECTORY linq DESTINATION include)

include(CTest)

if (BUILD_TESTING)
    add_executable(linq-test test.cpp)
    target_link_libraries(linq-test linq ${Boost_test_LIBRARY_RELEASE})
    add_test(NAME linq-test COMMAND linq-test)
endif (BUILD_TESTING)
