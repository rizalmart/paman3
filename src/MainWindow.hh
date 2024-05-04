#ifndef foomainwindowhhfoo
#define foomainwindowhhfoo

/* $Id: MainWindow.hh 86 2007-05-23 23:39:45Z lennart $ */

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
#include <pulse/pulseaudio.h>

class MainWinow;

#include "ServerInfoManager.hh"

class MainWindow : public Gtk::Window {
public:
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    virtual ~MainWindow();
    static MainWindow* create();

    Gtk::Label *statusLabel,
        *serverNameLabel,
        *serverVersionLabel,
        *defaultSampleTypeLabel,
        *userNameLabel,
        *hostNameLabel,
        *linkLibraryVersionLabel,
        *compiledLibraryVersionLabel,
        *titleLabel,
        *defaultSinkLabel,
        *defaultSourceLabel;

    Gtk::Button *deviceOpenButton,
        *clientOpenButton,
        *moduleOpenButton,
        *sampleOpenButton,
        *samplePlayButton,
        *connectButton,
        *disconnectButton,
        *statButton;

    Gtk::TreeView *deviceTreeView,
        *clientTreeView,
        *moduleTreeView,
        *sampleTreeView;

    Gtk::EventBox *titleEventBox;

    Gtk::ComboBox *sinkComboBox;

protected:

    class DeviceTreeModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        
        DeviceTreeModelColumns() {
            add(name);
            add(description);
            add(index);
            add(type);
        }

        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<Glib::ustring> description;
        Gtk::TreeModelColumn<uint32_t> index;
        Gtk::TreeModelColumn<int> type;
    };

    DeviceTreeModelColumns deviceTreeModelColumns;
    Glib::RefPtr<Gtk::TreeStore> deviceTreeStore;
    Gtk::TreeRowReference sinkRef, sourceRef;

    class ClientTreeModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ClientTreeModelColumns() {
            add(name);
            add(index);
        }

        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<uint32_t> index;
    };
    
    ClientTreeModelColumns clientTreeModelColumns;
    Glib::RefPtr<Gtk::TreeStore> clientTreeStore;

    class ModuleTreeModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModuleTreeModelColumns() {
            add(name);
            add(argument);
            add(index);
        }

        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<Glib::ustring> argument;
        Gtk::TreeModelColumn<uint32_t> index;
    };
    
    ModuleTreeModelColumns moduleTreeModelColumns;
    Glib::RefPtr<Gtk::TreeStore> moduleTreeStore;

    class SampleTreeModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        SampleTreeModelColumns() {
            add(name);
            add(index);
        }

        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<uint32_t> index;
    };
    
    SampleTreeModelColumns sampleTreeModelColumns;
    Glib::RefPtr<Gtk::TreeStore> sampleTreeStore;

    class SinkTreeModelColumns  : public Gtk::TreeModel::ColumnRecord {
    public:
        SinkTreeModelColumns() {
            add(name);
            add(index);
        }

        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<uint32_t> index;
    };
    
    SinkTreeModelColumns sinkTreeModelColumns;
    Glib::RefPtr<Gtk::ListStore> sinkListStore;
    
public:
    virtual void updateInfo(const struct pa_server_info &i);
    virtual void updateInfo(SinkInfo &i);
    virtual void updateInfo(SourceInfo &i);
    virtual void updateInfo(ClientInfo &i);
    virtual void updateInfo(ModuleInfo &i);
    virtual void updateInfo(SinkInputInfo &i);
    virtual void updateInfo(SourceOutputInfo &i);
    virtual void updateInfo(SampleInfo &i);

    virtual void removeInfo(SinkInfo &i);
    virtual void removeInfo(SourceInfo &i);
    virtual void removeInfo(ClientInfo &i);
    virtual void removeInfo(ModuleInfo &i);
    virtual void removeInfo(SinkInputInfo &i);
    virtual void removeInfo(SourceOutputInfo &i);
    virtual void removeInfo(SampleInfo &i);
    
    virtual void onDeviceTreeViewCursorChanged();
    virtual void onDeviceTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);
    virtual void onClientTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);
    virtual void onModuleTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);
    virtual void onSampleTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);

    virtual void showSuccess(const char *t);
    virtual void showFailure(const char *t);
    virtual void clearAllData();

    virtual void onDeviceOpenButton();
    virtual void onClientOpenButton();
    virtual void onModuleOpenButton();
    virtual void onSampleOpenButton();
    virtual void onSamplePlayButton();
    virtual void onConnectButton();
    virtual void onDisconnectButton();
    virtual void onStatButton();

    virtual void showDeviceWindow(const Gtk::TreePath &p);
    virtual void showClientWindow(const Gtk::TreePath &p);
    virtual void showModuleWindow(const Gtk::TreePath &p);
    virtual void showSampleWindow(const Gtk::TreePath &p);
};


#endif
