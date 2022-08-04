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
 * @file main.c
 *
 * @description Tiny Kiosk Browser entrypoint.
 *
 *******************************************************************************/

#include "Core/Inc/browser.h"

static void Browser_authenticate(
    __attribute__((unused)) WebKitWebView *webView,
    WebKitAuthenticationRequest *request,
    gpointer user_data
);

/*
 * @brief       Provide HTTP Basic Authentication credentials when required.
 *
 * @description This function is connected to the "authenticate" signal of
 *              the browser's WebKitWebView instance.
 */
static void Browser_authenticate(
    __attribute__((unused)) WebKitWebView *webView,
    WebKitAuthenticationRequest *request,
    gpointer user_data
) {
    Browser* browser = user_data;
    WebKitCredential* credential;
    const gchar* host = webkit_authentication_request_get_host(request);

    if (browser->basicAuth.host == NULL) {
      g_message("HTTP Basic Authentication requested but no credentials provided.");
    } else {
        g_message("Authenticating using HTTP Basic Authentication.");
        if (
            !g_str_equal("*", browser->basicAuth.host) &&
            !g_str_equal(host, browser->basicAuth.host)
        ){
            g_message(
                "Provided authentication credentials don't apply to host: %s",
                host
            );
            return;
        }

        credential = webkit_credential_new(
            browser->basicAuth.username,
            browser->basicAuth.password,
            WEBKIT_CREDENTIAL_PERSISTENCE_FOR_SESSION
        );
        webkit_authentication_request_authenticate(request, credential);

        webkit_credential_free(credential);
    }
}

void Browser_init(GApplication* app, Browser* browser) {
    if (Browser_validateConfig(browser) != 0) {
        g_application_quit(app);
    }

    // Create the main browser window.
    browser->mainWindow = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(browser->mainWindow), "Tiny Kiosk Browser");
    gtk_window_set_default_size(GTK_WINDOW(browser->mainWindow), 200, 200);

    // Create the web view component.
    browser->webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    g_signal_connect(browser->webView, "authenticate", G_CALLBACK(Browser_authenticate), browser);
    gtk_container_add(GTK_CONTAINER(browser->mainWindow), GTK_WIDGET(browser->webView));

    gtk_widget_show_all(browser->mainWindow);
}


int Browser_validateConfig(const Browser* browser) {
    gboolean a = browser->basicAuth.host == NULL;
    gboolean b = browser->basicAuth.username == NULL;
    gboolean c = browser->basicAuth.password == NULL;

    // Check that either all of the HTTP Basic Auth credentials are NULL
    // or none of them are.
    if (a || b || c) {
        if (a && b && c) {
            return 0;
        } else {
            g_message(
                "You must supply all of the HTTP Basic Auth "
                "flags (--username, --password and --host)."
            );
            return -1;
        }
    }

    return 0;
}

void Browser_load(Browser* browser, const gchar* url) {
    webkit_web_view_load_uri(browser->webView, url);
}
