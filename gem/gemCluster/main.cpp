#include <string>
#include <iostream>
#include <memory>

#include <unistd.h> //getopt

#include <boost/foreach.hpp>


#include <TFile.h>
#include <TTree.h>

#include <reconstruction/GEM/GemDetector.h>
#include <TObjArray.h>
#include <TClonesArray.h>



using namespace std;
using namespace gem;

int main(int argc, char * argv[]) {
	int opt;
	bool inplace = false;
	string outputFilename, inputFilename = "daq.root";
	while((opt = getopt(argc, argv, "ho:ai:"))!=-1) {
		switch(opt) {
			case 'o': outputFilename = optarg; break;
			case 'a': inplace = true; break;
			case 'i': inputFilename = optarg; break;
			default:
				cout <<
				"Reconstructs GEM clusters\n" <<
				" -i filename - read input from ROOT file\n" <<
				" -a          - add output to existing ROOT tree\n" <<
				" -o filename - write output to file (conflicts with -a), file is overwritten\n";
				return 2;
		}
	}
	if (outputFilename.size() && inplace) {
		cerr << "-a and -o options can't be used together." << endl;
		return 1;
	}

	if (!inplace && !outputFilename.size()) {
		cerr << "Either -o or -a option should be set " << endl;
		return 1;
	}
	string openMode = "READ";
	if (inplace)
		openMode = "UPDATE";
	TFile input(inputFilename.c_str(), openMode.c_str());

	if (!input.IsOpen()) {
		cerr << "Failed to open " << inputFilename << endl;
		return 2;
	}

	TTree * pTree = dynamic_cast<TTree*>(input.Get("daq"));
	if (!pTree) {
		cerr << "No daq tree in file " << inputFilename << endl;
		return 2;
	}

	GemEvent data;

	if (pTree->SetBranchAddress("gem",&data) != 0) {
		cerr << "Gem branch is absent in the input or has wrong type" << endl;
		return 3;
	}
	pTree->SetBranchStatus("*", 0);
	pTree->SetBranchStatus("gem", 1);
	Long64_t entries = pTree->GetEntries();

        cout << "Reconstruct GEM clusters in file " << inputFilename << " and\n store in "
             << outputFilename << "\n";

	Detector detectors[] = {
		gem::Detector(0),
		gem::Detector(1),
		gem::Detector(2),
		gem::Detector(3)};

	TClonesArray allClusters("GemCluster");
	TClonesArray detClusters("GemCluster",4);
	allClusters.SetOwner(kTRUE);
	detClusters.SetOwner(kTRUE);
	TBranch *allClustersBranch = 0, *detClustersBranch = 0;

	auto_ptr<TFile> output;
	TTree * outTree = pTree;
	if (!inplace) {
		output.reset(new TFile(outputFilename.c_str(), "RECREATE"));
		if (!output->IsOpen()) {
			cerr << "Failed to open " << outputFilename << " for writing" << endl;
			return 2;
		}
		outTree = new TTree("gem", "Local 2D gem clusters.");
	}
	allClustersBranch = outTree->Branch("allClusters", &allClusters);
	detClustersBranch = outTree->Branch("detClusters", &detClusters);


	vector<LinearCluster> unbound;
	for (Long64_t eventIdx = 0; eventIdx < entries; ++eventIdx) {
		pTree->GetEvent(eventIdx);
		allClusters.Clear();
		detClusters.Clear();
		unsigned i=0;
		BOOST_FOREACH(Detector d, detectors) {
			vector<PlaneCluster> planeClusters = d.buildLocalClusters(data, unbound);
			GemCluster gemCluster;
			gemCluster.detector = d.index();
			if( !planeClusters.empty() )
				planeClusters[0].save(gemCluster);
			new (detClusters[d.index()]) GemCluster(gemCluster);
			if( allClusters.GetSize()<=i+planeClusters.size() ) allClusters.Expand(2*(i+planeClusters.size()));
			BOOST_FOREACH(PlaneCluster pCluster, planeClusters) {
				pCluster.save(gemCluster);
				assert(gemCluster.isValid());
				new (allClusters[i++]) GemCluster(gemCluster);
			}
		}
		//cout << "Number of clusters: " << allClusters.GetEntries() << "\n";
		allClustersBranch->Fill();
		detClustersBranch->Fill();
	}
	if (allClustersBranch->GetEntries() != entries || detClustersBranch->GetEntries() != entries) {
		cerr << "Wrong count of branch entries" << endl;
		return 5;
	}
	outTree->SetEntries(entries);
	outTree->Write();

        cout << entries << " entries processed\n";

	return 0;
}
