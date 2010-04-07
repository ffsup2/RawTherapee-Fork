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
#include <RawDemosaicing.h>
#include <options.h>
#include <guiutils.h>
using namespace rtengine;
using namespace rtengine::procparams;


RawDemosaicing::RawDemosaicing ()
{
   Gtk::HBox* hb1 = Gtk::manage (new Gtk::HBox ());
   hb1->pack_start (*Gtk::manage (new Gtk::Label ( M("PREFERENCES_DMETHOD") +": ")));
   dmethod = Gtk::manage (new Gtk::ComboBoxText ());
   dmethod->append_text ("EAHD");
   dmethod->append_text ("HPHD");
   dmethod->append_text ("VNG-4");
   dmethod->set_active(0);
   hb1->pack_start (*dmethod);
   pack_start( *hb1, Gtk::PACK_SHRINK, 4);

   Gtk::HBox* hb2 = Gtk::manage (new Gtk::HBox ());
   hb2->pack_start (*Gtk::manage (new Gtk::Label ( M("PREFERENCES_FALSECOLOR") +": ")));
   ccSteps = Gtk::manage (new Gtk::SpinButton ());
   ccSteps ->set_range (0, 10 );
   ccSteps ->set_digits (0);
   ccSteps ->set_increments (1,100);
   ccSteps ->set_value (1);
   ccSteps ->set_size_request (60, -1);
   hb2->pack_start(*ccSteps);
   pack_start( *hb2, Gtk::PACK_SHRINK, 4);

   stepsconn = ccSteps->signal_value_changed().connect ( sigc::mem_fun(*this, &RawDemosaicing::ccStepsChanged), true);
   methodconn = dmethod->signal_changed().connect( sigc::mem_fun(*this, &RawDemosaicing::methodChanged) );
}


void RawDemosaicing::read(const rtengine::procparams::ProcParams* pp, const ParamsEdited* pedited)
{
   disableListener ();
   stepsconn.block (true);
   methodconn.block (true);
   if( pp->rawdemosaic.dmethod == "eahd")
	   dmethod->set_active(0);
   else if( pp->rawdemosaic.dmethod == "hphd")
	   dmethod->set_active(1);
   else if( pp->rawdemosaic.dmethod == "vng4")
	   dmethod->set_active(2);
   else
	   dmethod->set_active(3); // Null

   ccSteps->set_value (pp->rawdemosaic.ccSteps);


   ccStepsDirty = false;
   if(pedited ){
	   ccStepsDirty = pedited->rawdemosaic.ccSteps;
	   if( !pedited->rawdemosaic.dmethod )
		   dmethod->set_active(3);
   }

   stepsconn.block (false);
   methodconn.block (false);
   enableListener ();
}

void RawDemosaicing::write( rtengine::procparams::ProcParams* pp, ParamsEdited* pedited)
{
	pp->rawdemosaic.ccSteps = ccSteps->get_value_as_int();

	if (dmethod->get_active_row_number()==0)
		pp->rawdemosaic.dmethod = "eahd";
	else if (dmethod->get_active_row_number()==1)
		pp->rawdemosaic.dmethod = "hphd";
	else if (dmethod->get_active_row_number()==2)
		pp->rawdemosaic.dmethod = "vng4";

	if (pedited) {
		pedited->rawdemosaic.ccSteps = ccStepsDirty;
		pedited->rawdemosaic.dmethod = dmethod->get_active_row_number() != 3;
	}
}

void RawDemosaicing::setBatchMode(bool batchMode)
{
   ToolPanel::setBatchMode (batchMode);
}

void RawDemosaicing::ccStepsChanged ()
{
    if (listener)
        listener->panelChanged (EvCcSteps, Glib::ustring(M("PREFERENCES_FALSECOLOR")) );
}
void RawDemosaicing::methodChanged ()
{
    if (listener)
        listener->panelChanged (EvDemosaic, Glib::ustring(M("PREFERENCES_DMETHOD")) );
}
