{

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



float N_protons = 300e+6;
float Current = 1.6e-19 * N_protons*1000; // accelarator cuccrent  in mA
float sqMM_2_sqCM =100;


TFile *f = new TFile("/home/maxim/Programs/Geant4/MedPhys/BNCT/DATA/data/HaverstedHistos_1.root");

TH1F  *hFluence1D_1 = (TH1F*)f->Get("hFluence1D_1");
TH1F  *hFluence1D_2 = (TH1F*)f->Get("hFluence1D_2");
TH1F  *hFluence1D_3 = (TH1F*)f->Get("hFluence1D_3");

TH1F  *hFluence_phi_1 = (TH1F*)f->Get("Fluence_phi_1");
TH1F  *hFluence_phi_2 = (TH1F*)f->Get("Fluence_phi_2");
TH1F  *hFluence_phi_3 = (TH1F*)f->Get("Fluence_phi_3");

TH1F  *hSpectrum = (TH1F*)f->Get("Spectrum1D");
TH2F  *hSpectrum2D = (TH2F*)f->Get("Spectrum2D");
TH2F  *hFluence2D = (TH2F*)f->Get("Fluence2D");



 hFluence1D_1 ->Scale(sqMM_2_sqCM/Current);
 hFluence1D_2 ->Scale(sqMM_2_sqCM/Current);
 hFluence1D_3 ->Scale(sqMM_2_sqCM/Current);

for(int i = 1; i <= hSpectrum -> GetNbinsX(); i++){
 hSpectrum ->SetBinContent(i, hSpectrum -> GetBinContent(i)/ hSpectrum -> GetBinWidth(i));
}


//====================  Making plots   =========================================

c1 = new TCanvas("c1", "c1", 960, 720);
c1 ->SetLogy(1);
c1 ->SetLogx(1);

hSpectrum->GetYaxis()->SetTickLength(0.02);
hSpectrum->GetYaxis()->SetNdivisions(505);
hSpectrum->GetXaxis()->CenterTitle();
hSpectrum->GetYaxis()->CenterTitle();
hSpectrum->GetXaxis()->SetTitle("E, MeV");
hSpectrum->GetYaxis()->SetTitle("#frac{dN}{dE} (MeV^{-1})");
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
hFluence1D_1->GetYaxis()->SetTickLength(0.02);
hFluence1D_1->GetYaxis()->SetNdivisions(505);
hFluence1D_1->GetXaxis()->CenterTitle();
hFluence1D_1->GetYaxis()->CenterTitle();
hFluence1D_1->GetYaxis()->SetTitle("#frac{dN}{dr^{2} dI_{a}} (cm^{-2}mA^{-1})");
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
hFluence_phi_1->GetYaxis()->SetTitle("#frac{dN}{d#phi} (rad^{-1})");
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


}
