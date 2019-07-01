add_executable(yawiel_test
  yawiel_test.cpp
  corpus_test.cpp
  ngram_counter_test.cpp
  eam_test.cpp
  colext_test.cpp
  container_test.cpp
)

target_link_libraries(yawiel_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_LIBRARIES}
  )

if (STATIC_BUILD)
  set(CMAKE_EXE_LINKER_FLAGS "-static ${CMAKE_EXE_LINKER_FLAGS}")
  target_link_libraries(yawiel_test -static-libgcc -static-libstdc++)
endif()
