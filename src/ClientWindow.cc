/* $Id: ClientWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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
#include "ClientWindow.hh"

#define GLADE_NAME "clientWindow"

ClientWindow::ClientWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    driverLabel(nullptr),
    indexLabel(nullptr),
    ownerModuleLabel(nullptr),
    closeButton(nullptr),
    toOwnerModuleButton(nullptr),
    killButton(nullptr) {

    builder->get_widget("nameLabel4", nameLabel);
    builder->get_widget("driverLabel", driverLabel);
    builder->get_widget("indexLabel4", indexLabel);
    builder->get_widget("ownerModuleLabel3", ownerModuleLabel);
    builder->get_widget("closeButton4", closeButton);
    builder->get_widget("toOwnerModuleButton2", toOwnerModuleButton);
    builder->get_widget("killButton1", killButton);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &ClientWindow::onCloseButton));
    toOwnerModuleButton->signal_clicked().connect(sigc::mem_fun(*this, &ClientWindow::onToOwnerModuleButton));
    killButton->signal_clicked().connect(sigc::mem_fun(*this, &ClientWindow::onKillButton));
}

ClientWindow* ClientWindow::create() {
    ClientWindow *w = nullptr;
    auto builder = Gtk::Builder::create();
    builder->add_from_file(GLADE_FILE);
    builder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void ClientWindow::updateInfo(const ClientInfo &i) {
    char t[20];

    nameLabel->set_text(i.name);
    driverLabel->set_text(i.driver);
    snprintf(t, sizeof(t), "#%u", i.index);
    indexLabel->set_text(t);
    snprintf(t, sizeof(t), "#%u", i.owner_module);
    ownerModuleLabel->set_text(t);
    set_title("Client: " + i.name);
    
    owner_module = i.owner_module;
    toOwnerModuleButton->set_sensitive(owner_module != static_cast<uint32_t>(-1));
    index = i.index;
}

void ClientWindow::onCloseButton() {
    hide();
}

void ClientWindow::onToOwnerModuleButton() {
    if (owner_module != static_cast<uint32_t>(-1))
        serverInfoManager->showModuleWindow(owner_module);
}

bool ClientWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}

void ClientWindow::onKillButton() {
    serverInfoManager->killClient(index);
}
