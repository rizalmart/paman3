/* $Id: ModuleWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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
#include "ModuleWindow.hh"

#define GLADE_NAME "moduleWindow"

ModuleWindow::ModuleWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    argumentLabel(nullptr),
    indexLabel(nullptr),
    autoloadedLabel(nullptr),
    usageLabel(nullptr),
    closeButton(nullptr) {

    refGlade->get_widget("nameLabel3", nameLabel);
    refGlade->get_widget("argumentLabel", argumentLabel);
    refGlade->get_widget("indexLabel3", indexLabel);
    refGlade->get_widget("autoloadedLabel", autoloadedLabel);
    refGlade->get_widget("usageLabel", usageLabel);
    refGlade->get_widget("closeButton3", closeButton);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &ModuleWindow::onCloseButton));
}

ModuleWindow* ModuleWindow::create() {
    ModuleWindow *w = nullptr;
    
    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file(GLADE_FILE);
    
    refBuilder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void ModuleWindow::updateInfo(const ModuleInfo &i) {
    char t[20];

    nameLabel->set_text(i.name);
    if (!i.argument.empty())
        argumentLabel->set_text(i.argument);
    else
        argumentLabel->set_markup("<i>None specified</i>");
    
    snprintf(t, sizeof(t), "#%u", i.index);
    indexLabel->set_text(t);
    autoloadedLabel->set_text(i.autoloaded ? "Yes" : "No");

    if (i.used != static_cast<uint32_t>(-1)) {
        snprintf(t, sizeof(t), "%u", i.used);
        usageLabel->set_text(t);
    } else
        usageLabel->set_markup("<i>n/a</i>");

    set_title("Module: "+i.name);
}

void ModuleWindow::onCloseButton() {
    hide();
}

bool ModuleWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}
