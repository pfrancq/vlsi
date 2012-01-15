/*

	RVLSI Project Library

	KAppOptions.cpp

	Application options - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).

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
	: KDialog(parent), Ui_KAppOptions()
{
	setCaption("Preferences");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
}


//------------------------------------------------------------------------------
void KAppOptions::exec(KDevVLSI* app)
{
	// Init
	MainTab->setCurrentIndex(MainTabIdx);
	Step->setChecked(app->Step);
	ComputeFreePolygons->setChecked(app->ComputeFreePolygons);
	UseFreePolygons->setChecked(app->UseFreePolygons);
	AllOrientations->setChecked(app->AllOrientations);
	HeurAreaP->setValue(app->HeurArea->Get<RParamValue>("P")->GetDouble());
	HeurAreaQ->setValue(app->HeurArea->Get<RParamValue>("Q")->GetDouble());
	HeurAreaWeight->setValue(app->HeurArea->Get<RParamValue>("Weight")->GetDouble());
	HeurDistP->setValue(app->HeurDist->Get<RParamValue>("P")->GetDouble());
	HeurDistQ->setValue(app->HeurDist->Get<RParamValue>("Q")->GetDouble());
	HeurDistWeight->setValue(app->HeurDist->Get<RParamValue>("Weight")->GetDouble());
	SelectWeightP->setValue(app->SelectWeight->Get<RParamValue>("P")->GetDouble());
	SelectWeightQ->setValue(app->SelectWeight->Get<RParamValue>("Q")->GetDouble());
	SelectWeightWeight->setValue(app->SelectWeight->Get<RParamValue>("Weight")->GetDouble());
	SelectDistP->setValue(app->SelectDist->Get<RParamValue>("P")->GetDouble());
	SelectDistQ->setValue(app->SelectDist->Get<RParamValue>("Q")->GetDouble());
	SelectDistWeight->setValue(app->SelectDist->Get<RParamValue>("Weight")->GetDouble());
	MaxGen->setValue(app->MaxGen);
	StepGen->setValue(app->StepGen);
	Heuristic->setCurrentIndex(Heuristic->findText(ToQString(app->Heuristic)));
	PopSize->setValue(app->PopSize);

	if(KDialog::exec())
	{
		app->Step=Step->isChecked();
		app->ComputeFreePolygons=ComputeFreePolygons->isChecked();
		app->UseFreePolygons=UseFreePolygons->isChecked();
		app->AllOrientations=AllOrientations->isChecked();
		app->HeurArea->Get<RParamValue>("P")->SetDouble(HeurAreaP->value());
		app->HeurArea->Get<RParamValue>("Q")->SetDouble(HeurAreaQ->value());
		app->HeurArea->Get<RParamValue>("Weight")->SetDouble(HeurAreaWeight->value());
		app->HeurDist->Get<RParamValue>("P")->SetDouble(HeurDistP->value());
		app->HeurDist->Get<RParamValue>("Q")->SetDouble(HeurDistQ->value());
		app->HeurDist->Get<RParamValue>("Weight")->SetDouble(HeurDistWeight->value());
		app->SelectWeight->Get<RParamValue>("P")->SetDouble(SelectWeightP->value());
		app->SelectWeight->Get<RParamValue>("Q")->SetDouble(SelectWeightQ->value());
		app->SelectWeight->Get<RParamValue>("Weight")->SetDouble(SelectWeightWeight->value());
		app->SelectDist->Get<RParamValue>("P")->SetDouble(SelectDistP->value());
		app->SelectDist->Get<RParamValue>("Q")->SetDouble(SelectDistQ->value());
		app->SelectDist->Get<RParamValue>("Weight")->SetDouble(SelectDistWeight->value());
		app->MaxGen=MaxGen->value();
		app->StepGen=StepGen->value();
		app->Heuristic=FromQString(Heuristic->currentText());
		app->PopSize=PopSize->value();
		app->Apply();
	}
	MainTabIdx=MainTab->currentIndex();
}


//------------------------------------------------------------------------------
void KAppOptions::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"Configure");

	MainTabIdx=General.readEntry("MainTabIdx",0);
}


//------------------------------------------------------------------------------
void KAppOptions::saveOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"Configure");

	General.writeEntry("MainTabIdx",MainTabIdx);
}
