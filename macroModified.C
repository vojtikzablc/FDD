// -*- C++ -*-
// $Id$
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

void macroModified(const Char_t* inputfile="inputSN.txt"){

  string SNumber;
  ifstream myfile (inputfile);


  if (myfile.is_open())
  {

    while ( getline (myfile,SNumber) )
    {
      if (!IsPathExist("data/"+SNumber)){
        cout << " SN does not exist" << endl;
        continue;
      }
      else{
        cout << SNumber << endl;
      }
      ofstream outfile ("results/"+SNumber+".txt");
      outfile << "HV/D:Amp/D:AmpError/D:RefAmp/D:RefAmpError/D:FDDQ/D:RefQ/D:AmpoverRefAmp/D"<< endl;
      if(outfile.is_open()){
      for (int i = 13; i < 15; i++){
        std::string voltage = std::to_string(i);

      cout << "Analysing PMT " << SNumber << " with voltage of " <<  voltage << "00v" << endl;
        string fileRoot="data/"+SNumber+"/"+voltage+"00v"+"/result.root";
      	TString aFile = fileRoot;

      	TFile *f = TFile::Open(aFile);
      	TTree *TE = (TTree*)f->Get("RawDataTree");
      	TE->Print();
      	TE->Show(10);

      		gStyle->SetOptStat("neRM");

        Double_t w = 1400;
        Double_t h = 1000;

          TE->Draw("main_FDDPMT.Amplitude>>hAmp(601,0,600)", "", "GOFF");
        TH1 *hAmp = TE->GetHistogram();
        TF1 *fFDDAmp = new TF1("fFDDAmp", "gaus", -100, 100);
        hAmp->SetTitle("Amplitude;Volts;Entries");

        	TE->Draw("(main_FDDPMT.time_end-main_FDDPMT.time_begin) >> hdT(100,0,50)", "", "GOFF");
      	TH1 *hdT = TE->GetHistogram();
      	hdT -> SetTitle("Signal width at 10% of peak;time (ns);Entries");

          TE->Draw("main_FDDPMT.Charge_GATE >> hQ(200,-50,80)", "", "GOFF");
        TH1 *hQ = TE->GetHistogram();
        TF1 *fFDDQ = new TF1("fFDDQ", "gaus", -100, 100);
        hQ->SetTitle("Signal FDD Charge;Charge;Entries");

        	TE->Draw("(main_FDDPMT.Charge_GATE):main_FDDPMT.Amplitude >> h2AmpQ(600,0,600, 130,-50,80)", "", "GOFF");
      	TH1 *h2AmpQ = TE->GetHistogram();
        h2AmpQ->SetTitle("Charge and Amplitude;Amplitude (mV);Charge");

        	TE->Draw("(main_FDDPMT.time_end-main_FDDPMT.time_begin):main_FDDPMT.Amplitude >> h2AmpdT(600,0,600, 100,0,50)", "", "GOFF");
      	TH1 *h2AmpdT = TE->GetHistogram();
        h2AmpdT->SetTitle("Width (10%) and Amplitude;Amplitude (volts);#Deltat (ns)");

        	TE->Draw("(main_FDDPMT.time_end-main_FDDPMT.time_begin):main_FDDPMT.Charge_GATE >> h2QdT(130,-50,80, 100,0,50)", "", "GOFF");
      	TH1 *h2QdT = TE->GetHistogram();
        h2QdT->SetTitle("Width (10%) and Charge;Charge;#Deltat (ns)");

       	TCanvas * c2 = new TCanvas("c2","FDD PMT Analysis", w, h);
      	c2->Divide(2,3);
      	c2->cd(1);
      	hAmp->Draw();
        hAmp->Fit(fFDDAmp);
        fFDDAmp->Draw("L SAME");
      	c2->cd(2);
      	hdT->Draw();
      	c2->cd(3);
      	c2->cd(3)->SetLogz();
      	hQ->Draw("COLZ");
        hQ->Fit(fFDDQ);
        fFDDQ->Draw("L SAME");
      	c2->cd(4);
      	h2AmpQ->Draw("COLZ");
      	c2->cd(5);
      	c2->cd(5)->SetLogz();
      	h2AmpdT->Draw("COLZ");
      	c2->cd(6);
      	h2QdT->Draw("COLZ");

/////////////////////////////

        TE->Draw("main_RefPMT.Amplitude>>hRefAmp(512,0,600)", "", "GOFF");
      TH1 *hRefAmp = TE->GetHistogram();
      TF1 *fRefAmp = new TF1("fRefAmp", "gaus", -100, 100);
      hRefAmp->SetTitle("Amplitude;Volts;Entries");

        TE->Draw("(main_RefPMT.time_end-main_RefPMT.time_begin) >> hRefdT(100,0,50)", "", "GOFF");
      TH1 *hRefdT = TE->GetHistogram();
      hRefdT -> SetTitle("Signal width at 10% of peak;time (ns);Entries");

        TE->Draw("main_RefPMT.Charge_GATE >> hRefQ(200,-50,80)", "", "GOFF");
      TH1 *hRefQ = TE->GetHistogram();
      TF1 *fRefQ = new TF1("fRefQ", "gaus", -100, 100);
      hRefQ->SetTitle("Signal FDD Charge;Charge;Entries");

        TE->Draw("(main_RefPMT.Charge_GATE):main_RefPMT.Amplitude >> h2RefAmpQ(200,0,600, 200,-50,80)", "", "GOFF");
      TH1 *h2RefAmpQ = TE->GetHistogram();
      h2RefAmpQ->SetTitle("Charge and Amplitude;Amplitude (mV);Charge");

        TE->Draw("(main_RefPMT.time_end-main_RefPMT.time_begin):main_RefPMT.Amplitude >> h2RefAmpdT(200,0,600, 200,0,50)", "", "GOFF");
      TH1 *h2RefAmpdT = TE->GetHistogram();
      h2RefAmpdT->SetTitle("Width (10%) and Amplitude;Amplitude (volts);#Deltat (ns)");

        TE->Draw("(main_RefPMT.time_end-main_RefPMT.time_begin):main_RefPMT.Charge_GATE >> h2RefQdT(200,-50,80, 200,0,50)", "", "GOFF");
      TH1 *h2RefQdT = TE->GetHistogram();
      h2RefQdT->SetTitle("Width (10%) and Charge;Charge;#Deltat (ns)");

      TCanvas * c1 = new TCanvas("c1","Ref PMT Analysis", w, h);
      c1->Divide(2,3);
      c1->cd(1);
      hRefAmp->Draw();
      hRefAmp->Fit(fRefAmp);
      fRefAmp->Draw("L SAME");
      c1->cd(2);
      hRefdT->Draw();
      c1->cd(3);
      c1->cd(3)->SetLogz();
      hRefQ->Draw("COLZ");
      hRefQ->Fit(fRefQ);
      fRefQ->Draw("L SAME");
      c1->cd(4);
      h2RefAmpQ->Draw("COLZ");
      c1->cd(5);
      c1->cd(5)->SetLogz();
      h2RefAmpdT->Draw("COLZ");
      c1->cd(6);
      h2RefQdT->Draw("COLZ");



        outfile << voltage << "00" << "\t" << hAmp->GetMean() << "\t" << hAmp->GetMeanError() << "\t" << hRefAmp->GetMean() << "\t" << hRefAmp->GetMeanError() << "\t"  << fRefAmp->GetParameter(1) << "\t" << fRefQ->GetParameter(1) << std::endl;

        }
        outfile.close();
      }
    }
    myfile.close();

  }
  else cout << "Unable to open file";

//TODO fit the charge and amplitude , add it as another column to the outfile SOMEWHAT DONE
//TODO save the plots to pdf file
//TODO add header to the outfile so it can be used in previous program (gain curve) SOMEWHAT DONE
//TODO make plots of charge resp to HV
//TODO make mean resp to HV
//need REF PM data 
// finish by next week




}
