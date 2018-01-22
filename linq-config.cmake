include(CMakeFindDependencyMacro)

find_dependency(Boost)

add_library(boost::boost INTERFACE IMPORTED)
set_target_properties(boost::boost PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

include(${CMAKE_CURRENT_LIST_DIR}/linq-targets.cmake)
