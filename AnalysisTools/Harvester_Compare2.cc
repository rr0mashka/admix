#include <filesystem>


namespace fs = filesystem;
string pathtodata = "/home/martin/geant4/MedPhys/BNCT/devYanaStacking/BNCT-build";

double GetError(vector<double> vect){
  double mean = 0;
  double disp = 0;
  if (vect.size()<2) return 0;
  for (int i=0;i<vect.size();i++){
    mean +=vect.at(i)/vect.size();
  }
  for (int i=0;i<vect.size();i++){
    disp+= (mean-vect.at(i))*(mean-vect.at(i))/(vect.size()-1);
  }
  return sqrt(disp);
}

vector<string> splitString(string str, char splitter){
    vector<std::string> result = {};
    string current = "";
    for(int i = 0; i < str.size(); i++){
        if(str[i] == splitter){
            if(current != ""){
                result.push_back(current);
                current = "";
            }
            continue;
        }
        current += str[i];
    }
    if(current.size() != 0)
        result.push_back(current);
    return result;
};




vector<string> LastFiles(string path){
  vector<string> out = {};
  vector<string> temp = {};
  string strtemp;
  string date;
  vector<string> filesplit;
  vector<string> file2split;
  for (const auto & entry : fs::directory_iterator(path)){
      filesplit = splitString(entry.path(), '.');
      if (filesplit.size()<2) continue;
      if (filesplit[1] == "root") {
        strtemp = filesplit[0];
        file2split = splitString(strtemp, '_');
        if (file2split.size()<3) continue;
        date = file2split[file2split.size()-2] + "_" + file2split[file2split.size()-1];
        temp.push_back(date);
      };
  };
  if (temp.size()<2) {
    return {};
  };
  sort(temp.begin(), temp.end());
// get last two files for comparison
  for (int i=0; i<2; i++){
      for (const auto & entry : fs::directory_iterator(path)){
        filesplit = splitString(entry.path(), '.');
        if (filesplit[1] == "root") {
          strtemp = filesplit[0];
          file2split = splitString(strtemp, '_');
          if (file2split.size()<3) continue;
          date = file2split[file2split.size()-2] + "_" + file2split[file2split.size()-1];
          if (date == temp[temp.size()-2+i]){
            out.push_back(entry.path());
          };
        };
      };
  };
  return out;
};

void Harvester_Compare2() {
      vector<string> filesvect = LastFiles(pathtodata);
      vector<double> EdepVectIn = {};
      vector<double> EdepBoronVectIn = {};
      vector<double> EdepVectOut = {};
      vector<double> EdepBoronVectOut = {};
      gStyle->SetErrorX(0.);
      if(filesvect.size()<2){
        cout << "Not enough data in your folder" << endl;
        return;
      };
      TFile *f1 = new TFile(filesvect.at(0).c_str());
      TFile *f2 = new TFile(filesvect.at(1).c_str());
      vector<string> spl1, spl2;
      spl1 = splitString(splitString(filesvect.at(0),'.')[0],'_');
      spl2 = splitString(splitString(filesvect.at(1),'.')[0],'_');
      if ((spl1.size()<3) || (spl2.size()<3)) {
        cout << "WTF!! Naming conventions are wrong!!" << endl;
        return;
      }
      string binlab1 = spl1[spl1.size()-3];
      string binlab2 = spl2[spl2.size()-3];
      TFile* fout = new TFile("./data/HarverstHistos.root", "RECREATE");


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

         TH1F  *hDoseProfile_1_1 = new TH1F("hDoseProfile_1_1","Edep Outer Tubes",2,0,2);
         TH1F  *hDoseProfile_1_2 = new TH1F("hDoseProfile_1_2","Edep Inner Tubes",2,0,2);

         TH1F  *hBoronDoseProfile_1_1 = new TH1F("hBoronDoseProfile_1_1","Edep Boron Outer Tubes",2,0,2);
         TH1F  *hBoronDoseProfile_1_2 = new TH1F("hBoronDoseProfile_1_2","Edep Boron Inner Tubes",2,0,2);

         int nentries = (int)t1->GetEntries();
         for (int i=0; i<nentries; i++) {
         t1 -> GetEntry(i);
         float R_ = sqrt(X*X + Y*Y);
            if(R_ < 58 && R_ >42){
              hDoseProfile_1_1 -> Fill(binlab1.c_str(), Edep/16);
              hBoronDoseProfile_1_1 -> Fill(binlab1.c_str(), EdepBoron/16);
              EdepVectOut.push_back(Edep);
              EdepBoronVectOut.push_back(EdepBoron);
            };

            if(X*X+Y*Y<1090 && X*X+Y*Y>288){
              hDoseProfile_1_2 -> Fill(binlab1.c_str(), Edep/8);
              hBoronDoseProfile_1_2 -> Fill(binlab1.c_str(), EdepBoron/8);
              EdepVectIn.push_back(Edep);
              EdepBoronVectIn.push_back(EdepBoron);
            };
      }

      hDoseProfile_1_1->SetBinError (1, GetError(EdepVectOut));
      hBoronDoseProfile_1_1->SetBinError (1, GetError(EdepBoronVectOut));
      hDoseProfile_1_2->SetBinError (1, GetError(EdepVectIn));
      hBoronDoseProfile_1_2->SetBinError (1, GetError(EdepBoronVectIn));


      EdepVectIn = {};
      EdepBoronVectIn = {};
      EdepVectOut = {};
      EdepBoronVectOut = {};

      TTree *t2 = (TTree*)f2->Get("Doses in phantom");

      t2->SetBranchAddress("X",&X);
      t2->SetBranchAddress("Y",&Y);
      t2->SetBranchAddress("Z",&Z);
      t2->SetBranchAddress("Edep",&Edep);
      t2->SetBranchAddress("EdepBoron",&EdepBoron);


      nentries = (int)t2->GetEntries();
      for (int i=0; i<nentries; i++) {
      t2 -> GetEntry(i);
      float R_ = sqrt(X*X + Y*Y);
         if(R_ < 58 && R_ >42){
           hDoseProfile_1_1 -> Fill(binlab2.c_str(), Edep/16);
           hBoronDoseProfile_1_1 -> Fill(binlab2.c_str(), EdepBoron/16);
           EdepVectOut.push_back(Edep);
           EdepBoronVectOut.push_back(EdepBoron);
          }

         if(X*X+Y*Y<1090 && X*X+Y*Y>288){
           hDoseProfile_1_2 -> Fill(binlab2.c_str(), Edep/8);
           hBoronDoseProfile_1_2 -> Fill(binlab2.c_str(), EdepBoron/8);
           EdepVectIn.push_back(Edep);
           EdepBoronVectIn.push_back(EdepBoron);
         }
      }

      hDoseProfile_1_1->SetBinError (2, GetError(EdepVectOut));
      hBoronDoseProfile_1_1->SetBinError (2, GetError(EdepBoronVectOut));
      hDoseProfile_1_2->SetBinError (2, GetError(EdepVectIn));
      hBoronDoseProfile_1_2->SetBinError (2, GetError(EdepBoronVectIn));


      //====================  Making plots   =========================================


      TCanvas *c2 = new TCanvas("c2", "c2", 960, 720);

      hDoseProfile_1_1->GetYaxis()->SetTickLength(0.02);
      hDoseProfile_1_1->GetYaxis()->SetNdivisions(505);
      hDoseProfile_1_1->GetXaxis()->CenterTitle();
      hDoseProfile_1_1->GetYaxis()->CenterTitle();
      hDoseProfile_1_1->GetXaxis()->SetTitle("Dose Contributions");
      hDoseProfile_1_1->GetYaxis()->SetTitle("Edep, MeV");
      hDoseProfile_1_1->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
      hDoseProfile_1_1->GetYaxis()->SetTitleOffset(1.2);
      hDoseProfile_1_1->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
      hDoseProfile_1_1->GetXaxis()->SetTitleOffset(1.0);
      hDoseProfile_1_1->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
      hDoseProfile_1_1->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
      hDoseProfile_1_1->GetYaxis()->SetLabelFont(42);
      hDoseProfile_1_1->GetYaxis()->SetTitleFont(42);
      hDoseProfile_1_1->GetXaxis()->SetTitleFont(42);
      hDoseProfile_1_1->SetLineColor(ModelAttr[0].colour);
      hDoseProfile_1_1->SetLineWidth(ModelAttr[0].width);
      hDoseProfile_1_1->SetMarkerStyle(ModelAttr[0].marker);
      hDoseProfile_1_1->SetMarkerSize(ModelAttr[0].size);
      hDoseProfile_1_1->SetMarkerColor(ModelAttr[0].colour);
      hDoseProfile_1_1->GetYaxis()->SetRangeUser(hBoronDoseProfile_1_1->GetBinContent(1)-0.4*hBoronDoseProfile_1_1->GetBinContent(1),hDoseProfile_1_1->GetBinContent(1)+0.4*hDoseProfile_1_1->GetBinContent(1));
      hDoseProfile_1_1->Draw();
      hBoronDoseProfile_1_1->SetLineColor(ModelAttr[1].colour);
      hBoronDoseProfile_1_1->SetLineWidth(ModelAttr[1].width);
      hBoronDoseProfile_1_1->SetMarkerStyle(ModelAttr[1].marker);
      hBoronDoseProfile_1_1->SetMarkerSize(ModelAttr[1].size);
      hBoronDoseProfile_1_1->SetMarkerColor(ModelAttr[1].colour);
      hBoronDoseProfile_1_1->Draw("SAME");

      TLegend* legend2 = new TLegend(0.50, 0.65, 0.7, 0.80);//
      legend2->SetTextSize(0.036);
      legend2->SetTextFont(42);
      legend2->SetHeader("Energies:");
      legend2->AddEntry(hDoseProfile_1_1, "Full energy deposition");
      legend2->AddEntry(hBoronDoseProfile_1_1, "Boron energy deposition");
      legend2->SetFillColor(kWhite);
      legend2->SetLineColor(kWhite);
      legend2->Draw();

      TCanvas *c1 = new TCanvas("c1", "c1", 960, 720);

      hDoseProfile_1_2->GetYaxis()->SetTickLength(0.02);
      hDoseProfile_1_2->GetYaxis()->SetNdivisions(505);
      hDoseProfile_1_2->GetXaxis()->CenterTitle();
      hDoseProfile_1_2->GetYaxis()->CenterTitle();
      hDoseProfile_1_2->GetXaxis()->SetTitle("Dose Contributions");
      hDoseProfile_1_2->GetYaxis()->SetTitle("Edep, MeV");
      hDoseProfile_1_2->GetYaxis()->SetTitleSize(0.045*TextSizeScale);
      hDoseProfile_1_2->GetYaxis()->SetTitleOffset(1.2);
      hDoseProfile_1_2->GetXaxis()->SetTitleSize(0.045*TextSizeScale);
      hDoseProfile_1_2->GetXaxis()->SetTitleOffset(1.0);
      hDoseProfile_1_2->GetYaxis()->SetLabelSize(0.04*TextSizeScale);
      hDoseProfile_1_2->GetXaxis()->SetLabelSize(0.04*TextSizeScale);
      hDoseProfile_1_2->GetYaxis()->SetLabelFont(42);
      hDoseProfile_1_2->GetYaxis()->SetTitleFont(42);
      hDoseProfile_1_2->GetXaxis()->SetTitleFont(42);
      hDoseProfile_1_2->SetLineColor(ModelAttr[0].colour);
      hDoseProfile_1_2->SetLineWidth(ModelAttr[0].width);
      hDoseProfile_1_2->SetMarkerStyle(ModelAttr[0].marker);
      hDoseProfile_1_2->SetMarkerSize(ModelAttr[0].size);
      hDoseProfile_1_2->SetMarkerColor(ModelAttr[0].colour);
      hDoseProfile_1_2->GetYaxis()->SetRangeUser(hBoronDoseProfile_1_2->GetBinContent(1)-0.4*hBoronDoseProfile_1_2->GetBinContent(1),hDoseProfile_1_2->GetBinContent(1)+0.4*hDoseProfile_1_2->GetBinContent(1));
      hDoseProfile_1_2->Draw();
      hBoronDoseProfile_1_2->SetLineColor(ModelAttr[1].colour);
      hBoronDoseProfile_1_2->SetLineWidth(ModelAttr[1].width);
      hBoronDoseProfile_1_2->SetMarkerStyle(ModelAttr[1].marker);
      hBoronDoseProfile_1_2->SetMarkerSize(ModelAttr[1].size);
      hBoronDoseProfile_1_2->SetMarkerColor(ModelAttr[1].colour);
      hBoronDoseProfile_1_2->Draw("same");



      TLegend* legend = new TLegend(0.50, 0.65, 0.7, 0.80);//
      legend->SetTextSize(0.036);
      legend->SetTextFont(42);
      legend->SetHeader("Energies:");
      legend->AddEntry(hDoseProfile_1_2, "Full energy deposition");
      legend->AddEntry(hBoronDoseProfile_1_2, "Boron energy deposition");
      legend->SetFillColor(kWhite);
      legend->SetLineColor(kWhite);
      legend->Draw();
  /*   TCanvas *c3 = new TCanvas("c3", "c3", 960, 720);

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
      c1->Write();
      c2->Write();

    //  fout->Close();
      /*fout->WriteObject(hDoseProfile_1_1, "hDoseProfile_1_1");
      fout->WriteObject(hDoseProfile_1_2, "hDoseProfile_1_2");
      fout->WriteObject(hBoronDoseProfile_1_1, "hBoronDoseProfile_1_1");
      fout->WriteObject(hBoronDoseProfile_1_2, "hBoronDoseProfile_1_2");*/
      //fout->WriteObject(hBoronDoseProfile_1, "hBoronDoseProfile_1");
}
