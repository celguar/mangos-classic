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

# Immersive mod
if(BUILD_IMMERSIVE)
  include(FetchContent)

  FetchContent_Declare(
    immersive
    GIT_REPOSITORY "https://github.com/davidonete/cmangos-immersive.git"
	GIT_TAG "main"
  )

  if(NOT immersive_POPULATED)
    message(STATUS "Fetching Immersive module...")

    FetchContent_Populate(immersive)
	
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Immersive)
      file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Immersive)
    endif()

    file(COPY ${immersive_SOURCE_DIR}/. DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/Immersive)

    message(STATUS "Immersive module fetched and populated in ${CMAKE_CURRENT_SOURCE_DIR}/modules/Immersive")
  endif()

  add_subdirectory(modules/Immersive)
else()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Immersive)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Immersive)
  endif()
endif()

# Achievements mod
if(BUILD_ACHIEVEMENTS)
  include(FetchContent)

  FetchContent_Declare(
    achievements
    GIT_REPOSITORY "https://github.com/davidonete/cmangos-achievements.git"
	GIT_TAG "main"
  )

  if(NOT achievements_POPULATED)
    message(STATUS "Fetching Achievements module...")

    FetchContent_Populate(achievements)
	
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Achievements)
      file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Achievements)
    endif()

    file(COPY ${achievements_SOURCE_DIR}/. DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/Achievements)

    message(STATUS "Achievements module fetched and populated in ${CMAKE_CURRENT_SOURCE_DIR}/modules/Achievements")
  endif()
  
  add_subdirectory(modules/Achievements)
else()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Achievements)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Achievements)
  endif()
endif()

# Hardcore mod
if(BUILD_HARDCORE)
  include(FetchContent)

  FetchContent_Declare(
    hardcore
    GIT_REPOSITORY "https://github.com/davidonete/cmangos-hardcore.git"
	GIT_TAG "main"
  )

  if(NOT hardcore_POPULATED)
    message(STATUS "Fetching Hardcore module...")

    FetchContent_Populate(hardcore)
  
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Hardcore)
      file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Hardcore)
    endif()

    file(COPY ${hardcore_SOURCE_DIR}/. DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/Hardcore)

    message(STATUS "Hardcore module fetched and populated in ${CMAKE_CURRENT_SOURCE_DIR}/modules/Hardcore")
  endif()
  
  add_subdirectory(modules/Hardcore)
else()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Hardcore)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Hardcore)
  endif()
endif()

# Transmog mod
if(BUILD_TRANSMOG)
  include(FetchContent)

  FetchContent_Declare(
    transmog
    GIT_REPOSITORY "https://github.com/davidonete/cmangos-transmog.git"
	GIT_TAG "main"
  )

  if(NOT transmog_POPULATED)
    message(STATUS "Fetching Transmog module...")

    FetchContent_Populate(transmog)
  
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Transmog)
      file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Transmog)
    endif()

    file(COPY ${transmog_SOURCE_DIR}/. DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/Transmog)

    message(STATUS "Transmog module fetched and populated in ${CMAKE_CURRENT_SOURCE_DIR}/modules/Transmog")
  endif()
  
  add_subdirectory(modules/Transmog)
else()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Transmog)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Transmog)
  endif()
endif()

# Dual spec module
if(BUILD_DUALSPEC)
  include(FetchContent)

  FetchContent_Declare(
    dualspec
    GIT_REPOSITORY "https://github.com/davidonete/cmangos-dual-spec.git"
	GIT_TAG "main"
  )

  if(NOT dualspec_POPULATED)
	message(STATUS "Fetching Dual spec module...")

    FetchContent_Populate(dualspec)

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Dualspec)
      file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Dualspec)
    endif()

	file(COPY ${dualspec_SOURCE_DIR}/. DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/Dualspec)
    message(STATUS "Dual spec module fetched and populated in ${CMAKE_CURRENT_SOURCE_DIR}/modules/Dualspec")
  endif()

  add_subdirectory(modules/Dualspec)
else()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/Dualspec)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/Dualspec)
  endif()
endif()