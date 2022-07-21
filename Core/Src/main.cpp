/*******************************************************************************
 * Copyright (c) 2022, Eero Talus
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 *
 * @file main.cpp
 *
 * @description Tiny Kiosk Browser entrypoint.
 *
 *******************************************************************************/

#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>

#include "Core/Inc/browser.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationVersion(VERSION);
    QApplication app(argc, argv);

    /*
     * Parse CLI arguments.
     */

    QCommandLineParser parser;

    parser.setApplicationDescription("Tiny Kiosk Browser");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("url", "Destination URL.");

    QCommandLineOption userOption(
        QStringList() << "u" << "username",
        "Specify a <username> to use for HTTP Basic Auth.",
        "username"
    );
    parser.addOption(userOption);

    QCommandLineOption passwordOption(
        QStringList() << "p" << "password",
        "Specify a <password> to use for HTTP Basic Auth.",
        "passsword"
    );
    parser.addOption(passwordOption);

    parser.process(app);

    /*
     * Initialize and run the browser widget.
     */

    TinyKioskBrowser browserWidget;
    QStringList posArgs = parser.positionalArguments();

    // Set destination URL.
    if (!posArgs.isEmpty()) {
        QUrl url(posArgs.at(0));

        qDebug() << "Navigating to"
                 << url.toString()
                 << "(possible username/password redacted for security)";

        if (url.scheme().isEmpty()) {
            qWarning() << "URL scheme not provided. Falling back to https://.";
            url.setScheme("https");
        }

        // Set HTTP Basic Auth credentials if provided.
        if (parser.isSet(userOption) != parser.isSet(passwordOption)) {
            qCritical("Please provide both a username and a password for HTTP Basic Auth.");
            return 1;
        } else if (parser.isSet(userOption) && parser.isSet(passwordOption)){
            url.setUserName(parser.value(userOption));
            url.setPassword(parser.value(passwordOption));
        }

        browserWidget.navigate(url);
    } else {
        browserWidget.showIntroPage();
    }

    browserWidget.show();

    return app.exec();
}
