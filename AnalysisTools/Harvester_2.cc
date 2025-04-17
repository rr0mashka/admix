{
TFile *f1 = new TFile("/home/petrunya/BNCT-test-protons-build/Run_Run_output.root");
TFile* fout = new TFile("/home/petrunya/BNCT-test-protons/AnalysisTools/HarverstHistos.root", "RECREATE");


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
   double X, Y, Z, Edep, EdepBoron;

   t1->SetBranchAddress("X",&X);
   t1->SetBranchAddress("Y",&Y);
   t1->SetBranchAddress("Z",&Z);
   t1->SetBranchAddress("Edep",&Edep);
   t1->SetBranchAddress("EdepBoron",&EdepBoron);

   TH1F  *hDoseProfile_1 = new TH1F("hDoseProfile_1","Edep Outer Tubes",5,140,190);
   TH1F  *hDoseProfile_2 = new TH1F("hDoseProfile_2","Edep Inner Tubes",5,140,190);

   TH1F  *hBoronDoseProfile_1 = new TH1F("hBoronDoseProfile_1","Edep Boron Profile 1",5,140,190);
   TH1F  *hBoronDoseProfile_2 = new TH1F("hBoronDoseProfile_2","Edep Boron Profile 2",5,140,190);

   int nentries = (int)t1->GetEntries();
   for (int i=0; i<nentries; i++) {
   t1 -> GetEntry(i);
   float R_ = sqrt(X*X + Y*Y);
      if(R_ < 58 && R_ >42){
        hDoseProfile_1 -> Fill(Z, Edep/16);
        hBoronDoseProfile_1 -> Fill(Z, EdepBoron/16);
         }

      if(X*X+Y*Y<1090 && X*X+Y*Y>288){
        hDoseProfile_2 -> Fill(Z, Edep/8);
        hBoronDoseProfile_2 -> Fill(Z, EdepBoron/8);
      }
}

//====================  Making plots   =========================================


TCanvas *c2 = new TCanvas("c2", "c2", 960, 720);

hDoseProfile_1->GetYaxis()->SetTickLength(0.02);
hDoseProfile_1->GetYaxis()->SetNdivisions(505);
hDoseProfile_1->GetXaxis()->CenterTitle();
hDoseProfile_1->GetYaxis()->CenterTitle();
hDoseProfile_1->GetXaxis()->SetTitle("z (mm)");
hDoseProfile_1->GetYaxis()->SetTitle("Edep, eV");
hDoseProfile_1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_1->GetYaxis()->SetTitleOffset(1.2);
hDoseProfile_1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_1->GetXaxis()->SetTitleOffset(1.0);
hDoseProfile_1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_1->GetYaxis()->SetLabelFont(42);
hDoseProfile_1->GetYaxis()->SetTitleFont(42);
hDoseProfile_1->GetXaxis()->SetTitleFont(42);
hDoseProfile_1->SetLineColor(ModelAttr[0].colour);
hDoseProfile_1->SetLineWidth(ModelAttr[0].width);
hDoseProfile_1->SetMarkerStyle(ModelAttr[0].marker);
hDoseProfile_1->SetMarkerSize(ModelAttr[0].size);
hDoseProfile_1->SetMarkerColor(ModelAttr[0].colour);
hDoseProfile_1->Draw();
hBoronDoseProfile_1->SetLineColor(ModelAttr[1].colour);
hBoronDoseProfile_1->SetLineWidth(ModelAttr[1].width);
hBoronDoseProfile_1->SetMarkerStyle(ModelAttr[1].marker);
hBoronDoseProfile_1->SetMarkerSize(ModelAttr[1].size);
hBoronDoseProfile_1->SetMarkerColor(ModelAttr[1].colour);
hBoronDoseProfile_1->Draw("same");

TLegend* legend = new TLegend(0.50, 0.65, 0.7, 0.80);//
legend->SetTextSize(0.036);
legend->SetTextFont(42);
legend->SetHeader("Energies:");
legend->AddEntry(hDoseProfile_1, "Full energy deposition");
legend->AddEntry(hBoronDoseProfile_1, "Boron energy deposition");
legend->SetFillColor(kWhite);
legend->SetLineColor(kWhite);
legend->Draw();

TCanvas *c1 = new TCanvas("c1", "c1", 960, 720);

hDoseProfile_2->GetYaxis()->SetTickLength(0.02);
hDoseProfile_2->GetYaxis()->SetNdivisions(505);
hDoseProfile_2->GetXaxis()->CenterTitle();
hDoseProfile_2->GetYaxis()->CenterTitle();
hDoseProfile_2->GetXaxis()->SetTitle("z (mm)");
hDoseProfile_2->GetYaxis()->SetTitle("Edep, eV");
hDoseProfile_2->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_2->GetYaxis()->SetTitleOffset(1.2);
hDoseProfile_2->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hDoseProfile_2->GetXaxis()->SetTitleOffset(1.0);
hDoseProfile_2->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_2->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hDoseProfile_2->GetYaxis()->SetLabelFont(42);
hDoseProfile_2->GetYaxis()->SetTitleFont(42);
hDoseProfile_2->GetXaxis()->SetTitleFont(42);
hDoseProfile_2->SetLineColor(ModelAttr[0].colour);
hDoseProfile_2->SetLineWidth(ModelAttr[0].width);
hDoseProfile_2->SetMarkerStyle(ModelAttr[0].marker);
hDoseProfile_2->SetMarkerSize(ModelAttr[0].size);
hDoseProfile_2->SetMarkerColor(ModelAttr[0].colour);
hDoseProfile_2->Draw();
hBoronDoseProfile_2->SetLineColor(ModelAttr[1].colour);
hBoronDoseProfile_2->SetLineWidth(ModelAttr[1].width);
hBoronDoseProfile_2->SetMarkerStyle(ModelAttr[1].marker);
hBoronDoseProfile_2->SetMarkerSize(ModelAttr[1].size);
hBoronDoseProfile_2->SetMarkerColor(ModelAttr[1].colour);
hBoronDoseProfile_2->Draw("same");

TLegend* legend2 = new TLegend(0.50, 0.65, 0.7, 0.80);//
legend2->SetTextSize(0.036);
legend2->SetTextFont(42);
legend2->SetHeader("Energies:");
legend2->AddEntry(hDoseProfile_1, "Full energy deposition");
legend2->AddEntry(hBoronDoseProfile_1, "Boron energy deposition");
legend2->SetFillColor(kWhite);
legend2->SetLineColor(kWhite);
legend2->Draw();

/*TCanvas *c3 = new TCanvas("c3", "c3", 960, 720);

hBoronDoseProfile_1->GetYaxis()->SetTickLength(0.02);
hBoronDoseProfile_1->GetYaxis()->SetNdivisions(505);
hBoronDoseProfile_1->GetXaxis()->CenterTitle();
hBoronDoseProfile_1->GetYaxis()->CenterTitle();
hBoronDoseProfile_1->GetXaxis()->SetTitle("z (mm)");
hBoronDoseProfile_1->GetYaxis()->SetTitle("Edep, eV");
hBoronDoseProfile_1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hBoronDoseProfile_1->GetYaxis()->SetTitleOffset(1.2);
hBoronDoseProfile_1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hBoronDoseProfile_1->GetXaxis()->SetTitleOffset(1.0);
hBoronDoseProfile_1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hBoronDoseProfile_1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hBoronDoseProfile_1->GetYaxis()->SetLabelFont(42);
hBoronDoseProfile_1->GetYaxis()->SetTitleFont(42);
hBoronDoseProfile_1->GetXaxis()->SetTitleFont(42);
hBoronDoseProfile_1->SetLineColor(kBlue);
hBoronDoseProfile_1->Draw();

TCanvas *c4 = new TCanvas("c4", "c4", 960, 720);

hBoronDoseProfile_2->GetYaxis()->SetTickLength(0.02);
hBoronDoseProfile_2->GetYaxis()->SetNdivisions(505);
hBoronDoseProfile_2->GetXaxis()->CenterTitle();
hBoronDoseProfile_2->GetYaxis()->CenterTitle();
hBoronDoseProfile_2->GetXaxis()->SetTitle("z (mm)");
hBoronDoseProfile_2->GetYaxis()->SetTitle("Edep, eV");
hBoronDoseProfile_2->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hBoronDoseProfile_2->GetYaxis()->SetTitleOffset(1.2);
hBoronDoseProfile_2->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hBoronDoseProfile_2->GetXaxis()->SetTitleOffset(1.0);
hBoronDoseProfile_2->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hBoronDoseProfile_2->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hBoronDoseProfile_2->GetYaxis()->SetLabelFont(42);
hBoronDoseProfile_2->GetYaxis()->SetTitleFont(42);
hBoronDoseProfile_2->GetXaxis()->SetTitleFont(42);
hBoronDoseProfile_2->SetLineColor(kBlue);
hBoronDoseProfile_2->Draw();*/

fout->WriteObject(hDoseProfile_1, "hDoseProfile_1");
fout->WriteObject(hDoseProfile_2, "hDoseProfile_2");
fout->WriteObject(hBoronDoseProfile_1, "hBoronDoseProfile_1");
fout->WriteObject(hBoronDoseProfile_2, "hBoronDoseProfile_2");
//fout->WriteObject(hBoronDoseProfile_1, "hBoronDoseProfile_1");

}
