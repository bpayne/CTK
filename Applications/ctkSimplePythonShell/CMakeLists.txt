PROJECT(ctkSimplePythonShell)

#
# See CTK/CMake/ctkMacroBuildApp.cmake for details
#

SET(KIT_SRCS
  ctkSimplePythonManager.cpp
  ctkSimplePythonManager.h
  ctkSimplePythonQtDecorators.h
  ctkSimplePythonShellMain.cpp
)

# Headers that should run through moc
SET(KIT_MOC_SRCS
  ctkSimplePythonManager.h
  ctkSimplePythonQtDecorators.h
  )

# UI files
SET(KIT_UI_FORMS
)

# Resources
SET(KIT_resources
)

# Target libraries - See CMake/ctkMacroGetTargetLibraries.cmake
# The following macro will read the target libraries from the file 'target_libraries.cmake'
ctkMacroGetTargetLibraries(KIT_target_libraries)

# Additional directories to include - Not that CTK_INCLUDE_LIBRARIES is already included
SET(KIT_include_directories
  )

ctkMacroBuildApp(
  NAME ${PROJECT_NAME}
  INCLUDE_DIRECTORIES ${KIT_include_directories}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_FORMS ${KIT_UI_FORMS}
  TARGET_LIBRARIES ${KIT_target_libraries}
  RESOURCES ${KIT_resources}
  )

# Testing
IF(BUILD_TESTING)
#   ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)