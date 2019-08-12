void result()
{ 
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);
  gROOT->SetStyle("Plain");
  gStyle -> SetPalette(1);
  TCanvas *c = new TCanvas("c","c",1400, 800);
  c->Divide(2,4);

  int y1, y2;
  Double_t var1, var2, var3, var4;
  std::vector< float > Mean, Mean_Error, Sigma, Sigma_Error, Ycoor, Ycoor_Error;
 
  for( int i=0; i<=12; i+=3 )
  {
      y1=14+i;
      y2=17+i;
      ifstream res(TString::Format("/home/ovtin/development/LYSO/out/lysozero_res_%d-%d.dat",y1,y2).Data());
      string s;
      while( getline(res, s) )
      {
        istringstream iss(s);
        iss >> var1 >> var2 >> var3 >> var4;
        cout << var1 <<"\t"<< var2 <<"\t"<< var3 <<"\t"<< var4 <<endl;
        Mean.push_back(var1);
        Mean_Error.push_back(var2);
        Sigma.push_back(var3);
        Sigma_Error.push_back(var4);
        Ycoor.push_back((y1+y2)/2.);
        Ycoor_Error.push_back(0.);
      }
      res.close();

  }

  c->cd(1);
  TGraphErrors* gr1=new TGraphErrors(Ycoor.size(),&Ycoor[0],&Sigma[0],&Ycoor_Error[0],&Sigma_Error[0]);
  gr1->SetMarkerStyle(21);
  gr1->SetMarkerSize(0.8);
  gr1->SetMarkerColor(3);
  gr1->SetLineWidth(2);
  gr1->SetLineColor(3);
  gr1->SetTitle("lysozero");
  gr1->SetName("lysozero");
  gr1->GetXaxis()->SetTitle("Y, mm");
  gr1->GetYaxis()->SetTitle("#sigma, ns");
  gr1->Draw("apl");

  c->cd(2);
  TGraphErrors* gr1Mean=new TGraphErrors(Ycoor.size(),&Ycoor[0],&Mean[0],&Ycoor_Error[0],&Mean_Error[0]);
  gr1Mean->SetMarkerStyle(21);
  gr1Mean->SetMarkerSize(0.8);
  gr1Mean->SetMarkerColor(3);
  gr1Mean->SetLineWidth(2);
  gr1Mean->SetLineColor(3);
  gr1Mean->SetTitle("lysozero");
  gr1Mean->SetName("lysozero");
  gr1Mean->GetXaxis()->SetTitle("Y, mm");
  gr1Mean->GetYaxis()->SetTitle("Mean, ns");
  gr1Mean->Draw("apl");

  Ycoor.clear(); 
  Ycoor_Error.clear(); 
  Sigma.clear(); 
  Sigma_Error.clear(); 
  Mean.clear(); 
  Mean_Error.clear(); 
  //vector becomes empty 
  cout<<" "<<endl;

  for( int i=0; i<=12; i+=3 )
  {
      y1=14+i;
      y2=17+i;
      ifstream res(TString::Format("/home/ovtin/development/LYSO/out/lyso30plus_res_%d-%d.dat",y1,y2).Data());
      string s;
      while( getline(res, s) )
      {
        istringstream iss(s);
        iss >> var1 >> var2 >> var3 >> var4;
        cout << var1 <<"\t"<< var2 <<"\t"<< var3 <<"\t"<< var4 <<endl;
        Mean.push_back(var1);
        Mean_Error.push_back(var2);
        Sigma.push_back(var3);
        Sigma_Error.push_back(var4);
        Ycoor.push_back((y1+y2)/2.);
        Ycoor_Error.push_back(0.);
      }
      res.close();

  }

  c->cd(3);
  TGraphErrors* gr2=new TGraphErrors(Ycoor.size(),&Ycoor[0],&Sigma[0],&Ycoor_Error[0],&Sigma_Error[0]);
  gr2->SetMarkerStyle(21);
  gr2->SetMarkerSize(0.8);
  gr2->SetMarkerColor(3);
  gr2->SetLineWidth(2);
  gr2->SetLineColor(3);
  gr2->SetTitle("lyso30plus");
  gr2->SetName("lyso30plus");
  gr2->GetXaxis()->SetTitle("Y, mm");
  gr2->GetYaxis()->SetTitle("#sigma, ns");
  gr2->Draw("apl");

  c->cd(4);
  TGraphErrors* gr2Mean=new TGraphErrors(Ycoor.size(),&Ycoor[0],&Mean[0],&Ycoor_Error[0],&Mean_Error[0]);
  gr2Mean->SetMarkerStyle(21);
  gr2Mean->SetMarkerSize(0.8);
  gr2Mean->SetMarkerColor(3);
  gr2Mean->SetLineWidth(2);
  gr2Mean->SetLineColor(3);
  gr2Mean->SetTitle("lyso30plus");
  gr2Mean->SetName("lyso30plus");
  gr2Mean->GetXaxis()->SetTitle("Y, mm");
  gr2Mean->GetYaxis()->SetTitle("Mean, ns");
  gr2Mean->Draw("apl");

  Ycoor.clear(); 
  Ycoor_Error.clear(); 
  Sigma.clear(); 
  Sigma_Error.clear(); 
  Mean.clear(); 
  Mean_Error.clear(); 
  //vector becomes empty 
  cout<<" "<<endl;

  for( int i=0; i<=12; i+=3 )
  {
      y1=14+i;
      y2=17+i;
      ifstream res(TString::Format("/home/ovtin/development/LYSO/out/lyso30minus_res_%d-%d.dat",y1,y2).Data());
      string s;
      while( getline(res, s) )
      {
        istringstream iss(s);
        iss >> var1 >> var2 >> var3 >> var4;
        cout << var1 <<"\t"<< var2 <<"\t"<< var3 <<"\t"<< var4 <<endl;
        Mean.push_back(var1);
        Mean_Error.push_back(var2);
        Sigma.push_back(var3);
        Sigma_Error.push_back(var4);
        Ycoor.push_back((y1+y2)/2.);
        Ycoor_Error.push_back(0.);
      }
      res.close();

  }

  c->cd(5);
  TGraphErrors* gr3=new TGraphErrors(Ycoor.size(),&Ycoor[0],&Sigma[0],&Ycoor_Error[0],&Sigma_Error[0]);
  gr3->SetMarkerStyle(21);
  gr3->SetMarkerSize(0.8);
  gr3->SetMarkerColor(3);
  gr3->SetLineWidth(2);
  gr3->SetLineColor(3);
  gr3->SetTitle("lyso30minus");
  gr3->SetName("lyso30minus");
  gr3->GetXaxis()->SetTitle("Y, mm");
  gr3->GetYaxis()->SetTitle("#sigma, ns");
  gr3->Draw("apl");

  c->cd(6);
  TGraphErrors* gr3Mean=new TGraphErrors(Ycoor.size(),&Ycoor[0],&Mean[0],&Ycoor_Error[0],&Mean_Error[0]);
  gr3Mean->SetMarkerStyle(21);
  gr3Mean->SetMarkerSize(0.8);
  gr3Mean->SetMarkerColor(3);
  gr3Mean->SetLineWidth(2);
  gr3Mean->SetLineColor(3);
  gr3Mean->SetTitle("lyso30minus");
  gr3Mean->SetName("lyso30minus");
  gr3Mean->GetXaxis()->SetTitle("Y, mm");
  gr3Mean->GetYaxis()->SetTitle("Mean, ns");
  gr3Mean->Draw("apl");

  Ycoor.clear(); 
  Ycoor_Error.clear(); 
  Sigma.clear(); 
  Sigma_Error.clear(); 
  Mean.clear(); 
  Mean_Error.clear();  

//==========================================================================================================
  float angle;
  std::vector< float > vecangle;
  Int_t k=1;
  for( int i=0; i<=12; i+=3 )
  {
      k++;
      cout<<k<<endl;
      y1=14+i;
      y2=17+i;
      ifstream res1(TString::Format("/home/ovtin/development/LYSO/out/lyso30minus_res_%d-%d.dat",y1,y2).Data());
      string s1;
      while( getline(res1, s1) )
      {
        istringstream iss(s1);
        iss >> var1 >> var2 >> var3 >> var4;
        Mean.push_back(var1);
        Mean_Error.push_back(var2);
        Sigma.push_back(var3);
        Sigma_Error.push_back(var4);
        Ycoor.push_back((y1+y2)/2.);
        Ycoor_Error.push_back(0.);
        angle=-30;
        vecangle.push_back(angle); 
     }
      res1.close();

      ifstream res2(TString::Format("/home/ovtin/development/LYSO/out/lysozero_res_%d-%d.dat",y1,y2).Data());
      string s2;
      while( getline(res2, s2) )
      {
        istringstream iss(s2);
        iss >> var1 >> var2 >> var3 >> var4;
        Mean.push_back(var1);
        Mean_Error.push_back(var2);
        Sigma.push_back(var3);
        Sigma_Error.push_back(var4);
        Ycoor.push_back((y1+y2)/2.);
        Ycoor_Error.push_back(0.);
        angle=0;
        vecangle.push_back(angle); 
      }
      res2.close();

      ifstream res3(TString::Format("/home/ovtin/development/LYSO/out/lyso30plus_res_%d-%d.dat",y1,y2).Data());
      string s3;
      while( getline(res3, s3) )
      {
        istringstream iss(s3);
        iss >> var1 >> var2 >> var3 >> var4;
        Mean.push_back(var1);
        Mean_Error.push_back(var2);
        Sigma.push_back(var3);
        Sigma_Error.push_back(var4);
        Ycoor.push_back((y1+y2)/2.);
        Ycoor_Error.push_back(0.);
        angle=30;
        vecangle.push_back(angle); 
      }
      res3.close();

      c->cd(7);
      TGraphErrors* gr4=new TGraphErrors(vecangle.size(),&vecangle[0],&Sigma[0],0,&Sigma_Error[0]);
      gr4->SetMarkerStyle(21);
      gr4->SetMarkerSize(0.8);
      gr4->SetMarkerColor(k);
      gr4->SetLineWidth(2);
      gr4->SetLineColor(k);
      gr4->SetTitle("Total");
      gr4->SetName("Total");
      gr4->GetXaxis()->SetTitle("Angle");
      gr4->GetYaxis()->SetTitle("#sigma, ns");
      gr4->GetYaxis()->SetRangeUser(0., 1.2);
      if(i==0) gr4->Draw("APL sames");
      gr4->Draw("PL sames");

      c->cd(8);
      TGraphErrors* gr4Mean=new TGraphErrors(vecangle.size(),&vecangle[0],&Mean[0],0,&Mean_Error[0]);
      gr4Mean->SetMarkerStyle(21);
      gr4Mean->SetMarkerSize(0.8);
      gr4Mean->SetMarkerColor(k);
      gr4Mean->SetLineWidth(2);
      gr4Mean->SetLineColor(k);
      gr4Mean->SetTitle("Total");
      gr4Mean->SetName("Total");
      gr4Mean->GetXaxis()->SetTitle("Angle");
      gr4Mean->GetYaxis()->SetTitle("Mean, ns");
      gr4Mean->GetYaxis()->SetRangeUser(4., 10);
      if(i==0) gr4Mean->Draw("APL sames");
      gr4Mean->Draw("PL sames");
 
      Ycoor.clear(); 
      Ycoor_Error.clear(); 
      Sigma.clear(); 
      Sigma_Error.clear(); 
      Mean.clear(); 
      Mean_Error.clear();  
      vecangle.clear();     
      c->Modified();
      c->Update();
  }

}
