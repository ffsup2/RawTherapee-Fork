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
   for( size_t i=0; i< procparams::RAWParams::numMethods;i++)
	   dmethod->append_text(procparams::RAWParams::methodstring[i]);

   dmethod->set_active(0);
   hb1->pack_end (*dmethod);
   pack_start( *hb1, Gtk::PACK_SHRINK, 4);

   dcbOptions = Gtk::manage (new Gtk::VBox ());
   dcbOptions->set_border_width(4);
   Gtk::HBox* hb3 = Gtk::manage (new Gtk::HBox ());
   hb3->pack_start (*Gtk::manage (new Gtk::Label ( M("PREFERENCES_DCBITERATIONS") +": ")), Gtk::PACK_SHRINK, 4);
   dcbIterations = Gtk::manage (new Gtk::SpinButton ());
   dcbIterations ->set_range (0, 10 );
   dcbIterations ->set_digits (0);
   dcbIterations ->set_increments (1,100);
   dcbIterations ->set_value (1);
   dcbIterations ->set_size_request (60, -1);
   hb3->pack_start(*dcbIterations, Gtk::PACK_SHRINK, 4);
   dcbEnhance = Gtk::manage (new Gtk::CheckButton(M("PREFERENCES_DCBENHANCE")));
   dcbOptions->pack_start(*hb3);
   dcbOptions->pack_start(*dcbEnhance);
   pack_start( *dcbOptions, Gtk::PACK_SHRINK, 4);
   pack_start( *Gtk::manage( new Gtk::HSeparator()), Gtk::PACK_SHRINK, 4 );

   Gtk::HBox* hb2 = Gtk::manage (new Gtk::HBox ());
   hb2->pack_start (*Gtk::manage (new Gtk::Label ( M("PREFERENCES_FALSECOLOR") +": ")), Gtk::PACK_SHRINK, 4);
   ccSteps = Gtk::manage (new Gtk::SpinButton ());
   ccSteps ->set_range (0, 10 );
   ccSteps ->set_digits (0);
   ccSteps ->set_increments (1,100);
   ccSteps ->set_value (1);
   ccSteps ->set_size_request (60, -1);
   hb2->pack_start(*ccSteps, Gtk::PACK_SHRINK, 4);
   pack_start( *hb2, Gtk::PACK_SHRINK, 4);

   stepsconn = ccSteps->signal_value_changed().connect ( sigc::mem_fun(*this, &RawDemosaicing::ccStepsChanged), true);
   methodconn = dmethod->signal_changed().connect( sigc::mem_fun(*this, &RawDemosaicing::methodChanged) );
   dcbIterconn = dcbIterations->signal_value_changed().connect ( sigc::mem_fun(*this, &RawDemosaicing::rawParamsChanged), true);
   dcbEnhconn = dcbEnhance->signal_toggled().connect ( sigc::mem_fun(*this, &RawDemosaicing::rawParamsChanged), true);
}


void RawDemosaicing::read(const rtengine::procparams::ProcParams* pp, const ParamsEdited* pedited)
{
   disableListener ();
   stepsconn.block (true);
   methodconn.block (true);
   dcbIterconn.block (true);
   dcbEnhconn.block (true);

   dmethod->set_active(procparams::RAWParams::numMethods);
   for( size_t i=0; i< procparams::RAWParams::numMethods;i++)
	   if( pp->rawdemosaic.dmethod == procparams::RAWParams::methodstring[i]){
		   dmethod->set_active(i);
		   break;
	   }

   dcbIterations->set_value (pp->rawdemosaic.dcb_iterations);
   dcbEnhance->set_active(pp->rawdemosaic.dcb_enhance);
   ccSteps->set_value (pp->rawdemosaic.ccSteps);
   if (pp->rawdemosaic.dmethod == procparams::RAWParams::methodstring[procparams::RAWParams::dcb]){
	   dcbOptions->show();
   }else{
	   dcbOptions->hide();
   }
   ccStepsDirty = false;
   dcbIterDirty = false;
   if(pedited ){
	   ccStepsDirty = pedited->rawdemosaic.ccSteps;
	   dcbIterDirty = pedited->rawdemosaic.dcbIterations;
	   if( !pedited->rawdemosaic.dmethod )
		   dmethod->set_active(procparams::RAWParams::numMethods); // No name
   }

   stepsconn.block (false);
   methodconn.block (false);
   dcbIterconn.block (false);
   dcbEnhconn.block (false);
   enableListener ();
}

void RawDemosaicing::write( rtengine::procparams::ProcParams* pp, ParamsEdited* pedited)
{
	pp->rawdemosaic.ccSteps = ccSteps->get_value_as_int();
	pp->rawdemosaic.dcb_iterations = dcbIterations->get_value_as_int();
	pp->rawdemosaic.dcb_enhance = dcbEnhance->get_active();

	if( dmethod->get_active_row_number() < procparams::RAWParams::numMethods)
		pp->rawdemosaic.dmethod = procparams::RAWParams::methodstring[dmethod->get_active_row_number()];

	if (pedited) {
		pedited->rawdemosaic.ccSteps = ccStepsDirty;
		pedited->rawdemosaic.dmethod = dmethod->get_active_row_number() != procparams::RAWParams::numMethods;
	}
}

void RawDemosaicing::setBatchMode(bool batchMode)
{
   dmethod->set_active(procparams::RAWParams::numMethods); // No name
   dcbOptions->hide();
   ToolPanel::setBatchMode (batchMode);
}

void RawDemosaicing::ccStepsChanged ()
{
    if (listener)
        listener->panelChanged (EvCcSteps, Glib::ustring::format (ccSteps->get_value_as_int()) );
}
void RawDemosaicing::methodChanged ()
{
	int  curSelection = dmethod->get_active_row_number();
	if ( curSelection == procparams::RAWParams::dcb){
		dcbOptions->show();
	}else{
		dcbOptions->hide();
	}
	Glib::ustring s="";
	if( curSelection>=0 && curSelection < procparams::RAWParams::numMethods)
	    s = procparams::RAWParams::methodstring[curSelection];

    if (listener)
        listener->panelChanged (EvDemosaic, Glib::ustring(M("PREFERENCES_DMETHOD"))+ "="+ s);
}
void RawDemosaicing::rawParamsChanged ()
{
    if (listener)
        listener->panelChanged (EvDemosaic, Glib::ustring("params") );
}
