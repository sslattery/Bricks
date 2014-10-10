INCLUDE(TribitsLibraryMacros)

#
# A) Package-specific configuration options
#

TRIBITS_CONFIGURE_FILE(${PACKAGE_NAME}_config.hpp)

#
# B) Define the header and source files (and directories)
#

SET(HEADERS "")
SET(SOURCES "")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(HEADERS ${HEADERS}
  ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_config.hpp
  )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

APPEND_SET(HEADERS
  Bricks_AbstractBuildableObject.hpp
  Bricks_AbstractBuildableObject_impl.hpp
  Bricks_AbstractBuilder.hpp
  Bricks_AbstractBuilder_impl.hpp
  Bricks_AbstractIterator.hpp
  Bricks_AbstractIterator_impl.hpp
  Bricks_AbstractObjectRegistry.hpp
  Bricks_AbstractObjectRegistry_impl.hpp
  Bricks_AbstractSerializableObject.hpp
  Bricks_AbstractSerializableObject_impl.hpp
  Bricks_AbstractSerializer.hpp
  Bricks_AbstractSerializer_impl.hpp
  Bricks_CommIndexer.hpp
  Bricks_CommTools.hpp
  Bricks_DataSerializer.hpp
  Bricks_DBC.hpp
  Bricks_PredicateComposition.hpp
  Bricks_PredicateComposition_impl.hpp
  ) 

APPEND_SET(SOURCES
  Bricks_CommIndexer.cpp
  Bricks_CommTools.cpp
  Bricks_DBC.cpp
  )

#
# C) Define the targets for package's library(s)
#

TRIBITS_ADD_LIBRARY(
  bricks
  HEADERS ${HEADERS}
  SOURCES ${SOURCES}
  )

INSTALL(FILES 
  ${HEADERS} 
  DESTINATION 
  ${CMAKE_INSTALL_PREFIX}/include/
)
