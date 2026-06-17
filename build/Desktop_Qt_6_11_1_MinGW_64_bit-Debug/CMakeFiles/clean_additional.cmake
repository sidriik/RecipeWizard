# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appRecipeWizard_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appRecipeWizard_autogen.dir\\ParseCache.txt"
  "appRecipeWizard_autogen"
  )
endif()
