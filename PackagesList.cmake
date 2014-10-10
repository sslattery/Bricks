#
# See documentation in Trilinos preCopyrightTrilinos/ExtraExternalRepositories.cmake
#

INCLUDE(TribitsListHelpers)

SET( Bricks_PACKAGES_AND_DIRS_AND_CLASSIFICATIONS
  Bricks         .     SS
  )

TRIBITS_DISABLE_PACKAGE_ON_PLATFORMS(Bricks Windows)
