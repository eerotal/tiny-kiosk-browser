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
 * @file browser.cpp
 *
 * @description Main browser widget class definitions.
 *
 *******************************************************************************/

#include <QBoxLayout>

#include "Core/Inc/browser.hpp"

static const char* kIntroPageHtml =
    "<html lang=\"en\">"
    "    <head></head>"
    "    <body style=\"text-align: center\">"
    "        <h1>Tiny Kiosk Browser</h1>"
    "        <p>Please specify a URL to load.</p>"
    "    </body>"
    "</html>";

TinyKioskBrowser::TinyKioskBrowser(QWidget* parent) : QWidget(parent) {
    // Setup web view.
    webView = new QWebEngineView();
    webView->setUrl(QUrl(QStringLiteral("")));

    // Setup layout.
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(webView);
    layout->setMargin(0);

    setLayout(layout);
    setWindowTitle("Tiny Kiosk Browser");
}

TinyKioskBrowser::~TinyKioskBrowser() {
    delete webView;
}

void TinyKioskBrowser::navigate(const QUrl& url) {
    webView->setUrl(url);
}

void TinyKioskBrowser::showIntroPage(void) {
    webView->setHtml(kIntroPageHtml);
}
