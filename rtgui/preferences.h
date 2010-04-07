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
#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <gtkmm.h>
#include <adjuster.h>
#include <options.h>
#include <vector>

class Preferences : public Gtk::Dialog {

        class ExtensionColumns : public Gtk::TreeModel::ColumnRecord {
            public:
                Gtk::TreeModelColumn<bool>  enabled;
                Gtk::TreeModelColumn<Glib::ustring>  ext;
                ExtensionColumns() { add(enabled); add(ext); }
        };
        ExtensionColumns extensionColumns;
        Glib::RefPtr<Gtk::ListStore> extensionModel;


        class BehavColumns : public Gtk::TreeModel::ColumnRecord {
            public:
                Gtk::TreeModelColumn<Glib::ustring> label;
                Gtk::TreeModelColumn<bool>          badd;
                Gtk::TreeModelColumn<bool>          bset;
                Gtk::TreeModelColumn<bool>          visible;
                BehavColumns() { add(label); add(badd); add(bset); add(visible);}
        };
        Glib::RefPtr<Gtk::TreeStore> behModel;
        BehavColumns behavColumns;


  protected:
    Gtk::ComboBoxText* rprofiles;
    Gtk::ComboBoxText* iprofiles;
    Gtk::ComboBoxText* dmethod;
    Gtk::ComboBoxText* languages;
    Gtk::Entry* dateformat;
    Gtk::Entry* startupdir;
    Gtk::RadioButton* sdcurrent;
    Gtk::RadioButton* sdlast;
    Gtk::RadioButton* sdhome;
    Gtk::RadioButton* sdother;
    Gtk::FileChooserButton* gimpDir;
    Gtk::FileChooserButton* psDir;
    Gtk::Entry* editorToSendTo;
    Gtk::RadioButton* edGimp;
    Gtk::RadioButton* edPS;
    Gtk::RadioButton* edOther;
    

    Gtk::CheckButton* showDateTime;
    Gtk::CheckButton* showBasicExif;

    Gtk::SpinButton*  ccSteps;
    Gtk::FileChooserButton* iccDir;
    Gtk::FileChooserButton* monProfile;

    Gtk::CheckButton* blinkClipped;
	Gtk::SpinButton*  hlThresh;
	Gtk::SpinButton*  shThresh;

    Gtk::ComboBoxText* intent;

    Gtk::ComboBoxText* theme;
	
    Gtk::ComboBoxText* cformat;
    Gtk::SpinButton*   maxThumbSize;
    Gtk::SpinButton*   maxCacheEntries;
    Gtk::Button*       clearThumbnails;
    Gtk::Button*       clearProfiles;
    Gtk::Button*       clearAll;
    Gtk::Entry*     extension;
    Gtk::TreeView*  extensions;
    Gtk::Button*    addExt;
    Gtk::Button*    delExt;
    Gtk::CheckButton* overlayedFileNames;

    Gtk::CheckButton* saveParamsFile;
    Gtk::CheckButton* saveParamsCache;
    Gtk::ComboBoxText* loadParamsPreference;
	
    Options moptions;
    sigc::connection dmconn, tconn, addc, setc;

    void fillPreferences (){};
    void storePreferences (){};

    void dmethodChanged ();

    //void themeChanged ();

    void appendBehavList (Gtk::TreeModel::iterator& parent, Glib::ustring label, bool set);

    Gtk::Widget* getProcParamsPanel ();
    Gtk::Widget* getColorManagementPanel ();
    Gtk::Widget* getFileBrowserPanel ();
    Gtk::Widget* getGeneralPanel ();
    Gtk::Widget* getBatchProcPanel ();
    
  public:
    static void parseDir       (Glib::ustring dirname, std::vector<Glib::ustring>& items, Glib::ustring ext);
    Preferences (int initialPage=0);
    
    void aboutPressed ();

    //void selectStartupDir ();
    //void addExtPressed ();
    //void delExtPressed ();

    //void clearProfilesPressed ();
    //void clearThumbImagesPressed ();
    //void clearAllPressed ();
    void behAddRadioToggled (const Glib::ustring& path);
    void behSetRadioToggled (const Glib::ustring& path);
//    void selectICCProfileDir ();
//    void selectMonitorProfile ();
};

class GlobalPreferencesPanel : public Gtk::VBox {
    class ExtensionColumns : public Gtk::TreeModel::ColumnRecord {
        public:
            Gtk::TreeModelColumn<bool>  enabled;
            Gtk::TreeModelColumn<Glib::ustring>  ext;
            ExtensionColumns() { add(enabled); add(ext); }
    };
    ExtensionColumns extensionColumns;
    Glib::RefPtr<Gtk::ListStore> extensionModel;

protected:
    // Thumbnail options
    Gtk::ComboBoxText* cformat; // Format of cached thumb
    Gtk::SpinButton*   maxThumbSize;
    Gtk::CheckButton*  showDateTime;
    Gtk::CheckButton*  showBasicExif;
    Gtk::CheckButton*  overlayedFileNames;
    Gtk::SpinButton*   maxCacheEntries;
    Gtk::Button*       clearThumbnails;
    Gtk::Button*       clearProfiles;
    Gtk::Button*       clearAll;

    // Startup directory
    Gtk::RadioButton* sdcurrent;
    Gtk::RadioButton* sdlast;
    Gtk::RadioButton* sdhome;
    Gtk::RadioButton* sdother;
    Gtk::Entry* startupdir; // Startup directory for other
    Gtk::Button* sdselect;

    // File extension
    Gtk::TreeView*  extensions; // List of current extensions
    Gtk::Entry*     extension;
    Gtk::Button*    addExt;
    Gtk::Button*    delExt;

    // Color Management
    Gtk::ComboBoxText* intent;
    Gtk::FileChooserButton* iccDir;
    Gtk::FileChooserButton* monProfile;

    // Blink options
    Gtk::CheckButton* blinkClipped;
	Gtk::SpinButton*  hlThresh;
	Gtk::SpinButton*  shThresh;

	// Processing parameters save options
    Gtk::ComboBoxText* rprofiles; // Processing profile for RAW
    Gtk::ComboBoxText* iprofiles; // Processing profile for other images
    Gtk::CheckButton* saveParamsFile; // Save next to file
    Gtk::CheckButton* saveParamsCache; // Save in cache
    Gtk::ComboBoxText* loadParamsPreference;

    Gtk::ComboBoxText* languages; // Default language: need restart
    Gtk::ComboBoxText* theme; // Color theme
    Gtk::Entry* dateformat; // Formatting string for date

    Gtk::FileChooserButton* gimpDir; // Directory for GIMP
    Gtk::FileChooserButton* psDir;   // Directory for Photoshop
    Gtk::Entry* editorToSendTo; // other editor
    Gtk::RadioButton* edGimp;
    Gtk::RadioButton* edPS;
    Gtk::RadioButton* edOther;

    Gtk::Widget* createPanelThumbnails();
    Gtk::Widget* createPanelFiles();
    Gtk::Widget* createPanelAppearance();
    Gtk::Widget* createPanelClipping();
    Gtk::Widget* createPanelProfiles();
    Gtk::Widget* createPanelColor();
    Gtk::Widget* createPanelExternals();

    void fillPreferences ();
    void connectEvents ();
    void storePreferences ();


    void selectStartupDir ();
    void selectStartup ();
    void addExtPressed ();
    void delExtPressed ();
    void paramChanged();
    void themeChanged ();
    void thumbChanged();
    void clearProfilesPressed ();
    void clearThumbImagesPressed ();
    void clearAllPressed ();
public:
    GlobalPreferencesPanel();
};

#endif
