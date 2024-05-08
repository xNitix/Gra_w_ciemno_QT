# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Gra_W_Ciemno_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Gra_W_Ciemno_autogen.dir\\ParseCache.txt"
  "Gra_W_Ciemno_autogen"
  )
endif()
