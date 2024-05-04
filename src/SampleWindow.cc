/* $Id: SampleWindow.cc 86 2007-05-23 23:39:45Z lennart $ */

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
#include "SampleWindow.hh"

#define GLADE_NAME "sampleWindow"

SampleWindow::SampleWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject),
    nameLabel(nullptr),
    indexLabel(nullptr),
    volumeLabel(nullptr),
    sampleTypeLabel(nullptr),
    durationLabel(nullptr),
    sizeLabel(nullptr),
    lazyLabel(nullptr),
    filenameLabel(nullptr),
    closeButton(nullptr) {

    refGlade->get_widget("nameLabel7", nameLabel);
    refGlade->get_widget("indexLabel7", indexLabel);
    refGlade->get_widget("volumeLabel4", volumeLabel);
    refGlade->get_widget("sampleTypeLabel4", sampleTypeLabel);
    refGlade->get_widget("durationLabel", durationLabel);
    refGlade->get_widget("sizeLabel", sizeLabel);
    refGlade->get_widget("lazyLabel", lazyLabel);
    refGlade->get_widget("filenameLabel", filenameLabel);
    refGlade->get_widget("closeButton0", closeButton);

    closeButton->signal_clicked().connect(sigc::mem_fun(*this, &SampleWindow::onCloseButton));
}

SampleWindow* SampleWindow::create() {
    SampleWindow *w = nullptr;
    
    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file(GLADE_FILE);
    
    refBuilder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void SampleWindow::updateInfo(const SampleInfo &i) {
    char t[60], ss[PA_SAMPLE_SPEC_SNPRINT_MAX];

    nameLabel->set_text(i.name);
    snprintf(t, sizeof(t), "#%u", i.index);
    indexLabel->set_text(t);

    snprintf(t, sizeof(t), "%0.0f%%", (double) pa_sw_volume_to_linear(pa_cvolume_avg(&i.volume)) * 100);
    volumeLabel->set_text(t);

    if (i.bytes > 0) {
        pa_sample_spec_snprint(ss, sizeof(ss), &i.sample_spec);
        sampleTypeLabel->set_text(ss);

        snprintf(t, sizeof(t), "%0.1fs", (double) i.duration/1000000);
        durationLabel->set_text(t);

        pa_bytes_snprint(t, sizeof(t), i.bytes);
        sizeLabel->set_text(t);
    } else {
        sampleTypeLabel->set_markup("<i>n/a</i>");
        durationLabel->set_markup("<i>n/a</i>");
        sizeLabel->set_markup("<i>n/a</i>");
    }

    lazyLabel->set_text(i.lazy ? "yes" : "no");

    if (i.filename_valid)
        filenameLabel->set_text(i.filename);
    else
        filenameLabel->set_markup("<i>n/a</i>");

    set_title("Sample: "+i.name);
}

void SampleWindow::onCloseButton() {
    hide();
}

bool SampleWindow::on_delete_event(GdkEventAny*) {
    hide();
    return false;
}
