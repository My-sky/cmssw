#ifndef DCCEBEVENTBLOCK_HH
#define DCCEBEVENTBLOCK_HH


/*
 *\ Class DCCEBEventBlock
 *
 * Specialization of the DCCEventBlock for the EB 
 * The class instintes the DCCTowerBlock, DCCEBTCBlock and DCCEBSRPBlock unpacking classes
 *
 * \file DCCEBEventBlock.h
 *
 * $Date: 2007/07/24 11:39:35 $
 * $Revision: 1.5 $
 *
 * \author N. Almeida
 * 
 *
*/

#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>
#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>
#include "DCCRawDataDefinitions.h"
#include "DCCEventBlock.h"


class DCCEBEventBlock : public DCCEventBlock{
	
  public :

   DCCEBEventBlock( DCCDataUnpacker * u, EcalElectronicsMapper *m ,  bool hU, bool srpU, bool tccU, bool feU, bool memU);
  	
  protected :
  
    int unpackTCCBlocks();
   
};

#endif
