/*
 *  See header file for a description of this class.
 *
 *  $Date: 2008/10/07 14:26:43 $
 *  $Revision: 1.26 $
 *  \author C. Battilana S. Marcellini - INFN Bologna
 */


// This class header
#include "DQM/DTMonitorClient/src/DTLocalTriggerTest.h"

// Framework headers
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

// Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"

// Root
#include "TF1.h"
#include "TProfile.h"


//C++ headers
#include <iostream>
#include <sstream>

using namespace edm;
using namespace std;


DTLocalTriggerTest::DTLocalTriggerTest(const edm::ParameterSet& ps){

  setConfig(ps,"DTLocalTrigger");
  baseFolder = "DT/03-LocalTrigger/";

}


DTLocalTriggerTest::~DTLocalTriggerTest(){

}

void DTLocalTriggerTest::beginJob(const edm::EventSetup& c){
  
  DTLocalTriggerBaseTest::beginJob(c);


  vector<string>::const_iterator iTr   = trigSources.begin();
  vector<string>::const_iterator trEnd = trigSources.end();
  vector<string>::const_iterator iHw   = hwSources.begin();
  vector<string>::const_iterator hwEnd = hwSources.end();


  //Booking
  if(parameters.getUntrackedParameter<bool>("staticBooking", true)){
    for (; iTr != trEnd; ++iTr){
      trigSource = (*iTr);
      for (; iHw != hwEnd; ++iHw){
	hwSource = (*iHw);
	// Loop over the TriggerUnits
	for (int wh=-2; wh<=2; ++wh){
	  // for (int sect=1; sect<=12; ++sect){
// 	    //bookSectorHistos(wh,sect,"","CorrectBXPhi");
// 	    bookSectorHistos(wh,sect,"","CorrFractionPhi");
// 	    bookSectorHistos(wh,sect,"","2ndFractionPhi");
// 	    if (hwSource!="DCC") {
// 	    //bookSectorHistos(wh,sect,"","CorrectBXTheta");
// 	    bookSectorHistos(wh,sect,"","HFractionTheta");
// 	    }
// 	  }
	  bookWheelHistos(wh,"","CorrectBXPhi");
	  bookWheelHistos(wh,"","ResidualBXPhi");
	  bookWheelHistos(wh,"","CorrFractionPhi");
	  bookWheelHistos(wh,"","2ndFractionPhi");
	  bookWheelHistos(wh,"","TriggerInclusivePhi");
	  if (hwSource!="DCC") {
	    bookWheelHistos(wh,"","CorrectBXTheta");
	    bookWheelHistos(wh,"","HFractionTheta");
	  }
	}
      }
    }
  }
  // Summary test histo booking (only static)
  for (iTr = trigSources.begin(); iTr != trEnd; ++iTr){
    trigSource = (*iTr);
    for (iHw = hwSources.begin(); iHw != hwSources.end(); ++iHw){
      hwSource = (*iHw);
      // Loop over the TriggerUnits
      for (int wh=-2; wh<=2; ++wh){
	bookWheelHistos(wh,"","CorrFractionSummary");
	bookWheelHistos(wh,"","2ndFractionSummary");
      }
      bookCmsHistos("CorrFractionSummary");
      bookCmsHistos("2ndFractionSummary");
    }	
  }

}

void DTLocalTriggerTest::runClientDiagnostic() {

  // Loop over Trig & Hw sources
  for (vector<string>::const_iterator iTr = trigSources.begin(); iTr != trigSources.end(); ++iTr){
    trigSource = (*iTr);
    for (vector<string>::const_iterator iHw = hwSources.begin(); iHw != hwSources.end(); ++iHw){
      hwSource = (*iHw);
      // Loop over the TriggerUnits
      for (int stat=1; stat<=4; ++stat){
	for (int wh=-2; wh<=2; ++wh){
	  for (int sect=1; sect<=12; ++sect){
	    DTChamberId chId(wh,stat,sect);
	    // int sector_id = (wh+3)+(sect-1)*5;
	    // uint32_t indexCh = chId.rawId();
	    

	    // Perform DCC/DDU common plot analysis (Phi ones)
	    TH2F * BXvsQual      = getHisto<TH2F>(dbe->get(getMEName("BXvsQual","LocalTriggerPhi", chId)));
	    TH1F * BestQual      = getHisto<TH1F>(dbe->get(getMEName("BestQual","LocalTriggerPhi", chId)));
	    TH2F * Flag1stvsQual = getHisto<TH2F>(dbe->get(getMEName("Flag1stvsQual","LocalTriggerPhi", chId))); 
	    if (BXvsQual && Flag1stvsQual && BestQual) {

	      int corrSummary   = 1;
	      int secondSummary = 1;

	      if (BestQual->GetEntries()>1) {
	      
		TH1D* BXHH    = BXvsQual->ProjectionY("",6,7,"");
		TH1D* Flag1st = Flag1stvsQual->ProjectionY();
		int BXOK_bin  = BXHH->GetEntries()>=1 ? BXHH->GetMaximumBin() : 51;
		double BXMean = BXHH->GetEntries()>=1 ? BXHH->GetMean() : 51;
		double BX_OK  = BXvsQual->GetYaxis()->GetBinCenter(BXOK_bin);
		double trigsFlag2nd = Flag1st->GetBinContent(2);
		double trigs = Flag1st->GetEntries();
		double besttrigs = BestQual->GetEntries();
		double besttrigsCorr = BestQual->Integral(5,7,"");
		delete BXHH;
		delete Flag1st;
		
		double corrFrac   = besttrigsCorr/besttrigs;
		double secondFrac = trigsFlag2nd/trigs;
		if (corrFrac < parameters.getUntrackedParameter<double>("corrFracError",.5)){
		  corrSummary = 2;
		}
		else if (corrFrac < parameters.getUntrackedParameter<double>("corrFracWarning",.6)){
		  corrSummary = 3;
		}
		else {
		  corrSummary = 0;
		}
		if (secondFrac > parameters.getUntrackedParameter<double>("secondFracError",.2)){
		  secondSummary = 2;
		}
		else if (secondFrac > parameters.getUntrackedParameter<double>("secondFracWarning",.1)){
		  secondSummary = 3;
		}
		else {
		  secondSummary = 0;
		}
		
		// if( secME[sector_id].find(fullName("CorrFractionPhi")) == secME[sector_id].end() ){
// 		  //bookSectorHistos(wh,sect,"","CorrectBXPhi");
// 		  bookSectorHistos(wh,sect,"","CorrFractionPhi");
// 		  bookSectorHistos(wh,sect,"","2ndFractionPhi");
// 		}
		if( whME[wh].find(fullName("CorrectBXPhi")) == whME[wh].end() ){
		  bookWheelHistos(wh,"","ResidualBXPhi");
		  bookWheelHistos(wh,"","CorrectBXPhi");
		  bookWheelHistos(wh,"","CorrFractionPhi");
		  bookWheelHistos(wh,"","2ndFractionPhi");
		  bookWheelHistos(wh,"","TriggerInclusivePhi");
		}
		//std::map<std::string,MonitorElement*> *innerME = &(secME[sector_id]);
		//innerME->find(fullName("CorrectBXPhi"))->second->setBinContent(stat,BX_OK);
		//innerME->find(fullName("CorrFractionPhi"))->second->setBinContent(stat,corrFrac);
		//innerME->find(fullName("2ndFractionPhi"))->second->setBinContent(stat,secondFrac);
	   
		std::map<std::string,MonitorElement*> *innerME = &(whME[wh]);
		innerME->find(fullName("CorrectBXPhi"))->second->setBinContent(sect,stat,BX_OK+0.00001);
		innerME->find(fullName("ResidualBXPhi"))->second->setBinContent(sect,stat,round(25.*(BXMean-BX_OK))+0.00001);
		innerME->find(fullName("CorrFractionPhi"))->second->setBinContent(sect,stat,corrFrac);
		innerME->find(fullName("TriggerInclusivePhi"))->second->setBinContent(sect,stat,besttrigs);
		innerME->find(fullName("2ndFractionPhi"))->second->setBinContent(sect,stat,secondFrac);
	    
	      }

		whME[wh].find(fullName("CorrFractionSummary"))->second->setBinContent(sect,stat,corrSummary);
		whME[wh].find(fullName("2ndFractionSummary"))->second->setBinContent(sect,stat,secondSummary);

	    }

	    // Perform DCC/DDU common plot analysis (Theta ones)	    
	    TH2F * ThetaBXvsQual = getHisto<TH2F>(dbe->get(getMEName("ThetaBXvsQual","LocalTriggerTheta", chId)));
	    TH1F * ThetaBestQual = getHisto<TH1F>(dbe->get(getMEName("ThetaBestQual","LocalTriggerTheta", chId)));
	
	    // no theta triggers in stat 4!
	    if (ThetaBXvsQual && ThetaBestQual && stat<4 && ThetaBestQual->GetEntries()>1) {
	      TH1D* BXH       = ThetaBXvsQual->ProjectionY("",4,4,"");
	      int    BXOK_bin = BXH->GetEffectiveEntries()>=1 ? BXH->GetMaximumBin(): 10;
	      double BX_OK    = ThetaBXvsQual->GetYaxis()->GetBinCenter(BXOK_bin);
	      double trigs    = ThetaBestQual->GetEntries(); 
	      double trigsH   = ThetaBestQual->GetBinContent(4);
	      delete BXH; 
	      
	      // if( secME[sector_id].find(fullName("HFractionTheta")) == secME[sector_id].end() ){
// 		// bookSectorHistos(wh,sect,"","CorrectBXTheta");
// 		bookSectorHistos(wh,sect,"","HFractionTheta");
// 	      }
	      //std::map<std::string,MonitorElement*> *innerME = &(secME.find(sector_id)->second);
	      // innerME->find(fullName("CorrectBXTheta"))->second->setBinContent(stat,BX_OK);
	      //innerME->find(fullName("HFractionTheta"))->second->setBinContent(stat,trigsH/trigs);

	      if( whME[wh].find(fullName("HFractionTheta")) == whME[wh].end() ){
		bookWheelHistos(wh,"","CorrectBXTheta");
		bookWheelHistos(wh,"","HFractionTheta");
	      }
	      std::map<std::string,MonitorElement*> *innerME = &(whME.find(wh)->second);
	      innerME->find(fullName("CorrectBXTheta"))->second->setBinContent(sect,stat,BX_OK);
	      innerME->find(fullName("HFractionTheta"))->second->setBinContent(sect,stat,trigsH/trigs);
	    
	    }

	  }
	}
      }
    }
  }	

  for (vector<string>::const_iterator iTr = trigSources.begin(); iTr != trigSources.end(); ++iTr){
    trigSource = (*iTr);
    for (vector<string>::const_iterator iHw = hwSources.begin(); iHw != hwSources.end(); ++iHw){
      hwSource = (*iHw);  
      for (int wh=-2; wh<=2; ++wh){
	std::map<std::string,MonitorElement*> *innerME = &(whME[wh]);
	TH2F* corrWhSummary   = getHisto<TH2F>(innerME->find(fullName("CorrFractionSummary"))->second);
	TH2F* secondWhSummary = getHisto<TH2F>(innerME->find(fullName("2ndFractionSummary"))->second);
	for (int sect=1; sect<=12; ++sect){
	  int corrErr      = 0;
	  int secondErr    = 0;
	  int corrNoData   = 0;
	  int secondNoData = 0;
	  for (int stat=1; stat<=4; ++stat){
	    switch (static_cast<int>(corrWhSummary->GetBinContent(sect,stat))) {
	    case 1:
	      corrNoData++;
	    case 2:
	      corrErr++;
	    }
	    switch (static_cast<int>(secondWhSummary->GetBinContent(sect,stat))) {
	    case 1:
	      secondNoData++;
	    case 2:
	      secondErr++;
	    }
	  }
	  if (corrNoData == 4)   corrErr   = 5;
	  if (secondNoData == 4) secondErr = 5;
	  cmsME.find(fullName("CorrFractionSummary"))->second->setBinContent(sect,wh+3,corrErr);
	  cmsME.find(fullName("2ndFractionSummary"))->second->setBinContent(sect,wh+3,secondErr);
	}
      }
    }
  }

}

