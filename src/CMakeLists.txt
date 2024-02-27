#
# This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

if(BUILD_GAME_SERVER OR BUILD_LOGIN_SERVER OR BUILD_EXTRACTORS)
  add_subdirectory(framework)
  add_subdirectory(shared)
endif()

if(BUILD_GAME_SERVER)
  add_subdirectory(game)
  add_subdirectory(mangosd)
endif()

if(BUILD_LOGIN_SERVER)
  add_subdirectory(realmd)
endif()

if(BUILD_IKE3_BOTS)
add_subdirectory(modules)
endif()

# Install modules
if(BUILD_MODULES)
  foreach(MODULE ${MODULES_LIST})
    string(TOLOWER ${MODULE} LOWER_MODULE_NAME)
    if(BUILD_MODULE_${MODULE} OR BUILD_${MODULE})
      include(FetchContent)

      FetchContent_Declare(
        ${LOWER_MODULE_NAME}
        GIT_REPOSITORY "https://github.com/davidonete/cmangos-${LOWER_MODULE_NAME}.git"
        GIT_TAG "main"
      )

      if(NOT ${LOWER_MODULE_NAME}_POPULATED)
        message(STATUS "Fetching ${LOWER_MODULE_NAME} module...")

        FetchContent_Populate(${LOWER_MODULE_NAME})

        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${LOWER_MODULE_NAME})
          file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/${LOWER_MODULE_NAME})
        endif()

        file(COPY ${${LOWER_MODULE_NAME}_SOURCE_DIR}/. DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/${LOWER_MODULE_NAME})

        message(STATUS "${LOWER_MODULE_NAME} module fetched and populated in ${CMAKE_CURRENT_SOURCE_DIR}/modules/${LOWER_MODULE_NAME}")
      endif()

      add_subdirectory(modules/${LOWER_MODULE_NAME})
    else()
      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${LOWER_MODULE_NAME})
        file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/${LOWER_MODULE_NAME})
      endif()
    endif()
  endforeach()
endif()