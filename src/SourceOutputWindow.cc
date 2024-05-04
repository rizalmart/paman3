/* $Id: SourceOutputWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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
#include "SourceOutputWindow.hh"

#define GLADE_NAME "sourceOutputWindow"

SourceOutputWindow::SourceOutputWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    indexLabel(nullptr),
    sampleTypeLabel(nullptr),
    channelMapLabel(nullptr),
    sourceLabel(nullptr),
    clientLabel(nullptr),
    ownerModuleLabel(nullptr),
    latencyLabel(nullptr),
    resampleMethodLabel(nullptr),
    closeButton(nullptr),
    toOwnerModuleButton(nullptr),
    toClientButton(nullptr),
    toSourceButton(nullptr),
    killButton(nullptr) {

    refGlade->get_widget("nameLabel6", nameLabel);
    refGlade->get_widget("indexLabel6", indexLabel);
    refGlade->get_widget("sampleTypeLabel3", sampleTypeLabel);
    refGlade->get_widget("channelMapLabel3", channelMapLabel);
    refGlade->get_widget("sourceLabel", sourceLabel);
    refGlade->get_widget("clientLabel2", clientLabel);
    refGlade->get_widget("ownerModuleLabel5", ownerModuleLabel);
    refGlade->get_widget("latencyLabel4", latencyLabel);
    refGlade->get_widget("closeButton6", closeButton);
    refGlade->get_widget("toOwnerModuleButton4", toOwnerModuleButton);
    refGlade->get_widget("toClientButton2", toClientButton);
    refGlade->get_widget("toSourceButton", toSourceButton);
    refGlade->get_widget("killButton", killButton);
    refGlade->get_widget("resampleMethodLabel2", resampleMethodLabel);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceOutputWindow::onCloseButton));
    toOwnerModuleButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceOutputWindow::onToOwnerModuleButton));
    toClientButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceOutputWindow::onToClientButton));
    toSourceButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceOutputWindow::onToSourceButton));
    killButton->signal_clicked().connect(sigc::mem_fun(*this, &SourceOutputWindow::onKillButton));
}

SourceOutputWindow* SourceOutputWindow::create() {
    SourceOutputWindow *w = nullptr;

    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file(GLADE_FILE);
    
    refBuilder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void SourceOutputWindow::updateInfo(const SourceOutputInfo &i) {
    char t[100], ss[PA_SAMPLE_SPEC_SNPRINT_MAX], cm[PA_CHANNEL_MAP_SNPRINT_MAX];

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

    snprintf(t, sizeof(t), "%0.0f µs (= buffer: %0.0f µs + source: %0.0f µs)", (double) i.buffer_usec+i.source_usec, (double) i.buffer_usec, (double) i.source_usec);
    latencyLabel->set_markup(t);

    SourceInfo *source = serverInfoManager->getSourceInfo(i.source);
    sourceLabel->set_text(source->name);

    if (i.client == PA_INVALID_INDEX)
        clientLabel->set_markup("<i>n/a</i>");
    else {
        ClientInfo *client = serverInfoManager->getClientInfo(i.client);
        clientLabel->set_text(client->name);
    }

    resampleMethodLabel->set_markup(i.resample_method);
    
    set_title("Source Output: "+i.name);
    
    this->source = i.source;
    owner_module = i.owner_module;
    client = i.client;
    index = i.index;

    toOwnerModuleButton->set_sensitive(owner_module != PA_INVALID_INDEX);
    toClientButton->set_sensitive(client != PA_INVALID_INDEX);
}

void SourceOutputWindow::onCloseButton() {
    hide();
}

void SourceOutputWindow::onToOwnerModuleButton() {
    if (owner_module != PA_INVALID_INDEX)
        serverInfoManager->showModuleWindow(owner_module);
}

void SourceOutputWindow::onToClientButton() {
    if (client != PA_INVALID_INDEX)
        serverInfoManager->showClientWindow(client);
}

void SourceOutputWindow::onToSourceButton() {
    if (source != PA_INVALID_INDEX)
        serverInfoManager->showSourceWindow(source);
}

bool SourceOutputWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}

void SourceOutputWindow::onKillButton() {
    serverInfoManager->killSourceOutput(index);
}

