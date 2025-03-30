set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Some default GCC settings
# aarch64-linux-gnu- must be part of path environment
set(TOOLCHAIN_PATH                  /opt/gcc-arm-10.2/bin/)
set(TOOLCHAIN_PREFIX                ${TOOLCHAIN_PATH}aarch64-none-linux-gnu-)

# Define compiler settings
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

# Set the sysroot to the cross-compilation environment
set(CMAKE_SYSROOT /opt/gcc-arm-10.2/aarch64-none-linux-gnu/libc)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)