# List sources.
set(SOURCES
  pmi.hpp
  pmi_impl.cpp
  dice.hpp
  dice_impl.cpp
  chi_squared.hpp
  chi_squared_impl.cpp
  log_likelihood.hpp
  log_likelihood_impl.cpp
)

# List of sources paths.
set(PATHS_SRCS)
foreach(source ${SOURCES})
  set(PATHS_SRCS ${PATHS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${source})
endforeach()

# Add paths to yawiel sources.
set(YAWIEL_SRCS ${YAWIEL_SRCS} ${PATHS_SRCS} PARENT_SCOPE)
