set(PROCESSLIB_INCLUDE_DIRS "/usr/local/include/processlib/")
set(PROCESSLIB_LIBRARIES "/usr/local/lib/")
set(PROCESSLIB_DEFINITIONS)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Processlib DEFAULT_MSG
  PROCESSLIB_LIBRARIES
  PROCESSLIB_INCLUDE_DIRS
)
