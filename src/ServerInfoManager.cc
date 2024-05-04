/* $Id: ServerInfoManager.cc 86 2007-05-23 23:39:45Z lennart $ */

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

#include <iostream>

#include <pulse/pulseaudio.h>

#include "ServerInfoManager.hh"
#include "paman.hh"

SinkInfo::SinkInfo(const pa_sink_info &i) :
    driver(i.driver),
    name(i.name),
    index(i.index),
    sample_spec(i.sample_spec),
    channel_map(i.channel_map),
    monitor_source(i.monitor_source),
    owner_module(i.owner_module),
    volume(i.volume),
    hw_volume_supported(!!(i.flags & PA_SINK_HW_VOLUME_CTRL)),
    latency(i.latency),
    monitor_source_name(i.monitor_source_name),
    window(NULL) {

    if (i.description)
        description = i.description;
}

SinkInfo::~SinkInfo() {
    if (window)
        delete window;
}
    
void SinkInfo::update(const pa_sink_info &i) {
    driver = Glib::ustring(i.driver);
    name = Glib::ustring(i.name);
    index = i.index;
    sample_spec = i.sample_spec;
    channel_map = i.channel_map;
    monitor_source = i.monitor_source;
    owner_module = i.owner_module;
    volume = i.volume;
    hw_volume_supported = !!(i.flags & PA_SINK_HW_VOLUME_CTRL),
    latency = i.latency;
    monitor_source_name = i.monitor_source_name;

    description = i.description ? i.description : "";
    
    if (window)
        window->updateInfo(*this);
    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void SinkInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = SinkWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

SourceInfo::SourceInfo(const pa_source_info &i) :
    driver(i.driver),
    name(i.name),
    index(i.index),
    sample_spec(i.sample_spec),
    channel_map(i.channel_map),
    owner_module(i.owner_module),
    monitor_of_sink(i.monitor_of_sink),
    volume(i.volume),
    hw_volume_supported(!!(i.flags & PA_SINK_HW_VOLUME_CTRL)),
    latency(i.latency),
    window(NULL) {

    if (i.description)
        description = i.description;
}

SourceInfo::~SourceInfo() {
    if (window)
        delete window;
}

void SourceInfo::update(const struct pa_source_info &i) {
    driver = i.driver;
    name = i.name;
    index = i.index;
    sample_spec = i.sample_spec;
    channel_map = i.channel_map;
    owner_module = i.owner_module;
    monitor_of_sink = i.monitor_of_sink;
    volume = i.volume;
    hw_volume_supported = !!(i.flags & PA_SOURCE_HW_VOLUME_CTRL),
    latency = i.latency;

    description = i.description ? i.description : "";

    if (window)
        window->updateInfo(*this);
    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void SourceInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = SourceWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

ClientInfo::ClientInfo(const struct pa_client_info &i) :
    index(i.index),
    driver(i.driver),
    name(i.name),
    owner_module(i.owner_module),
    window(NULL) {
}

ClientInfo::~ClientInfo() {
    if (window)
        delete window;
}

void ClientInfo::update(const struct pa_client_info &i) {
    driver = i.driver;
    name = i.name;
    index = i.index;
    owner_module = i.owner_module;

    if (window)
        window->updateInfo(*this);

    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void ClientInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = ClientWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

ModuleInfo::ModuleInfo(const struct pa_module_info &i) :
    index(i.index),
    name(i.name),
    autoloaded(i.auto_unload),
    used(i.n_used),
    window(NULL) {

    if (i.argument)
        argument = i.argument;
}

ModuleInfo::~ModuleInfo() {
    if (window)
        delete window;
}

void ModuleInfo::update(const struct pa_module_info &i) {
    name = i.name;
    index = i.index;
    autoloaded = i.auto_unload;
    used = i.n_used;

    argument = i.argument ? i.argument : "";
    
    if (window)
        window->updateInfo(*this);
    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void ModuleInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = ModuleWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

SinkInputInfo::SinkInputInfo(const struct pa_sink_input_info &i) :
    driver(i.driver),
    name(i.name),
    index(i.index),
    sample_spec(i.sample_spec),
    channel_map(i.channel_map),
    sink(i.sink),
    client(i.client),
    owner_module(i.owner_module),
    volume(i.volume),
    buffer_usec(i.buffer_usec),
    sink_usec(i.sink_usec),
    resample_method(i.resample_method ? i.resample_method : "<i>n/a</i>"),
    window(NULL) {
}

SinkInputInfo::~SinkInputInfo() {
    if (window)
        delete window;
}

void SinkInputInfo::update(const struct pa_sink_input_info &i) {
    driver = i.driver;
    name = i.name;
    index = i.index;
    sample_spec = i.sample_spec;
    channel_map = i.channel_map;
    sink = i.sink;
    client = i.client;
    owner_module = i.owner_module;
    volume = i.volume;
    buffer_usec = i.buffer_usec;
    sink_usec = i.sink_usec;
    resample_method = i.resample_method ? i.resample_method : "<i>n/a</i>";

    if (window)
        window->updateInfo(*this);
    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void SinkInputInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = SinkInputWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

SourceOutputInfo::SourceOutputInfo(const struct pa_source_output_info &i) :
    driver(i.driver),
    name(i.name),
    index(i.index),
    sample_spec(i.sample_spec),
    channel_map(i.channel_map),
    source(i.source),
    client(i.client),
    owner_module(i.owner_module),
    buffer_usec(i.buffer_usec),
    source_usec(i.source_usec),
    resample_method(i.resample_method ? i.resample_method : "<i>n/a</i>"),
    window(NULL) {
}

SourceOutputInfo::~SourceOutputInfo() {
    if (window)
        delete window;
}

void SourceOutputInfo::update(const struct pa_source_output_info &i) {
    driver = i.driver;
    name = i.name;
    index = i.index;
    sample_spec = i.sample_spec;
    channel_map = i.channel_map;
    source = i.source;
    client = i.client;
    owner_module = i.owner_module;
    buffer_usec = i.buffer_usec;
    source_usec = i.source_usec;
    resample_method = i.resample_method ? i.resample_method : "<i>n/a</i>";

    if (window)
        window->updateInfo(*this);
    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void SourceOutputInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = SourceOutputWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

SampleInfo::SampleInfo(const struct pa_sample_info &i) :
    index(i.index),
    name(i.name),
    sample_spec(i.sample_spec),
    duration(i.duration),
    bytes(i.bytes),
    volume(i.volume),
    lazy(!!i.lazy),
    filename_valid(false),
    window(NULL) {

    if (i.filename) {
        filename = i.filename;
        filename_valid = true;
    }
}

SampleInfo::~SampleInfo() {
    if (window)
        delete window;
}

void SampleInfo::update(const struct pa_sample_info &i) {
    index = i.index;
    name = i.name;
    sample_spec = i.sample_spec;
    duration = i.duration;
    bytes = i.bytes;
    volume = i.volume;
    lazy = !!i.lazy;

    filename_valid = !!i.filename;

    if (i.filename)
        filename = i.filename;
    
    if (window)
        window->updateInfo(*this);
    g_assert(mainWindow);
    mainWindow->updateInfo(*this);
}

void SampleInfo::showWindow() {
    if (window)
        window->present();
    else {
        window = SampleWindow::create();
        window->updateInfo(*this);
        window->show();
    }
}

extern "C" {
    static void server_info_callback(struct pa_context *c, const struct pa_server_info *i, void *userdata);
    static void sink_info_callback(struct pa_context *c, const struct pa_sink_info *i, int is_last, void *userdata);
    static void source_info_callback(struct pa_context *c, const struct pa_source_info *i, int is_last, void *userdata);
    static void client_info_callback(struct pa_context *c, const struct pa_client_info *i, int is_last, void *userdata);
    static void module_info_callback(struct pa_context *c, const struct pa_module_info *i, int is_last, void *userdata);
    static void sink_input_info_callback(struct pa_context *c, const struct pa_sink_input_info *i, int is_last, void *userdata);
    static void source_output_info_callback(struct pa_context *c, const struct pa_source_output_info *i, int is_last, void *userdata);
    static void sample_info_callback(struct pa_context *c, const struct pa_sample_info *i, int is_last, void *userdata);
    static void subscribe_callback(struct pa_context *c, enum pa_subscription_event_type t, uint32_t index, void *userdata);
};

static void server_info_callback(struct pa_context *c, const struct pa_server_info *i, void *) {
    if (!i) {
        mainWindow->showFailure(pa_strerror(pa_context_errno(c)));
        return;
    }

    mainWindow->updateInfo(*i);
}

static void sink_info_callback(struct pa_context *, const struct pa_sink_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void source_info_callback(struct pa_context *, const struct pa_source_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void client_info_callback(struct pa_context *, const struct pa_client_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void module_info_callback(struct pa_context *, const struct pa_module_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void sink_input_info_callback(struct pa_context *, const struct pa_sink_input_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void source_output_info_callback(struct pa_context *, const struct pa_source_output_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void sample_info_callback(struct pa_context *, const struct pa_sample_info *i, int, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;
    if (i) si->updateInfo(*i);
}

static void subscribe_callback(struct pa_context *c, enum pa_subscription_event_type t, uint32_t index, void *userdata) {
    ServerInfoManager *si = (ServerInfoManager*) userdata;

//    fprintf(stderr, "EV: %u %u\n", t, index);
    
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeSinkInfo(index);
            else
                pa_operation_unref(pa_context_get_sink_info_by_index(c, index, sink_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_SOURCE:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeSourceInfo(index);
            else
                pa_operation_unref(pa_context_get_source_info_by_index(c, index, source_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_MODULE:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeModuleInfo(index);
            else
                pa_operation_unref(pa_context_get_module_info(c, index, module_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_CLIENT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeClientInfo(index);
            else
                pa_operation_unref(pa_context_get_client_info(c, index, client_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeSinkInputInfo(index);
            else
                pa_operation_unref(pa_context_get_sink_input_info(c, index, sink_input_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeSourceOutputInfo(index);
            else
                pa_operation_unref(pa_context_get_source_output_info(c, index, source_output_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE:
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
                si->removeSampleInfo(index);
            else
                pa_operation_unref(pa_context_get_sample_info_by_index(c, index, sample_info_callback, si));
            break;
        case PA_SUBSCRIPTION_EVENT_SERVER:
            pa_operation_unref(pa_context_get_server_info(c, server_info_callback, si));
            break;
            
        default:
            fprintf(stderr, "OTHER EVENT\n");
            break;
    }            
}

ServerInfoManager::ServerInfoManager(struct pa_context &c) :
    context(c),
    statWindow(NULL) {
    pa_operation_unref(pa_context_get_server_info(&c, server_info_callback, this));
    pa_operation_unref(pa_context_get_sink_info_list(&c, sink_info_callback, this));
    pa_operation_unref(pa_context_get_source_info_list(&c, source_info_callback, this));
    pa_operation_unref(pa_context_get_module_info_list(&c, module_info_callback, this));
    pa_operation_unref(pa_context_get_client_info_list(&c, client_info_callback, this));
    pa_operation_unref(pa_context_get_sink_input_info_list(&c, sink_input_info_callback, this));
    pa_operation_unref(pa_context_get_source_output_info_list(&c, source_output_info_callback, this));
    pa_operation_unref(pa_context_get_sample_info_list(&c, sample_info_callback, this));
    
    pa_context_set_subscribe_callback(&c, subscribe_callback, this);

    pa_operation_unref(pa_context_subscribe(&c, (enum pa_subscription_mask)
                                            (PA_SUBSCRIPTION_MASK_SINK|
                                             PA_SUBSCRIPTION_MASK_SOURCE|
                                             PA_SUBSCRIPTION_MASK_MODULE|
                                             PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                             PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                             PA_SUBSCRIPTION_MASK_CLIENT|
                                             PA_SUBSCRIPTION_MASK_SAMPLE_CACHE|
                                             PA_SUBSCRIPTION_MASK_SERVER), NULL, NULL));
}


ServerInfoManager::~ServerInfoManager() {
	
    for (std::map<int, SinkInfo*>::iterator i = sinks.begin(); i != sinks.end(); i++)
        delete i->second;

    for (std::map<int, SourceInfo*>::iterator i = sources.begin(); i != sources.end(); i++)
        delete i->second;

    for (std::map<int, ClientInfo*>::iterator i = clients.begin(); i != clients.end(); i++)
        delete i->second;

    for (std::map<int, ModuleInfo*>::iterator i = modules.begin(); i != modules.end(); i++)
        delete i->second;

    for (std::map<int, SinkInputInfo*>::iterator i = sinkInputs.begin(); i != sinkInputs.end(); i++)
        delete i->second;

    for (std::map<int, SourceOutputInfo*>::iterator i = sourceOutputs.begin(); i != sourceOutputs.end(); i++)
        delete i->second;

    for (std::map<int, SampleInfo*>::iterator i = samples.begin(); i != samples.end(); i++)
        delete i->second;
    
    if (statWindow)
        delete statWindow;
}

void ServerInfoManager::updateInfo(const struct pa_sink_info &i) {
    SinkInfo *si;
    if ((si = sinks[i.index]))
        si->update(i);
    else {
        SinkInfo *n = new SinkInfo(i);
        sinks[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}

void ServerInfoManager::updateInfo(const struct pa_source_info &i) {
    SourceInfo *si;
    
    if ((si = sources[i.index]))
        si->update(i);
    else {
        SourceInfo *n = new SourceInfo(i);
        sources[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}

void ServerInfoManager::updateInfo(const struct pa_client_info &i) {
    ClientInfo *ci;

    if ((ci = clients[i.index]))
        ci->update(i);
    else {
        ClientInfo *n = new ClientInfo(i);
        clients[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}
void ServerInfoManager::updateInfo(const struct pa_module_info &i) {
    ModuleInfo *si;
    if ((si = modules[i.index]))
        si->update(i);
    else {
        ModuleInfo *n = new ModuleInfo(i);
        modules[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}

void ServerInfoManager::updateInfo(const struct pa_sink_input_info &i) {
    SinkInputInfo *si;
    if ((si = sinkInputs[i.index]))
        si->update(i);
    else {
        SinkInputInfo *n = new SinkInputInfo(i);
        sinkInputs[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}

void ServerInfoManager::updateInfo(const struct pa_source_output_info &i) {
    SourceOutputInfo *si;
    if ((si = sourceOutputs[i.index]))
        si->update(i);
    else {
        SourceOutputInfo *n = new SourceOutputInfo(i);
        sourceOutputs[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}

void ServerInfoManager::updateInfo(const struct pa_sample_info &i) {
    SampleInfo *si;
    if ((si = samples[i.index]))
        si->update(i);
    else {
        SampleInfo *n = new SampleInfo(i);
        samples[i.index] = n;
        mainWindow->updateInfo(*n);
    }
}

void ServerInfoManager::showSinkWindow(uint32_t index) {
    SinkInfo *i;

    if ((i = sinks[index]))
        i->showWindow();
}

void ServerInfoManager::showSourceWindow(uint32_t index) {
	
    SourceInfo *i;

    if ((i = sources[index]))
        i->showWindow();
}

void ServerInfoManager::showClientWindow(uint32_t index) {
    ClientInfo *i;

    if ((i = clients[index]))
        i->showWindow();
}

void ServerInfoManager::showModuleWindow(uint32_t index) {
    ModuleInfo *i;

    if ((i = modules[index]))
        i->showWindow();
}

void ServerInfoManager::showSinkInputWindow(uint32_t index) {
    SinkInputInfo *i;

    if ((i = sinkInputs[index]))
        i->showWindow();
}

void ServerInfoManager::showSourceOutputWindow(uint32_t index) {
    SourceOutputInfo *i;

    if ((i = sourceOutputs[index]))
        i->showWindow();
}

void ServerInfoManager::showSampleWindow(uint32_t index) {
    SampleInfo *i;

    if ((i = samples[index]))
        i->showWindow();
}


SourceInfo* ServerInfoManager::getSourceInfo(uint32_t index) {
    return sources[index];
}

SinkInfo* ServerInfoManager::getSinkInfo(uint32_t index) {
    return sinks[index];
}

ClientInfo* ServerInfoManager::getClientInfo(uint32_t index) {
    return clients[index];
}

ModuleInfo* ServerInfoManager::getModuleInfo(uint32_t index) {
    return modules[index];
}

SampleInfo* ServerInfoManager::getSampleInfo(uint32_t index) {
    return samples[index];
}


void ServerInfoManager::removeSinkInfo(uint32_t index) {
    SinkInfo *i;

    if ((i = sinks[index])) {
        sinks.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

void ServerInfoManager::removeSourceInfo(uint32_t index) {
    SourceInfo *i;
    if ((i = sources[index])) {
        sources.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

void ServerInfoManager::removeClientInfo(uint32_t index) {
    ClientInfo *i;

    if ((i = clients[index])) {
        clients.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

void ServerInfoManager::removeModuleInfo(uint32_t index) {
    ModuleInfo *i;
    if ((i = modules[index])) {
        modules.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

void ServerInfoManager::removeSinkInputInfo(uint32_t index) {
    SinkInputInfo *i;
    if ((i = sinkInputs[index])) {
        sinkInputs.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

void ServerInfoManager::removeSourceOutputInfo(uint32_t index) {
    SourceOutputInfo *i;
    if ((i = sourceOutputs[index])) {
        sourceOutputs.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

void ServerInfoManager::removeSampleInfo(uint32_t index) {
    SampleInfo *i;
    if ((i = samples[index])) {
        samples.erase(index);
        mainWindow->removeInfo(*i);
        delete i;
    }
}

/* FIXME: Handle all channels separately. */
void ServerInfoManager::setSinkVolume(uint32_t index, pa_volume_t volume) {
    pa_cvolume cvol;
    pa_cvolume_set(&cvol, sinks[index]->volume.channels, volume);
    pa_operation_unref(pa_context_set_sink_volume_by_index(&context, index, &cvol, NULL, NULL));
}

/* FIXME: Handle all channels separately. */
void ServerInfoManager::setSourceVolume(uint32_t index, pa_volume_t volume) {
    pa_cvolume cvol;
    pa_cvolume_set(&cvol, sources[index]->volume.channels, volume);
    pa_operation_unref(pa_context_set_source_volume_by_index(&context, index, &cvol, NULL, NULL));
}

/* FIXME: Handle all channels separately. */
void ServerInfoManager::setSinkInputVolume(uint32_t index, pa_volume_t volume) {
    pa_cvolume cvol;
    pa_cvolume_set(&cvol, sinkInputs[index]->volume.channels, volume);
    pa_operation_unref(pa_context_set_sink_input_volume(&context, index, &cvol, NULL, NULL));
}

void ServerInfoManager::showStatWindow() {
    if (statWindow)
        statWindow->present();
    else {
        statWindow = StatWindow::create();
        statWindow->show();
    }
}

void ServerInfoManager::playSample(uint32_t sample, uint32_t sink) {
    SinkInfo *sinki = getSinkInfo(sink);
    SampleInfo *samplei = getSampleInfo(sample);

    if (!sinki || !samplei)
        return;

    pa_operation_unref(pa_context_play_sample(&context, samplei->name.c_str(), sinki->name.c_str(), PA_VOLUME_NORM, NULL, NULL));
}

#ifndef PAVUMETER_PATH
#define PAVUMETER_PATH "pavumeter"
#endif

void ServerInfoManager::runVolumeMeter(const Glib::ustring &source) {
    char t[256];
    snprintf(t, sizeof(t), "%s --record %s", PAVUMETER_PATH, source.c_str());
    Glib::spawn_command_line_async(t);
}

bool ServerInfoManager::volumeMeterSupported() {
    gchar *c;
    c = g_find_program_in_path(PAVUMETER_PATH);
    g_free(c);

    return !!c;
}

void ServerInfoManager::killClient(uint32_t index) {
    pa_operation_unref(pa_context_kill_client(&context, index, NULL, NULL));
}

void ServerInfoManager::killSourceOutput(uint32_t index) {
   pa_operation_unref(pa_context_kill_source_output(&context, index, NULL, NULL));
}

void ServerInfoManager::killSinkInput(uint32_t index) {
   pa_operation_unref(pa_context_kill_sink_input(&context, index, NULL, NULL));
}
