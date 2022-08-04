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

#ifndef BROWSER_H
#define BROWSER_H

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

typedef struct {
    gchar* host;
    gchar* username;
    gchar* password;
} HttpBasicAuthCredentials;

typedef struct {
    GtkWidget *mainWindow;
    WebKitWebView *webView;

    HttpBasicAuthCredentials basicAuth;
} Browser;

/*
 * @brief          Initialize Browser widgets.
 *
 * @param app      The main GTK application instance.
 * @param browser  The Browser to initialize.
 */
void Browser_init(GApplication* app, Browser* browser);

/*
 * @brief         Check that a Browser instance is correctly configured.
 *
 * @param browser A Browser instance.
 *
 * @return        0 on success, -1 on failure.
 */
int Browser_validateConfig(const Browser* browser);

/*
 * @brief         Load a URL in a Browser.
 *
 * @param browser The Browser to load the URL in.
 * @param url     The URL to load.
 */
void Browser_load(Browser* browser, const gchar* url);

#endif
