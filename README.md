# 2025 HBT correlation code
This code is going to be used to measure the HBT correlations in 1 and 3D for charged hadrons in pO, OO and NeNe collisions with 2025 dataset.
## Instructions
The code was updated to work on LXPLUS9 in order to use more resources from the cluster in HTCondor. To login in LXPLUS9 use:
```
ssh username@lxplus9.cern.ch
```
Once logged into LXPLUS machines setup the CMSSW version (to be updated) as follows:
```
cmsrel CMSSW_13_0_5
cd CMSSW_13_0_5/src/
cmsenv
```
and work inside of the ```src``` folder.
The code was created to generate histograms as function of centrality or multiplicity.
- For centrality depentency we have the following bins: 0-10%, 10-30%, 30-50%, 50-70% and 70-100% (not a good idea to use given the higher EM contamination). This code takes a long time to run due the correlations between large number of tracks and also because of mixing. To get the centrality dependency, use:
```
git clone https://github.com/denerslemos/HBTCorrelation_2025.git
```
I have added same submission idea as XeXe, see here: ,also bellow the line but need to edit the submit*.py. So far we have one MC test file that can be used. All tests here are centrality dependency, check XeXe instructions to how move it to multiplicity dependency. For the small amount of events here I will not use condor.
 
You can run it using (no mixing, no 3D):
```
root -l -b -q "correlation_2025.C(\"inputdataset/OO_MC_4ktest.txt\",\"out_OO.root\", 1, 0, 1, 10, 1000, 50.0, 1, 0, 0, 0)" &> out.txt &
```
Adding mixing (no 3D):
```
root -l -b -q "correlation_2025.C(\"inputdataset/OO_MC_4ktest.txt\",\"out_OO.root\", 1, 0, 0, 10, 1000, 50.0, 1, 0, 0, 0)" &> out.txt &
```
Adding 3D (no mixing):
```
root -l -b -q "correlation_2025.C(\"inputdataset/OO_MC_4ktest.txt\",\"out_OO.root\", 1, 0, 1, 10, 1000, 50.0, 0, 0, 0, 0)" &> out.txt &
```

Adding all:
```
root -l -b -q "correlation_2025.C(\"inputdataset/OO_MC_4ktest.txt\",\"out_OO.root\", 1, 0, 0, 10, 1000, 50.0, 0, 0, 0, 0)" &> out.txt &
```

all of the examples run both gen and reco.
