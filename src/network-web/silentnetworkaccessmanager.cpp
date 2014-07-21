// This file is part of RSS Guard.
//
// Copyright (C) 2011-2014 by Martin Rotter <rotter.martinos@gmail.com>
//
// RSS Guard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RSS Guard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RSS Guard. If not, see <http://www.gnu.org/licenses/>.

#include "network-web/silentnetworkaccessmanager.h"

#include <QNetworkReply>
#include <QAuthenticator>


SilentNetworkAccessManager::SilentNetworkAccessManager(QObject *parent)
  : BaseNetworkAccessManager(parent) {
  connect(this, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
          this, SLOT(onAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
}

SilentNetworkAccessManager::~SilentNetworkAccessManager() {
  qDebug("Destroying SilentNetworkAccessManages instance.");
}

void SilentNetworkAccessManager::onAuthenticationRequired(QNetworkReply *reply,
                                                          QAuthenticator *authenticator) {
  QObject *originating_object = reply->request().originatingObject();

  if (originating_object->property("protected").toBool()) {
    // This feed contains authentication information, it is good.
    authenticator->setUser(originating_object->property("username").toString());
    authenticator->setPassword(originating_object->property("password").toString());

    qDebug("Feed '%s' requested authentication and got it.",
           qPrintable(reply->url().toString()));

    reply->setProperty("authentication-given", true);
  }
  else {
    // Authentication is required but this feed does not contain it.
    qDebug("Feed '%s' requested authentication but username/password is not available.",
           qPrintable(reply->url().toString()));

    reply->setProperty("authentication-given", false);
  }
}