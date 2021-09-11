/*

	RVLSI Project Library

	KAppOptions.cpp

	Application options - Implementation.

	Copyright 2000-2016 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).


	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// includes files for current application
#include <kdevvlsi.h>
#include <kappoptions.h>


//------------------------------------------------------------------------------
// Current tabs selected
static int MainTabIdx;



//------------------------------------------------------------------------------
//
// class KAppOptions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
KAppOptions::KAppOptions(QWidget* parent)
	: QDialog(parent), Ui_KAppOptions()
{
	setupUi(this);
	setWindowTitle("Preferences");
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


//------------------------------------------------------------------------------
void KAppOptions::exec(QVLSIApp* app)
{
	// Init
	MainTab->setCurrentIndex(MainTabIdx);
	Step->setChecked(app->Step);
	ComputeFreePolygons->setChecked(app->ComputeFreePolygons);
	UseFreePolygons->setChecked(app->UseFreePolygons);
	AllOrientations->setChecked(app->AllOrientations);

	HeurArea->Set(app->HeurArea);
	HeurDist->Set(app->HeurDist);
	SelectWeight->Set(app->SelectWeight);
	SelectDist->Set(app->SelectDist);

	MaxGen->setValue(app->MaxGen);
	StepGen->setValue(app->StepGen);
	Heuristic->setCurrentIndex(Heuristic->findText(ToQString(app->Heuristic)));
	PopSize->setValue(app->PopSize);
	WeightedDistances->setChecked(app->WeightedDistances);

	if(QDialog::exec())
	{
		app->Step=Step->isChecked();
		app->ComputeFreePolygons=ComputeFreePolygons->isChecked();
		app->UseFreePolygons=UseFreePolygons->isChecked();
		app->AllOrientations=AllOrientations->isChecked();

		HeurArea->Get(app->HeurArea);
		HeurDist->Get(app->HeurDist);
		SelectWeight->Get(app->SelectWeight);
		SelectDist->Get(app->SelectDist);

		app->MaxGen=MaxGen->value();
		app->StepGen=StepGen->value();
		app->Heuristic=FromQString(Heuristic->currentText());
		app->PopSize=PopSize->value();
		app->WeightedDistances=WeightedDistances->isChecked();
		app->Apply();
	}
	MainTabIdx=MainTab->currentIndex();
}
