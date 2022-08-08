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

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void startup(GApplication* app, gpointer user_data);

static void activate(
    __attribute__((unused)) GApplication* app,
    __attribute__((unused)) gpointer user_data
);

static void open(
    __attribute__((unused)) GApplication* app,
    gpointer files,
    gint n_files,
    __attribute__((unused)) const gchar* hint,
    gpointer user_data
);

static void setupCliParsing(GtkApplication* app, Browser* browser);


/*
 * @brief       Initialize the browser widgets.
 *
 * @description This function is connected to the "startup" signal of the
 *              main GtkApplication.
 */
static void startup(GApplication* app, gpointer user_data) {
    Browser* browser = user_data;
    Browser_init(app, browser);
}

/*
 * @brief       Activate the browser.
 *
 * @description This function is connected to the "activate" signal of the
 *              main GtkApplication.
 */
static void activate(
    __attribute__((unused)) GApplication* app,
    __attribute__((unused)) gpointer user_data
) {

}

/*
 * @brief       Open a URL in the browser.
 *
 * @description This function is connected to the "open" signal of the
 *              main GtkApplication.
 */
static void open(
    __attribute__((unused)) GApplication* app,
    gpointer files,
    gint n_files,
    __attribute__((unused)) const gchar* hint,
    gpointer user_data
) {
    GFile** gfiles = files;
    char* url = g_file_get_uri(gfiles[0]);
    Browser* browser = user_data;

    if (n_files > 1) {
        g_warning("More than one URL specified. Will load the first one.");
    }

    // Load the specified URL.
    g_message("Loading: %s\n", url);
    Browser_load(browser, url);
}

/*
 * @brief         Setup CLI option parsing.
 *
 * @param app     The GtkApplication to setup option parsing for.
 * @param browser The Browser instance associated with the application.
 */
static void setupCliParsing(GtkApplication* app, Browser* browser) {
    const GOptionEntry cli_params[] = {
        {
            .long_name = "host",
            .short_name = 'h',
            .flags = G_OPTION_FLAG_NONE,
            .arg = G_OPTION_ARG_STRING,
            .arg_data = &(browser->basicAuth.host),
            .description = "HTTP Basic Authentication hostname",
            .arg_description = "HOST"
        },
        {
            .long_name = "username",
            .short_name = 'u',
            .flags = G_OPTION_FLAG_NONE,
            .arg = G_OPTION_ARG_STRING,
            .arg_data = &(browser->basicAuth.username),
            .description = "HTTP Basic Authentication username",
            .arg_description = "USER"
        },
        {
            .long_name = "password",
            .short_name = 'p',
            .flags = G_OPTION_FLAG_NONE,
            .arg = G_OPTION_ARG_STRING,
            .arg_data = &(browser->basicAuth.password),
            .description = "HTTP Basic Authentication password",
            .arg_description = "PASS"
        },
        {
            .long_name = "width",
            .short_name = 'W',
            .flags = G_OPTION_FLAG_NONE,
            .arg = G_OPTION_ARG_INT,
            .arg_data = &(browser->geometry.width),
            .description = "Set the width of the browser window.",
            .arg_description = "W"
        },
        {
            .long_name = "height",
            .short_name = 'H',
            .flags = G_OPTION_FLAG_NONE,
            .arg = G_OPTION_ARG_INT,
            .arg_data = &(browser->geometry.height),
            .description = "Set the height of the browser window.",
            .arg_description = "H"
        },
        {
            .long_name = "fullscreen",
            .short_name = 'f',
            .flags = G_OPTION_FLAG_NONE,
            .arg = G_OPTION_ARG_NONE,
            .arg_data = &(browser->geometry.fullscreen),
            .description = "Make the browser window fullscreen.",
            .arg_description = NULL
        },
        {NULL}
    };
    g_application_add_main_option_entries(G_APPLICATION(app), cli_params);
}

/*
 * @brief Main entrypoint function.
 */
int main(int argc, char** argv) {
    int status = 0;
    GtkApplication* app;
    Browser browser;

    memset(&browser, 0, sizeof(browser));

    // Create the GTK application.
    app = gtk_application_new(
        "cc.talus.tiny-kiosk-browser",
        G_APPLICATION_HANDLES_OPEN
    );
    g_signal_connect(app, "startup", G_CALLBACK(startup), &browser);
    g_signal_connect(app, "activate", G_CALLBACK(activate), &browser);
    g_signal_connect(app, "open", G_CALLBACK(open), &browser);

    // Configure CLI option parsing.
    setupCliParsing(app, &browser);

    // Run the GTK application.
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
