## CMakeLists.txt


CONFIGURE_FILE (xhtmldbg.1.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.1
)

INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.1
  DESTINATION share/man/man1
)

##EOF
