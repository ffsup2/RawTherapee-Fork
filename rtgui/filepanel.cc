/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <filepanel.h>
#include <rtwindow.h>

int fbinit (void* data) {

    gdk_threads_enter ();
    ((FilePanel*)data)->init ();
    gdk_threads_leave ();

    return 0;
}

FilePanel::FilePanel () : parent(NULL) {

    dirpaned = new Gtk::HPaned ();
    dirpaned->set_position (options.dirBrowserWidth);
	leftNotebook = new Gtk::Notebook ();
	leftNotebook->set_tab_pos (Gtk::POS_TOP);

    dirBrowser = new DirBrowser ();
    placesBrowser = new PlacesBrowser ();
    recentBrowser = new RecentBrowser ();

    placespaned = new Gtk::VPaned ();
    placespaned->set_position (options.dirBrowserHeight);

    Gtk::VBox* obox = Gtk::manage (new Gtk::VBox ());
    obox->pack_start (*recentBrowser, Gtk::PACK_SHRINK, 4);
    obox->pack_start (*dirBrowser);

    placespaned->pack1 (*placesBrowser, false, true);
    placespaned->pack2 (*obox, true, true);

    Gtk::Label* dirLab = new Gtk::Label ("Files");
    leftNotebook->append_page (*placespaned, *dirLab);

    dirpaned->pack1 (*leftNotebook, Gtk::SHRINK);

    tpc = new BatchToolPanelCoordinator (this);
    fileCatalog = new FileCatalog (tpc->coarse, tpc->getToolBar());
    dirpaned->pack2 (*fileCatalog, Gtk::EXPAND|Gtk::SHRINK);

    placesBrowser->setDirBrowserRemoteInterface (dirBrowser);
    recentBrowser->setDirBrowserRemoteInterface (dirBrowser);
    dirBrowser->addDirSelectionListener (fileCatalog);
    dirBrowser->addDirSelectionListener (recentBrowser);
    dirBrowser->addDirSelectionListener (placesBrowser);
    fileCatalog->setFileSelectionListener (this);

    Gtk::ScrolledWindow* sFilterPanel = new Gtk::ScrolledWindow();
	filterPanel = new FilterPanel ();
	sFilterPanel->add (*filterPanel);
	sFilterPanel->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    Gtk::Label* filtLab = new Gtk::Label ("Filter");
    leftNotebook->append_page (*sFilterPanel, *filtLab);

	fileCatalog->setFilterPanel (filterPanel);
    fileCatalog->setImageAreaToolListener (tpc);

    Gtk::ScrolledWindow *sOptionsPanel = new Gtk::ScrolledWindow();
    preferencesPanel = new GlobalPreferencesPanel();
    sOptionsPanel->add(*preferencesPanel);
    sOptionsPanel->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    Gtk::Label* optLab = new Gtk::Label ("Options");
    leftNotebook->append_page (*sOptionsPanel, *optLab);
    
    //------------------
    rightBox = new Gtk::HBox ();

    history = new History (false);
    tpc->addPParamsChangeListener (history);
    history->setProfileChangeListener (tpc);

    Gtk::VPaned* tpcPaned = new Gtk::VPaned ();
    tpcPaned->pack1 (*tpc->toolPanelNotebook, true, true);
    tpcPaned->pack2 (*history, true, true);
    tpcPaned->set_position(options.browserToolPanelWidth);
    
    rightBox->pack_start (*tpcPaned);

    pack1(*dirpaned, true, true);
    pack2(*rightBox, false, true);

    fileCatalog->setFileSelectionChangeListener (tpc);
    fileCatalog->setFileSelectionListener (this);
    g_idle_add (fbinit, this);

    show_all ();
}

void FilePanel::on_realize () {
    
    Gtk::HPaned::on_realize ();
    rightBox->set_size_request (options.browserToolPanelWidth, -1);
}

void FilePanel::init () {
  
    dirBrowser->fillDirTree ();
    placesBrowser->refreshPlacesList ();

    if (argv1!="")
        dirBrowser->open (argv1);
    else {
        if (options.startupDir==STARTUPDIR_HOME) 
            dirBrowser->open (Glib::get_home_dir());
        else if (options.startupDir==STARTUPDIR_CURRENT)
            dirBrowser->open (argv0);
        else if (options.startupDir==STARTUPDIR_CUSTOM || options.startupDir==STARTUPDIR_LAST) 
            dirBrowser->open (options.startupPath);
    }
} 

bool FilePanel::fileSelected (Thumbnail* thm) {

    if (!parent)
        return false;

    // try to open the file
    bool succ = false;
    fileCatalog->setEnabled (false);
    rtengine::InitialImage* isrc = EditorPanel::loadImage (thm);
    if (isrc) {
        EditorPanel* epanel = Gtk::manage (new EditorPanel (thm, isrc));
        parent->addEditorPanel (epanel);
        succ = true;
    }
    else {
        Glib::ustring msg_ = Glib::ustring("<b>") + M("MAIN_MSG_CANNOTLOAD") + " \"" + thm->getFileName() + "\" .\n</b>";
        Gtk::MessageDialog msgd (msg_, true, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        msgd.run ();
    }
    fileCatalog->setEnabled (true);
    return succ;
}

void FilePanel::saveOptions () { 

    options.dirBrowserWidth = dirpaned->get_position ();
    options.dirBrowserHeight = placespaned->get_position ();
    options.browserToolPanelWidth = rightBox->get_width ();
    if (options.startupDir==STARTUPDIR_LAST && fileCatalog->lastSelectedDir ()!="")
        options.startupPath = fileCatalog->lastSelectedDir ();
    fileCatalog->closeDir (); 
}

void FilePanel::open (const Glib::ustring& d) {

    if (Glib::file_test (d, Glib::FILE_TEST_IS_DIR))
        dirBrowser->open (d.c_str());
    else if (Glib::file_test (d, Glib::FILE_TEST_EXISTS))
        dirBrowser->open (Glib::path_get_dirname(d), Glib::path_get_basename(d));
}

bool FilePanel::addBatchQueueJob (BatchQueueEntry* bqe) {

    if (parent)
        parent->addBatchQueueJob (bqe);
	return true;
}

void FilePanel::optionsChanged () {

    tpc->optionsChanged ();
    fileCatalog->refreshAll ();
}
