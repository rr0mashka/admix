{
TFile *f1 = new TFile("/Users/yana/geant4-projects/output.root");
    TFile* fout = new TFile("./HarverstedHistos.root", "RECREATE");
    
  gROOT->Reset();
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.12);

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

   vector<float> Fluence1D_Binning;
   float Bin_width = 5.0;
   for(int i =0; i<=30; i++ ) Fluence1D_Binning.push_back(i*Bin_width);
   TH1F  *hFluence1D_1_1  = new TH1F("hFluences1d_1_1 ","Neutron Field Density in Target 2mm ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_1  = new TH1F("hFluences1d_2_1 ","Neutron Field Density in Target 2mm ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_1  = new TH1F("hFluences1d_3_1 ","Neutron Field Density in Target 2mm ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_2  = new TH1F("hFluences1d_1_2 ","Neutron Field Density in Target 2mm ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_2  = new TH1F("hFluences1d_2_2 ","Neutron Field Density in Target 2mm ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_2  = new TH1F("hFluences1d_3_2 ","Neutron Field Density in Target 2mm ", 30, &Fluence1D_Binning[0]);

   TH1F  *hFluence1D_1_3  = new TH1F("hFluences1d_1_3 ","Neutron Field Density Before Target 2mm ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_2_3  = new TH1F("hFluences1d_2_3 ","Neutron Field Density Before Target 2mm ", 30, &Fluence1D_Binning[0]);
   TH1F  *hFluence1D_3_3  = new TH1F("hFluences1d_3_3 ","Neutron Field Density Before Target 2mm ", 30, &Fluence1D_Binning[0]);

   vector<float> Fluence1D_AzimuBinning;
   int N_az_bin = 20;
   float Azmizth_Bin_width = 2*3.14159/N_az_bin;
   for(int i =0; i<=N_az_bin; i++ ) Fluence1D_AzimuBinning.push_back(i*Azmizth_Bin_width);

   int nentries = (int)t1->GetEntries();
   for (int i=0; i<nentries; i++) {
     t1 -> GetEntry(i);

        double R = sqrt(pow(X,2)+pow(Y,2));
        int R_int = (int) R/Bin_width;
        float BinArea = 3.14159 * (  pow((R_int+1)* Bin_width, 2 ) -  pow( R_int * Bin_width, 2 ) );//area of rings at diffrent r
        double EnergyEV = Energy*1e+6; // converting MeV to eV for the better readability

         if(fabs(X)<150 && fabs(Y)<150){

            if(Zsurf>=1 && Zsurf<4)  {
             if( EnergyEV < 0.5 )hFluence1D_1_1 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 0.5 && EnergyEV < 10000 ) hFluence1D_2_1 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 10000 ) hFluence1D_3_1 -> Fill(R, 1.0/BinArea);}

	   if(Zsurf>=1 && Zsurf<4)  {
             if( EnergyEV < 100000 )hFluence1D_1_2 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 100000 && EnergyEV < 200000 ) hFluence1D_2_2 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 200000 ) hFluence1D_3_2 -> Fill(R, 1.0/BinArea);}

	   if(Zsurf>=-1 && Zsurf<1)  {
             if( EnergyEV < 100000 )hFluence1D_1_3 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 100000 && EnergyEV < 200000 ) hFluence1D_2_3 -> Fill(R, 1.0/BinArea);
             if( EnergyEV > 200000 ) hFluence1D_3_3 -> Fill(R, 1.0/BinArea);}
   }
       }
    
    float N_protons = 37.77e9;  //number of  incedent protons
    float Current = 1.6e-19 * N_protons*1000; // accelarator cuccrent  in mA
    float sqMM_2_sqCM =100; //conversion factor for square mm to square cm*/
    
    hFluence1D_1_1 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_2_1 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_3_1 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_1_2 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_2_2 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_3_2 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_1_3 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_2_3 ->Scale(sqMM_2_sqCM/Current);
    hFluence1D_3_3 ->Scale(sqMM_2_sqCM/Current);

//====================  Making plots   =========================================

    TCanvas *y = new TCanvas("y", "y", 2160, 500);
    y->Divide(3,1);
    
    y->cd(1);
    
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
    hFluence1D_1_1->GetYaxis()->SetRangeUser(0, 80e9);
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
    legenda1->AddEntry(hFluence1D_2_1, "0.5 eV < E_{n} #leq 10 keV", "l");
    legenda1->AddEntry(hFluence1D_3_1, "E_{n} > 10 keV","l");
    legenda1->SetFillColor(kWhite);
    legenda1->SetLineColor(kWhite);
    legenda1->Draw();
    
    y->cd(2);
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
    hFluence1D_1_2->GetYaxis()->SetRangeUser(0, 36e9);
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
    legenda2->AddEntry(hFluence1D_1_2, "E_{n} #leq 100 keV", "l");
    legenda2->AddEntry(hFluence1D_2_2, "100 keV < E_{n} #leq 200 keV", "l");
    legenda2->AddEntry(hFluence1D_3_2, "E_{n} > 200 keV","l");
    legenda2->SetFillColor(kWhite);
    legenda2->SetLineColor(kWhite);
    legenda2->Draw();
    
    y->cd(3);
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
    hFluence1D_1_3->GetYaxis()->SetRangeUser(0, 5.4e9);
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
    legenda3->AddEntry(hFluence1D_1_3, "E_{n} #leq 100 keV", "l");
    legenda3->AddEntry(hFluence1D_2_3, "100 keV < E_{n} #leq 200 keV", "l");
    legenda3->AddEntry(hFluence1D_3_3, "E_{n} > 200 keV","l");
    legenda3->SetFillColor(kWhite);
    legenda3->SetLineColor(kWhite);
    legenda3->Draw();
    
    fout->WriteObject(hFluence1D_1_1, "hFluence1D_1_1");
    fout->WriteObject(hFluence1D_2_1, "hFluence1D_2_1");
    fout->WriteObject(hFluence1D_3_1, "hFluence1D_3_1");

    fout->WriteObject(hFluence1D_1_2, "hFluence1D_1_2");
    fout->WriteObject(hFluence1D_2_2, "hFluence1D_2_2");
    fout->WriteObject(hFluence1D_3_2, "hFluence1D_3_2");

    fout->WriteObject(hFluence1D_1_3, "hFluence1D_1_3");
    fout->WriteObject(hFluence1D_2_3, "hFluence1D_2_3");
    fout->WriteObject(hFluence1D_3_3, "hFluence1D_3_3");

}
