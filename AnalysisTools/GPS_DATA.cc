{
TFile *f1 = new TFile("/home/razdrogova/buils/output_29092025_134600.root");
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

   TTree *t1 = (TTree*)f1->Get("Produced particles");
   double X, Y, En, Z, p_x, p_y, p_z;
   int particle_id;
   char particle_name[50];

   t1->SetBranchAddress("X",&X);
   t1->SetBranchAddress("Y",&Y);
   t1->SetBranchAddress("En",&En);
   t1->SetBranchAddress("Z",&Z);
   t1->SetBranchAddress("Particle_Id",&particle_id);
   t1->SetBranchAddress("Particle_Name",&particle_name);
   t1->SetBranchAddress("p_x",&p_x);
   t1->SetBranchAddress("p_y",&p_y);
   t1->SetBranchAddress("p_z",&p_z);

   vector<float> Energy1D_Binning;
   float width = 1.5e-3; //up to 300keV
   for(int i =0; i<=200; i++ ) Energy1D_Binning.push_back(i*width);

   vector<float> Fluence1D_Binning;
   float Bin_width = 5.0;
   for(int i =0; i<=50; i++ ) Fluence1D_Binning.push_back(i*Bin_width);

   vector<float> Fluence1D_AzimuBinning;
   int N_az_bin = 100;
   float Azmizth_Bin_width = 3.14159/N_az_bin;
   for(int i =0; i<=N_az_bin; i++ ) Fluence1D_AzimuBinning.push_back(i*Azmizth_Bin_width);
    
   TH2F  *hFluence  = new TH2F("hFluence","Output Neutron Field Density",(int)Energy1D_Binning.size() - 1 , &Energy1D_Binning[0], 100, &Fluence1D_AzimuBinning[0]);
    
   int nentries = (int)t1->GetEntries();
   for (int i=0; i<nentries; i++) {
     t1 -> GetEntry(i);

        double R = sqrt(pow(X,2)+pow(Y,2));
        int R_int = (int) R/Bin_width;
        float BinArea = 3.14159 * (  pow((R_int+1)* Bin_width, 2 ) -  pow( R_int * Bin_width, 2 ) );
        double EnergyEV = En*1e+6;

       if(fabs(X)<250 && fabs(Y)<250){
           if(Z>=-0.03 && Z<=0.03)  {
               float NeutonTan;
               if(p_z!=0) NeutonTan = sqrt(p_x*p_x+p_y*p_y)/fabs(p_z);
               else NeutonTan = sqrt(p_x*p_x+p_y*p_y)/(p_z+0.0001);
               float theta =  atan(NeutonTan);
               if (p_z < 0) theta = 3.14159 - theta;
               hFluence -> Fill(En, theta);
           }
       }
    }
TCanvas *c = new TCanvas("c", "c", 960, 720);
hFluence->GetYaxis()->SetTickLength(0.02);
hFluence->GetYaxis()->SetNdivisions(505);
hFluence->GetXaxis()->SetTickLength(0.02);
hFluence->GetXaxis()->SetNdivisions(505);
hFluence->GetXaxis()->CenterTitle();
hFluence->GetYaxis()->CenterTitle();
hFluence->GetXaxis()->SetTitle("Energy, MeV");
hFluence->GetYaxis()->SetTitle("#theta, rad");
hFluence->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence->GetYaxis()->SetTitleOffset(1.2);
hFluence->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
hFluence->GetXaxis()->SetTitleOffset(1.0);
hFluence->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
hFluence->GetYaxis()->SetLabelFont(42);
hFluence->GetYaxis()->SetTitleFont(42);
hFluence->GetXaxis()->SetTitleFont(42);
hFluence->SetLineWidth(ModelAttr[0].width);
hFluence->SetLineColor(ModelAttr[0].colour);
hFluence->SetLineStyle(ModelAttr[0].LineStyle);
hFluence->Draw(ModelAttr[0].option);

fout->WriteObject(hFluence , "hFluence");
    
}
