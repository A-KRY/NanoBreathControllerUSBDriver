# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\NanoBreathControllerUSBDriver_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\NanoBreathControllerUSBDriver_autogen.dir\\ParseCache.txt"
  "NanoBreathControllerUSBDriver_autogen"
  )
endif()
