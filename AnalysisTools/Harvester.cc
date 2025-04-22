{
TFile *f1 = new TFile("/home/yana/Geant4/geant4-projects/BNCT/build/output.root");
TFile* fout = new TFile("./HarverstedHistos.root", "RECREATE");


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

//==============================================================================

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

   TH1F *hSpectrum   = new TH1F("hSpectrum ","Neutron Energy Spectrum",(int)Energy1D_Binning.size() - 1 , &Energy1D_Binning[0]);

   TH2F  *hFluence2D  = new TH2F("hFluences2d ","Output Neutron Field Spot ",30,-120,+120, 30,-120,+120);
   TH2F  *hNeutronFlux_vs_Z_En1  = new TH2F("NeutronFlux_vs_Z_En1","Neutron Field Profile", 250,-300, 200, 100, 0,+200);
   TH2F  *hNeutronFlux_vs_Z_En2  = new TH2F("NeutronFlux_vs_Z_En2","Neutron Field Profile", 250,-300, 200, 100, 0,+200);
   TH2F  *hNeutronFlux_vs_Z_En3  = new TH2F("NeutronFlux_vs_Z_En3","Neutron Field Profile", 250,-300, 200, 100, 0,+200);

   vector<float> Fluence1D_Binning;
   float Bin_width = 5.0;
   for(int i =0; i<=30; i++ ) Fluence1D_Binning.push_back(i*Bin_width);
   TH1F  *hFluence1D_1_1  = new TH1F("hFluences1d_1_1 ","Output Neutron Field Density 1 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_1  = new TH1F("hFluences1d_2_1 ","Output Neutron Field Density 1 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_1  = new TH1F("hFluences1d_3_1 ","Output Neutron Field Density 1 ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_2  = new TH1F("hFluences1d_1_2 ","Output Neutron Field Density 2 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_2  = new TH1F("hFluences1d_2_2 ","Output Neutron Field Density 2 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_2  = new TH1F("hFluences1d_3_2 ","Output Neutron Field Density 2 ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_3  = new TH1F("hFluences1d_1_3 ","Output Neutron Field Density 3 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_3  = new TH1F("hFluences1d_2_3 ","Output Neutron Field Density 3 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_3  = new TH1F("hFluences1d_3_3 ","Output Neutron Field Density 3 ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_4  = new TH1F("hFluences1d_1_4 ","Output Neutron Field Density 4 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_4  = new TH1F("hFluences1d_2_4 ","Output Neutron Field Density 4 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_4  = new TH1F("hFluences1d_3_4 ","Output Neutron Field Density 4 ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_5  = new TH1F("hFluences1d_1_5 ","Output Neutron Field Density 5 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_5  = new TH1F("hFluences1d_2_5 ","Output Neutron Field Density 5 ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_5  = new TH1F("hFluences1d_3_5 ","Output Neutron Field Density 5 ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_6  = new TH1F("hFluences1d_1_5 ","Output Neutron Field Density Before Cells ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_6  = new TH1F("hFluences1d_2_5 ","Output Neutron Field Density Before Cells ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_6  = new TH1F("hFluences1d_3_5 ","Output Neutron Field Density Before Cells ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_7  = new TH1F("hFluences1d_1_5 ","Output Neutron Field Density After Moderator ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_7  = new TH1F("hFluences1d_2_5 ","Output Neutron Field Density After Moderator ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_7  = new TH1F("hFluences1d_3_5 ","Output Neutron Field Density After Moderator ", 30, &Fluence1D_Binning[0]);

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

        double R = sqrt(pow(X,2)+pow(Y,2));
        int R_int = (int) R/Bin_width;
        float BinArea = 3.14159 * (  pow((R_int+1)* Bin_width, 2 ) -  pow( R_int * Bin_width, 2 ) );//area of rings at diffrent r
        double EnergyEV = Energy*1e+6; // converting MeV to eV for the better readability

        if( EnergyEV < 0.5 )hNeutronFlux_vs_Z_En1 -> Fill(Zsurf, R, 1.0/BinArea);
        if( EnergyEV > 0.5 && EnergyEV < 1000 ) hNeutronFlux_vs_Z_En2 -> Fill(Zsurf, R, 1.0/BinArea);
        if( EnergyEV > 1000 ) hNeutronFlux_vs_Z_En3 -> Fill(Zsurf, R, 1.0/BinArea);


         if(fabs(X)<150 && fabs(Y)<150){

           hSpectrum2D->Fill(Zsurf, Energy);
           hSpectrum -> Fill(Energy);
           if(Zsurf>=110 && Zsurf<=112)  {
             if( EnergyEV < 0.5 )hFluence1D_1_7 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_7 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_7 -> Fill(R, 1.0/BinArea);
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
            if(R >= 75 && R < 100) hFluence_phi_3-> Fill(phi);}

	   if(Zsurf>=138 && Zsurf<=140)  {
             if( EnergyEV < 0.5 )hFluence1D_1_6 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_6 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_6 -> Fill(R, 1.0/BinArea);}

           if(Zsurf>=140 && Zsurf<=150)  {
             if( EnergyEV < 0.5 )hFluence1D_1_1 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_1 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_1 -> Fill(R, 1.0/BinArea);}

	   if(Zsurf>=150 && Zsurf<=160)  {
             if( EnergyEV < 0.5 )hFluence1D_1_2 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_2 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_2 -> Fill(R, 1.0/BinArea);}

	   if(Zsurf>=160 && Zsurf<=170)  {
             if( EnergyEV < 0.5 )hFluence1D_1_3 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_3 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_3 -> Fill(R, 1.0/BinArea);}

	   if(Zsurf>=170 && Zsurf<=180)  {
             if( EnergyEV < 0.5 )hFluence1D_1_4 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_4 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_4 -> Fill(R, 1.0/BinArea);}

	   if(Zsurf>=180 && Zsurf<=190)  {
             if( EnergyEV < 0.5 )hFluence1D_1_5 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 1000 ) hFluence1D_2_5 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 1000 ) hFluence1D_3_5 -> Fill(R, 1.0/BinArea);}
     }
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

TCanvas *a1 = new TCanvas("a1", "a1", 960, 720);
hFluence1D_1_1->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_1->GetYaxis()->SetNdivisions(505);
hFluence1D_1_1->GetXaxis()->CenterTitle();
hFluence1D_1_1->GetYaxis()->CenterTitle();
hFluence1D_1_1->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_1->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_1->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_1->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_1->GetYaxis()->SetLabelFont(42);
hFluence1D_1_1->GetYaxis()->SetTitleFont(42);
hFluence1D_1_1->GetXaxis()->SetTitleFont(42);
hFluence1D_1_1->Draw();

hFluence1D_1_1->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_1->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_1->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_1->Draw(ModelAttr[0].option);

hFluence1D_2_1->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_1->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_1->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_1->Draw(ModelAttr[1].option);

hFluence1D_3_1->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_1->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_1->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_1->Draw(ModelAttr[2].option);

TLegend* legenda1 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda1->SetTextSize(0.040);
legenda1->SetTextFont(42);
legenda1->AddEntry(hFluence1D_1_1, "E_{n} #leq 0.5 eV", "l");
legenda1->AddEntry(hFluence1D_2_1, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda1->AddEntry(hFluence1D_3_1, "E_{n} > 1 keV","l");
legenda1->SetFillColor(kWhite);
legenda1->SetLineColor(kWhite);
legenda1->Draw();

TCanvas *a2 = new TCanvas("a2", "a2", 960, 720);
hFluence1D_1_2->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_2->GetYaxis()->SetNdivisions(505);
hFluence1D_1_2->GetXaxis()->CenterTitle();
hFluence1D_1_2->GetYaxis()->CenterTitle();
hFluence1D_1_2->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_2->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_2->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_2->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_2->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_2->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_2->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_2->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_2->GetYaxis()->SetLabelFont(42);
hFluence1D_1_2->GetYaxis()->SetTitleFont(42);
hFluence1D_1_2->GetXaxis()->SetTitleFont(42);
hFluence1D_1_2->Draw();

hFluence1D_1_2->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_2->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_2->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_2->Draw(ModelAttr[0].option);

hFluence1D_2_2->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_2->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_2->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_2->Draw(ModelAttr[1].option);

hFluence1D_3_2->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_2->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_2->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_2->Draw(ModelAttr[2].option);

TLegend* legenda2 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda2->SetTextSize(0.040);
legenda2->SetTextFont(42);
legenda2->AddEntry(hFluence1D_1_2, "E_{n} #leq 0.5 eV", "l");
legenda2->AddEntry(hFluence1D_2_2, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda2->AddEntry(hFluence1D_3_2, "E_{n} > 1 keV","l");
legenda2->SetFillColor(kWhite);
legenda2->SetLineColor(kWhite);
legenda2->Draw();

TCanvas *a3 = new TCanvas("a3", "a3", 960, 720);
hFluence1D_1_3->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_3->GetYaxis()->SetNdivisions(505);
hFluence1D_1_3->GetXaxis()->CenterTitle();
hFluence1D_1_3->GetYaxis()->CenterTitle();
hFluence1D_1_3->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_3->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_3->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_3->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_3->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_3->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_3->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_3->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_3->GetYaxis()->SetLabelFont(42);
hFluence1D_1_3->GetYaxis()->SetTitleFont(42);
hFluence1D_1_3->GetXaxis()->SetTitleFont(42);
hFluence1D_1_3->Draw();

hFluence1D_1_3->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_3->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_3->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_3->Draw(ModelAttr[0].option);

hFluence1D_2_3->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_3->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_3->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_3->Draw(ModelAttr[1].option);

hFluence1D_3_3->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_3->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_3->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_3->Draw(ModelAttr[2].option);

TLegend* legenda3 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda3->SetTextSize(0.040);
legenda3->SetTextFont(42);
legenda3->AddEntry(hFluence1D_1_3, "E_{n} #leq 0.5 eV", "l");
legenda3->AddEntry(hFluence1D_2_3, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda3->AddEntry(hFluence1D_3_3, "E_{n} > 1 keV","l");
legenda3->SetFillColor(kWhite);
legenda3->SetLineColor(kWhite);
legenda3->Draw();

TCanvas *a4 = new TCanvas("a4", "a4", 960, 720);
hFluence1D_1_4->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_4->GetYaxis()->SetNdivisions(505);
hFluence1D_1_4->GetXaxis()->CenterTitle();
hFluence1D_1_4->GetYaxis()->CenterTitle();
hFluence1D_1_4->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_4->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_4->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_4->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_4->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_4->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_4->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_4->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_4->GetYaxis()->SetLabelFont(42);
hFluence1D_1_4->GetYaxis()->SetTitleFont(42);
hFluence1D_1_4->GetXaxis()->SetTitleFont(42);
hFluence1D_1_4->Draw();

hFluence1D_1_4->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_4->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_4->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_4->Draw(ModelAttr[0].option);

hFluence1D_2_4->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_4->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_4->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_4->Draw(ModelAttr[1].option);

hFluence1D_3_4->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_4->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_4->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_4->Draw(ModelAttr[2].option);

TLegend* legenda4 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda4->SetTextSize(0.040);
legenda4->SetTextFont(42);
legenda4->AddEntry(hFluence1D_1_4, "E_{n} #leq 0.5 eV", "l");
legenda4->AddEntry(hFluence1D_2_4, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda4->AddEntry(hFluence1D_3_4, "E_{n} > 1 keV","l");
legenda4->SetFillColor(kWhite);
legenda4->SetLineColor(kWhite);
legenda4->Draw();

TCanvas *a5 = new TCanvas("a5", "a5", 960, 720);
hFluence1D_1_5->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_5->GetYaxis()->SetNdivisions(505);
hFluence1D_1_5->GetXaxis()->CenterTitle();
hFluence1D_1_5->GetYaxis()->CenterTitle();
hFluence1D_1_5->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_5->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_5->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_5->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_5->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_5->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_5->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_5->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_5->GetYaxis()->SetLabelFont(42);
hFluence1D_1_5->GetYaxis()->SetTitleFont(42);
hFluence1D_1_5->GetXaxis()->SetTitleFont(42);
hFluence1D_1_5->Draw();

hFluence1D_1_5->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_5->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_5->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_5->Draw(ModelAttr[0].option);

hFluence1D_2_5->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_5->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_5->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_5->Draw(ModelAttr[1].option);

hFluence1D_3_5->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_5->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_5->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_5->Draw(ModelAttr[2].option);

TLegend* legenda5 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda5->SetTextSize(0.040);
legenda5->SetTextFont(42);
legenda5->AddEntry(hFluence1D_1_5, "E_{n} #leq 0.5 eV", "l");
legenda5->AddEntry(hFluence1D_2_5, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda5->AddEntry(hFluence1D_3_5, "E_{n} > 1 keV","l");
legenda5->SetFillColor(kWhite);
legenda5->SetLineColor(kWhite);
legenda5->Draw();

TCanvas *a6 = new TCanvas("a6", "a6", 960, 720);
hFluence1D_1_6->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_6->GetYaxis()->SetNdivisions(505);
hFluence1D_1_6->GetXaxis()->CenterTitle();
hFluence1D_1_6->GetYaxis()->CenterTitle();
hFluence1D_1_6->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_6->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_6->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_6->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_6->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_6->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_6->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_6->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_6->GetYaxis()->SetLabelFont(42);
hFluence1D_1_6->GetYaxis()->SetTitleFont(42);
hFluence1D_1_6->GetXaxis()->SetTitleFont(42);
hFluence1D_1_6->Draw();

hFluence1D_1_6->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_6->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_6->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_6->Draw(ModelAttr[0].option);

hFluence1D_2_6->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_6->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_6->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_6->Draw(ModelAttr[1].option);

hFluence1D_3_6->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_6->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_6->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_6->Draw(ModelAttr[2].option);

TLegend* legenda6 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda6->SetTextSize(0.040);
legenda6->SetTextFont(42);
legenda6->AddEntry(hFluence1D_1_6, "E_{n} #leq 0.5 eV", "l");
legenda6->AddEntry(hFluence1D_2_6, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda6->AddEntry(hFluence1D_3_6, "E_{n} > 1 keV","l");
legenda6->SetFillColor(kWhite);
legenda6->SetLineColor(kWhite);
legenda6->Draw();

TCanvas *a7 = new TCanvas("a7", "a7", 960, 720);
hFluence1D_1_7->GetYaxis()->SetTickLength(0.02);
hFluence1D_1_7->GetYaxis()->SetNdivisions(505);
hFluence1D_1_7->GetXaxis()->CenterTitle();
hFluence1D_1_7->GetYaxis()->CenterTitle();
hFluence1D_1_7->GetYaxis()->SetTitle("#frac{dN}{dr^{2}}");
hFluence1D_1_7->GetXaxis()->SetTitle("r (mm)");
hFluence1D_1_7->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_7->GetYaxis()->SetTitleOffset(1.2);
hFluence1D_1_7->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence1D_1_7->GetXaxis()->SetTitleOffset(1.0);
hFluence1D_1_7->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_7->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence1D_1_7->GetYaxis()->SetLabelFont(42);
hFluence1D_1_7->GetYaxis()->SetTitleFont(42);
hFluence1D_1_7->GetXaxis()->SetTitleFont(42);
hFluence1D_1_7->Draw();

hFluence1D_1_7->SetLineWidth(ModelAttr[0].width);
hFluence1D_1_7->SetLineColor(ModelAttr[0].colour);
hFluence1D_1_7->SetLineStyle(ModelAttr[0].LineStyle);
hFluence1D_1_7->Draw(ModelAttr[0].option);

hFluence1D_2_7->SetLineWidth(ModelAttr[1].width);
hFluence1D_2_7->SetLineColor(ModelAttr[1].colour);
hFluence1D_2_7->SetLineStyle(ModelAttr[1].LineStyle);
hFluence1D_2_7->Draw(ModelAttr[1].option);

hFluence1D_3_7->SetLineWidth(ModelAttr[2].width);
hFluence1D_3_7->SetLineColor(ModelAttr[2].colour);
hFluence1D_3_7->SetLineStyle(ModelAttr[2].LineStyle);
hFluence1D_3_7->Draw(ModelAttr[2].option);

TLegend* legenda7 = new TLegend(0.50, 0.70, 0.80, 0.90);
legenda7->SetTextSize(0.040);
legenda7->SetTextFont(42);
legenda7->AddEntry(hFluence1D_1_7, "E_{n} #leq 0.5 eV", "l");
legenda7->AddEntry(hFluence1D_2_7, "0.5 eV < E_{n} #leq 1 keV", "l");
legenda7->AddEntry(hFluence1D_3_7, "E_{n} > 1 keV","l");
legenda7->SetFillColor(kWhite);
legenda7->SetLineColor(kWhite);
legenda7->Draw();


TCanvas *c5 = new TCanvas("c45", "c5", 960, 720);

hFluence_phi_1->GetYaxis()->SetTickLength(0.02);
hFluence_phi_1->GetYaxis()->SetNdivisions(505);
hFluence_phi_1->GetXaxis()->CenterTitle();
hFluence_phi_1->GetYaxis()->CenterTitle();
hFluence_phi_1->GetYaxis()->SetTitle("#frac{dN}{d#phi}");
hFluence_phi_1->GetXaxis()->SetTitle("#phi (rad)");
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
hFluence_phi_1->Draw(ModelAttr[0].option);

hFluence_phi_2->SetLineWidth(ModelAttr[1].width);
hFluence_phi_2->SetLineColor(ModelAttr[1].colour);
hFluence_phi_2->SetLineStyle(ModelAttr[1].LineStyle);
hFluence_phi_2->Draw(ModelAttr[1].option);

hFluence_phi_3->SetLineWidth(ModelAttr[2].width);
hFluence_phi_3->SetLineColor(ModelAttr[2].colour);
hFluence_phi_3->SetLineStyle(ModelAttr[2].LineStyle);
hFluence_phi_3->Draw(ModelAttr[2].option);


TLegend* legend_2 = new TLegend(0.50, 0.70, 0.80, 0.90);
legend_2->SetTextSize(0.040);
legend_2->SetTextFont(42);
legend_2->AddEntry(hFluence_phi_1, "r #leq 50 mm", "l");
legend_2->AddEntry(hFluence_phi_2, "50 < r #leq 75 mm", "l");
legend_2->AddEntry(hFluence_phi_3, "75 < r #leq 100 mm","l");
legend_2->SetFillColor(kWhite);
legend_2->SetLineColor(kWhite);
legend_2->Draw();


fout->WriteObject(hFluence_phi_1, "Fluence_phi_1");
fout->WriteObject(hFluence_phi_2, "Fluence_phi_2");
fout->WriteObject(hFluence_phi_3, "Fluence_phi_3");

fout->WriteObject(hFluence1D_1_1, "hFluence1D_1_1");
fout->WriteObject(hFluence1D_2_1, "hFluence1D_2_1");
fout->WriteObject(hFluence1D_3_1, "hFluence1D_3_1");

fout->WriteObject(hFluence1D_1_2, "hFluence1D_1_2");
fout->WriteObject(hFluence1D_2_2, "hFluence1D_2_2");
fout->WriteObject(hFluence1D_3_2, "hFluence1D_3_2");

fout->WriteObject(hFluence1D_1_3, "hFluence1D_1_3");
fout->WriteObject(hFluence1D_2_3, "hFluence1D_2_3");
fout->WriteObject(hFluence1D_3_3, "hFluence1D_3_3");

fout->WriteObject(hFluence1D_1_4, "hFluence1D_1_4");
fout->WriteObject(hFluence1D_2_4, "hFluence1D_2_4");
fout->WriteObject(hFluence1D_3_4, "hFluence1D_3_4");

fout->WriteObject(hFluence1D_1_5, "hFluence1D_1_5");
fout->WriteObject(hFluence1D_2_5, "hFluence1D_2_5");
fout->WriteObject(hFluence1D_3_5, "hFluence1D_3_5");

fout->WriteObject(hFluence1D_1_6, "hFluence1D_1_6");
fout->WriteObject(hFluence1D_2_6, "hFluence1D_2_6");
fout->WriteObject(hFluence1D_3_6, "hFluence1D_3_6");

fout->WriteObject(hFluence1D_1_7, "hFluence1D_1_7");
fout->WriteObject(hFluence1D_2_7, "hFluence1D_2_7");
fout->WriteObject(hFluence1D_3_7, "hFluence1D_3_7");

fout->WriteObject(hSpectrum2D, "Spectrum2D");
fout->WriteObject(hSpectrum, "Spectrum1D");


fout->WriteObject(hFluence2D , "Fluence2D");

fout->WriteObject(hNeutronFlux_vs_Z_En1 , "NeutronFlux_vs_Z_En1");
fout->WriteObject(hNeutronFlux_vs_Z_En2 , "NeutronFlux_vs_Z_En2");
fout->WriteObject(hNeutronFlux_vs_Z_En3 , "NeutronFlux_vs_Z_En3");

}
