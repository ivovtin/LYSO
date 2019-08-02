#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>

#include <unistd.h> //getopt, access

#include <boost/foreach.hpp>

#include <TROOT.h>
#include <TMath.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TChain.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TEntryList.h>
#include <TClonesArray.h>
#include <TApplication.h>
#include <TString.h>
#include <TLatex.h>
#include <TSystem.h>
#include <TVector3.h>
#include <TProfile.h>
//#include <minuit.h>
#include <TMinuit.h>

#include "TreeFormats/GemEvent.h"
#include "reconstruction/GEM/GemDetector.h"
#include "reconstruction/GEM/GemGeometry.h"

using namespace std;

static const char* gemcfgfn = "gem_geom_config.dat";
struct trackinfo {
    float x0,y0,z0;
    float vx,vy,vz;
    float xs,ys,zs;
    float chi2;
    float prob;
    Bool_t trackis;
};
static const char * obranchspec="x0/F:y0:z0:vx:vy:vz:xs:ys:zs:chi2:prob:trackis/O";

//static double sigmax2[4] = { pow(0.07,2)+pow(0.0015,2), pow(0.2,2)+pow(1.512,2),  pow(0.07,2)+pow(1.12,2), pow(0.07,2)+pow(6.2,2)};
//static double sigmay2[4] = { pow(0.2,2)+pow(0.0015,2), pow(0.07,2)+pow(1.512,2), pow(0.2,2)+pow(1.12,2),  pow(0.2,2)+pow(6.2,2)};
static double sigmax2[4] = { pow(0.07,2), pow(0.2,2),  pow(0.07,2), pow(0.07,2)};
static double sigmay2[4] = { pow(0.2,2), pow(0.07,2), pow(0.2,2),  pow(0.2,2)};
static Bool_t usegem[4]={kTRUE, kTRUE, kTRUE, kFALSE}; //use these detectors to build tracks
static double Z0 = 0., Zs = 2260.0;

static const char* progname = "trackFinder";

static void Usage()
{
    cout << "Usage: " << progname << " [options] infile.root\n"
        "Find track and fill track TTree (" << obranchspec << ")\n"
        " -g file   - read GEM geometry configuation from file, default " << gemcfgfn << "\n"
        " -o file   - output ROOT file name, default track_<infile>.root\n"
        " -z Z      - Z-position of SiPM plane w.r.t the GEM reference system, mm\n"
        " -h        - show this help" << endl;
}

//==================================================================================================================

int main(int argc, char* argv[])
{
    progname = argv[0];
    int opt;

    if( argc<2 ) {
        Usage();
        return 0;
    }

    string outfn;

    while((opt = getopt(argc, argv, "hg:o:z:"))!=-1) {
        switch(opt) {
        case 'g': gemcfgfn = optarg; break;
        case 'o': outfn = optarg; break;
        case 'z': Zs = atof(optarg); break;
        case 'h': Usage(); return 0;
        default: Usage(); return 1;
        }
    }

    gem::Geometry geom;
    geom.readConfiguration(gemcfgfn);
    if( !geom.numdet() )
        return 1;

    Int_t nusegems = 0;
    double S_sx = 0., S_sy = 0., S_zx = 0., S_zy = 0., S_z2x = 0., S_z2y = 0.;
    for(int i=0; i<4; i++) {
        if( !usegem[i] ) continue;
        nusegems++;
        S_sx += 1/sigmax2[i];
        S_sy += 1/sigmay2[i];
        S_zx += (geom.z(i)-Z0) / sigmax2[i];
        S_zy += (geom.z(i)-Z0) / sigmay2[i];
        S_z2x += pow((geom.z(i)-Z0), 2) / sigmax2[i];
        S_z2y += pow((geom.z(i)-Z0), 2) / sigmay2[i];
    }

    string fn = argv[optind];
    TFile fin(fn.c_str(),"READ");
    if( !fin.IsOpen() ) {
        cerr << "Can not open input file " << fn << endl;
        return 1;
    }
    TTree* tree;
    fin.GetObject("gem",tree);
    if( !tree ) {
        cerr << "No 'gem' tree is found in " << fn << endl;
        return 2;
    }

    Long_t nentries = tree->GetEntries();
    cout << "Opened " << fn << " with GEM tree of " << nentries << " entries" << endl;
    if( !nentries ) return 2;

    TClonesArray *clusters2d = 0;
    if(tree->SetBranchAddress("detClusters", &clusters2d) != 0) {
        cerr << "'gem' branch is absent or has wrong type" << endl;
        return 2;
    }

    // -------------- Declaration of output TTree and TFile ----------------//
    if( outfn.empty() ) {
        outfn = fn;
        size_t pos = outfn.find_last_of('/');
        if( pos==string::npos )
            pos = 0;
        else
            pos++;
        outfn.insert(pos,"track_");
    }
    TFile fout(outfn.c_str(),"RECREATE","track");
    TTree* ttrack=new TTree("track","Track info");
    struct trackinfo track;
    ttrack->Branch("track",&track,obranchspec);

    TVector3 clusterCoords[4];
    //    cout<<"control 1 Nentries=" << nentries << endl;
    cout.precision(4);
    for(Long64_t entry = 0; entry < nentries; ++entry) {
        tree->GetEntry(entry);
        assert(clusters2d);
        assert(clusters2d->GetEntries()==4);

        if( entry%1000==0 )
            cout << setw(6) << entry << "\r" << flush;

        memset(&track,0,sizeof(track));
        Int_t ndet = 0;
        double S_x=0, S_y=0, S_xz=0, S_yz=0, kx=0, ky=0, X0=0, Y0=0;
        for (size_t i = 0; i < 4; ++i) {
            const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
            assert(cluster.detector == i);
            if( !usegem[i] ) continue;
            if( cluster.dx<=0 || cluster.dy<=0 ) continue;
            ndet++;
            clusterCoords[i].SetXYZ(cluster.x, cluster.y, 0.);
            geom.transformToLabFrame(cluster.detector, clusterCoords[i]);
            S_xz += clusterCoords[i].X() * (clusterCoords[i].Z()-Z0) / sigmax2[i];
            S_yz += clusterCoords[i].Y() * (clusterCoords[i].Z()-Z0) / sigmay2[i];
            S_x += clusterCoords[i].X() / sigmax2[i];
            S_y += clusterCoords[i].Y() / sigmay2[i];
        }
        if( ndet==nusegems ) {
            track.trackis = kTRUE;
            kx = (S_sx * S_xz - S_x * S_zx) / (S_sx * S_z2x - S_zx * S_zx);
            ky = (S_sy * S_yz - S_y * S_zy) / (S_sy * S_z2y - S_zy * S_zy);
            X0 = (S_x - kx * S_zx)/S_sx;
            Y0 = (S_y - ky * S_zy)/S_sy;
            Float_t chi2=0;
            for (int i=0; i < 4; i++){
                if( !usegem[i] ) continue;
                chi2+=pow((clusterCoords[i].X()-X0-kx*(clusterCoords[i].Z()-Z0)), 2)/sigmax2[i] + pow((clusterCoords[i].Y()-Y0-ky*(clusterCoords[i].Z()-Z0)), 2)/sigmay2[i];
            }
            // ----------------- Fill struct trackinfo ------------------ //
            track.x0=X0; track.y0=Y0; track.z0=Z0;
            track.vx=kx/sqrt(pow(kx,2)+pow(ky,2)+1);
            track.vy=ky/sqrt(pow(kx,2)+pow(ky,2)+1);
            track.vz=1./sqrt(pow(kx,2)+pow(ky,2)+1);
            track.zs=Zs;
            track.xs=kx*(Zs-Z0)+X0;
            track.ys=ky*(Zs-Z0)+Y0;
            track.chi2=chi2;
            track.prob=TMath::Prob(chi2,ndet*2-4);
        }
#ifdef DEBUG
        if( track.trackis )
            cout<<"(X0,Y0,Z0)=("<<track.x0<<","<<track.y0<<","<<track.z0<<") (Vx,Vy,Vz)=("<<track.vx<<","<<track.vy<<","<<track.vz<<") chi^2/ndf="<<track.chi2/(ndet*2-4)<<endl;
        else
            cout<<"No track: "<<ndet<<" clusters"<<endl;
#endif
        ttrack->Fill();
        //        Float_t Xs=kx*(Zs-Z0)+X0;
        //        Float_t Ys=ky*(Zs-Z0)+Y0;
        //        cout<<"Xs-track.xs="<<Xs<<"-"<<track.xs<<"\tYs-track.ys="<<Ys<<"-"<<track.ys<<endl;
    }
    ttrack->Write();
    fout.Close();

    cout << "Written track tree to " << outfn << endl;

    return 0;

}
