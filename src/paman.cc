/* $Id: paman.cc 86 2007-05-23 23:39:45Z lennart $ */

/***
  This file is part of paman.
 
  paman is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 2 of the License,
  or (at your option) any later version.
 
  paman is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with paman; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <signal.h>

#include <gtkmm.h>

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>

#include "paman.hh"
#include "SinkWindow.hh"
#include "MainWindow.hh"

MainWindow *mainWindow = NULL;
ServerInfoManager *serverInfoManager = NULL;
struct pa_context *context = NULL;
struct pa_mainloop_api *mainloop_api = NULL;

#define WINDOW_TITLE "PulseAudio Manager"

static void context_state_callback(struct pa_context *c, void *) {
    g_assert(c && mainWindow);

    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_CONNECTING: {
            char t[256];
            snprintf(t, sizeof(t), "Connection to <b>%s</b> ...", pa_context_get_server(c));
            mainWindow->showSuccess(t);
            mainWindow->connectButton->set_sensitive(false);
            return;
        }
            
        case PA_CONTEXT_AUTHORIZING:
            mainWindow->showSuccess("Authorizing ...");
            return;
            
        case PA_CONTEXT_SETTING_NAME:
            mainWindow->showSuccess("Setting name ...");
            return;
            
        case PA_CONTEXT_READY: {
            char t[256];
            snprintf(t, sizeof(t), WINDOW_TITLE" [%s]", pa_context_get_server(c));
            mainWindow->set_title(t);
            mainWindow->showSuccess("Ready");
            g_assert(!serverInfoManager);
            serverInfoManager = new ServerInfoManager(*c);
            mainWindow->statButton->set_sensitive(true);
            mainWindow->disconnectButton->set_sensitive(true);
            return;
        }
            
        case PA_CONTEXT_TERMINATED:
            mainWindow->set_title(WINDOW_TITLE);
            mainWindow->showSuccess("Disconnected");
            break;

        case PA_CONTEXT_FAILED:
        default:
            mainWindow->set_title(WINDOW_TITLE);
            mainWindow->showFailure(pa_strerror(pa_context_errno(c)));
            break;
            
    }
            
    killConnection();
}

void createConnection() {
    if (serverInfoManager) {
        delete serverInfoManager;
        serverInfoManager = NULL;
    }
    
    if (context) {
        pa_context_unref(context);
        context = NULL;
    }

    context = pa_context_new(mainloop_api, "PulseAudio Manager");
    g_assert(context);
    pa_context_set_state_callback(context, context_state_callback, NULL);
    pa_context_connect(context, NULL, (pa_context_flags_t) 0, NULL);
}

void killConnection() {
    if (serverInfoManager) {
        delete serverInfoManager;
        serverInfoManager = NULL;
    }
    
    if (context) {
        pa_context_unref(context);
        context = NULL;
    }

    mainWindow->connectButton->set_sensitive(true);
    mainWindow->disconnectButton->set_sensitive(false);
    mainWindow->statButton->set_sensitive(false);
    mainWindow->clearAllData();
    mainWindow->set_title(WINDOW_TITLE);
}

int main(int argc, char *argv[]) {
    struct pa_glib_mainloop *m;

    signal(SIGPIPE, SIG_IGN);
    
    Gtk::Main kit(argc, argv);

    mainWindow = MainWindow::create();
    g_assert(mainWindow);

    m = pa_glib_mainloop_new(g_main_context_default());
    g_assert(m);
    mainloop_api = pa_glib_mainloop_get_api(m);
    g_assert(mainloop_api);

    createConnection();

    Gtk::Main::run(*mainWindow);
    
    if (serverInfoManager)
        delete serverInfoManager;
    
    if (context)
        pa_context_unref(context);

    mainloop_api = NULL;
    if (mainWindow)
        delete mainWindow;

    pa_glib_mainloop_free(m);

    return 0;
}
