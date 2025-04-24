#!/usr/bin/env python

'''Add important stuff'''
import os.path
import optparse

outputfolder = "/eos/user/d/ddesouza/OOhistos"
cmsswrepo = "/afs/cern.ch/work/d/ddesouza/UIC/pPbMultAna/CMSSW_13_0_5/src"
pwdrepo = "/afs/cern.ch/work/d/ddesouza/UIC/pPbMultAna/CMSSW_13_0_5/src/HBTCorrelation_2025"
systematics = 0
inPut = 'OOMB_centrality_syst_'+str(systematics)

os.system("mkdir -p cond")
os.system("mkdir -p "+str(outputfolder)+"/"+str(inPut))
os.system("python3 HTCondor_submit_centrality_MC.py -i inputdataset/OO_MC_4ktest  -o "+str(outputfolder)+"/"+str(inPut)+"/OO_MC_ -f nextweek -c 4 -n 40 -s OO_MC -u "+str(systematics)+" -w "+str(cmsswrepo)+" -p "+str(pwdrepo))
