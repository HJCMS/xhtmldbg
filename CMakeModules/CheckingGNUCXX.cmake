## Check_GNU_CXX.cmake

IF (CMAKE_SYSTEM_NAME MATCHES Linux)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wundef -Wcast-align -Wchar-subscripts -Wpointer-arith -Wformat-security -fno-exceptions -fno-check-new -fno-common")
  ADD_DEFINITIONS (-D_BSD_SOURCE -DQT_NO_EXCEPTIONS)
ENDIF (CMAKE_SYSTEM_NAME MATCHES Linux)

# gcc under Windows
IF (MINGW)
  SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--disable-auto-import")
  SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--disable-auto-import")
  ADD_DEFINITIONS(-DQT_NO_DEBUG)
ENDIF (MINGW)

# visibility support http://gcc.gnu.org/wiki/Visibility
CHECK_CXX_COMPILER_FLAG (-fvisibility=hidden __CHECK_GCC_VISIBILITY)
SET (__CHECK_GCC_VISIBILITY ${__CHECK_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

# stack-protector support
CHECK_CXX_COMPILER_FLAG(-fstack-protector __CHECK_GCC_STACK_PROTECTOR)
SET (__CHECK_GCC_STACK_PROTECTOR ${__CHECK_GCC_STACK_PROTECTOR} CACHE BOOL "GCC support for stack-protector")

# buffer checks
CHECK_CXX_COMPILER_FLAG(-D_FORTIFY_SOURCE=2 __CHECK_GCC_FORTIFY_SOURCE)
SET (__CHECK_GCC_FORTIFY_SOURCE ${__CHECK_GCC_FORTIFY_SOURCE} CACHE BOOL "GCC support for -DFORTIFY_SOURCE")

# emit position-independent code
CHECK_CXX_COMPILER_FLAG(-fPIC __CHECK_GCC_FPIC)
SET (__CHECK_GCC_FPIC ${__CHECK_GCC_FPIC} CACHE BOOL "GCC support for -fPIC")

# gives some advantages over fPIC
CHECK_CXX_COMPILER_FLAG(-pie __CHECK_GCC_FPIE)
SET (__CHECK_GCC_FPIE ${__CHECK_GCC_FPIE} CACHE BOOL "GCC support for -pie")

# Pass the flag -export-dynamic to the ELF linker, on targets that support it.
CHECK_CXX_COMPILER_FLAG(-rdynamic __CHECK_LINK_RDYNAMIC)
SET (__CHECK_LINK_RDYNAMIC ${__CHECK_LINK_RDYNAMIC} CACHE BOOL "compiler support for -rdynamic")

# get the gcc version
EXECUTE_PROCESS (COMMAND ${CMAKE_C_COMPILER} --version OUTPUT_VARIABLE _gcc_version_info)

STRING (REGEX MATCH "[345]\\.[0-9]\\.[0-9]" _gcc_version "${_gcc_version_info}")
# gcc on mac just reports: "gcc (GCC) 3.3 20030304 ..." without the patch level, handle this here:
IF (NOT _gcc_version)
  STRING (REGEX REPLACE ".*\\(GCC\\).* ([34]\\.[0-9]) .*" "\\1.0" _gcc_version "${_gcc_version_info}")
ENDIF (NOT _gcc_version)

MESSAGE (STATUS "Found GCC : ${_gcc_version}")

MACRO_ENSURE_VERSION ("4.1.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_1)
MACRO_ENSURE_VERSION ("4.2.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_2)
MACRO_ENSURE_VERSION ("4.3.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_3)
MACRO_ENSURE_VERSION ("4.4.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_4)
MACRO_ENSURE_VERSION ("4.5.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_5)

IF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

  IF (GCC_IS_NEWER_THAN_4_2)
      SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
  ENDIF (GCC_IS_NEWER_THAN_4_2)

ENDIF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)

IF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_VISIBILITY")
  ## Bugfix gcc-4.3 need explicit definition
  ADD_DEFINITIONS (-DHAVE_VISIBILITY)
ENDIF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)

IF (__CHECK_GCC_STACK_PROTECTOR)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector -Wstack-protector")
  IF(__CHECK_GCC_FORTIFY_SOURCE)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
  ENDIF(__CHECK_GCC_FORTIFY_SOURCE)
ENDIF (__CHECK_GCC_STACK_PROTECTOR)

IF (__CHECK_GCC_FPIC)
  SET (LIB_FPIC_CXXFLAGS "-fPIC")
ELSE (__CHECK_GCC_FPIC)
  SET (LIB_FPIC_CXXFLAGS "")
ENDIF (__CHECK_GCC_FPIC)

IF (__CHECK_GCC_FPIE)
  SET (APP_PIE_CXXFLAGS "-fPIE")
  SET (APP_PIE_LDFLAGS "-pie")
ELSE (__CHECK_GCC_FPIE)
  SET (APP_PIE_CXXFLAGS "")
  SET (APP_PIE_LDFLAGS "")
ENDIF (__CHECK_GCC_FPIE)

IF (__CHECK_LINK_RDYNAMIC)
  SET (LIB_RDYNAMIC_CXXFLAGS "-rdynamic")
ELSE (__CHECK_LINK_RDYNAMIC)
  SET (LIB_RDYNAMIC_CXXFLAGS "")
ENDIF (__CHECK_LINK_RDYNAMIC)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -W -Wabi -Wextra -Wno-long-long -Wctor-dtor-privacy -Wreorder -Wold-style-cast -Woverloaded-virtual -DXHTMLDBG_DEBUG")
  IF (XHTMLDBG_BUILD_VERBOSE)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DXHTMLDBG_DEBUG_VERBOSE -Wmissing-include-dirs -Wwrite-strings")
  ENDIF (XHTMLDBG_BUILD_VERBOSE)
ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wno-long-long")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")
