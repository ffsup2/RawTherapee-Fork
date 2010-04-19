/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2010 Fabio Suprani
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
#ifndef _RAWDEMOSAICING_H_
#define _RAWDEMOSAICING_H_

#include <gtkmm.h>
#include <toolpanel.h>


class RawDemosaicing : public Gtk::VBox, public ToolPanel{

  protected:

    Gtk::ComboBoxText* dmethod;
    Gtk::SpinButton* ccSteps;
    Gtk::VBox *dcbOptions;
    Gtk::SpinButton* dcbIterations;
    Gtk::ToggleButton* dcbEnhance;
    Gtk::Label* methodl;
    Gtk::Label* ccStepsl;

    bool ccStepsDirty;
    bool dcbIterDirty;
    sigc::connection stepsconn,methodconn,dcbIterconn,dcbEnhconn;
  public:

    RawDemosaicing ();

    void read           (const rtengine::procparams::ProcParams* pp, const ParamsEdited* pedited=NULL);
    void write          (rtengine::procparams::ProcParams* pp, ParamsEdited* pedited=NULL);
    void setBatchMode   (bool batchMode);

    void ccStepsChanged ();
    void methodChanged ();
    void rawParamsChanged();
};

#endif
