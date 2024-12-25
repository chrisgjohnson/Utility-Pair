cmake_minimum_required (VERSION 3.13)
include(pico_sdk_import.cmake)
project(computercard C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

macro (add_pair _name1 _name2)
    add_executable(${_name1}_${_name2})
    if (TARGET ${_name1}_${_name2})
	  target_compile_definitions(${_name1}_${_name2} PRIVATE PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)
	  target_compile_definitions(${_name1}_${_name2} PRIVATE UTILITY1=${_name1})
	  target_compile_definitions(${_name1}_${_name2} PRIVATE UTILITY2=${_name2})
	  target_include_directories(${_name1}_${_name2} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
      target_link_libraries(${_name1}_${_name2} pico_unique_id pico_stdlib  hardware_dma hardware_i2c hardware_pwm hardware_adc hardware_spi)
	  pico_add_extra_outputs(${_name1}_${_name2})
	  target_sources(${_name1}_${_name2} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
	  
	  pico_enable_stdio_usb(${_name1}_${_name2} 1)
    endif()
  endmacro()

  
########################################
### Full list of 625 targets
### takes a few hours to compile and produces a ~6GB build directory!

set(targetlist "delay" "euclidean" "bitcrush" "attenuvert" "clockdiv" "slopesplus" "maxrect" "chords" "windowcomp" "karplusstrong" "cross" "cvmix" "vco" "chorus" "wavefolder" "lpg" "sandh" "quantiser" "bernoulli" "supersaw" "vca" "looper" "slowlfo" "turing185" "glitch")

########################################
### Quicker to restrict to a much shorter list of utilities, as follows

# set(targetlist "lpg" "wavefolder" "delay")

########################################
### Or, empty targetlist, and just specify which pairs should be built

# set(targetlist )
# add_pair("delay" "delay")
# add_pair("wavefolder" "lpg")
# add_pair("looper" "glitch")


# Compile each pair in 'targetlist'
foreach(targ1 IN LISTS targetlist)
  foreach(targ2 IN LISTS targetlist)
	add_pair(${targ1} ${targ2})
  endforeach()
endforeach()
