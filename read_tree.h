#include "call_libraries.h"  // call libraries from ROOT and C++

// declare variables

// event quantities
float vertexz; // event z vertex
int hiBin; // event centrality
float hfplus; // event hf + deposit of energy
float hfminus; // event hf - deposit of energy
int PrimaryVertexFilter; 
int BeamScrapingFilter;
int HfCoincFilter;

// reco tracks
int ntrk;                 // number of track
std::vector<float> *trkpt = 0;       // track pT
std::vector<float> *trketa = 0;      // track eta
std::vector<float> *trkphi = 0;     // track phi
std::vector<float> *trkpterr = 0;    // track pT error (uncertainty)
std::vector<float> *trkdcaxy = 0;    // track dxy impact parameter (transverse distance between primary vertex and collision - distance of closest approuch - DCA)
std::vector<float> *trkdcaz = 0;    // track dz impact parameter (longitudinal distance between primary vertex and collision - distance of closest approuch - DCA)
std::vector<float> *trkdcaxyerr = 0; // track dxy error (uncertainty)
std::vector<float> *trkdcazerr = 0;  // track dxy error (uncertainty)
std::vector<float> *trknormchi2 = 0;     // track reconstruction chi2 of the fitting
std::vector<char> *trkcharge = 0;     // track charge
std::vector<char> *trknhits = 0;     // number of hits in the tracker
std::vector<char> *trknlayer = 0;    // number of layers with measurement in the tracker
std::vector<char> *trkpixhits = 0;   // number of hits in the pixel detector
std::vector<bool> *highpur = 0;     // tracker steps MVA selection
std::vector<int> *trkpdgid = 0;     // tracker steps MVA selection

// gen tracks
int genmult;
std::vector<float> *gen_trkpt = 0;  // gen particle pT
std::vector<float> *gen_trketa = 0; // gen particle eta
std::vector<float> *gen_trkphi = 0; // gen particle phi
std::vector<int> *gen_trkchg = 0;   // gen particle charge
std::vector<int> *gen_trkpid = 0;   // gen particle pid

//All variables listed above are readed in the function bellow
/*
Function to read the Forest/Skim tree
Arguments ->  transfer quantities from trees to our variables
tree: input TChain from jet_analyzer.C file
is_MC: true -> MC; false -> Data
*/
void read_tree(TChain *tree, bool is_MC){

    tree->SetBranchStatus("*", 0); // disable all branches - this is important while reading big files

    // enable branches of interest -> see definition of each variables above
    // event quantities
    tree->SetBranchStatus("vz", 1);
    tree->SetBranchAddress("vz", &vertexz);
	tree->SetBranchStatus("hiBin", 1); 
	tree->SetBranchAddress("hiBin", &hiBin);
	tree->SetBranchStatus("hiHFplus", 1); 
	tree->SetBranchAddress("hiHFplus", &hfplus);
	tree->SetBranchStatus("hiHFminus", 1); 
	tree->SetBranchAddress("hiHFminus", &hfminus);
	// Other event information can be added here
/*  // Event filters to be added here
	tree->SetBranchStatus("pPAprimaryVertexFilter", 1); 
	tree->SetBranchAddress("pPAprimaryVertexFilter", &PrimaryVertexFilter);	
	tree->SetBranchStatus("pBeamScrapingFilter", 1); 
	tree->SetBranchAddress("pBeamScrapingFilter", &BeamScrapingFilter);
	tree->SetBranchStatus("phfCoincFilter3", 1);
	tree->SetBranchAddress("phfCoincFilter3", &HfCoincFilter);
*/	
    // track quantities
    tree->SetBranchStatus("nTrk", 1);
    tree->SetBranchStatus("trkPt", 1);
    tree->SetBranchStatus("trkEta", 1);
    tree->SetBranchStatus("trkPhi", 1);
    tree->SetBranchStatus("trkPtError", 1);
    tree->SetBranchStatus("trkDxyFirstVtx", 1);
    tree->SetBranchStatus("trkDxyErrFirstVtx", 1);
    tree->SetBranchStatus("trkDzFirstVtx", 1);
    tree->SetBranchStatus("trkDzErrFirstVtx", 1);
    tree->SetBranchStatus("trkNormChi2", 1);
    tree->SetBranchStatus("trkCharge", 1);
    tree->SetBranchStatus("trkNHits", 1);
    tree->SetBranchStatus("trkNLayers", 1);
    tree->SetBranchStatus("trkNPixHits", 1);
    tree->SetBranchStatus("highPurity", 1);
    tree->SetBranchStatus("trkPDGId", 1);

    tree->SetBranchAddress("nTrk", &ntrk);
    tree->SetBranchAddress("trkPt", &trkpt);
    tree->SetBranchAddress("trkEta", &trketa);
    tree->SetBranchAddress("trkPhi", &trkphi);
    tree->SetBranchAddress("trkPtError", &trkpterr);
    tree->SetBranchAddress("trkDxyFirstVtx", &trkdcaxy);
    tree->SetBranchAddress("trkDxyErrFirstVtx", &trkdcaxyerr);
    tree->SetBranchAddress("trkDzFirstVtx", &trkdcaz);
    tree->SetBranchAddress("trkDzErrFirstVtx", &trkdcazerr);
    tree->SetBranchAddress("trkNormChi2", &trknormchi2);
    tree->SetBranchAddress("trkCharge", &trkcharge);
    tree->SetBranchAddress("trkNHits", &trknhits);
    tree->SetBranchAddress("trkNLayers", &trknlayer);
    tree->SetBranchAddress("trkNPixHits", &trkpixhits);
    tree->SetBranchAddress("highPurity", &highpur);
    tree->SetBranchAddress("trkPDGId",&trkpdgid);

    // gen particle quantities
    if(is_MC){
    	tree->SetBranchStatus("mult", 1);
        tree->SetBranchStatus("pt", 1);
        tree->SetBranchStatus("eta", 1);
        tree->SetBranchStatus("phi", 1);
        tree->SetBranchStatus("chg", 1);
        tree->SetBranchStatus("pdg", 1);

    	tree->SetBranchAddress("mult", &genmult);
        tree->SetBranchAddress("pt", &gen_trkpt);
        tree->SetBranchAddress("eta", &gen_trketa);
        tree->SetBranchAddress("phi", &gen_trkphi);
        tree->SetBranchAddress("chg", &gen_trkchg);
        tree->SetBranchAddress("pdg", &gen_trkpid);
    }

}
