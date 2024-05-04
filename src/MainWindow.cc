/* $Id: MainWindow.cc 86 2007-05-23 23:39:45Z lennart $ */
/* ported by rizalmart */

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
#include <string>
#include <sstream>

#include "paman.hh"
#include "MainWindow.hh"

#define GLADE_NAME "mainWindow"

enum {
    ROW_TYPE_SINK_CATEGORY,
    ROW_TYPE_SOURCE_CATEGORY,
    ROW_TYPE_SINK,
    ROW_TYPE_SOURCE,
    ROW_TYPE_SINK_INPUT,
    ROW_TYPE_SOURCE_OUTPUT,
};

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder) :
    Gtk::Window(cobject),
    statusLabel(nullptr),
    serverNameLabel(nullptr),
    serverVersionLabel(nullptr),
    defaultSampleTypeLabel(nullptr),
    userNameLabel(nullptr),
    hostNameLabel(nullptr),
    linkLibraryVersionLabel(nullptr),
    compiledLibraryVersionLabel(nullptr),
    titleLabel(nullptr),
    defaultSinkLabel(nullptr),
    defaultSourceLabel(nullptr),
    deviceOpenButton(nullptr),
    clientOpenButton(nullptr),
    moduleOpenButton(nullptr),
    sampleOpenButton(nullptr),
    samplePlayButton(nullptr),
    connectButton(nullptr),
    disconnectButton(nullptr),
    statButton(nullptr),
    deviceTreeView(nullptr),
    clientTreeView(nullptr),
    moduleTreeView(nullptr),
    sampleTreeView(nullptr),
    titleEventBox(nullptr),
    sinkComboBox(nullptr) {
    
    refBuilder->get_widget("statusLabel", statusLabel);
    refBuilder->get_widget("serverNameLabel", serverNameLabel);
    refBuilder->get_widget("serverVersionLabel", serverVersionLabel);
    refBuilder->get_widget("defaultSampleTypeLabel", defaultSampleTypeLabel);
    refBuilder->get_widget("hostNameLabel", hostNameLabel);
    refBuilder->get_widget("userNameLabel", userNameLabel);
    refBuilder->get_widget("titleLabel", titleLabel);
    refBuilder->get_widget("defaultSinkLabel", defaultSinkLabel);
    refBuilder->get_widget("defaultSourceLabel", defaultSourceLabel);
    refBuilder->get_widget("deviceTreeView", deviceTreeView);
    refBuilder->get_widget("clientTreeView", clientTreeView);
    refBuilder->get_widget("moduleTreeView", moduleTreeView);
    refBuilder->get_widget("sampleCacheTreeView", sampleTreeView);
    refBuilder->get_widget("deviceOpenButton", deviceOpenButton);
    refBuilder->get_widget("clientOpenButton", clientOpenButton);
    refBuilder->get_widget("moduleOpenButton", moduleOpenButton);
    refBuilder->get_widget("sampleCacheOpenButton", sampleOpenButton);
    refBuilder->get_widget("sampleCachePlayButton", samplePlayButton);
    refBuilder->get_widget("connectButton", connectButton);
    refBuilder->get_widget("disconnectButton", disconnectButton);
    refBuilder->get_widget("linkLibraryVersionLabel", linkLibraryVersionLabel);
    refBuilder->get_widget("compiledLibraryVersionLabel", compiledLibraryVersionLabel);
    refBuilder->get_widget("statButton", statButton);
    refBuilder->get_widget("titleEventBox", titleEventBox);
    refBuilder->get_widget("sinkComboBox", sinkComboBox);

    deviceTreeStore = Gtk::TreeStore::create(deviceTreeModelColumns);
    deviceTreeView->set_model(deviceTreeStore);
    deviceTreeView->append_column("Name", deviceTreeModelColumns.name);
    deviceTreeView->append_column("Description", deviceTreeModelColumns.description);
    deviceTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::onDeviceTreeViewRowActivated));
    deviceTreeView->signal_cursor_changed().connect(sigc::mem_fun(*this, &MainWindow::onDeviceTreeViewCursorChanged));

    clientTreeStore = Gtk::TreeStore::create(clientTreeModelColumns);
    clientTreeView->set_model(clientTreeStore);
    clientTreeView->append_column("Name", clientTreeModelColumns.name);
    clientTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::onClientTreeViewRowActivated));

    moduleTreeStore = Gtk::TreeStore::create(moduleTreeModelColumns);
    moduleTreeView->set_model(moduleTreeStore);
    moduleTreeView->append_column("Name", moduleTreeModelColumns.name);
    moduleTreeView->append_column("Argument", moduleTreeModelColumns.argument);
    moduleTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::onModuleTreeViewRowActivated));

    sampleTreeStore = Gtk::TreeStore::create(sampleTreeModelColumns);
    sampleTreeView->set_model(sampleTreeStore);
    sampleTreeView->append_column("Name", sampleTreeModelColumns.name);
    sampleTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::onSampleTreeViewRowActivated));

    sinkListStore = Gtk::ListStore::create(sinkTreeModelColumns);    
    sinkComboBox->set_model(sinkListStore);
    sinkComboBox->pack_start(sinkTreeModelColumns.name);
        
	//sinkComboBox->append_column("Name", sinkTreeModelColumns.name);

    connectButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onConnectButton));
    disconnectButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onDisconnectButton));
    deviceOpenButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onDeviceOpenButton));
    clientOpenButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onClientOpenButton));
    moduleOpenButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onModuleOpenButton));
    sampleOpenButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onSampleOpenButton));
    samplePlayButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onSamplePlayButton));
    statButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onStatButton));

    linkLibraryVersionLabel->set_text(pa_get_library_version());
    compiledLibraryVersionLabel->set_text(pa_get_headers_version());

    statusLabel->set_text("Connecting ...");

    clearAllData();

    Gdk::RGBA c;
    c.set_rgba(1.0, 1.0, 1.0, 1.0);
    titleEventBox->override_background_color(c);
}
 

MainWindow::~MainWindow() {
}

MainWindow* MainWindow::create() {
    MainWindow *w;
        
    auto builder = Gtk::Builder::create();
    builder->add_from_file(GLADE_FILE);
	builder->get_widget_derived(GLADE_NAME, w);
    return w;
}

void MainWindow::updateInfo(SinkInfo &i) {
    if (!i.treeRef) {
        Gtk::TreeIter iter = deviceTreeStore->get_iter(sinkRef.get_path());
        i.treeRef = Gtk::TreeRowReference(deviceTreeStore, Gtk::TreePath(deviceTreeStore->append(iter->children())));
    }

    Gtk::TreeRow row = *(deviceTreeStore->get_iter(i.treeRef.get_path()));
    row[deviceTreeModelColumns.name] = i.name;
    row[deviceTreeModelColumns.description] = i.description;
    row[deviceTreeModelColumns.index] = i.index;
    row[deviceTreeModelColumns.type] = ROW_TYPE_SINK;

    deviceTreeView->expand_row(sinkRef.get_path(), false);
    onDeviceTreeViewCursorChanged();

    if (!i.sinkComboBoxTreeRef)
        i.sinkComboBoxTreeRef = Gtk::TreeRowReference(sinkListStore, Gtk::TreePath(sinkListStore->append()));

    row = *(sinkListStore->get_iter(i.sinkComboBoxTreeRef.get_path()));
    row[sinkTreeModelColumns.name] = i.name;
    row[sinkTreeModelColumns.index] = i.index;

    if (sinkComboBox->get_active_row_number() == -1)
        sinkComboBox->set_active(0);

    bool b = !sampleTreeStore->children().empty();
    sinkComboBox->set_sensitive(b);
    samplePlayButton->set_sensitive(b);
}

void MainWindow::updateInfo(SourceInfo &i) {
    if (!i.treeRef) {
        Gtk::TreeIter iter = deviceTreeStore->get_iter(sourceRef.get_path());
        i.treeRef = Gtk::TreeRowReference(deviceTreeStore, Gtk::TreePath(deviceTreeStore->append(iter->children())));
    }

    Gtk::TreeRow row = *(deviceTreeStore->get_iter(i.treeRef.get_path()));
    row[deviceTreeModelColumns.name] = i.name;
    row[deviceTreeModelColumns.description] = i.description;
    row[deviceTreeModelColumns.index] = i.index;
    row[deviceTreeModelColumns.type] = ROW_TYPE_SOURCE;

    deviceTreeView->expand_row(sourceRef.get_path(), false);
    onDeviceTreeViewCursorChanged();
}

void MainWindow::updateInfo(ClientInfo &i) {

    if (!i.treeRef)
        i.treeRef = Gtk::TreeRowReference(clientTreeStore, Gtk::TreePath(clientTreeStore->append()));

    Gtk::TreeRow row = *(clientTreeStore->get_iter(i.treeRef.get_path()));
    row[clientTreeModelColumns.name] = i.name;
    row[clientTreeModelColumns.index] = i.index;
    clientOpenButton->set_sensitive(true);
}

void MainWindow::updateInfo(ModuleInfo &i) {
    if (!i.treeRef)
        i.treeRef = Gtk::TreeRowReference(moduleTreeStore, Gtk::TreePath(moduleTreeStore->append()));

    Gtk::TreeRow row = *(moduleTreeStore->get_iter(i.treeRef.get_path()));
    row[moduleTreeModelColumns.name] = i.name;
    row[moduleTreeModelColumns.argument] = i.argument;
    row[moduleTreeModelColumns.index] = i.index;
    moduleOpenButton->set_sensitive(true);
}

void MainWindow::updateInfo(SinkInputInfo &i) {
    char t[256];
    
    SinkInfo *si = serverInfoManager->getSinkInfo(i.sink);
    if (!si)
        return;
    
    if (i.treeRef) {
        Gtk::TreePath path = i.treeRef.get_path();
        path.up();
        
        if (path != si->treeRef.get_path())
            deviceTreeStore->erase(deviceTreeStore->get_iter(i.treeRef.get_path()));
    }

    if (!i.treeRef) {
        Gtk::TreeIter iter = deviceTreeStore->get_iter(si->treeRef.get_path());
        i.treeRef = Gtk::TreeRowReference(deviceTreeStore, Gtk::TreePath(deviceTreeStore->append(iter->children())));
    }

    Gtk::TreeRow row = *(deviceTreeStore->get_iter(i.treeRef.get_path()));
    snprintf(t, sizeof(t), "#%i", i.index);
    row[deviceTreeModelColumns.name] = t;
    row[deviceTreeModelColumns.description] = i.name;
    row[deviceTreeModelColumns.index] = i.index;
    row[deviceTreeModelColumns.type] = ROW_TYPE_SINK_INPUT;

    deviceTreeView->expand_row(sinkRef.get_path(), true);
    onDeviceTreeViewCursorChanged();
}

void MainWindow::updateInfo(SourceOutputInfo &i) {
    char t[256];
    
    SourceInfo *si = serverInfoManager->getSourceInfo(i.source);
    if (!si)
        return;

    if (i.treeRef) {
        Gtk::TreePath path = i.treeRef.get_path();
        path.up();
        
        if (path != si->treeRef.get_path())
            deviceTreeStore->erase(deviceTreeStore->get_iter(i.treeRef.get_path()));
    }
    
    if (!i.treeRef) {
        Gtk::TreeIter iter = deviceTreeStore->get_iter(si->treeRef.get_path());
        i.treeRef = Gtk::TreeRowReference(deviceTreeStore, Gtk::TreePath(deviceTreeStore->append(iter->children())));
    }

    Gtk::TreeRow row = *(deviceTreeStore->get_iter(i.treeRef.get_path()));
    snprintf(t, sizeof(t), "#%i", i.index);
    row[deviceTreeModelColumns.name] = t;
    row[deviceTreeModelColumns.description] = i.name;
    row[deviceTreeModelColumns.index] = i.index;
    row[deviceTreeModelColumns.type] = ROW_TYPE_SOURCE_OUTPUT;

    deviceTreeView->expand_row(sourceRef.get_path(), true);
    onDeviceTreeViewCursorChanged();
}

void MainWindow::updateInfo(SampleInfo &i) {
    if (!i.treeRef)
        i.treeRef = Gtk::TreeRowReference(sampleTreeStore, Gtk::TreePath(sampleTreeStore->append()));

    Gtk::TreeRow row = *(sampleTreeStore->get_iter(i.treeRef.get_path()));
    row[sampleTreeModelColumns.name] = i.name;
    row[sampleTreeModelColumns.index] = i.index;
    sampleOpenButton->set_sensitive(true);

    bool b = !sinkListStore->children().empty();
    samplePlayButton->set_sensitive(b);
    sinkComboBox->set_sensitive(b);
}

void MainWindow::removeInfo(SinkInfo &i) {
    if (i.treeRef)
        deviceTreeStore->erase(deviceTreeStore->get_iter(i.treeRef.get_path()));

    onDeviceTreeViewCursorChanged();

    if (i.sinkComboBoxTreeRef)
        sinkListStore->erase(sinkListStore->get_iter(i.sinkComboBoxTreeRef.get_path()));

    bool b = !sinkListStore->children().empty() && !sampleTreeStore->children().empty();
    samplePlayButton->set_sensitive(b);
    sinkComboBox->set_sensitive(b);
}

void MainWindow::removeInfo(SourceInfo &i) {
    if (i.treeRef)
        deviceTreeStore->erase(deviceTreeStore->get_iter(i.treeRef.get_path()));
        
    onDeviceTreeViewCursorChanged();
}

void MainWindow::removeInfo(ClientInfo &i) {
    if (i.treeRef)
        clientTreeStore->erase(clientTreeStore->get_iter(i.treeRef.get_path()));

    clientOpenButton->set_sensitive(!moduleTreeStore->children().empty());
}

void MainWindow::removeInfo(ModuleInfo &i) {
    if (i.treeRef)
        moduleTreeStore->erase(moduleTreeStore->get_iter(i.treeRef.get_path()));

    moduleOpenButton->set_sensitive(!moduleTreeStore->children().empty());
}

void MainWindow::removeInfo(SinkInputInfo &i) {
    if (i.treeRef)
        deviceTreeStore->erase(deviceTreeStore->get_iter(i.treeRef.get_path()));
        
    onDeviceTreeViewCursorChanged();
}

void MainWindow::removeInfo(SourceOutputInfo &i) {
    if (i.treeRef)
        deviceTreeStore->erase(deviceTreeStore->get_iter(i.treeRef.get_path()));
        
    onDeviceTreeViewCursorChanged();
}

void MainWindow::removeInfo(SampleInfo &i) {
    if (i.treeRef)
        sampleTreeStore->erase(sampleTreeStore->get_iter(i.treeRef.get_path()));
        
    sampleOpenButton->set_sensitive(!sampleTreeStore->children().empty());

    bool b = !sinkListStore->children().empty() && !sampleTreeStore->children().empty();
    samplePlayButton->set_sensitive(b);
    sinkComboBox->set_sensitive(b);
}

void MainWindow::onDeviceTreeViewCursorChanged() {
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn *c;
    deviceTreeView->get_cursor(p, c);

    if (!p.gobj())
        return;
    
    deviceOpenButton->set_sensitive((sourceRef.get_path() != p) && (sinkRef.get_path() != p)); 
}

void MainWindow::onDeviceTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */) {
    showDeviceWindow(path);
}

void MainWindow::onClientTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */) {
    showClientWindow(path);
}

void MainWindow::onModuleTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */) {
    showModuleWindow(path);
}

void MainWindow::onSampleTreeViewRowActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */) {
    showSampleWindow(path);
}

void MainWindow::updateInfo(const struct pa_server_info &i) {
    char t[PA_SAMPLE_SPEC_SNPRINT_MAX];
    serverNameLabel->set_text(i.server_name);
    serverVersionLabel->set_text(i.server_version);
    pa_sample_spec_snprint(t, sizeof(t), &i.sample_spec);
    defaultSampleTypeLabel->set_text(t);
    hostNameLabel->set_text(i.host_name);
    userNameLabel->set_text(i.user_name);
    defaultSinkLabel->set_markup(i.default_sink_name ? i.default_sink_name : "<i>not set</i>");
    defaultSourceLabel->set_markup(i.default_source_name ? i.default_source_name: "<i>not set</i>");
}

void MainWindow::showSuccess(const char *t) {
    statusLabel->set_text(t);
}

void MainWindow::showFailure(const char *t) {
    char s[256];
    snprintf(s, sizeof(s), "<b>Failure:</b> %s", t);
    statusLabel->set_markup(s);
}

void MainWindow::clearAllData() {
    deviceTreeStore->clear();

    Gtk::TreeIter i = deviceTreeStore->append();
    sinkRef = Gtk::TreeRowReference(deviceTreeStore, Gtk::TreePath(i));
    (*i)[deviceTreeModelColumns.name] = "Sinks";
    (*i)[deviceTreeModelColumns.index] = (uint32_t) -1;
    (*i)[deviceTreeModelColumns.type] = ROW_TYPE_SINK_CATEGORY;

    i = deviceTreeStore->append();
    sourceRef = Gtk::TreeRowReference(deviceTreeStore, Gtk::TreePath(i));
    (*i)[deviceTreeModelColumns.name] = "Sources";
    (*i)[deviceTreeModelColumns.index] = (uint32_t) -1;
    (*i)[deviceTreeModelColumns.type] = ROW_TYPE_SOURCE_CATEGORY;

    clientTreeStore->clear();
    moduleTreeStore->clear();
    sampleTreeStore->clear();
    sinkListStore->clear();
    
    deviceOpenButton->set_sensitive(false);
    clientOpenButton->set_sensitive(false);
    moduleOpenButton->set_sensitive(false);
    sampleOpenButton->set_sensitive(false);
    samplePlayButton->set_sensitive(false);
    sinkComboBox->set_sensitive(false);

    serverNameLabel->set_markup("<i>n/a</i>");
    serverVersionLabel->set_markup("<i>n/a</i>");
    defaultSampleTypeLabel->set_markup("<i>n/a</i>");
    hostNameLabel->set_markup("<i>n/a</i>");
    userNameLabel->set_markup("<i>n/a</i>");
    defaultSinkLabel->set_markup("<i>n/a</i>");
    defaultSourceLabel->set_markup("<i>n/a</i>");
}

void MainWindow::onDeviceOpenButton() {
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn *c;
    deviceTreeView->get_cursor(p, c);

    if (p.gobj())
        showDeviceWindow(p);
}

void MainWindow::onClientOpenButton() {
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn *c;
    clientTreeView->get_cursor(p, c);

    if (p.gobj())
        showClientWindow(p);
}

void MainWindow::onModuleOpenButton() {
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn *c;
    moduleTreeView->get_cursor(p, c);

    if (p.gobj())
        showModuleWindow(p);
}

void MainWindow::onSampleOpenButton() {
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn *c;
    sampleTreeView->get_cursor(p, c);

    if (p.gobj())
        showSampleWindow(p);
}

void MainWindow::onConnectButton() {
    createConnection();
}

void MainWindow::onDisconnectButton() {
    killConnection();
}

void MainWindow::showDeviceWindow(const Gtk::TreePath &p) {
    
    if (!serverInfoManager)
        return;

    Gtk::TreeModel::Row row = *(deviceTreeStore->get_iter(p));
    
    if (row[deviceTreeModelColumns.type] == ROW_TYPE_SINK)
        serverInfoManager->showSinkWindow(row[deviceTreeModelColumns.index]);
    else if (row[deviceTreeModelColumns.type] == ROW_TYPE_SOURCE)
        serverInfoManager->showSourceWindow(row[deviceTreeModelColumns.index]);
    else if (row[deviceTreeModelColumns.type] == ROW_TYPE_SINK_INPUT)
        serverInfoManager->showSinkInputWindow(row[deviceTreeModelColumns.index]);
    else if (row[deviceTreeModelColumns.type] == ROW_TYPE_SOURCE_OUTPUT)
        serverInfoManager->showSourceOutputWindow(row[deviceTreeModelColumns.index]);
}

void MainWindow::showClientWindow(const Gtk::TreePath &p) {
    if (!serverInfoManager)
        return;
    
    Gtk::TreeModel::Row row = *(clientTreeStore->get_iter(p));
    serverInfoManager->showClientWindow(row[clientTreeModelColumns.index]);
}

void MainWindow::showModuleWindow(const Gtk::TreePath &p) {
    if (!serverInfoManager)
        return;
    
    Gtk::TreeModel::Row row = *(moduleTreeStore->get_iter(p));
    serverInfoManager->showModuleWindow(row[moduleTreeModelColumns.index]);
}

void MainWindow::showSampleWindow(const Gtk::TreePath &p) {
    if (!serverInfoManager)
        return;
    
    Gtk::TreeModel::Row row = *(sampleTreeStore->get_iter(p));
    serverInfoManager->showSampleWindow(row[sampleTreeModelColumns.index]);
}

void MainWindow::onSamplePlayButton() {
    Gtk::TreeModel::Path p;
    Gtk::TreeViewColumn *c;
    sampleTreeView->get_cursor(p, c);

    if (!p.gobj())
        return;
    
    Gtk::TreeModel::Row sampleRow = *(sampleTreeStore->get_iter(p));
    Gtk::TreeModel::Row sinkRow = *sinkComboBox->get_active();

    uint32_t sampleIndex = sampleRow[sampleTreeModelColumns.index];
    uint32_t sinkIndex = sinkRow[sinkTreeModelColumns.index];

    serverInfoManager->playSample(sampleIndex, sinkIndex);
}

void MainWindow::onStatButton() {
    
    if(!serverInfoManager){
    return;
    }
    
    serverInfoManager->showStatWindow();
}
