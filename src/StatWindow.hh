#ifndef foostatwindowhhfoo
#define foostatwindowhhfoo

/* $Id: StatWindow.hh 86 2007-05-23 23:39:45Z lennart $ */

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

#include <gtkmm.h>

class StatWindow;

#include "ServerInfoManager.hh"

class StatWindow: public Gtk::Window {
public:
    StatWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~StatWindow();
    static StatWindow* create();

    Gtk::Label *totalLabel,
        *totalSizeLabel,
        *allocatedLabel,
        *allocatedSizeLabel,
        *sampleCacheLabel;

    Gtk::Button *closeButton, *refreshButton;
        
    virtual void onCloseButton();
    virtual void onRefreshButton();
    virtual bool on_delete_event(GdkEventAny* e);

    struct pa_operation *operation;

    static void stat_cb(pa_context* context, const pa_stat_info* i, void* userdata);

    // Override the present() function if necessary
    virtual void present();
};


#endif
