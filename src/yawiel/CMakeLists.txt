set(YAWIEL_SRCS ${YAWIEL_SRCS}
  "${CMAKE_CURRENT_SOURCE_DIR}/prereqs.hpp"
)

# Add subdirectories.
add_subdirectory(core)
add_subdirectory(colext)

# Build yawiel library.
add_library(yawiel SHARED ${YAWIEL_SRCS})

# Build tests if required.
if (BUILD_TESTS)
  add_subdirectory(tests)
endif()
