#include <string>
#include <Riostream.h>
//#include <Rtypes.h>
//#include <fstream>
//#include <iostream>
//#include <stdio.h>
//#include <time.h>
//#include <math.h>
#include <TROOT.h>
#include <TObject.h>
#include <TChain.h>
#include <TCanvas.h>
//#include <TPad.h>
#include <TString.h>
#include <TF1.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TFile.h>
//#include <TMultiGraph.h>
#include <TGraph.h>
#include <TTree.h>
#include <TROOT.h>
//using namespace std;
//std::string
//using namespace std;//
//using std::ifstream;
//using std::ios;

void make_chain(TString fname, TString chfname, TString opt)
{
//#include <iostream.h>
///	gROOT->Reset();
//    char** Files=new char* [27];
//	char **files[27];
		char*  spool=new char [100];
		Int_t NumFiles=-1;
		ifstream listtest(fname,ios::in);
    	while(listtest.get()!=EOF)
   		{
			listtest>>spool;
//            cout<<"SPOOL:"<<spool<<endl;
			NumFiles++;
		}
		listtest.close();
//		ErrMsg(trace)<<"NumFiles:"<<NumFiles<<endmsg;
		char** Files=new char* [NumFiles];
		ifstream list(fname,ios::in);
        for(Int_t i=0;i<NumFiles;i++)
		{
			Files[i]=new char [63];
			list>>Files[i];
//			ErrMsg(trace)<<"File:"<<Files[i]<<endmsg;
		}
		TChain chain(opt);
		for(Int_t i=0;i<NumFiles;i++)
		{
			chain.Add(Files[i]);
			cout << Files[i] << endl;
		}
//		delete spool;

		TFile* f=new TFile(chfname,"RECREATE",chfname);

	 chain.Write();
     f->Close();
    // return 0;
}
