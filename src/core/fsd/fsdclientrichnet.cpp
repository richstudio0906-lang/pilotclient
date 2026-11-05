// SPDX-FileCopyrightText: Copyright (C) 2026 RichStudio and contributors
// SPDX-License-Identifier: GPL-3.0-or-later

//! \file
//! Login to our own network (openfsd), without the closed VATSIM client authentication library.

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScopedPointer>

#include "core/application.h"
#include "core/data/globalsetup.h"
#include "core/fsd/clientidentification.h"
#include "core/fsd/fsdclient.h"
#include "misc/logmessage.h"
#include "misc/network/richnet.h"

using namespace swift::misc;
using namespace swift::misc::network;

namespace swift::core::fsd
{
    void CFSDClient::handleRichNetIdentification(const QStringList &tokens)
    {
        // The server challenge in $DI is only needed for the VATSIM client authentication
        Q_UNUSED(tokens)

        if (m_protocolRevision < PROTOCOL_REVISION_VATSIM_AUTH)
        {
            CLogMessage(this).error(u"%1 needs FSD protocol revision %2 or newer, disconnecting")
                << richnet::networkName() << PROTOCOL_REVISION_VATSIM_AUTH;
            this->disconnectFromServer();
            return;
        }

        const CUser user = this->getServer().getUser();
        const QString cid = user.getId();

        // openfsd only initializes the VATSIM client authentication for a non-empty challenge, so leave it empty.
        // No system UID is sent.
        const ClientIdentification clientIdentification(getOwnCallsignAsString(), richnet::fsdClientId, m_clientName,
                                                        m_versionMajor, m_versionMinor, cid, {}, {});
        this->sendQueuedMessage(clientIdentification);

        // openfsd waits for #AP after $ID, it is sent as soon as we have the JWT
        this->getRichNetAuthToken(cid, user.getPassword(), { this, [this](const QString &token) {
                                      this->sendLogin(token);
                                      this->updateConnectionStatus(CConnectionStatus::Connected);
                                  } });
        increaseStatisticsValue(QStringLiteral("sendClientIdentification"));
    }

    void CFSDClient::getRichNetAuthToken(const QString &cid, const QString &password,
                                         const CSlot<void(const QString &)> &callback)
    {
        Q_ASSERT_X(sApp, Q_FUNC_INFO, "Need app");
        const CUrl url = sApp->getGlobalSetup().getFsdAuthUrl();
        if (url.isEmpty())
        {
            CLogMessage(this).error(u"No auth URL for %1 configured (fsdAuthUrl in bootstrap.json), disconnecting")
                << richnet::networkName();
            this->disconnectFromServer();
            return;
        }

        QNetworkRequest nwRequest(url);
        nwRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        const QJsonObject jsonRequest { { "cid", cid }, { "password", password } };

        sApp->postToNetwork(
            nwRequest, CApplication::NoLogRequestId, QJsonDocument(jsonRequest).toJson(),
            { this, [=, this](QNetworkReply *nwReplyPtr) {
                 QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> nwReply(nwReplyPtr);

                 // the user may have cancelled the login meanwhile
                 if (!this->getConnectionStatus().isConnecting())
                 {
                     return;
                 }

                 const QJsonObject json = QJsonDocument::fromJson(nwReply->readAll()).object();
                 const QString token = json.value("token").toString();
                 if (json.value("success").toBool() && !token.isEmpty())
                 {
                     callback(token);
                     return;
                 }

                 const QString error =
                     json.value("error_msg").isString() ? json.value("error_msg").toString() : nwReply->errorString();
                 CLogMessage(this).error(u"Login to %1 with CID %2 failed: %3")
                     << richnet::networkName() << cid << error;
                 this->disconnectFromServer();
             } });
    }
} // namespace swift::core::fsd
