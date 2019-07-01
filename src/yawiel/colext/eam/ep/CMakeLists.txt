# List sources.
set(SOURCES
  g0.hpp
  g0_impl.cpp
  g1.hpp
  g1_impl.cpp
  g2.hpp
  g2_impl.cpp
  g3.hpp
  g3_impl.cpp
  g4.hpp
  g4_impl.cpp
  g5.hpp
  g5_impl.cpp
  g6.hpp
  g6_impl.cpp
)

# List of sources paths.
set(PATHS_SRCS)
foreach(source ${SOURCES})
  set(PATHS_SRCS ${PATHS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
endforeach()

# Add paths to yawiel sources.
set(YAWIEL_SRCS ${YAWIEL_SRCS} ${PATHS_SRCS} PARENT_SCOPE)
