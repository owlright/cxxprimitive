include_directories(${CMAKE_SOURCE_DIR})

foreach(test_name IN ITEMS line rgb2gray shape_triangle)
  add_executable("test_${test_name}" "test_${test_name}.cpp")
  target_link_libraries("test_${test_name}" PRIVATE primitive)
endforeach()