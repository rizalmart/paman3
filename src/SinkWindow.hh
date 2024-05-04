#ifndef foosinkwindowhhfoo
#define foosinkwindowhhfoo

/* $Id: SinkWindow.hh 86 2007-05-23 23:39:45Z lennart $ */

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

class SinkWindow;

#include "ServerInfoManager.hh"

class SinkWindow : public Gtk::Window {
public:
    SinkWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    static SinkWindow* create();

    Gtk::Label *nameLabel,
        *descriptionLabel,
        *indexLabel,
        *sampleTypeLabel,
        *channelMapLabel,
        *latencyLabel,
        *ownerModuleLabel,
        *monitorSourceLabel,
        *volumeLabel;

    Gtk::Button *closeButton,
        *toMonitorSourceButton,
        *toOwnerModuleButton,
        *volumeResetButton,
        *volumeMuteButton,
        *volumeMeterButton;

    Gtk::Scale *volumeScale;

    uint32_t index, owner_module, monitor_source;
    Glib::ustring monitor_source_name;
    bool scaleEnabled;
    
    void updateInfo(const SinkInfo &i);

protected:
    virtual void on_close_button_clicked();
    virtual void on_to_monitor_source_button_clicked();
    virtual void on_to_owner_module_button_clicked();
    virtual void on_volume_scale_value_changed();
    virtual void on_volume_reset_button_clicked();
    virtual void on_volume_mute_button_clicked();
    virtual void on_volume_meter_button_clicked();
    virtual bool on_delete_event(GdkEventAny* e);
};


#endif
