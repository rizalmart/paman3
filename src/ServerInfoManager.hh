#ifndef fooserverinfohhfoo
#define fooserverinfohhfoo

/* $Id: ServerInfoManager.hh 86 2007-05-23 23:39:45Z lennart $ */

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
#include <map>

#include <pulse/pulseaudio.h>

class SinkInfo;
class SourceInfo;
class ServerInfo;
class ClientInfo;
class ModuleInfo;
class SinkInputInfo;
class SourceOutputInfo;
class SampleInfo;

#include "SinkWindow.hh"
#include "SourceWindow.hh"
#include "ClientWindow.hh"
#include "ModuleWindow.hh"
#include "MainWindow.hh"
#include "SinkInputWindow.hh"
#include "SourceOutputWindow.hh"
#include "StatWindow.hh"
#include "SampleWindow.hh"
 
class SinkInfo {
public:

    SinkInfo(const struct pa_sink_info &i);
    ~SinkInfo();
    
    void update(const struct pa_sink_info &i);
    void showWindow();
    
    Glib::ustring driver, name, description;
    uint32_t index;
    struct pa_sample_spec sample_spec;
    struct pa_channel_map channel_map;
    uint32_t monitor_source;
    uint32_t owner_module;
    pa_cvolume volume;
    int hw_volume_supported;
    pa_usec_t latency;
    Glib::ustring monitor_source_name;

    Gtk::TreeRowReference treeRef, sinkComboBoxTreeRef;

    SinkWindow *window;
};

class SourceInfo {
public:
    SourceInfo(const struct pa_source_info &i);
    ~SourceInfo();

    void update(const struct pa_source_info &i);
    void showWindow();
    
    Glib::ustring driver, name, description;
    uint32_t index;
    struct pa_sample_spec sample_spec;
    struct pa_channel_map channel_map;
    uint32_t owner_module;
    uint32_t monitor_of_sink;
    pa_cvolume volume;
    int hw_volume_supported;
    pa_usec_t latency;

    Gtk::TreeRowReference treeRef;
    SourceWindow *window;
};

class ModuleInfo {
public:
    ModuleInfo(const struct pa_module_info &i);
    ~ModuleInfo();

    void update(const struct pa_module_info &i);
    void showWindow();
    
    uint32_t index;
    Glib::ustring name, argument;
    bool autoloaded;
    uint32_t used;

    Gtk::TreeRowReference treeRef;
    ModuleWindow *window;
};

class ClientInfo {
public:
    ClientInfo(const struct pa_client_info &i);
    ~ClientInfo();

    void update(const struct pa_client_info &i);
    void showWindow();
    
    uint32_t index;
    Glib::ustring driver, name;
    uint32_t owner_module;

    Gtk::TreeRowReference treeRef;
    ClientWindow *window;
};

class SinkInputInfo {
public:

    SinkInputInfo(const struct pa_sink_input_info &i);
    ~SinkInputInfo();
    
    void update(const struct pa_sink_input_info &i);
    void showWindow();
    
    Glib::ustring driver, name;
    uint32_t index;
    struct pa_sample_spec sample_spec;
    struct pa_channel_map channel_map;
    uint32_t sink;
    uint32_t client;
    uint32_t owner_module;
    pa_cvolume volume;
    pa_usec_t buffer_usec, sink_usec;
    Glib::ustring resample_method;

    Gtk::TreeRowReference treeRef;

    SinkInputWindow *window;
};

class SourceOutputInfo {
public:

    SourceOutputInfo(const struct pa_source_output_info &i);
    ~SourceOutputInfo();
    
    void update(const struct pa_source_output_info &i);
    void showWindow();
    
    Glib::ustring driver, name;
    uint32_t index;
    struct pa_sample_spec sample_spec;
    struct pa_channel_map channel_map;
    uint32_t source;
    uint32_t client;
    uint32_t owner_module;
    pa_usec_t buffer_usec, source_usec;
    Glib::ustring resample_method;

    Gtk::TreeRowReference treeRef;

    SourceOutputWindow *window;
};

class SampleInfo {
public:

    SampleInfo(const struct pa_sample_info &i);
    ~SampleInfo();
    
    void update(const struct pa_sample_info &i);
    void showWindow();

    uint32_t index;
    Glib::ustring name;
    struct pa_sample_spec sample_spec;
    uint32_t duration, bytes;
    pa_cvolume volume;
    bool lazy;
    Glib::ustring filename;

    bool filename_valid;

    Gtk::TreeRowReference treeRef;

    SampleWindow *window;
};

class ServerInfoManager {
public:
    ServerInfoManager(struct pa_context &c);
    ~ServerInfoManager();

    void updateInfo(const struct pa_sink_info &i);
    void updateInfo(const struct pa_source_info &i);
    void updateInfo(const struct pa_client_info &i);
    void updateInfo(const struct pa_module_info &i);
    void updateInfo(const struct pa_sink_input_info &i);
    void updateInfo(const struct pa_source_output_info &i);
    void updateInfo(const struct pa_sample_info &i);

    void showSinkWindow(uint32_t index);
    void showSourceWindow(uint32_t index);
    void showClientWindow(uint32_t index);
    void showModuleWindow(uint32_t index);
    void showSinkInputWindow(uint32_t index);
    void showSourceOutputWindow(uint32_t index);
    void showSampleWindow(uint32_t index);

    SourceInfo* getSourceInfo(uint32_t index);
    SinkInfo* getSinkInfo(uint32_t index);
    ClientInfo* getClientInfo(uint32_t index);
    ModuleInfo* getModuleInfo(uint32_t index);
    SampleInfo* getSampleInfo(uint32_t index);

    void removeSinkInfo(uint32_t index);
    void removeSourceInfo(uint32_t index);
    void removeClientInfo(uint32_t index);
    void removeModuleInfo(uint32_t index);
    void removeSinkInputInfo(uint32_t index);
    void removeSourceOutputInfo(uint32_t index);
    void removeSampleInfo(uint32_t index);

    void setSinkVolume(uint32_t index, pa_volume_t volume);
    void setSourceVolume(uint32_t index, pa_volume_t volume);
    void setSinkInputVolume(uint32_t index, pa_volume_t volume);

    void showStatWindow();

    void playSample(uint32_t sample, uint32_t sink);

    void runVolumeMeter(const Glib::ustring &source);
    bool volumeMeterSupported();
    
    void killClient(uint32_t index);
    void killSourceOutput(uint32_t index);
    void killSinkInput(uint32_t index);
    
protected:
    std::map<int, SinkInfo*> sinks;
    std::map<int, SourceInfo*> sources;
    std::map<int, ClientInfo*> clients;
    std::map<int, ModuleInfo*> modules;
    std::map<int, SinkInputInfo*> sinkInputs;
    std::map<int, SourceOutputInfo*> sourceOutputs;
    std::map<int, SampleInfo*> samples;

    struct pa_context &context;
    StatWindow *statWindow;
};

#endif
