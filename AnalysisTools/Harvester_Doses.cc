{
TFile *f1 = new TFile("/Users/yana/geant4-projects/output.root");
//TFile *f2 = new TFile("/Users/yana/geant4-projects/output2.root");
// TFile* fout = new TFile("./HarverstHistos.root", "RECREATE");


  gROOT->Reset();
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadTopMargin(0.07);//!!
  gStyle->SetPadBottomMargin(0.12);//!!

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
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
  	{ 4, 34, 1.5, 2.0, 1, "#full energy", "HIST" },
    { 8, 43, 1.5, 3.0, 1, "#boron energy", "HIST" },
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
  // TTree *t2 = (TTree*)f2->Get("Doses in phantom");
    double X1, Y1, Z1, Edep1, EdepBoron1;//,X2, Y2, Z2, Edep2, EdepBoron2;

   t1->SetBranchAddress("X",&X1);
   t1->SetBranchAddress("Y",&Y1);
   t1->SetBranchAddress("Z",&Z1);
   t1->SetBranchAddress("Edep",&Edep1);
   t1->SetBranchAddress("EdepBoron",&EdepBoron1);
/*
   t2->SetBranchAddress("X",&X2);
   t2->SetBranchAddress("Y",&Y2);
   t2->SetBranchAddress("Z",&Z2);
   t2->SetBranchAddress("Edep",&Edep2);
   t2->SetBranchAddress("EdepBoron",&EdepBoron2);*/

   TH1F  *hDoseProfile_1_With = new TH1F("hDoseProfile_1_With","PolyBiz Torus Edep Outer Tubes With Boron",1,160,170);
   TH1F  *hDoseProfile_2_With = new TH1F("hDoseProfile_2_With","PolyBiz Torus Edep Inner Tubes With Boron",1,160,170);

   TH1F  *h_result1 = new TH1F("h_result1","PolyBiz Torus Result Edep Outer Tubes",1,160,170);
   TH1F  *h_result2 = new TH1F("h_result2","PolyBiz Torus Result Edep Inner Tubes",1,160,170);
/*
   TH1F  *hDoseProfile_1_Without = new TH1F("hDoseProfile_1_With","PolyBiz Torus Edep Outer Tubes Without Boron",1,160,170);
   TH1F  *hDoseProfile_2_Without = new TH1F("hDoseProfile_2_With","PolyBiz Torus Edep Inner Tubes Without Boron",1,160,170);*/

   int nentries = (int)t1->GetEntries();
   for (int i=0; i<nentries; i++) {
   t1 -> GetEntry(i);
   float R_ = sqrt(X1*X1 + Y1*Y1);
      if(R_ < 58 && R_ >42){
        hDoseProfile_1_With -> Fill(Z1, Edep1/16);
        h_result1 -> Fill(Z1, EdepBoron1/16);
         }

      if(X1*X1+Y1*Y1<1090 && X1*X1+Y1*Y1>288){
        hDoseProfile_2_With -> Fill(Z1, Edep1/8);
        h_result2 -> Fill(Z1, EdepBoron1/8);
      }
}
/*
nentries = (int)t2->GetEntries();
for (int i=0; i<nentries; i++) {
t2 -> GetEntry(i);
float R_ = sqrt(X2*X2 + Y2*Y2);
   if(R_ < 58 && R_ >42){
     hDoseProfile_1_Without -> Fill(Z2, Edep2/16);
      }

   if(X2*X2+Y2*Y2<1090 && X2*X2+Y2*Y2>288){
     hDoseProfile_2_Without -> Fill(Z2, Edep2/8);
   }
}

h_result1->Add(hDoseProfile_1_Without, -1);
h_result2->Add(hDoseProfile_2_Without, -1);*/
    
double RatioProtons = 2.247e19/4e10;
double Mass = 1.52e-3;
double Joule = 1.6e-13;

hDoseProfile_1_With->Scale(RatioProtons*Joule/Mass);
hDoseProfile_2_With->Scale(RatioProtons*Joule/Mass);
h_result1->Scale(RatioProtons*Joule/Mass);
h_result2->Scale(RatioProtons*Joule/Mass);
    
    hDoseProfile_1_With->SaveAs("hDoseProfile_1_With.txt", "ASCII");
    hDoseProfile_2_With->SaveAs("hDoseProfile_2_With.txt", "ASCII");
    h_result1->SaveAs("h_result1.txt", "ASCII");
    h_result2->SaveAs("h_result2.txt", "ASCII");

//====================  Making plots   =========================================


TCanvas *c1 = new TCanvas("c1", "c1", 960, 720);

hDoseProfile_1_With->GetYaxis()->SetTickLength(0.02);
hDoseProfile_1_With->GetYaxis()->SetNdivisions(505);
hDoseProfile_1_With->GetXaxis()->CenterTitle();
hDoseProfile_1_With->GetYaxis()->CenterTitle();
hDoseProfile_1_With->GetXaxis()->SetTitle("z (mm)");
hDoseProfile_1_With->GetYaxis()->SetTitle("#frac{Gy}{mA*h}");
hDoseProfile_1_With->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_1_With->GetYaxis()->SetTitleOffset(1.2);
hDoseProfile_1_With->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_1_With->GetXaxis()->SetTitleOffset(1.0);
hDoseProfile_1_With->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_1_With->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_1_With->GetYaxis()->SetLabelFont(42);
hDoseProfile_1_With->GetYaxis()->SetTitleFont(42);
hDoseProfile_1_With->GetXaxis()->SetTitleFont(42);
hDoseProfile_1_With->GetYaxis()->SetRangeUser(0,4.5);
hDoseProfile_1_With->SetLineColor(ModelAttr[0].colour);
hDoseProfile_1_With->SetLineWidth(ModelAttr[0].width);
hDoseProfile_1_With->SetMarkerStyle(ModelAttr[0].marker);
hDoseProfile_1_With->SetMarkerSize(ModelAttr[0].size);
hDoseProfile_1_With->SetMarkerColor(ModelAttr[0].colour);
hDoseProfile_1_With->Draw();
    h_result1->SetLineColor(ModelAttr[1].colour);
    h_result1->SetLineWidth(ModelAttr[1].width);
    h_result1->SetMarkerStyle(ModelAttr[1].marker);
    h_result1->SetMarkerSize(ModelAttr[1].size);
    h_result1->SetMarkerColor(ModelAttr[1].colour);
    h_result1->Draw("same");

TCanvas *c2 = new TCanvas("c2", "c2", 960, 720);

hDoseProfile_2_With->GetYaxis()->SetTickLength(0.02);
hDoseProfile_2_With->GetYaxis()->SetNdivisions(505);
hDoseProfile_2_With->GetXaxis()->CenterTitle();
hDoseProfile_2_With->GetYaxis()->CenterTitle();
hDoseProfile_2_With->GetXaxis()->SetTitle("z (mm)");
hDoseProfile_2_With->GetYaxis()->SetTitle("#frac{Gy}{mA*h}");
hDoseProfile_2_With->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_2_With->GetYaxis()->SetTitleOffset(1.2);
hDoseProfile_2_With->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_2_With->GetXaxis()->SetTitleOffset(1.0);
hDoseProfile_2_With->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_2_With->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_2_With->GetYaxis()->SetLabelFont(42);
hDoseProfile_2_With->GetYaxis()->SetTitleFont(42);
hDoseProfile_2_With->GetXaxis()->SetTitleFont(42);
hDoseProfile_2_With->GetYaxis()->SetRangeUser(0,4.5);
hDoseProfile_2_With->SetLineColor(ModelAttr[0].colour);
hDoseProfile_2_With->SetLineWidth(ModelAttr[0].width);
hDoseProfile_2_With->SetMarkerStyle(ModelAttr[0].marker);
hDoseProfile_2_With->SetMarkerSize(ModelAttr[0].size);
hDoseProfile_2_With->SetMarkerColor(ModelAttr[0].colour);
hDoseProfile_2_With->Draw();
    h_result2->SetLineColor(ModelAttr[1].colour);
    h_result2->SetLineWidth(ModelAttr[1].width);
    h_result2->SetMarkerStyle(ModelAttr[1].marker);
    h_result2->SetMarkerSize(ModelAttr[1].size);
    h_result2->SetMarkerColor(ModelAttr[1].colour);
    h_result2->Draw("same");

}
