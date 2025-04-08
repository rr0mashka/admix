{
TFile *f1 = new TFile("/home/yana/Geant4/geant4-projects/CellsPhantom/build/Run_Run_output.root");
TFile* fout = new TFile("/home/yana/Geant4/geant4-projects/CellsPhantom/BNCT/AnalysisTools/HarverstHistos.root", "RECREATE");


  gROOT->Reset();
  //gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadTopMargin(0.07);//!!
  gStyle->SetPadBottomMargin(0.12);//!!

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  //gPad->SetLogy();
  //TGaxis::SetMaxDigits(3);
  double TextSizeScale = 1.0;

  struct GraphAttr {
  	int colour;
  	int marker;
  	float width;
  	float size;
    int LineStyle;
  	char  ModelName[100];
  	char option[20];
  };

gStyle->SetLineStyleString(11,"32 18");

//========= drawing options for each field======================================
  GraphAttr ModelAttr[] = {
  	{ 4, 21, 4.0, 1.2, 1, "#gamma, 6MV", "HIST" },
  	{ 6, 33, 4.0, 1.5, 2,"p, distal side", "HIST same" },
  	{ 3, 34, 4.0, 1.5, 8, "p, frontal side", "HIST same" },
    { 44, 34, 4.0, 1.5, 11, "p, frontal side", "HIST same" },
  };
//==============================================================================


  vector<int> PIDofInterest;
  PIDofInterest.push_back(2212);   //proton
  PIDofInterest.push_back(2112);   //neutron
  PIDofInterest.push_back(11);     //electron
  PIDofInterest.push_back(22);     //gamma
  PIDofInterest.push_back(1000020040);  //alpha
  //1000020040//alfa

//==============================================================================

   TTree *t1 = (TTree*)f1->Get("Doses in phantom");
   //TTree *t1 = (TTree*)f1->Get("Fluences");
   double X, Y, Z, Edep, dose;
   //int particle_id;
   //char particle_name[50];
   //int Event;

   t1->SetBranchAddress("X",&X);
   t1->SetBranchAddress("Y",&Y);
   t1->SetBranchAddress("Z",&Z);
   //t1->SetBranchAddress("Edep",&Edep);
   t1->SetBranchAddress("dose",&dose);

   TH2F  *hHead2Ds1  = new TH2F("hHead2Ds1","Dose slice 1",25,-75, 75, 25,-75, 75);
   TH2F  *hHead2Ds2  = new TH2F("hHead2Ds2","Dose slice 2",25,-75, 75, 25,-75, 75);
   TH2F  *hHead2Ds3  = new TH2F("hHead2Ds3","Dose slice 3",25,-75, 75, 25,-75, 75);
   TH2F  *hHead2Ds4  = new TH2F("hHead2Ds4","Dose slice 4",25,-75, 75, 25,-75, 75);
   TH2F  *hHead2Ds5  = new TH2F("hHead2Ds5","Dose slice 5",25,-75, 75, 25,-75, 75);
   TH2F  *hHead2Ds6  = new TH2F("hHead2Ds6","Dose slice 6",25,-75, 75, 25,-75, 75);

   int nentries = (int)t1->GetEntries();
  for (int i=0; i<nentries; i++) {
     t1 -> GetEntry(i);

      if(abs(X)<75 && abs(Y)<75){
          if (Z>130&&Z<141){
             hHead2Ds1 -> Fill(X,Y, dose);
          }
          if (Z>140&&Z<151){
             hHead2Ds2 -> Fill(X,Y, dose);
          }
          if (Z>150&&Z<161){
             hHead2Ds3 -> Fill(X,Y, dose);
          }
          if (Z>160&&Z<171){
             hHead2Ds4 -> Fill(X,Y, dose);
          }
          if (Z>170&&Z<181){
             hHead2Ds5 -> Fill(X,Y, dose);
          }
          if (Z>180&&Z<191){
             hHead2Ds6 -> Fill(X,Y, dose);
          }
         }
       }

//====================  Making plots   =========================================


TCanvas *x_1 = new TCanvas("x_1", "x_1", 960, 720);

hHead2Ds1->GetYaxis()->SetTickLength(0.02);
hHead2Ds1->GetYaxis()->SetNdivisions(505);
hHead2Ds1->GetXaxis()->CenterTitle();
hHead2Ds1->GetYaxis()->CenterTitle();
hHead2Ds1->GetYaxis()->SetTitle("y (mm)");
hHead2Ds1->GetXaxis()->SetTitle("x (mm)");
hHead2Ds1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds1->GetYaxis()->SetTitleOffset(1.2);
hHead2Ds1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds1->GetXaxis()->SetTitleOffset(1.0);
hHead2Ds1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds1->GetYaxis()->SetLabelFont(42);
hHead2Ds1->GetYaxis()->SetTitleFont(42);
hHead2Ds1->GetXaxis()->SetTitleFont(42);
hHead2Ds1->GetZaxis()->SetRangeUser(0, 0.1e-9);
hHead2Ds1->Draw("colz");

TCanvas *x_2 = new TCanvas("x_2", "x_2", 960, 720);

hHead2Ds2->GetYaxis()->SetTickLength(0.02);
hHead2Ds2->GetYaxis()->SetNdivisions(505);
hHead2Ds2->GetXaxis()->CenterTitle();
hHead2Ds2->GetYaxis()->CenterTitle();
hHead2Ds2->GetYaxis()->SetTitle("y (mm)");
hHead2Ds2->GetXaxis()->SetTitle("x (mm)");
hHead2Ds2->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds2->GetYaxis()->SetTitleOffset(1.2);
hHead2Ds2->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds2->GetXaxis()->SetTitleOffset(1.0);
hHead2Ds2->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds2->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds2->GetYaxis()->SetLabelFont(42);
hHead2Ds2->GetYaxis()->SetTitleFont(42);
hHead2Ds2->GetXaxis()->SetTitleFont(42);
hHead2Ds2->GetZaxis()->SetRangeUser(0, 0.1e-9);
hHead2Ds2->Draw("colz");

TCanvas *x_3 = new TCanvas("x_3", "x_3", 960, 720);

hHead2Ds3->GetYaxis()->SetTickLength(0.02);
hHead2Ds3->GetYaxis()->SetNdivisions(505);
hHead2Ds3->GetXaxis()->CenterTitle();
hHead2Ds3->GetYaxis()->CenterTitle();
hHead2Ds3->GetYaxis()->SetTitle("y (mm)");
hHead2Ds3->GetXaxis()->SetTitle("x (mm)");
hHead2Ds3->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds3->GetYaxis()->SetTitleOffset(1.2);
hHead2Ds3->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds3->GetXaxis()->SetTitleOffset(1.0);
hHead2Ds3->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds3->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds3->GetYaxis()->SetLabelFont(42);
hHead2Ds3->GetYaxis()->SetTitleFont(42);
hHead2Ds3->GetXaxis()->SetTitleFont(42);
hHead2Ds3->GetZaxis()->SetRangeUser(0, 0.1e-9);
hHead2Ds3->Draw("colz");

TCanvas *x_4 = new TCanvas("x_4", "x_4", 960, 720);

hHead2Ds4->GetYaxis()->SetTickLength(0.02);
hHead2Ds4->GetYaxis()->SetNdivisions(505);
hHead2Ds4->GetXaxis()->CenterTitle();
hHead2Ds4->GetYaxis()->CenterTitle();
hHead2Ds4->GetYaxis()->SetTitle("y (mm)");
hHead2Ds4->GetXaxis()->SetTitle("x (mm)");
hHead2Ds4->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds4->GetYaxis()->SetTitleOffset(1.2);
hHead2Ds4->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds4->GetXaxis()->SetTitleOffset(1.0);
hHead2Ds4->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds4->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds4->GetYaxis()->SetLabelFont(42);
hHead2Ds4->GetYaxis()->SetTitleFont(42);
hHead2Ds4->GetXaxis()->SetTitleFont(42);
hHead2Ds4->GetZaxis()->SetRangeUser(0, 0.1e-9);
hHead2Ds4->Draw("colz");

TCanvas *x_5 = new TCanvas("x_5", "x_5", 960, 720);

hHead2Ds5->GetYaxis()->SetTickLength(0.02);
hHead2Ds5->GetYaxis()->SetNdivisions(505);
hHead2Ds5->GetXaxis()->CenterTitle();
hHead2Ds5->GetYaxis()->CenterTitle();
hHead2Ds5->GetYaxis()->SetTitle("y (mm)");
hHead2Ds5->GetXaxis()->SetTitle("x (mm)");
hHead2Ds5->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds5->GetYaxis()->SetTitleOffset(1.2);
hHead2Ds5->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds5->GetXaxis()->SetTitleOffset(1.0);
hHead2Ds5->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds5->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds5->GetYaxis()->SetLabelFont(42);
hHead2Ds5->GetYaxis()->SetTitleFont(42);
hHead2Ds5->GetXaxis()->SetTitleFont(42);
hHead2Ds5->GetZaxis()->SetRangeUser(0, 0.1e-9);
hHead2Ds5->Draw("colz");

TCanvas *x_6 = new TCanvas("x_6", "x_6", 960, 720);

hHead2Ds6->GetYaxis()->SetTickLength(0.02);
hHead2Ds6->GetYaxis()->SetNdivisions(505);
hHead2Ds6->GetXaxis()->CenterTitle();
hHead2Ds6->GetYaxis()->CenterTitle();
hHead2Ds6->GetYaxis()->SetTitle("y (mm)");
hHead2Ds6->GetXaxis()->SetTitle("x (mm)");
hHead2Ds6->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds6->GetYaxis()->SetTitleOffset(1.2);
hHead2Ds6->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hHead2Ds6->GetXaxis()->SetTitleOffset(1.0);
hHead2Ds6->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds6->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hHead2Ds6->GetYaxis()->SetLabelFont(42);
hHead2Ds6->GetYaxis()->SetTitleFont(42);
hHead2Ds6->GetXaxis()->SetTitleFont(42);
hHead2Ds6->GetZaxis()->SetRangeUser(0, 0.1e-9);
hHead2Ds6->Draw("colz");

TCanvas *y = new TCanvas("y", "y", 1100, 720);
y->Divide(3,2);
y->cd(1);
hHead2Ds1->Draw();
y->cd(2);
hHead2Ds2->Draw();
y->cd(3);
hHead2Ds3->Draw();
y->cd(4);
hHead2Ds4->Draw();
y->cd(5);
hHead2Ds5->Draw();
y->cd(6);
hHead2Ds6->Draw();

fout->WriteObject(hHead2Ds1, "head slice 1");
fout->WriteObject(hHead2Ds2, "head slice 2");
fout->WriteObject(hHead2Ds3, "head slice 3");
fout->WriteObject(hHead2Ds4, "head slice 4");
fout->WriteObject(hHead2Ds5, "head slice 5");
fout->WriteObject(hHead2Ds6, "head slice 6");

//fout->Close();

}
