set(CMAKE_SYSTEM_NAME               Linux)

# Some default GCC settings
set(CMAKE_C_COMPILER                gcc)
set(CMAKE_CXX_COMPILER              g++)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY                   objcopy)
set(CMAKE_SIZE                      size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)