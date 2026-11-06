# SPDX-FileCopyrightText: Copyright (C) 2026 RichStudio and contributors
# SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

# SimConnect from the MSFS 2024 SDK (SWIFT_MSFS2024_SDK_DIR) instead of the private externals repository.
# The SDK header has no P3D extensions, so only the MSFS 2024 plugin can be built this way.

if(SWIFT_BUILD_P3D_PLUGIN OR SWIFT_BUILD_MSFS_PLUGIN)
    message(FATAL_ERROR "SWIFT_MSFS2024_SDK_DIR only supports the MSFS 2024 plugin, turn off the P3D and MSFS plugins")
endif()

set(SIMCONNECT_SDK_DIR "${SWIFT_MSFS2024_SDK_DIR}/SimConnect SDK")
if(NOT EXISTS "${SIMCONNECT_SDK_DIR}/include/SimConnect.h" OR NOT EXISTS "${SIMCONNECT_SDK_DIR}/lib/SimConnect.dll")
    message(FATAL_ERROR "SimConnect not found in '${SIMCONNECT_SDK_DIR}'")
endif()
message(STATUS "SimConnect: MSFS 2024 SDK in ${SWIFT_MSFS2024_SDK_DIR}")

add_library(externals_simconnect INTERFACE IMPORTED GLOBAL)
target_compile_definitions(externals_simconnect INTERFACE SIMCONNECT_H_NOMANIFEST SWIFT_SIMCONNECT_MSFS2024_SDK)
target_include_directories(externals_simconnect INTERFACE "${SIMCONNECT_SDK_DIR}/include")

# swift loads the DLL at runtime under this name, see loadAndResolveMSFS2024SimConnect()
add_custom_target(external_simconnects_dlls
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SIMCONNECT_SDK_DIR}/lib/SimConnect.dll"
                "${PROJECT_BINARY_DIR}/out/bin/SimConnect.MSFS2024.dll")
add_dependencies(externals_simconnect external_simconnects_dlls)
install(FILES "${SIMCONNECT_SDK_DIR}/lib/SimConnect.dll" DESTINATION bin RENAME SimConnect.MSFS2024.dll)
