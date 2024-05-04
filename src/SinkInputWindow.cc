/* $Id: SinkInputWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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
#include "SinkInputWindow.hh"

#define GLADE_NAME "sinkInputWindow"

SinkInputWindow::SinkInputWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    indexLabel(nullptr),
    sampleTypeLabel(nullptr),
    channelMapLabel(nullptr),
    latencyLabel(nullptr),
    sinkLabel(nullptr),
    clientLabel(nullptr),
    ownerModuleLabel(nullptr),
    volumeLabel(nullptr),
    resampleMethodLabel(nullptr),
    closeButton(nullptr),
    toOwnerModuleButton(nullptr),
    toClientButton(nullptr),
    toSinkButton(nullptr),
    volumeResetButton(nullptr),
    volumeMuteButton(nullptr),
    killButton(nullptr),
    volumeScale(nullptr),
    scaleEnabled(true) {

    refGlade->get_widget("nameLabel5", nameLabel);
    refGlade->get_widget("indexLabel5", indexLabel);
    refGlade->get_widget("sampleTypeLabel2", sampleTypeLabel);
    refGlade->get_widget("channelMapLabel2", channelMapLabel);
    refGlade->get_widget("latencyLabel3", latencyLabel);
    refGlade->get_widget("sinkLabel", sinkLabel);
    refGlade->get_widget("clientLabel", clientLabel);
    refGlade->get_widget("ownerModuleLabel4", ownerModuleLabel);
    refGlade->get_widget("closeButton5", closeButton);
    refGlade->get_widget("toOwnerModuleButton3", toOwnerModuleButton);
    refGlade->get_widget("toClientButton", toClientButton);
    refGlade->get_widget("toSinkButton", toSinkButton);
    refGlade->get_widget("volumeLabel3", volumeLabel);
    refGlade->get_widget("volumeScale3", volumeScale);
    refGlade->get_widget("volumeResetButton3", volumeResetButton);
    refGlade->get_widget("volumeMuteButton3", volumeMuteButton);
    refGlade->get_widget("killButton2", killButton);
    refGlade->get_widget("resampleMethodLabel", resampleMethodLabel);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_close_button_clicked));
    toOwnerModuleButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_to_owner_module_button_clicked));
    toClientButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_to_client_button_clicked));
    toSinkButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_to_sink_button_clicked));
    volumeScale->signal_value_changed().connect(sigc::mem_fun(*this, &SinkInputWindow::on_volume_scale_value_changed));
    volumeResetButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_volume_reset_button_clicked));
    volumeMuteButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_volume_mute_button_clicked));
    killButton->signal_clicked().connect(sigc::mem_fun(*this, &SinkInputWindow::on_kill_button_clicked));
}

SinkInputWindow* SinkInputWindow::create() {
    SinkInputWindow *w = nullptr;
    
    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file(GLADE_FILE);
    
    refBuilder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void SinkInputWindow::updateInfo(const SinkInputInfo &i) {
    char t[80], ss[PA_SAMPLE_SPEC_SNPRINT_MAX], cm[PA_CHANNEL_MAP_SNPRINT_MAX];
    double percent, db;

    nameLabel->set_text(i.name);
    snprintf(t, sizeof(t), "#%u", i.index);
    indexLabel->set_text(t);

    sampleTypeLabel->set_text(pa_sample_spec_snprint(ss, sizeof(ss), &i.sample_spec));
    channelMapLabel->set_text(pa_channel_map_snprint(cm, sizeof(cm), &i.channel_map));

    if (i.owner_module == PA_INVALID_INDEX)
        ownerModuleLabel->set_markup("<i>n/a</i>");
    else {
        snprintf(t, sizeof(t), "#%u", i.owner_module);
        ownerModuleLabel->set_text(t);
    }

    snprintf(t, sizeof(t), "%0.0f &#956;s (= buffer: %0.0f &#956;s + sink: %0.0f &#956;s)", (double) i.buffer_usec+i.sink_usec, (double) i.buffer_usec, (double) i.sink_usec);
    latencyLabel->set_markup(t);

    SinkInfo *sinkInfo = serverInfoManager->getSinkInfo(i.sink);
    if (sinkInfo != nullptr)
        sinkLabel->set_text(sinkInfo->name);
    else
        sinkLabel->set_text("<i>n/a</i>");

    if (i.client == PA_INVALID_INDEX)
        clientLabel->set_markup("<i>n/a</i>");
    else {
        ClientInfo *clientInfo = serverInfoManager->getClientInfo(i.client);
        if (clientInfo != nullptr)
            clientLabel->set_text(clientInfo->name);
        else
            clientLabel->set_text("<i>n/a</i>");
    }

    percent = pa_sw_volume_to_linear(pa_cvolume_avg(&i.volume)) * 100;

    /* FIXME: Hardware volume doesn't translate well to dB. */
    db = pa_sw_volume_to_dB(pa_cvolume_avg(&i.volume));
    scaleEnabled = false;
    volumeScale->set_value(percent);
    scaleEnabled = true;
    if (db != PA_DECIBEL_MININFTY)
        snprintf(t, sizeof(t), "%0.0f%% (%0.2fdB)", percent, db);
    else
        snprintf(t, sizeof(t), "%0.0f%% (-&#8734;dB)", percent);
    volumeLabel->set_markup(t);

    resampleMethodLabel->set_markup(i.resample_method);
    
    set_title("Sink Input: "+i.name);

    this->sink = i.sink;
    this->client = i.client;
    this->owner_module = i.owner_module;
    this->index = i.index;

    toOwnerModuleButton->set_sensitive(owner_module != PA_INVALID_INDEX);
    toClientButton->set_sensitive(client != PA_INVALID_INDEX);
}

void SinkInputWindow::on_close_button_clicked() {
    hide();
}

void SinkInputWindow::on_to_owner_module_button_clicked() {
    if (owner_module != PA_INVALID_INDEX)
        serverInfoManager->showModuleWindow(owner_module);
}

void SinkInputWindow::on_to_sink_button_clicked() {
    serverInfoManager->showSinkWindow(sink);
}

void SinkInputWindow::on_to_client_button_clicked() {
    serverInfoManager->showClientWindow(client);
}

void SinkInputWindow::on_volume_scale_value_changed() {
    if (scaleEnabled)
        serverInfoManager->setSinkInputVolume(index, pa_sw_volume_from_linear(volumeScale->get_value()/100));
}

void SinkInputWindow::on_volume_reset_button_clicked() {
    serverInfoManager->setSinkInputVolume(index, PA_VOLUME_NORM);
}

void SinkInputWindow::on_volume_mute_button_clicked() {
    serverInfoManager->setSinkInputVolume(index, PA_VOLUME_MUTED);
}

void SinkInputWindow::on_kill_button_clicked() {
    serverInfoManager->killSinkInput(index);
}

bool SinkInputWindow::on_delete_event(GdkEventAny* ) {
    hide();
    return false;
}
