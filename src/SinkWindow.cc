/* $Id: SinkWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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

#include "paman.hh"
#include "SinkWindow.hh"

#define GLADE_NAME "sinkWindow"

SinkWindow::SinkWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    descriptionLabel(nullptr),
    indexLabel(nullptr),
    sampleTypeLabel(nullptr),
    channelMapLabel(nullptr),
    latencyLabel(nullptr),
    ownerModuleLabel(nullptr),
    monitorSourceLabel(nullptr),
    volumeLabel(nullptr),
    closeButton(nullptr),
    toMonitorSourceButton(nullptr),
    toOwnerModuleButton(nullptr),
    volumeResetButton(nullptr),
    volumeMuteButton(nullptr),
    volumeMeterButton(nullptr),
    volumeScale(nullptr),
    scaleEnabled(true) {

    refGlade->get_widget("nameLabel", nameLabel);
    refGlade->get_widget("descriptionLabel2", descriptionLabel);
    refGlade->get_widget("indexLabel", indexLabel);
    refGlade->get_widget("sampleTypeLabel", sampleTypeLabel);
    refGlade->get_widget("channelMapLabel", channelMapLabel);
    refGlade->get_widget("latencyLabel", latencyLabel);
    refGlade->get_widget("ownerModuleLabel", ownerModuleLabel);
    refGlade->get_widget("monitorSourceLabel", monitorSourceLabel);
    refGlade->get_widget("closeButton1", closeButton);
    refGlade->get_widget("toMonitorSourceButton", toMonitorSourceButton);
    refGlade->get_widget("toOwnerModuleButton", toOwnerModuleButton);
    refGlade->get_widget("volumeLabel", volumeLabel);
    refGlade->get_widget("volumeScale", volumeScale);
    refGlade->get_widget("volumeResetButton", volumeResetButton);
    refGlade->get_widget("volumeMuteButton", volumeMuteButton);
    refGlade->get_widget("volumeMeterButton", volumeMeterButton);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkWindow::on_close_button_clicked));
    toMonitorSourceButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkWindow::on_to_monitor_source_button_clicked));
    toOwnerModuleButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkWindow::on_to_owner_module_button_clicked));
    volumeScale->signal_value_changed().connect(sigc::mem_fun(*this, &SinkWindow::on_volume_scale_value_changed));
    volumeResetButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkWindow::on_volume_reset_button_clicked));
    volumeMuteButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkWindow::on_volume_mute_button_clicked));
    volumeMeterButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkWindow::on_volume_meter_button_clicked));

    volumeMeterButton->set_sensitive(serverInfoManager->volumeMeterSupported());
}

SinkWindow* SinkWindow::create() {
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(GLADE_FILE);
    SinkWindow *w = nullptr;
    refBuilder->get_widget_derived(GLADE_NAME, w);
    return w;
}


void SinkWindow::updateInfo(const SinkInfo &i) {
    char t[64], ss[PA_SAMPLE_SPEC_SNPRINT_MAX], cm[PA_CHANNEL_MAP_SNPRINT_MAX];
    double percent;

    nameLabel->set_text(i.name);
    descriptionLabel->set_text(i.description);
    snprintf(t, sizeof(t), "#%u", i.index);
    indexLabel->set_text(t);
    sampleTypeLabel->set_text(pa_sample_spec_snprint(ss, sizeof(ss), &i.sample_spec));
    channelMapLabel->set_text(pa_channel_map_snprint(cm, sizeof(cm), &i.channel_map));
    snprintf(t, sizeof(t), "#%u", i.owner_module);
    ownerModuleLabel->set_text(t);

    snprintf(t, sizeof(t), "%0.0f usec", (double) i.latency);
    latencyLabel->set_text(t);

    SourceInfo *source = serverInfoManager->getSourceInfo(i.monitor_source);
    monitorSourceLabel->set_text(source->name);

    percent = ((double) pa_cvolume_avg(&i.volume) * 100) / PA_VOLUME_NORM; 
    scaleEnabled = false;
    volumeScale->set_value(percent);
    scaleEnabled = true;

    if (i.hw_volume_supported)
        snprintf(t, sizeof(t), "%0.0f%%", percent);
    else {
        double db;
        
        db = pa_sw_volume_to_dB(pa_cvolume_avg(&i.volume));

        if (db != PA_DECIBEL_MININFTY)
            snprintf(t, sizeof(t), "%0.0f%% (%0.2fdB)", percent, db);
        else
            snprintf(t, sizeof(t), "%0.0f%% (-&#8734;dB)", percent);
    }
    volumeLabel->set_markup(t);
    
    set_title("Sink: "+i.name);
    
    monitor_source = i.monitor_source;
    owner_module = i.owner_module;
    index = i.index;
    monitor_source_name = i.monitor_source_name;

    toOwnerModuleButton->set_sensitive(owner_module != PA_INVALID_INDEX);
}

void SinkWindow::on_close_button_clicked() {
    hide();
}

void SinkWindow::on_to_monitor_source_button_clicked() {
    serverInfoManager->showSourceWindow(monitor_source);
}

void SinkWindow::on_to_owner_module_button_clicked() {
    if (owner_module != (uint32_t) -1)
        serverInfoManager->showModuleWindow(owner_module);
}

void SinkWindow::on_volume_scale_value_changed() {
    if (scaleEnabled)
        serverInfoManager->setSinkVolume(index, (pa_volume_t) (((double) volumeScale->get_value()/100) * PA_VOLUME_NORM));
}

void SinkWindow::on_volume_reset_button_clicked() {
    serverInfoManager->setSinkVolume(index, PA_VOLUME_NORM);
}

void SinkWindow::on_volume_mute_button_clicked() {
    serverInfoManager->setSinkVolume(index, PA_VOLUME_MUTED);
}

bool SinkWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}

void SinkWindow::on_volume_meter_button_clicked() {
    serverInfoManager->runVolumeMeter(monitor_source_name);
}
