add_library(systematics CohDiffractiveSystematics.cpp)
target_link_libraries(systematics ${ROOT_LIBRARIES})
install(TARGETS systematics DESTINATION lib)
