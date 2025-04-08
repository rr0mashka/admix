{
TFile *f1 = new TFile("/home/yana/Geant4/geant4-projects/CellsPhantom/build/output.root");
//TFile *f2 = new TFile("/home/yana/Geant4/geant4-projects/devSD/build/output.root");
TFile* fout = new TFile("./HarverstedHistos.root", "RECREATE");


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

   //TTree *t1 = (TTree*)f1->Get("Doses in phantom");
   TTree *t1 = (TTree*)f1->Get("Fluences");
   double X, Y, Energy, Zsurf;
   int particle_id;
   char particle_name[50];
   int Event;

   t1->SetBranchAddress("X",&X);
   t1->SetBranchAddress("Y",&Y);
   t1->SetBranchAddress("Energy",&Energy);
   t1->SetBranchAddress("Zsurf",&Zsurf);
   t1->SetBranchAddress("particle_id",&particle_id);
   t1->SetBranchAddress("particle_name",&particle_name);
   //t1->SetBranchAddress("dose",&dose);
   t1->SetBranchAddress("Event",&Event);


   vector<float> Energy1D_Binning;
   float width1 = 1e-8; //up to 1 eV
   float width2 = 2e-6; //up to 100eV
   float width3 = 2e-4; //up to 10keV
   float width4 = 2e-2; //up to 1MeV

   for(int i =0; i<=100; i++ ) Energy1D_Binning.push_back(i*width1);
   for(int i =1; i<=50; i++ ) Energy1D_Binning.push_back(i*width2 + 100*width1);
   for(int i =1; i<=50; i++ ) Energy1D_Binning.push_back(i*width3 + 50*width2 + 100*width1);
   for(int i =1; i<=50; i++ ) Energy1D_Binning.push_back(i*width4 + 50*width3 + 50*width2 + 100*width1);
//(int)Energy1D_Binning.size() - 1

   TH1F *hSpectrum   = new TH1F("hSpectrum ","Neutron Energy Spectrum",(int)Energy1D_Binning.size() - 1 , &Energy1D_Binning[0]);

   TH2F  *hFluence2D  = new TH2F("hFluences2d ","Output Neutron Field Spot ",30,-120,+120, 30,-120,+120);
   TH2F  *hHead2Ds1  = new TH2F("hHead2Ds1","Dose slice 1",20,-50, 50, 20,-50, 50);
   TH2F  *hHead2Ds2  = new TH2F("hHead2Ds2","Dose slice 2",20,-50, 50, 20,-50, 50);
   TH2F  *hHead2Ds3  = new TH2F("hHead2Ds3","Dose slice 3",20,-50, 50, 20,-50, 50);
   TH2F  *hNeutronFlux_vs_Z_En1  = new TH2F("NeutronFlux_vs_Z_En1","Neutron Field Profile", 250,-300, 200, 100, 0,+200);
   TH2F  *hNeutronFlux_vs_Z_En2  = new TH2F("NeutronFlux_vs_Z_En2","Neutron Field Profile", 250,-300, 200, 100, 0,+200);
   TH2F  *hNeutronFlux_vs_Z_En3  = new TH2F("NeutronFlux_vs_Z_En3","Neutron Field Profile", 250,-300, 200, 100, 0,+200);

   vector<float> Fluence1D_Binning;
   float Bin_width = 5.0;
   for(int i =0; i<=30; i++ ) Fluence1D_Binning.push_back(i*Bin_width);
   TH1F  *hFluence1D_1  = new TH1F("hFluences1d_1 ","Output Neutron Field Density ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2  = new TH1F("hFluences1d_2 ","Output Neutron Field Density ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3  = new TH1F("hFluences1d_3 ","Output Neutron Field Density ", 30, &Fluence1D_Binning[0]);

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
      //if(particle_id == 2112) {    //neutrons

        double R = sqrt(pow(X,2)+pow(Y,2));
        int R_int = (int) R/Bin_width;
        float BinArea = 3.14159 * (  pow((R_int+1)* Bin_width, 2 ) -  pow( R_int * Bin_width, 2 ) );//area of rings at diffrent r
        double EnergyEV = Energy*1e+6; // converting MeV to eV for the better readability

        if( EnergyEV < 0.5 )hNeutronFlux_vs_Z_En1 -> Fill(Zsurf, R, 1.0/BinArea);
        if( EnergyEV > 0.5 && EnergyEV < 10000 ) hNeutronFlux_vs_Z_En2 -> Fill(Zsurf, R, 1.0/BinArea);
        if( EnergyEV > 10000 ) hNeutronFlux_vs_Z_En3 -> Fill(Zsurf, R, 1.0/BinArea);


         if(fabs(X)<150 && fabs(Y)<150){


           hSpectrum2D->Fill(Zsurf, Energy);

           if(Zsurf>=120 && Zsurf<121)  {
             hSpectrum -> Fill(Energy);

             //cout<<"iBin:  "<<R_int<<"   BinArea:  "<<BinArea<<endl;
             if( EnergyEV < 0.5 )hFluence1D_1 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 10000 ) hFluence1D_2 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 10000 ) hFluence1D_3 -> Fill(R, 1.0/BinArea);
             hFluence2D -> Fill(X,Y);


            float NeutonTan;
            if(X!=0) NeutonTan = Y/fabs(X);
            else NeutonTan = Y/(X+0.0001);
            float phi =  atan(NeutonTan);
            if(X < 0) phi = 3.14159 - phi;
            phi = 3.14159/2.0 + phi;//sgifthing origin for a better view
            cout<<"R:  "<<R<<endl;
            if(R <  50) hFluence_phi_1 -> Fill(phi);
            if(R >= 50 && R < 75) hFluence_phi_2 -> Fill(phi);
            if(R >= 75 && R < 100) hFluence_phi_3-> Fill(phi);

           if (Zsurf>140 && Zsurf<150){
             hHead2Ds1 -> Fill(X,Y, Energy);
          }
          if (Zsurf>185 && Zsurf<195){
             hHead2Ds2 -> Fill(X,Y, Energy);
           }
          if (Zsurf>230 && Zsurf<240){
             hHead2Ds3 -> Fill(X,Y, Energy);
           }
     }
       }
     //if(strcmp(particle_name, "alpha") == 0 ) hFluence[4] -> Fill(Zsurf);
     //if(strcmp(particle_name, "alpha") == 0 ) cout<< particle_name << "   PDG ID = "<< particle_id <<endl;
   }

//====================  Making plots   =========================================

TCanvas *c1 = new TCanvas("c1", "c1", 960, 720);

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


TCanvas *c3_1 = new TCanvas("c3_1", "c3_1", 960, 720);
hNeutronFlux_vs_Z_En1->GetYaxis()->SetTickLength(0.02);
hNeutronFlux_vs_Z_En1->GetYaxis()->SetNdivisions(505);
hNeutronFlux_vs_Z_En1->GetXaxis()->CenterTitle();
hNeutronFlux_vs_Z_En1->GetYaxis()->CenterTitle();
hNeutronFlux_vs_Z_En1->GetYaxis()->SetTitle("R (mm)");
hNeutronFlux_vs_Z_En1->GetXaxis()->SetTitle("Z (mm)");
hNeutronFlux_vs_Z_En1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hNeutronFlux_vs_Z_En1->GetYaxis()->SetTitleOffset(1.2);
hNeutronFlux_vs_Z_En1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hNeutronFlux_vs_Z_En1->GetXaxis()->SetTitleOffset(1.0);
hNeutronFlux_vs_Z_En1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hNeutronFlux_vs_Z_En1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hNeutronFlux_vs_Z_En1->GetYaxis()->SetLabelFont(42);
hNeutronFlux_vs_Z_En1->GetYaxis()->SetTitleFont(42);
hNeutronFlux_vs_Z_En1->GetXaxis()->SetTitleFont(42);
hNeutronFlux_vs_Z_En1->Draw("colz");


TCanvas *c3_2 = new TCanvas("c3_2", "c3_2", 960, 720);
hNeutronFlux_vs_Z_En2->GetYaxis()->SetTickLength(0.02);
hNeutronFlux_vs_Z_En2->GetYaxis()->SetNdivisions(505);
hNeutronFlux_vs_Z_En2->GetXaxis()->CenterTitle();
hNeutronFlux_vs_Z_En2->GetYaxis()->CenterTitle();
hNeutronFlux_vs_Z_En2->GetYaxis()->SetTitle("R (mm)");
hNeutronFlux_vs_Z_En2->GetXaxis()->SetTitle("Z (mm)");
hNeutronFlux_vs_Z_En2->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hNeutronFlux_vs_Z_En2->GetYaxis()->SetTitleOffset(1.2);
hNeutronFlux_vs_Z_En2->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hNeutronFlux_vs_Z_En2->GetXaxis()->SetTitleOffset(1.0);
hNeutronFlux_vs_Z_En2->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hNeutronFlux_vs_Z_En2->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hNeutronFlux_vs_Z_En2->GetYaxis()->SetLabelFont(42);
hNeutronFlux_vs_Z_En2->GetYaxis()->SetTitleFont(42);
hNeutronFlux_vs_Z_En2->GetXaxis()->SetTitleFont(42);
hNeutronFlux_vs_Z_En2->Draw("colz");

TCanvas *c3_3 = new TCanvas("c3_3", "c3_3", 960, 720);
hNeutronFlux_vs_Z_En3->GetYaxis()->SetTickLength(0.02);
hNeutronFlux_vs_Z_En3->GetYaxis()->SetNdivisions(505);
hNeutronFlux_vs_Z_En3->GetXaxis()->CenterTitle();
hNeutronFlux_vs_Z_En3->GetYaxis()->CenterTitle();
hNeutronFlux_vs_Z_En3->GetYaxis()->SetTitle("R (mm)");
hNeutronFlux_vs_Z_En3->GetXaxis()->SetTitle("Z (mm)");
hNeutronFlux_vs_Z_En3->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hNeutronFlux_vs_Z_En3->GetYaxis()->SetTitleOffset(1.2);
hNeutronFlux_vs_Z_En3->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hNeutronFlux_vs_Z_En3->GetXaxis()->SetTitleOffset(1.0);
hNeutronFlux_vs_Z_En3->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hNeutronFlux_vs_Z_En3->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hNeutronFlux_vs_Z_En3->GetYaxis()->SetLabelFont(42);
hNeutronFlux_vs_Z_En3->GetYaxis()->SetTitleFont(42);
hNeutronFlux_vs_Z_En3->GetXaxis()->SetTitleFont(42);
hNeutronFlux_vs_Z_En3->Draw("colz");

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

TCanvas *y = new TCanvas("y", "y", 960, 720);

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
hHead2Ds3->Draw("colz");

TCanvas *c4 = new TCanvas("c4", "c4", 960, 720);
hFluence1D_1->GetYaxis()->SetTickLength(0.02);
hFluence1D_1->GetYaxis()->SetNdivisions(505);
hFluence1D_1->GetXaxis()->CenterTitle();
hFluence1D_1->GetYaxis()->CenterTitle();
hFluence1D_1->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1->GetXaxis()->SetTitle("r (mm)");
//hSpectrum->GetYaxis()->SetRangeUser(1, 200000);
hFluence1D_1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1->GetYaxis()->SetLabelFont(42);
hFluence1D_1->GetYaxis()->SetTitleFont(42);
hFluence1D_1->GetXaxis()->SetTitleFont(42);
hFluence1D_1->Draw();

hFluence1D_1->SetLineWidth(ModelAttr[0].width);
hFluence1D_1->SetLineColor(ModelAttr[0].colour);
hFluence1D_1->SetLineStyle(ModelAttr[0].LineStyle);
//hFluence1D_1[i]->SetMarkerStyle(ModelAttr[0].marker);
//hFluence1D_1[i]->SetMarkerColor(ModelAttr[0].colour);
//hFluence1D_1[i]->SetMarkerSize(ModelAttr[0].size);
hFluence1D_1->Draw(ModelAttr[0].option);

hFluence1D_2->SetLineWidth(ModelAttr[1].width);
hFluence1D_2->SetLineColor(ModelAttr[1].colour);
hFluence1D_2->SetLineStyle(ModelAttr[1].LineStyle);
//hFluence1D_2[i]->SetMarkerStyle(ModelAttr[0].marker);
//hFluence1D_2[i]->SetMarkerColor(ModelAttr[0].colour);
//hFluence1D_2[i]->SetMarkerSize(ModelAttr[0].size);
hFluence1D_2->Draw(ModelAttr[1].option);

hFluence1D_3->SetLineWidth(ModelAttr[2].width);
hFluence1D_3->SetLineColor(ModelAttr[2].colour);
hFluence1D_3->SetLineStyle(ModelAttr[2].LineStyle);
//hFluence1D_1[i]->SetMarkerStyle(ModelAttr[0].marker);
//hFluence1D_1[i]->SetMarkerColor(ModelAttr[0].colour);
//hFluence1D_1[i]->SetMarkerSize(ModelAttr[0].size);
hFluence1D_3->Draw(ModelAttr[2].option);

TLegend* legend_1 = new TLegend(0.50, 0.70, 0.80, 0.90);//
legend_1->SetTextSize(0.040);
legend_1->SetTextFont(42);
//legend_2->SetHeader("GNP, D = 20nm");
legend_1->AddEntry(hFluence1D_1, "E_{n} #leq 0.5 eV", "l");
legend_1->AddEntry(hFluence1D_2, "0.5 eV < E_{n} #leq 10 keV", "l");
legend_1->AddEntry(hFluence1D_3, "E_{n} > 10 keV","l");
legend_1->SetFillColor(kWhite);
legend_1->SetLineColor(kWhite);
legend_1->Draw();


TCanvas *c5 = new TCanvas("c45", "c5", 960, 720);

hFluence_phi_1->GetYaxis()->SetTickLength(0.02);
hFluence_phi_1->GetYaxis()->SetNdivisions(505);
hFluence_phi_1->GetXaxis()->CenterTitle();
hFluence_phi_1->GetYaxis()->CenterTitle();
hFluence_phi_1->GetYaxis()->SetTitle("#frac{dN}{d#phi}");
hFluence_phi_1->GetXaxis()->SetTitle("#phi (rad)");
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

hFluence_phi_1->SetLineWidth(ModelAttr[0].width);
hFluence_phi_1->SetLineColor(ModelAttr[0].colour);
hFluence_phi_1->SetLineStyle(ModelAttr[0].LineStyle);
//hFluence_phi_1[i]->SetMarkerStyle(ModelAttr[0].marker);
//hFluence_phi_1[i]->SetMarkerColor(ModelAttr[0].colour);
//hFluence_phi_1[i]->SetMarkerSize(ModelAttr[0].size);
hFluence_phi_1->Draw(ModelAttr[0].option);

hFluence_phi_2->SetLineWidth(ModelAttr[1].width);
hFluence_phi_2->SetLineColor(ModelAttr[1].colour);
hFluence_phi_2->SetLineStyle(ModelAttr[1].LineStyle);
//hFluence_phi_2[i]->SetMarkerStyle(ModelAttr[0].marker);
//hFluence_phi_2[i]->SetMarkerColor(ModelAttr[0].colour);
//hFluence_phi_2[i]->SetMarkerSize(ModelAttr[0].size);
hFluence_phi_2->Draw(ModelAttr[1].option);

hFluence_phi_3->SetLineWidth(ModelAttr[2].width);
hFluence_phi_3->SetLineColor(ModelAttr[2].colour);
hFluence_phi_3->SetLineStyle(ModelAttr[2].LineStyle);
//hFluence_phi_1[i]->SetMarkerStyle(ModelAttr[0].marker);
//hFluence_phi_1[i]->SetMarkerColor(ModelAttr[0].colour);
//hFluence_phi_1[i]->SetMarkerSize(ModelAttr[0].size);
hFluence_phi_3->Draw(ModelAttr[2].option);


TLegend* legend_2 = new TLegend(0.50, 0.70, 0.80, 0.90);//
legend_2->SetTextSize(0.040);
legend_2->SetTextFont(42);
//legend_2->SetHeader("GNP, D = 20nm");
legend_2->AddEntry(hFluence_phi_1, "r #leq 50 mm", "l");
legend_2->AddEntry(hFluence_phi_2, "50 < r #leq 75 mm", "l");
legend_2->AddEntry(hFluence_phi_3, "75 < r #leq 100 mm","l");
legend_2->SetFillColor(kWhite);
legend_2->SetLineColor(kWhite);
legend_2->Draw();


fout->WriteObject(hFluence_phi_1, "Fluence_phi_1");
fout->WriteObject(hFluence_phi_2, "Fluence_phi_2");
fout->WriteObject(hFluence_phi_3, "Fluence_phi_3");

fout->WriteObject(hFluence1D_1, "hFluence1D_1");
fout->WriteObject(hFluence1D_2, "hFluence1D_2");
fout->WriteObject(hFluence1D_3, "hFluence1D_3");

fout->WriteObject(hSpectrum2D, "Spectrum2D");
fout->WriteObject(hSpectrum, "Spectrum1D");


fout->WriteObject(hFluence2D , "Fluence2D");
fout->WriteObject(hHead2Ds1, "head slice 1");
fout->WriteObject(hHead2Ds2, "head slice 2");
fout->WriteObject(hHead2Ds3, "head slice 3");

fout->WriteObject(hNeutronFlux_vs_Z_En1 , "NeutronFlux_vs_Z_En1");
fout->WriteObject(hNeutronFlux_vs_Z_En2 , "NeutronFlux_vs_Z_En2");
fout->WriteObject(hNeutronFlux_vs_Z_En3 , "NeutronFlux_vs_Z_En3");

//fout->Close();

}
