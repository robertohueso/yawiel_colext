# Add subdirectories.
add_subdirectory(eam)

# List sources.
set(SOURCES
  colext.hpp
  colext_impl.cpp
)

# List of sources paths.
set(PATHS_SRCS)
foreach(source ${SOURCES})
  set(PATHS_SRCS ${PATHS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
endforeach()

# Add paths to yawiel sources.
set(YAWIEL_SRCS ${YAWIEL_SRCS} ${PATHS_SRCS} PARENT_SCOPE)
