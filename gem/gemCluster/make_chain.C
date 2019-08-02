#include <TROOT.h>
#include <TObject.h>
#include <TChain.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <iostream.h>

using namespace std;


//void make_chain(TString infile, TString outfile, TString opt) {
void make_chain() {

	ifstream listtest("gem_list.dat",ios::in);//infile
	char *spool = new char[100];
	Int_t NumFiles = -1;
	while(listtest.get()!=EOF) {
		listtest >>spool;
		cout <<"SPOOL:" <<spool <<endl;
		NumFiles++;
	}
	listtest.close();


	ifstream list("gem_list.dat",ios::in);//infile
	char **Files = new char* [NumFiles];
    for(Int_t i=0; i<NumFiles; i++) {
		Files[i]=new char[100];
		list>>Files[i];
	}
	
	
	TChain chain("gem");//opt
    for(Int_t i=0; i<NumFiles; i++) {
		chain.Add(Files[i]);
		cout <<"Add " <<Files[i] <<endl;
	}
	

	TFile outFile=TFile("gem_chain.root","RECREATE","gem_chain.root");//outfile
	chain.Write();
	outFile.Close();


	delete spool;
	delete Files;
}
