#include <iostream>
#include "TMath.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TLegend.h"
#include "TH1.h"
#include "TTree.h"
#include "TCanvas.h"

Double_t HV=0;
Double_t Amp=0;
Double_t RefAmp=0;
Double_t AmpoverRefAmp=0;
Int_t rn=0;

void gain_All(){

	const Char_t *serial[]={"DA1550", "DA1551", "DA1555", "DA1562","DA1563", "DA1564", "DA1566", "DA1574", "DA1577", "DA1580", "DA1586", "DA1587","DA1588", "DA1592","DA1593", "DA1595", "DA1596", "DA1600","DA1601", "DA1603"};
	Double_t pLevel= 98.5;
	const Int_t nfil= sizeof(serial)/sizeof(*serial);
	const Int_t nfile= nfil;

	cout <<"size of serial:"<< sizeof(serial)<<endl;
	cout <<"size of *serial:"<< sizeof(*serial)<<endl;
	cout <<"size of serial/*serial:"<< nfile <<endl;

	TCanvas *c1 = new TCanvas("c1", "Canvas1", 1400, 700);

//	TH1* hf = c1->DrawFrame(1100,85,2000,100);
	TH1* hf = c1->DrawFrame(1250,0,2150,10);//If not ZOOM
	c1->SetGridx();
	c1->SetGridy();

	//Double_t offx=0.62;
	Double_t offx=0.5;//If not Zoom
	Double_t offy=-0.5;//If not Zoom

	TLegend * leg = new TLegend(0.65-offx,-0.2-offy,1-offx,0.35-offy);
	//leg->SetNColumns(3);
	//leg->SetHeader("PMT V0A-S2R0");
	leg->SetHeader("#bf{Serial Number}","C");
	leg-> SetNColumns(2);

	const Int_t nf =0;

	hf->GetXaxis()->SetTitle("Voltage (V)");
	hf->GetYaxis()->SetTitle("Amplitude ratio (FDD/Ref.)");
	hf->SetTitle(TString::Format("PMT's"));

   TString Tserial;
   TString file;
   TTree t[nfile];

   TGraphErrors *gr[nfile];//= new TGraphErrors();
   for(Int_t j=nfile-1;j>=0;j--){
			cout<<"j->"<<j<<endl;
			gr[j]=new TGraphErrors;
		}


	TF1 *fHV[nfile];
	for(Int_t j=nfile-1;j>=0;j--){
fHV[j]	= new TF1(TString::Format("fHV%d",j), "TMath::Power(x/[0],[1])");
fHV[j]->SetParameters(1000, 7);
fHV[j]->SetParLimits(0, 100, 2000);
fHV[j]->SetParLimits(1,   1,   10);
}
   //for(Int_t j=nfile-1;j>=0;j--){
   for(Int_t j=0;j<nfile;j++){

 	Tserial=serial[j];
	file="data/PMT_SinglePh_"+Tserial+".tsv";
	cout<<"Open file: "<<file <<endl;

	t[j].ReadFile(file);
	t[j].SetBranchAddress("HV", &HV);
	t[j].SetBranchAddress("Amp", &Amp);
	t[j].SetBranchAddress("RefAmp", &RefAmp);
	//t[j].SetBranchAddress("rn", &rn);

  	Int_t n=t[j].GetEntries();

  	cout<<"====>N="<<n<<endl;
   	Int_t bColor=1;
   	for(Int_t i=0;i<n;i++){
	   	t[j].GetEntry(i);
	   	cout<<"i="<<i<<endl;
	   	cout<<"GetN->"<<gr[j]->GetN()<<"\tHV="<<HV*1000<<"\tAmp="<<Amp<<endl;
			cout<<"GetN->"<<gr[j]->GetN()<<"\tAmpoverRefAmp="<<Amp/RefAmp<<endl;
	   	gr[j]->SetPoint(i, HV*1000, Amp/RefAmp);



			gr[j]->Fit(fHV[j]);
			fHV[j]->SetLineColor(bColor+j);
			fHV[j]->Draw("PEL SAME");




	}
  	gr[j]->SetMarkerStyle(kFullCircle);
	if(j==6) bColor=kMagenta;
	if(j==9) bColor=kGreen;
  	gr[j]->SetLineColor(bColor+j);
  	gr[j]->SetMarkerColor(bColor+j);

  	gr[j]->SetLineWidth(2);
  	gr[j]->SetLineStyle(1);
	gr[j]->Draw("PE SAME");






   	 leg->AddEntry(gr[j],Form("#it{%s}",serial[j]),"pl");
   	 //leg->AddEntry(gr[j],Form("%.0f",HV),"");
   	 //leg->AddEntry(gr[j],Form("%.2f #pm %.2f",eff,err),"");

	gr[j]->Draw("SAME PE");
   }

   leg->Draw();

	//c1->SaveAs("pdf/gains.pdf");
	//c1->SaveAs("png/gains.png");
}
