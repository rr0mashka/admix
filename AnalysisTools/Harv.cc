{
TFile *f1 = new TFile("../../BNCT-build/Run_Run_output.root");
TFile* fout = new TFile("../../BNCT-build/HarverstHistos.root", "RECREATE");


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

   TH2F  *hHead2Ds1  = new TH2F("hHead2Ds1","Dose slice 1",50,-100, 100, 50,-100, 100);
   TH2F  *hHead2Ds2  = new TH2F("hHead2Ds2","Dose slice 2",20,-50, 50, 20,-50, 50);
   TH2F  *hHead2Ds3  = new TH2F("hHead2Ds3","Dose slice 3",20,-50, 50, 20,-50, 50);

   int nentries = (int)t1->GetEntries();
  for (int i=0; i<nentries; i++) {
     t1 -> GetEntry(i);

      // if(particle_id == 2212) {   //protons
      //if(particle_id == 2112) {    //neutrons
      if(abs(X)<100 && abs(Y)<100){
          if (Z>150){
             hHead2Ds1 -> Fill(X,Y, dose);
        }
        /*  if (Z>185 && Z<195){
             hHead2Ds2 -> Fill(X,Y, Edep);
           }
          if (Z>230 && Z<240){
             hHead2Ds3 -> Fill(X,Y, Edep);
           }*/
         }
       }
     //if(strcmp(particle_name, "alpha") == 0 ) hFluence[4] -> Fill(Zsurf);
     //if(strcmp(particle_name, "alpha") == 0 ) cout<< particle_name << "   PDG ID = "<< particle_id <<endl;

//====================  Making plots   =========================================


TCanvas *x = new TCanvas("x", "x", 960, 720);

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
//hHead2Ds1->GetZaxis()->SetRangeUser(0, 12e-9);
hHead2Ds1->Draw("colz");

/*TCanvas *y = new TCanvas("y", "y", 960, 720);

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
//hHead2Ds2->GetZaxis()->SetRangeUser(0, 12e-9);
hHead2Ds2->Draw("colz");

TCanvas *z = new TCanvas("z", "z", 960, 720);

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
//hHead2Ds3->GetZaxis()->SetRangeUser(0, 12e-9);
hHead2Ds3->Draw("colz");*/



fout->WriteObject(hHead2Ds1, "head slice 1");
//fout->WriteObject(hHead2Ds2, "head slice 2");
//fout->WriteObject(hHead2Ds3, "head slice 3");


//fout->Close();

}
