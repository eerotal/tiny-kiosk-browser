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

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

struct Browser {
    GtkApplication* app;
    GtkWidget *mainWindow;
    WebKitWebView *webView;
};

static void authenticate(WebKitWebView *webView, WebKitAuthenticationRequest *request, gpointer user_data) {
    WebKitCredential* credential;

    g_message("Authenticating with provided credentials.");

    credential = webkit_credential_new(
        "infotv",
        "vtofni",
        WEBKIT_CREDENTIAL_PERSISTENCE_FOR_SESSION
    );
    webkit_authentication_request_authenticate(request, credential);

    webkit_credential_free(credential);
}

static void startup(GApplication* app, gpointer user_data) {
    struct Browser* browser = user_data;

    // Create the main application window.
    browser->mainWindow = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(browser->mainWindow), "Tiny Kiosk Browser");
    gtk_window_set_default_size(GTK_WINDOW(browser->mainWindow), 200, 200);

    // Create the web view component.
    browser->webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    g_signal_connect(browser->webView, "authenticate", G_CALLBACK(authenticate), NULL);
    gtk_container_add(GTK_CONTAINER(browser->mainWindow), GTK_WIDGET(browser->webView));
    gtk_widget_show_all(browser->mainWindow);
}

static void activate(GApplication* app, gpointer user_data) {
    
}

static void open(GApplication* app, gpointer files, gint n_files, const gchar* hint, gpointer user_data) {
    struct Browser* browser = user_data;
    GFile** gfiles = files;
    char* url = g_file_get_uri(gfiles[0]);

    if (n_files > 1) {
        g_warning("More than one URL specified. Will load the first one.");
    }

    // Load the specified URL in WebKit
    g_message("Loading: %s\n", url);
    webkit_web_view_load_uri(browser->webView, url);
}

int main(int argc, char** argv) {
    int status;
    struct Browser browser;

    browser.app = gtk_application_new("cc.talus.tiny-kiosk-browser", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(browser.app, "startup", G_CALLBACK(startup), &browser);
    g_signal_connect(browser.app, "activate", G_CALLBACK(activate), &browser);
    g_signal_connect(browser.app, "open", G_CALLBACK(open), &browser);

    status = g_application_run(G_APPLICATION(browser.app), argc, argv);
    g_object_unref(browser.app);

    return status;
}
