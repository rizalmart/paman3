/* $Id: SourceWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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

#include "paman.hh"
#include "SourceWindow.hh"

#define GLADE_NAME "sourceWindow"

SourceWindow::SourceWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    descriptionLabel(nullptr),
    indexLabel(nullptr),
    sampleTypeLabel(nullptr),
    ownerModuleLabel(nullptr),
    monitorOfSinkLabel(nullptr),
    latencyLabel(nullptr),
    volumeLabel(nullptr),
    closeButton(nullptr),
    toParentSinkButton(nullptr),
    toOwnerModuleButton(nullptr),
    volumeResetButton(nullptr),
    volumeMuteButton(nullptr),
    volumeMeterButton(nullptr),
    volumeScale(nullptr) {

    refBuilder->get_widget("nameLabel2", nameLabel);
    refBuilder->get_widget("descriptionLabel1", descriptionLabel);
    refBuilder->get_widget("indexLabel2", indexLabel);
    refBuilder->get_widget("sampleTypeLabel1", sampleTypeLabel);
    refBuilder->get_widget("channelMapLabel1", channelMapLabel);
    refBuilder->get_widget("ownerModuleLabel2", ownerModuleLabel);
    refBuilder->get_widget("monitorOfSinkLabel", monitorOfSinkLabel);
    refBuilder->get_widget("latencyLabel2", latencyLabel);
    refBuilder->get_widget("closeButton2", closeButton);
    refBuilder->get_widget("toParentSinkButton", toParentSinkButton);
    refBuilder->get_widget("toOwnerModuleButton1", toOwnerModuleButton);
    refBuilder->get_widget("volumeLabel2", volumeLabel);
    refBuilder->get_widget("volumeScale2", volumeScale);
    refBuilder->get_widget("volumeResetButton2", volumeResetButton);
    refBuilder->get_widget("volumeMuteButton2", volumeMuteButton);
    refBuilder->get_widget("volumeMeterButton2", volumeMeterButton);
    
    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceWindow::onCloseButton));
    toParentSinkButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceWindow::onParentSinkButton));
    toOwnerModuleButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceWindow::onToOwnerModuleButton));
    volumeScale->signal_value_changed().connect(sigc::mem_fun(*this, &SourceWindow::onVolumeScaleValueChanged));
    volumeResetButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceWindow::onVolumeResetButton));
    volumeMuteButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceWindow::onVolumeMuteButton));
    volumeMeterButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceWindow::onVolumeMeterButton));

    volumeMeterButton->set_sensitive(serverInfoManager->volumeMeterSupported());
}

SourceWindow* SourceWindow::create() {
    SourceWindow *w = nullptr;
    auto builder = Gtk::Builder::create_from_file(GLADE_FILE);
    builder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void SourceWindow::updateInfo(const SourceInfo &i) {
    char t[20], ss[PA_SAMPLE_SPEC_SNPRINT_MAX], cm[PA_CHANNEL_MAP_SNPRINT_MAX];
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
    
    monitorOfSinkLabel->set_markup("<i>n/a</i>");
    toParentSinkButton->set_sensitive(false);
    if (i.monitor_of_sink != (uint32_t) -1) {
        SinkInfo *sink = serverInfoManager->getSinkInfo(i.monitor_of_sink);
        if (sink) {
            monitorOfSinkLabel->set_text(sink->name);
            toParentSinkButton->set_sensitive(true);
        }
    }

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

    set_title("Source: "+i.name);

    monitor_of_sink = i.monitor_of_sink;
    owner_module = i.owner_module;
    index = i.index;
    source_name = i.name;

    toOwnerModuleButton->set_sensitive(owner_module != (uint32_t) -1);
}

void SourceWindow::onCloseButton() {
    hide();
}

void SourceWindow::onParentSinkButton() {
    if (monitor_of_sink != (uint32_t) -1)
        serverInfoManager->showSinkWindow(monitor_of_sink);
}

void SourceWindow::onToOwnerModuleButton() {
    if (owner_module != (uint32_t) -1)
        serverInfoManager->showModuleWindow(owner_module);
}

void SourceWindow::onVolumeScaleValueChanged() {
    if (scaleEnabled)
        serverInfoManager->setSourceVolume(index, (pa_volume_t) (((double) volumeScale->get_value()/100) * PA_VOLUME_NORM));
}

void SourceWindow::onVolumeResetButton() {
    serverInfoManager->setSourceVolume(index, PA_VOLUME_NORM);
}

void SourceWindow::onVolumeMuteButton() {
    serverInfoManager->setSourceVolume(index, PA_VOLUME_MUTED);
}

bool SourceWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}

void SourceWindow::onVolumeMeterButton() {
    serverInfoManager->runVolumeMeter(source_name);
}

