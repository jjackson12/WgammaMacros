# functions to centrally store Hgamma parameters
# John Hakala -  May 16, 2016
# Jacob Jackson - June 18, 2018
from ROOT import *

def getSamplesDirs():
  response = {}
  response["sigSmall3sDir"] = "../MC/Signal/smallified"
  response["bkgSmall3sDir"]       = "../MC/Background/smallified"
  response["dataSmall3sDir"]       = "../WgammaCondor/smallified_data"

  response["dataSmall3File"]       = "../WgammaCondor/smallified_data/smallified_singlePhoton2016.root"

  response["bkgDDdir"]            = "../MC/Background/selected"
  response["sigDDdir"]         = "../MC/Signal/selected"
  response["dataDDdir"]          = "./"
  response["dataDDFile"]          = "selected_singlePhoton2016.root"
  return response

#TODO: What
def getNormalizations():
  normalizations = {}
  normalizations["650"]  = 1.0
  normalizations["750"]  = 1.0
  normalizations["850"]  = 1.0
  normalizations["1000"] = 0.8
  normalizations["1150"] = 0.8
  normalizations["1300"] = 0.4
  normalizations["1450"] = 0.4
  normalizations["1600"] = 0.4
  normalizations["1750"] = 0.4
  normalizations["1900"] = 0.2
  normalizations["2050"] = 0.2
  normalizations["2450"] = 0.2
  normalizations["2850"] = 0.2  
  normalizations["3250"] = 0.2  
  return normalizations

def getMassWindows():
  # TODO: this needs updating for the new signals
  massWindows = {}
  massWindows[650]  = [600,   700]
  massWindows[750]  = [700,   800]
  massWindows[850]  = [800,   900]
  massWindows[1000] = [900,  1100]
  massWindows[1150] = [1050, 1250]
  massWindows[1300] = [1175, 1425]
  massWindows[1450] = [1300, 1600]
  massWindows[1600] = [1400, 1800]
  massWindows[1750] = [1550, 1950]
  massWindows[1900] = [1700, 2100]
  massWindows[2050] = [1850, 2250]
  massWindows[2450] = [2200, 2700]
  massWindows[2850] = [2600, 3100]
  massWindows[3250] = [3000, 3500]
  #return massWindows
  return []

def getSigNevents():
  sigNevents = {}
  for mass in getNormalizations().keys():
    flattuple = TFile("%s/smallified_sig_m%s.root"%(getSamplesDirs()["signalsSmall3sDir"], mass))
    hCounter = flattuple.Get("ntuplizer/hCounter")
    sigNevents[mass] = hCounter.GetBinContent(1)
  return sigNevents

def getVariableDict():
  varDict = {}
  varDict["WJet_puppi_abseta"]    = "#||{#eta_{J}}"
  varDict["WJet_puppi_eta"]       = "#eta_{J}"
  varDict["WJet_puppi_phi"]       = "#phi_{J}"
  varDict["WJet_puppi_pt"]        = "p_{T}^{J}"
  varDict["leadingPhEta"]              = "#eta_{#gamma}"
  varDict["phJetDeltaR_W"]         = "#DeltaR(#gamma, jet)"
  varDict["leadingPhPt"]               = "p_{T}^{#gamma} (GeV)"
  varDict["WJetTau21"]              = "#tau_{21}"
  varDict["leadingPhAbsEta"]           = "#||{#eta_{#gamma}}"
  varDict["phPtOverMgammaj"]           = "p_{T}^{#gamma}/m_{#gammaJ}  "
  varDict["WJetPtOverMgammaj"]           = "W^{#gamma}/m_{#gammaJ}  "
  varDict["leadingPhPhi"]              = "#phi_{#gamma}"
  varDict["cosThetaStar"]              = "#||{cos(#theta*)}"
  varDict["phJetInvMass_puppi_softdrop_W"] = "m_{#gammaJ} (GeV)"
  varDict["WPuppi_softdropJetCorrMass"]    = "m_{J}^{PUPPI+SD} (GeV)"
  return varDict


