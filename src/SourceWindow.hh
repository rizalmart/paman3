#ifndef foosourcewindowhhfoo
#define foosourcewindowhhfoo

/* $Id: SourceWindow.hh 86 2007-05-23 23:39:45Z lennart $ */

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

class SourceWindow : public Gtk::Window {
public:
    SourceWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    static SourceWindow* create();

    Gtk::Label *nameLabel,
        *descriptionLabel,
        *indexLabel,
        *sampleTypeLabel,
        *channelMapLabel,
        *ownerModuleLabel,
        *monitorOfSinkLabel,
        *latencyLabel,
        *volumeLabel;

    Gtk::Button *closeButton,
        *toParentSinkButton,
        *toOwnerModuleButton,
        *volumeResetButton,
        *volumeMuteButton,
        *volumeMeterButton;

    Gtk::Scale *volumeScale; // Gtk::HScale is deprecated, using Gtk::Scale instead
    
    guint32 index, monitor_of_sink, owner_module; // changed uint32_t to guint32
    Glib::ustring source_name;
    bool scaleEnabled;
    
    void updateInfo(const SourceInfo &i);

    virtual void onCloseButton();
    virtual void onParentSinkButton();
    virtual void onToOwnerModuleButton();
    virtual void onVolumeScaleValueChanged();
    virtual void onVolumeResetButton();
    virtual void onVolumeMuteButton();
    virtual void onVolumeMeterButton();

    virtual bool on_delete_event(GdkEventAny* e);
};


#endif
