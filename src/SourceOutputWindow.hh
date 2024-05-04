#ifndef foosourceoutputwindowhhfoo
#define foosourceoutputwindowhhfoo

/* $Id: SourceOutputWindow.hh 86 2007-05-23 23:39:45Z lennart $ */

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
//#include <libglademm.h>

class SourceOutputWindow;

#include "ServerInfoManager.hh"

class SourceOutputWindow : public Gtk::Window {
public:
    SourceOutputWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    static SourceOutputWindow* create();

    Gtk::Label *nameLabel,
        *indexLabel,
        *sampleTypeLabel,
        *channelMapLabel,
        *sourceLabel,
        *clientLabel,
        *ownerModuleLabel,
        *latencyLabel,
        *resampleMethodLabel;

    Gtk::Button *closeButton,
        *toOwnerModuleButton,
        *toClientButton,
        *toSourceButton,
        *killButton;

    uint32_t owner_module, source, client, index;
    
    void updateInfo(const SourceOutputInfo &i);

    virtual void onCloseButton();
    virtual void onToOwnerModuleButton();
    virtual void onToClientButton();
    virtual void onToSourceButton();
    virtual bool on_delete_event(GdkEventAny* e);
    virtual void onKillButton();
};


#endif
