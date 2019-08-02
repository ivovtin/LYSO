#include <iostream>
#include <iterator>
#include <numeric>
#include <memory>
#include <stdexcept>
#include <assert.h>
#include <algorithm>    // std::min_element, std::max_element

#include "TRint.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"

#include "GemEvent.h"

using namespace std;

class TDaqDataFile {
	std::auto_ptr<TFile> rootFile;
	TTree * pTree;
	int number;
public:
	GemEvent gem_data;
public:
	void open(const char* filename );
	inline int  next()     { return pTree->GetEvent(number++); /* Should be positive. */ }
	inline int  getCount() { return pTree->GetEntries(); }
};
//--------------------------------------------------------------
void TDaqDataFile::open(const char* filename) {
	cout << "Opening " << filename << endl;
	rootFile.reset( new TFile(filename, "READ") );
	pTree = dynamic_cast<TTree*>(rootFile->Get("daq"));
	if (!pTree) {
		throw std::runtime_error("No 'daq' tree in file");
	}
	
	pTree->SetBranchAddress("gem", &gem_data);
	cout << "with " << getCount() << " entries" << endl;
	//pTree->Print();
	number = 0;
}
//--------------------------------------------------------------

//const int selected_frame = 12;
const int selected_frame = 4;
const int threshold = 200;
const int threshold_oldgem = 5;
const double r8a_factor = 71.9;

const struct {
	int binx;
	int biny;
} THIN =  { 160, 40 };

const struct {
	int binx;
	int biny;
} THICK =  { 128, 36 };

// Layout of the GEM data in the ROOT file
struct gem_data {
	short thick [10][512];
	short thin  [4][16][640];
};

// Agregated data over run
int hitmap_thick [2][512];
int hitmap_thin  [4][16][320][2];

// Data of current event to process
int thin [4][16][2][320];
int thick[2][2][256];

struct search_result {
	int maxx; // channel number
	int maxf; // pipeline frame
	int maxval; // peak value
	int det;    // detector number
	int coor;   // coordinate direct/stereo aka strait/inclined
};

search_result do_search(int det, int coor) {
	search_result result = {-1, -1, 0, det, coor};
		
//	for (int frame=selected_frame-1; frame<=selected_frame+1; ++frame) /// 9-14
	for (int frame=1; frame<=4; ++frame) /// 9-14
	for (int i=4; i<320; ++i) {
		int value = thin[det][frame][coor][i];
		if (abs(value)>result.maxval) {
			result.maxx=i;
			result.maxf=frame;
			result.maxval = abs(value);
		}
	}
	return result;
}

struct cluster_result {
	int vertex;
	float gravity;
	int width;
	int charge;
};

cluster_result extract_cluster(search_result v) {
	cluster_result result = { v.maxx, 0., 0, 0 };
	int l, r;
	for (l=v.maxx; l>1   && thin[v.det][v.maxf][v.coor][l-1]>0; --l);
	for (r=v.maxx; r<318 && thin[v.det][v.maxf][v.coor][r+1]>0; ++r);
	
	result.width = r-l+1;
	l=v.maxx-1; r=v.maxx+1;
	for (int i=l; i<=r; ++i) {
		int value = thin[v.det][v.maxf][v.coor][i];
		result.gravity += abs(i*value);
		result.charge  += abs(value);
	}
	result.gravity /= result.charge;
	
	return result;
}

string working_file = "/home/kudryavtsev/scan_hv_thin_gems/2014-04-09_14:00.root";

void tuneStyle(TVirtualPad *c, TH2 *h) {
	h->GetXaxis()->SetNdivisions(540);
	c->SetGridx();
	c->SetGridy();
}

int main (int argc, char* argv[]) {
	if (argc>1) working_file = argv[1];
	argc=1;
	TRint app("App", &argc, argv);
	TDaqDataFile daq_file;
	assert(sizeof(gem_data)==sizeof(daq_file.gem_data.amplitudes));
	gem_data *data = reinterpret_cast<gem_data*>(daq_file.gem_data.amplitudes);
	
	daq_file.open(working_file.c_str());

	TH2 *h2xy1 = new TH2F("xy1", "XY 1st thin(160x40mm)", THIN.binx, 0, 160, THIN.biny, -20, 20);
	TH2 *h2xy2 = new TH2F("xy2", "XY 2nd thin(160x40mm)", THIN.binx, 0, 160, THIN.biny, -20, 20);
	TH2 *h2xy3 = new TH2F("xy3", "XY 3rd thin(160x40mm)", THIN.binx, 0, 160, THIN.biny, -20, 20);
	TH2 *h2xy4 = new TH2F("xy4", "XY 4th thin(160x40mm)", THIN.binx, 0, 160, THIN.biny, -20, 20);

	for (int k=0, k_end=daq_file.getCount(); k<k_end; ++k) {

	if ( daq_file.next()<=0 ) break;
		// Wipe noise in non-working area of 2nd thin gem detector
		for (int frame=0; frame<32; ++frame)
			for (int i=550; i<640; ++i) 
				data->thin[1][frame][i] = 0;
		// Unpack thin detector data into array of [detector_number 0-1][pipeline_frame 0-31][inclined/straight strips 0-1][strip number 0-319]
		for (int f=0; f<16; ++f)
		for (int i=0; i<640; ++i) {
			thin[0][f][i%2][i/2] = data->thin[0][f][i];
			thin[1][f][i%2][i/2] = data->thin[1][f][i];
			thin[2][f][i%2][i/2] = data->thin[2][f][i];
			thin[3][f][i%2][i/2] = data->thin[3][f][i];
		}
		
		search_result  result[]  = { do_search(0, 0), do_search(0, 1), do_search(1, 0), do_search(1, 1),
									 do_search(2, 0), do_search(2, 1), do_search(3, 0), do_search(3, 1)};
		int direct = result[0].maxx;
		int stereo = result[1].maxx;
		if (direct>=0 && stereo>=0 && (direct-stereo)<=40 && (direct-stereo)>=0) {
			h2xy1->Fill( (160.0-0.5*direct), (direct-stereo)-20.0 );
		}
		direct = result[2].maxx;
		stereo = result[3].maxx;
		if (direct>=0 && stereo>=0 && (direct-stereo)<=40 && (direct-stereo)>=0) {
			h2xy2->Fill( (160.0-0.5*direct), (direct-stereo)-20.0 );
		}
		// 3rd detector has strips reversed
		direct = result[5].maxx;
		stereo = result[4].maxx;
		if (direct>=0 && stereo>=0 && (direct-stereo)<=40 && (direct-stereo)>=0) {
			h2xy3->Fill( (160.0-0.5*direct), (direct-stereo)-20.0 );
		}
		direct = result[6].maxx;
		stereo = result[7].maxx;
		if (direct>=0 && stereo>=0 && (direct-stereo)<=40 && (direct-stereo)>=0) {
			h2xy4->Fill( (160.0-0.5*direct), (direct-stereo)-20.0 );
		}

	}

	TCanvas *c2 = new TCanvas("c2", "XY", 10,10, 800, 800);
	c2->Divide(1,4);
	
	c2->cd(1);  h2xy1->Draw("colz");
	c2->cd(2);  h2xy2->Draw("colz");
	c2->cd(3);  h2xy3->Draw("colz");
	c2->cd(4);  h2xy4->Draw("colz");

	tuneStyle(c2->cd(1), h2xy1);
	tuneStyle(c2->cd(2), h2xy2);
	tuneStyle(c2->cd(3), h2xy3);
	tuneStyle(c2->cd(4), h2xy4);

	gStyle->SetPalette(1);
	c2->Draw();

	app.Run();
}
