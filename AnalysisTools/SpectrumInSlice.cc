{

  gROOT->Reset();
  //gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.03);
  gStyle->SetPadTopMargin(0.1);//!!
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
  	char  ModelName[100];
  	char option[20];
  };


  GraphAttr ModelAttr[] = {
  	{ 4, 21, 2.0, 1.2, "protons", "" },
  	{ 3, 33, 2.0, 1.5, "neutrons", "same" },
  	{ 2, 34, 2.0, 1.5, "electrons", "same" },
  	{ 45, 29, 2.0, 1.6, "photons", "same" },
    { 5, 29, 2.0, 1.6, "alpha", "same" },
  };

  vector<int> PIDofInterest;
  PIDofInterest.push_back(2212);   //proton
  PIDofInterest.push_back(2112);   //neutron
  PIDofInterest.push_back(11);     //electron
  PIDofInterest.push_back(22);     //gamma
  PIDofInterest.push_back(1000020040);  //alpha
  //1000020040//alfa


TFile *f = new TFile("/home/maxim/Programs/Geant4/MedPhys/BNCT/V3/BNCT-build/output.root");
//==============================================================================

   TTree *t1 = (TTree*)f->Get("Fluences");
   double X, Y, Zsurf, Energy;
   int particle_id;
   char particle_name[50];
   int Event;

   t1->SetBranchAddress("X",&X);
   t1->SetBranchAddress("Y",&Y);
   t1->SetBranchAddress("Zsurf",&Zsurf);
   t1->SetBranchAddress("Energy",&Energy);
   t1->SetBranchAddress("particle_id",&particle_id);
   t1->SetBranchAddress("particle_name",&particle_name);
   //t1->SetBranchAddress("Event",&Event);

   //TH1F *hFluence_Ref   = new TH1F("hFluence","Fluence (z)",120,0,120);
   //TH1F *hFluence[5];
   //for(int i=0; i<5; i++) hFluence[i] = (TH1F*) hFluence_Ref->Clone();
   TH1F *hSpectrum   = new TH1F("hSpectrum ","Proton Energy Spectrum",100,0,300);

   TH2F *hSpectrum2D   = new TH2F("hSpectrum ","Proton Energy Spectrum",200,-200,+200, 300, 0, +300);

   int nentries = (int)t1->GetEntries();
  for (int i=0; i<nentries; i++) {
     t1 -> GetEntry(i);

       if(particle_id == 2212) {
         if(fabs(X)<10 && fabs(Y)<10){

           hSpectrum2D->Fill(Zsurf, Energy);
           if(Zsurf>=-121 && Zsurf<-120)  hSpectrum -> Fill(Energy);


         }
       }
     //if(strcmp(particle_name, "alpha") == 0 ) hFluence[4] -> Fill(Zsurf);
     //if(strcmp(particle_name, "alpha") == 0 ) cout<< particle_name << "   PDG ID = "<< particle_id <<endl;
   }

//====================  Making plots   =========================================

c1 = new TCanvas("c1", "c1", 960, 720);

hSpectrum->GetYaxis()->SetTickLength(0.02);
hSpectrum->GetYaxis()->SetNdivisions(505);
hSpectrum->GetXaxis()->CenterTitle();
hSpectrum->GetYaxis()->CenterTitle();
hSpectrum->GetXaxis()->SetTitle("E, MeV");
hSpectrum->GetYaxis()->SetTitle("#frac{dN}{dE}");
//hSpectrum->GetYaxis()->SetRangeUser(1, 200000);
hSpectrum->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hSpectrum->GetYaxis()->SetTitleOffset(1.2);
hSpectrum->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hSpectrum->GetXaxis()->SetTitleOffset(1.0);
hSpectrum->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hSpectrum->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hSpectrum->GetYaxis()->SetLabelFont(42);
hSpectrum->GetYaxis()->SetTitleFont(42);
hSpectrum->GetXaxis()->SetTitleFont(42);
  //hFluence[pid_it]->SetLineWidth(ModelAttr[pid_it].width);
  //hFluence[pid_it]->SetLineColor(ModelAttr[pid_it].colour);
  //hFluence[pid_it]->SetMarkerStyle(ModelAttr[pid_it].marker);
  //hFluence[pid_it]->SetMarkerColor(ModelAttr[pid_it].colour);
  //hFluence[pid_it]->SetMarkerSize(ModelAttr[pid_it].size);
hSpectrum->Draw();


TCanvas *c2 = new TCanvas("c2", "c2", 960, 720);

hSpectrum2D->GetYaxis()->SetTickLength(0.02);
hSpectrum2D->GetYaxis()->SetNdivisions(505);
hSpectrum2D->GetXaxis()->CenterTitle();
hSpectrum2D->GetYaxis()->CenterTitle();
hSpectrum2D->GetYaxis()->SetTitle("E (MeV)");
hSpectrum2D->GetXaxis()->SetTitle("z (mm)");
//hSpectrum->GetYaxis()->SetRangeUser(1, 200000);
hSpectrum2D->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hSpectrum2D->GetYaxis()->SetTitleOffset(1.2);
hSpectrum2D->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hSpectrum2D->GetXaxis()->SetTitleOffset(1.0);
hSpectrum2D->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hSpectrum2D->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hSpectrum2D->GetYaxis()->SetLabelFont(42);
hSpectrum2D->GetYaxis()->SetTitleFont(42);
hSpectrum2D->GetXaxis()->SetTitleFont(42);
hSpectrum2D->Draw("colz");





}
