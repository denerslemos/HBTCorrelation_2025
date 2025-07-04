#include "call_libraries.h"  // call libraries from ROOT and C++
#define coscut 0.99996
#define dptcut 0.04
#define pimass 0.1396

/*
Find Ntrk offline -> updated for all systems (and easy to update for future systems)
The Ntrk offline is a definition with specific cuts (we should not change it). The track systematics must be applied using the input_variables.h!
--> Arguments
size: track collection size per event
pt: track pT
eta: track eta
charge: track charge
hp: track high purity workflow
pterr: track pT uncertainty
dcaxy: track DCA in the transverse plane
dcaxyerr: track DCA in the transverse plane uncertainty
dcaz: track DCA in the longitudinal plane
dcazerr: track DCA in the longitudinal plane uncertainty
*/
int get_Ntrkoff(std::vector<float>* pt,
                std::vector<float>* eta,
                std::vector<bool>* hp,
                std::vector<float>* pterr,
                std::vector<float>* dcaxy,
                std::vector<float>* dcaxyerr,
                std::vector<float>* dcaz,
                std::vector<float>* dcazerr) {
    
    int Ntrk_off = 0;
    int size = pt->size();

    for (int ii = 0; ii < size; ++ii) {
        if (pt->at(ii) <= 0.3) continue;
        if (std::fabs(eta->at(ii)) > 2.4) continue;
        if (!hp->at(ii)) continue;
        if (pterr->at(ii) / pt->at(ii) >= 0.1) continue;
        if (std::fabs(dcaxy->at(ii) / dcaxyerr->at(ii)) >= 3.0) continue;
        if (std::fabs(dcaz->at(ii) / dcazerr->at(ii)) >= 3.0) continue;
        Ntrk_off = Ntrk_off+1;
    }

    return Ntrk_off;
}

/*
Calculate q invariant
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
cos_cut: cut used in the cosine of angle between the particles
dpt_cut: cut used in the difference in pT of the particles
*/
bool splitcomb(ROOT::Math::PtEtaPhiMVector &vec1,ROOT::Math::PtEtaPhiMVector &vec2, double cos_cut, double dpt_cut){
   bool issplit=false;
   double cosa = TMath::Abs(vec1.Px()*vec2.Px() + vec1.Py()*vec2.Py() + vec1.Pz()*vec2.Pz())/(vec1.P()*vec2.P());
   double deltapt = fabs(vec1.Pt() - vec2.Pt());
   if ( (cosa > cos_cut) && (deltapt < dpt_cut)) { issplit = true;}
   return issplit;
}

/*
Calculate q invariant
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQ(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   ROOT::Math::PtEtaPhiMVector Sum4V = p1+p2;
   Double_t q = Sum4V.M2() - 4.0*p1.mass()*p2.mass();
   return (  q > 0 ?  TMath::Sqrt(q) : -TMath::Sqrt(-q)  );
}

/*
Calculate q long in the LCMS
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQlongLCMS(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   Double_t num = 2.0*( (p1.Pz())*(p2.E()) - (p2.Pz())*(p1.E()) );
   Double_t den = TMath::Sqrt( (p1.E()+p2.E())*(p1.E()+p2.E()) - (p1.Pz()+p2.Pz())*(p1.Pz()+p2.Pz()) );
   Double_t qlongLCMS = 0.0;
   if(den != 0) qlongLCMS = fabs(num/den);
   return qlongLCMS;
}

/*
Calculate q out in the LCMS
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQout(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   TVector3 qT;
   qT.SetXYZ(p1.Px()-p2.Px(),p1.Py()-p2.Py(),0.0);
   TVector3 kT;
   kT.SetXYZ( (p1.Px()+p2.Px())/2.0 , (p1.Py()+p2.Py())/2.0 ,0.0);
   TVector3 qout;
   qout = qT.Dot(kT.Unit())*kT.Unit();
   Double_t absValue = qout.Mag();
   return absValue; 
}

/*
Calculate q side in the LCMS
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQside(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   TVector3 qT;
   qT.SetXYZ(p1.Px()-p2.Px(),p1.Py()-p2.Py(),0.0);
   TVector3 kT;
   kT.SetXYZ( (p1.Px()+p2.Px())/2.0 , (p1.Py()+p2.Py())/2.0 ,0.0);
   TVector3 qout;
   qout = qT.Dot(kT.Unit())*kT.Unit();
   TVector3 qsid;
   qsid = qT - qout;
   Double_t absValue = qsid.Mag();
   return absValue;
}

/*
Invert Px, Py and Pz for reference sample
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
ROOT::Math::PtEtaPhiMVector InvertPVector( ROOT::Math::PtEtaPhiMVector &vec){
   ROOT::Math::PtEtaPhiMVector ovec = vec;
   ovec.SetPxPyPzE(-vec.Px(),-vec.Py(),-vec.Pz(),vec.E());
   return ovec;
}

/*
Rotate X and Y for reference sample
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
ROOT::Math::PtEtaPhiMVector InvertXYVector( ROOT::Math::PtEtaPhiMVector &vec){
  ROOT::Math::PtEtaPhiMVector ovec = vec;
  ovec.SetXYZT(-vec.X(),-vec.Y(),vec.Z(),vec.T());
  return ovec;
}

/*
Return the weight factor due to Coloumb repulsion [Gamow] same charge
--> Arguments
q: q invariant
systematic: systematic number
*/
const double CoulombSS(const double &q, int systematic){
   const double alpha = 1./137.;
   double x = 2.0*TMath::Pi()*(alpha*pimass/q);
   double weight = 1.0;
   if(systematic == 9) weight = 1.15; //for syst. +15%
   if(systematic == 10) weight = 0.85; //for syst. -15%
   return weight*( (TMath::Exp(x) - 1.0)/ x - 1.0 ) + 1.0;
}

/*
Return the weight factor due to Coloumb attraction [Gamow] opposite charge
--> Arguments
q: q invariant
systematic: systematic number
*/
const double CoulombOS(const double &q, int systematic){
   const double alpha = 1./137.;
   double x = 2.0*TMath::Pi()*(alpha*pimass/q);
   double weight = 1.0;
   if(systematic == 9) weight = 1.15; //for syst. +15%
   if(systematic == 10) weight = 0.85; //for syst. -15%
   return weight*( (1.-TMath::Exp(-x)) / x - 1.0 ) + 1.0;
}

/*
Measure the 2 particle correlation
--> Arguments
tracks: vector with track informations
tracks_charge: vector with track charge informations
tracks_weight: vector with track efficiency informations
*/
void twoparticlecorrelation(std::vector<ROOT::Math::PtEtaPhiMVector> tracks, std::vector<int> tracks_charge, std::vector<double> tracks_weight, TH1D* pairmass_samesign, TH2D* costhetadpt_samesign, THnSparse* histo_2pcorr_samesign,  THnSparse* histo_2pcorr_samesign_inverted,  THnSparse* histo_2pcorr_samesign_rotated, THnSparse* histo_2pcorr_samesign3D,  THnSparse* histo_2pcorr_samesign3D_inverted,  THnSparse* histo_2pcorr_samesign3D_rotated, TH1D* pairmass_oppsign, TH2D* costhetadpt_oppsign, THnSparse* histo_2pcorr_oppsign, THnSparse* histo_2pcorr_oppsign_inverted, THnSparse* histo_2pcorr_oppsign_rotated, THnSparse* histo_2pcorr_oppsign3D, THnSparse* histo_2pcorr_oppsign3D_inverted, THnSparse* histo_2pcorr_oppsign3D_rotated, int cent, bool docostdptcut, bool do_hbt3d, bool dogamovcorrection, int systematic){
	// get correlation histograms
	for (int ipair = 0; ipair < (tracks.size()*tracks.size()); ipair++){ // start loop over tracks
	
        int a = ipair / tracks.size();
        int b = ipair % tracks.size();
        if( b <= a ) continue; // This makes it correlate like: b = a + 1;  

		double eff_trk_a = tracks_weight[a];
		double eff_trk_b = tracks_weight[b];
		double tot_eff = eff_trk_a*eff_trk_b;

		if(docostdptcut){if(splitcomb(tracks[a],tracks[b],coscut,dptcut)) continue;}

        ROOT::Math::PtEtaPhiMVector psum2 = tracks[a] + tracks[b];
        double kt = (psum2.Pt())/2.;
        ROOT::Math::PtEtaPhiMVector psum2_inverted = tracks[a] + InvertPVector(tracks[b]);
        double kt_inverted = (psum2_inverted.Pt())/2.;
        ROOT::Math::PtEtaPhiMVector psum2_rotated = tracks[a] + InvertXYVector(tracks[b]);
        double kt_rotated = (psum2_rotated.Pt())/2.;

		ROOT::Math::PtEtaPhiMVector trackb_inverted = InvertPVector(tracks[b]);
		ROOT::Math::PtEtaPhiMVector trackb_rotated = InvertXYVector(tracks[b]);

		double qinv = GetQ(tracks[a],tracks[b]);
		double qinv_inverted = GetQ(tracks[a], trackb_inverted);
		double qinv_rotated = GetQ(tracks[a], trackb_rotated);
		double x_2pc_hbt[3]={qinv, kt, (double)cent}; 
		double x_2pc_hbt_inv[3]={qinv_inverted, kt_inverted, (double)cent}; 
		double x_2pc_hbt_rot[3]={qinv_rotated, kt_rotated, (double)cent}; 

		double qlong = GetQlongLCMS(tracks[a],tracks[b]);
		double qlong_inverted = GetQlongLCMS(tracks[a], trackb_inverted);
		double qlong_rotated = GetQlongLCMS(tracks[a], trackb_rotated);
		double qout = GetQout(tracks[a],tracks[b]);
		double qout_inverted = GetQout(tracks[a], trackb_inverted);
		double qout_rotated = GetQout(tracks[a], trackb_rotated);
		double qside = GetQside(tracks[a],tracks[b]);
		double qside_inverted = GetQside(tracks[a], trackb_inverted);
		double qside_rotated = GetQside(tracks[a], trackb_rotated);
		double x_2pc_hbt_3D[5]={qlong, qout, qside, kt, (double)cent}; 
		double x_2pc_hbt_3D_inv[5]={qlong_inverted, qout_inverted, qside_inverted, kt_inverted, (double)cent}; 
		double x_2pc_hbt_3D_rot[5]={qlong_rotated, qout_rotated, qside_rotated, kt_rotated, (double)cent}; 
			
		double coulomb_ss = 1.0;
		double coulomb_os = 1.0;
		if(dogamovcorrection){
			coulomb_ss = CoulombSS(qinv,systematic);
			coulomb_os = CoulombOS(qinv,systematic);		
		}
		
		if(fabs(tracks[a].Eta() - tracks[b].Eta()) == 0 && fabs(tracks[a].Phi() - tracks[b].Phi()) == 0) continue;			
		double costheta = TMath::Abs(tracks[a].Px()*tracks[b].Px() + tracks[a].Py()*tracks[b].Py() + tracks[a].Pz()*tracks[b].Pz())/(tracks[a].P()*tracks[b].P());
   		double dpt = fabs(tracks[a].Pt() - tracks[b].Pt());
   		double pairmass = psum2.M();

		if(tracks_charge[a]*tracks_charge[b] > 0){
			pairmass_samesign->Fill(pairmass,tot_eff);
			costhetadpt_samesign->Fill(costheta,dpt,tot_eff);
			histo_2pcorr_samesign->Fill(x_2pc_hbt,coulomb_ss*tot_eff);
			histo_2pcorr_samesign_inverted->Fill(x_2pc_hbt_inv,coulomb_ss*tot_eff);
			histo_2pcorr_samesign_rotated->Fill(x_2pc_hbt_rot,coulomb_ss*tot_eff);
			if(do_hbt3d){
				histo_2pcorr_samesign3D->Fill(x_2pc_hbt_3D,coulomb_ss*tot_eff);
				histo_2pcorr_samesign3D_inverted->Fill(x_2pc_hbt_3D_inv,coulomb_ss*tot_eff);
				histo_2pcorr_samesign3D_rotated->Fill(x_2pc_hbt_3D_rot,coulomb_ss*tot_eff);
			}
		}else{
			pairmass_oppsign->Fill(pairmass,tot_eff);
			costhetadpt_oppsign->Fill(costheta,dpt,tot_eff);
			histo_2pcorr_oppsign->Fill(x_2pc_hbt,coulomb_os*tot_eff);			
			histo_2pcorr_oppsign_inverted->Fill(x_2pc_hbt_inv,coulomb_os*tot_eff);			
			histo_2pcorr_oppsign_rotated->Fill(x_2pc_hbt_rot,coulomb_os*tot_eff);			
			if(do_hbt3d){
				histo_2pcorr_oppsign3D->Fill(x_2pc_hbt_3D,coulomb_os*tot_eff);			
				histo_2pcorr_oppsign3D_inverted->Fill(x_2pc_hbt_3D_inv,coulomb_os*tot_eff);			
				histo_2pcorr_oppsign3D_rotated->Fill(x_2pc_hbt_3D_rot,coulomb_os*tot_eff);			
			}
		}
	} // 2PC loop
}
