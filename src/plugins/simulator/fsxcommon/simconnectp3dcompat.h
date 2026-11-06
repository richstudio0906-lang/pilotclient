// SPDX-FileCopyrightText: Copyright (C) 2026 RichStudio and contributors
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

//! \file
//! P3D v4 SimConnect types missing in the SimConnect.h of the MSFS 2024 SDK.
//! They only appear in the P3D function pointers and wrappers in simconnectsymbols.cpp. Those functions are
//! resolved for P3D only, an MSFS 2024 build never calls them, so the types just have to exist.
//! Included by simconnectsymbols.cpp when building against the MSFS 2024 SDK (SWIFT_SIMCONNECT_MSFS2024_SDK).

#ifndef SWIFT_SIMPLUGIN_FSX_SIMCONNECTP3DCOMPAT_H
#define SWIFT_SIMPLUGIN_FSX_SIMCONNECTP3DCOMPAT_H

#ifndef SWIFT_SIMCONNECT_MSFS2024_SDK
#    error "Only for builds against the MSFS 2024 SDK"
#endif

//! @{
//! Placeholder for a P3D v4 type, not usable with MSFS
enum SIMCONNECT_GROUND_INFO_LATLON_FORMAT : DWORD
{
};
enum SIMCONNECT_GROUND_INFO_ALT_FORMAT : DWORD
{
};
enum SIMCONNECT_GROUND_INFO_SOURCE_FLAG : DWORD
{
};
enum SIMCONNECT_CAMERA_TYPE : DWORD
{
};
struct SIMCONNECT_DATA_OBSERVER
{};
//! @}

#endif // SWIFT_SIMPLUGIN_FSX_SIMCONNECTP3DCOMPAT_H
