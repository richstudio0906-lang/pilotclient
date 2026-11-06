// SPDX-FileCopyrightText: Copyright (C) 2026 RichStudio and contributors
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

//! \file

#ifndef SWIFT_MISC_NETWORK_RICHNET_H
#define SWIFT_MISC_NETWORK_RICHNET_H

#include <QString>
#include <QtGlobal>

//! Constants of our own FSD network (served by openfsd).
//! Server addresses and URLs are not here, they come from bootstrap.json.
namespace swift::misc::network::richnet
{
    //! Display name of the network (working title, rename it here only)
    inline const QString &networkName()
    {
        static const QString name("RichNet");
        return name;
    }

    //! Client id sent in the $ID packet. openfsd parses it as 16 bit hex and does not check the value.
    constexpr quint16 fsdClientId = 0x524e;
} // namespace swift::misc::network::richnet

#endif // SWIFT_MISC_NETWORK_RICHNET_H
