import FWCore.ParameterSet.Config as cms

def run22XonSummer08AODSIM(process, layers=[0,1]) :
    ## (A): Drop FLow on input
    process.source.inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_particleFlow_*_*',
            #'drop *_particleFlowBlock_*_*',
    )
    ## (B): Also switch to CaloTau (temporary workaround)
    print "WARNING: to run on Summer08AODSIM from 2.2.X requirs to drop ParticleFlow,\n\tso PAT will switch from PFTau to CaloTau"
    from PhysicsTools.PatAlgos.tools.tauTools import switchToCaloTau
    switchToCaloTau(process)
    ## (C): Compute CaloTau discriminators which are missing in 21X AOD
    process.patAODExtraReco += process.patCaloTauDiscrimination
    process.allLayer1Taus.tauIDSources = cms.PSet(  
            leadingTrackFinding = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackFinding"),
            leadingTrackPtCut   = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
            byIsolation         = cms.InputTag("caloRecoTauDiscriminationByIsolation"),
           #againstElectron     = cms.InputTag("caloRecoTauDiscriminationAgainstElectron"),  # Not on AOD
    )


