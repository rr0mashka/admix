{

  gROOT->Reset();
  //gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.15);
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


//TFile *f = new TFile("/home/maxim/Programs/Geant4/MedPhys/BNCT/V5/BNCT-build/output_2.2MeV_40M_Plex.root");
//TFile *f = new TFile("/home/maxim/Programs/Geant4/MedPhys/BNCT/V5/BNCT-build/output_2.2MeV_40M_Plex_NoPhantom.root");
TFile *f = new TFile("/home/maxim/Programs/Geant4/MedPhys/BNCT/V5/BNCT-build/output_2.2MeV_40M_Plex_Phantom1cm.root");
//TFile *f = new TFile("/home/maxim/Programs/Geant4/MedPhys/BNCT/V5/BNCT-build/output_2.2MeV_40M_Plex_NoPhantom_AMg6.root");

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


   TH1F *hSpectrum   = new TH1F("hSpectrum ","Neutron Energy Spectrum",100,0, 1e-6);

   TH2F  *hFluence2D  = new TH2F("hFluences2d ","Output Neutron Field Spot ",20,-100,+100, 20,-100,+100);

   vector<float> Fluence1D_Binning;
   float Bin_width = 5.0;
   for(int i =0; i<=20; i++ ) Fluence1D_Binning.push_back(i*Bin_width);
   TH1F  *hFluence1D  = new TH1F("hFluences1d ","Output Neutron Field Density ", 20, &Fluence1D_Binning[0]);

   vector<float> Fluence1D_AzimuBinning;
   int N_az_bin = 20;
   float Azmizth_Bin_width = 2*3.14159/N_az_bin;
   for(int i =0; i<=N_az_bin; i++ ) Fluence1D_AzimuBinning.push_back(i*Azmizth_Bin_width);
   TH1F  *hFluence_phi_1  = new TH1F("hFluence_phi_1","Output Neutron Field Density ", 20, &Fluence1D_AzimuBinning[0]);
   TH1F  *hFluence_phi_2  = new TH1F("hFluence_phi_2","Output Neutron Field Density ", 20, &Fluence1D_AzimuBinning[0]);
   TH1F  *hFluence_phi_3  = new TH1F("hFluence_phi_3","Output Neutron Field Density ", 20, &Fluence1D_AzimuBinning[0]);

   TH2F  *hSpectrum2D = new TH2F("hSpectrum2d ","Neutron Energy Spectrum",150,-150,+150, 300, 0, +0.6);

   int nentries = (int)t1->GetEntries();
  for (int i=0; i<nentries; i++) {
     t1 -> GetEntry(i);

      // if(particle_id == 2212) {   //protons
      if(particle_id == 2112) {    //neutrons
         if(fabs(X)<100 && fabs(Y)<100){


           hSpectrum2D->Fill(Zsurf, Energy);

           if(Zsurf>=10 && Zsurf<11)  {
             double R = sqrt(pow(X,2)+pow(Y,2));
             int R_int = (int) R/Bin_width;
             float BinArea = 4 * 3.14159 * (  pow((R_int+1)* Bin_width, 2 ) -  pow( R_int * Bin_width, 2 ) );
             cout<<"iBin:  "<<R_int<<"   BinArea:  "<<BinArea<<endl;
             hFluence1D -> Fill(R, 1.0/BinArea);
             hFluence2D -> Fill(X,Y);
             hSpectrum -> Fill(Energy);

            float NeutonTan;
            if(X!=0) NeutonTan = Y/fabs(X);
            else NeutonTan = Y/(X+0.0001);
            float phi =  atan(NeutonTan);
            if(X < 0) phi = 3.14159 - phi;
            phi = 3.14159/2.0 + phi;
            if(R < 150) hFluence_phi_1 -> Fill(phi);
            if(R >= 50 && R < 75) hFluence_phi_2 -> Fill(phi);
            if(R >= 75 && R < 100) hFluence_phi_2 -> Fill(phi);
           }

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


TCanvas *c3 = new TCanvas("c3", "c3", 960, 720);

hFluence2D->GetYaxis()->SetTickLength(0.02);
hFluence2D->GetYaxis()->SetNdivisions(505);
hFluence2D->GetXaxis()->CenterTitle();
hFluence2D->GetYaxis()->CenterTitle();
hFluence2D->GetYaxis()->SetTitle("y (mm)");
hFluence2D->GetXaxis()->SetTitle("x (mm)");
//hSpectrum->GetYaxis()->SetRangeUser(1, 200000);
hFluence2D->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence2D->GetYaxis()->SetTitleOffset(1.2);
hFluence2D->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence2D->GetXaxis()->SetTitleOffset(1.0);
hFluence2D->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence2D->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence2D->GetYaxis()->SetLabelFont(42);
hFluence2D->GetYaxis()->SetTitleFont(42);
hFluence2D->GetXaxis()->SetTitleFont(42);
hFluence2D->Draw("colz");


TCanvas *c4 = new TCanvas("c4", "c4", 960, 720);

hFluence1D->GetYaxis()->SetTickLength(0.02);
hFluence1D->GetYaxis()->SetNdivisions(505);
hFluence1D->GetXaxis()->CenterTitle();
hFluence1D->GetYaxis()->CenterTitle();
hFluence1D->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D->GetXaxis()->SetTitle("R (mm)");
//hSpectrum->GetYaxis()->SetRangeUser(1, 200000);
hFluence1D->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D->GetYaxis()->SetTitleOffset(1.2);
hFluence1D->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D->GetXaxis()->SetTitleOffset(1.0);
hFluence1D->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D->GetYaxis()->SetLabelFont(42);
hFluence1D->GetYaxis()->SetTitleFont(42);
hFluence1D->GetXaxis()->SetTitleFont(42);
hFluence1D->Draw();


TCanvas *c5 = new TCanvas("c45", "c5", 960, 720);

hFluence_phi_1->GetYaxis()->SetTickLength(0.02);
hFluence_phi_1->GetYaxis()->SetNdivisions(505);
hFluence_phi_1->GetXaxis()->CenterTitle();
hFluence_phi_1->GetYaxis()->CenterTitle();
hFluence_phi_1->GetYaxis()->SetTitle("#frac{dN}{d#phi}");
hFluence1D->GetXaxis()->SetTitle("R (mm)");
//hSpectrum->GetYaxis()->SetRangeUser(1, 200000);
hFluence_phi_1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence_phi_1->GetYaxis()->SetTitleOffset(1.2);
hFluence_phi_1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence_phi_1->GetXaxis()->SetTitleOffset(1.0);
hFluence_phi_1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence_phi_1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence_phi_1->GetYaxis()->SetLabelFont(42);
hFluence_phi_1->GetYaxis()->SetTitleFont(42);
hFluence_phi_1->GetXaxis()->SetTitleFont(42);
hFluence_phi_1->Draw();


}
