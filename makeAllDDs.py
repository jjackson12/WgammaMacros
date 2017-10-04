from os import path, makedirs, getcwd
from glob import glob
from runHgammaSelector import processHg
from pprint import pprint

debug = False

baseDir = path.join(getcwd(), "organize_smallifications")
categories = ["backgrounds", "signals", "data"]
catDirs = {}
for category in categories:
  catDirs[category] = path.join(baseDir, category)

pprint(catDirs)

outDir = baseDir.replace("smallifications", "DDs")
if not path.exists(outDir):
  makedirs(outDir)
print "catDirs", catDirs
first = True
for catDir in catDirs:
  catOutDir = path.join(outDir, catDir)
  inputFiles = glob("%s/%s/*.root" % (baseDir, catDir))
  
  if not path.exists(catOutDir):
    makedirs(catOutDir)
  for inputFile in inputFiles:
    if first:
      print "about to call the first processHg" 
      processHg(inputFile, inputFile.replace("smallified", "ddTree").replace("smallifications", "DDs_pupCorrs"), False)
      first=False
    elif not debug:
      processHg(inputFile, inputFile.replace("smallified", "ddTree").replace("smallifications", "DDs_pupCorrs"), True, True)
