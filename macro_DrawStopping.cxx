//#include "../../CZhang/CZhangNew.h"

void macro_DrawStopping(){

	//TFile * f = new TFile("SimBeamStop_20mm.root");
	//TFile * f = new TFile("SimBeamStop_22mm.root");
	//TFile * f = new TFile("SimBeamStop_23mm.root");
	//TFile * f = new TFile("SimBeamStop_20mm_DG390.root");
	//TFile * f = new TFile("SimBeamStop_20mm_DG450.root");
	//TFile * f = new TFile("SimBeamStop_200523_H-line_DG475_TDR.root");
	//TFile * f = new TFile("SimBeamStop_200526_H-line_DG475_3321746.root");
	//TFile * f = new TFile("SimBeamStop_200526_H-line_DG475_3326844.root");
	//TFile * f = new TFile("SimBeamStop_200601_1e6_000_164994.root");
	TFile * f = new TFile("/home/had/zhangce/g-2/Stopping/SimBeamStop_200605_2e6_000_328933.root");
	//TFile * f = new TFile("SimBeamStop.root");
	TTree * t1 = (TTree*)f->Get("position");
	TTree * t2 = (TTree*)f->Get("end");

	t1->Draw("z>>h1");
	TH1F * h1 = gDirectory->Get("h1");
	t2->Draw("endPstVtxZ>>h2");
	TH1F * h2 = gDirectory->Get("h2");


	TCanvas * c = new TCanvas("c","c",1000,500);
	c->Divide(2,1);
	c->cd(1);
	h1->Draw();
	c->cd(2);
	h2->Draw();


}
