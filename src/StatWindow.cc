/* $Id: StatWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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
#include "StatWindow.hh"

#define GLADE_NAME "statWindow"

StatWindow::StatWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) :
    Gtk::Window(cobject),
    totalLabel(nullptr),
    totalSizeLabel(nullptr),
    allocatedLabel(nullptr),
    allocatedSizeLabel(nullptr),
    sampleCacheLabel(nullptr),
    closeButton(nullptr),
    refreshButton(nullptr),
    operation(nullptr) {

    builder->get_widget("totalLabel", totalLabel);
    builder->get_widget("totalSizeLabel", totalSizeLabel);
    builder->get_widget("allocatedLabel", allocatedLabel);
    builder->get_widget("allocatedSizeLabel", allocatedSizeLabel);
    builder->get_widget("sampleCacheLabel", sampleCacheLabel);
    builder->get_widget("closeButton", closeButton);
    builder->get_widget("refreshButton", refreshButton);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &StatWindow::onCloseButton));
    refreshButton->signal_clicked().connect(sigc::mem_fun(*this, &StatWindow::onRefreshButton));

    onRefreshButton();
}


StatWindow::~StatWindow() {
    if (operation) {
        pa_operation_cancel(operation);
        pa_operation_unref(operation);
    }
}

StatWindow* StatWindow::create() {
	
    StatWindow *w = nullptr;
    
    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file(GLADE_FILE);  
    
    refBuilder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void StatWindow::onCloseButton() {
    hide();
}


void StatWindow::stat_cb(pa_context *, const pa_stat_info *i, void *userdata) {
    char t[20];
    auto s = static_cast<StatWindow*>(userdata);

    snprintf(t, sizeof(t), "%u", i->memblock_total);
    s->totalLabel->set_text(t);
    pa_bytes_snprint(t, sizeof(t), i->memblock_total_size);
    s->totalSizeLabel->set_text(t);
    snprintf(t, sizeof(t), "%u", i->memblock_allocated);
    s->allocatedLabel->set_text(t);
    pa_bytes_snprint(t, sizeof(t), i->memblock_allocated_size);
    s->allocatedSizeLabel->set_text(t);
    pa_bytes_snprint(t, sizeof(t), i->scache_size);
    s->sampleCacheLabel->set_text(t);

    pa_operation_unref(s->operation);
    s->operation = nullptr;
}

void StatWindow::onRefreshButton() {
    if (operation)
        return;

    g_assert(context);
    operation = pa_context_stat(context, stat_cb, this);
}

void StatWindow::present() {
    Gtk::Window::present();
    onRefreshButton();
}

bool StatWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}
