#include <iostream>
#include <fstream>
#include <vector>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TAttMarker.h>


void plotGraphFromFile(const std::string& filename) {
    std::vector<double> x_full, y_full, x_err_full, y_err_full;
    std::vector<double> x_boron, y_boron, x_err_boron, y_err_boron;

    std::ifstream infile(filename);

    if (!infile.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

    std::string line;
    std::getline(infile, line);

    double x_val, full_dose, boron_dose, x_err_val, full_err, boron_err;
       while (infile >> x_val >> full_dose >> boron_dose >> x_err_val >> full_err >> boron_err) {
           x_full.push_back(x_val);      // x для Full Dose
           y_full.push_back(full_dose);
           x_err_full.push_back(x_err_val);
           y_err_full.push_back(full_err);

           x_boron.push_back(x_val);    // x для Boron Dose (то же значение)
           y_boron.push_back(boron_dose);
           x_err_boron.push_back(x_err_val);
           y_err_boron.push_back(boron_err);
       }
       infile.close();

       // Проверка на пустые данные (как в предыдущем примере)

    int nPoints = x_full.size(); // или x_boron.size(), они равны

       TGraphErrors* graphFull = new TGraphErrors(nPoints, &x_full[0], &y_full[0], &x_err_full[0], &y_err_full[0]);
       TGraphErrors* graphBoron = new TGraphErrors(nPoints, &x_boron[0], &y_boron[0], &x_err_boron[0], &y_err_boron[0]);

       // Настройки для graphFull
       graphFull->SetTitle("Inner Radius Comparison Article/GPS");
     //  graphFull->GetXaxis()->SetTitle("L, mm");
       graphFull->GetXaxis()->SetTitleSize(0.055);
       graphFull->GetXaxis()->SetLabelSize(0.045);
       graphFull->GetXaxis()->SetTitleOffset(0.9);
       graphFull->GetYaxis()->SetTitle("D_{aii}, #frac{Gy}{mA*h}");
       graphFull->GetYaxis()->SetTitleSize(0.055);
       graphFull->GetYaxis()->SetLabelSize(0.045);
       graphFull->GetYaxis()->SetTitleOffset(1.15);
       graphFull->GetYaxis()->SetRangeUser(0, 5); // Если нужно, установите пределы для оси Y
       graphFull->SetMarkerStyle(20);
       graphFull->SetMarkerSize(2.5);
       graphFull->SetMarkerColor(kBlue);
       graphFull->SetLineWidth(2);
       graphFull->SetLineColor(kBlue);

       // Настройки для graphBoron (стили маркера и цвета другие)
       graphBoron->SetMarkerStyle(71);
       graphBoron->SetMarkerSize(2.5);
       graphBoron->SetMarkerColor(kRed);
       graphBoron->SetLineWidth(2);
       graphBoron->SetLineColor(kRed);


       TCanvas* canvas = new TCanvas("canvas", "Dependence", 1600, 1300);
       canvas->SetMargin(0.15, 0.1, 0.08, 0.1);
    
    // Создание легенды
       TLegend* legend = new TLegend(0.7, 0.78, 0.8, 0.88); // x1, y1, x2, y2 (координаты в процентах от холста)
       legend->AddEntry(graphFull, "Full Dose", "lep"); // "lep" - маркер, линия, ошибка
       legend->AddEntry(graphBoron, "Boron Dose", "lep");
       legend->SetTextSize(0.037);
       legend->SetBorderSize(0); // Убираем рамку у легенды


       graphFull->Draw("AP"); // Рисуем первый график с осями и точками
       graphBoron->Draw("P same"); // Добавляем второй график, только точки
    
      legend->Draw(); // Рисуем легенду поверх графиков


     canvas->SaveAs("graph1.png");

    delete legend;
    delete graphFull;
    delete graphBoron;

 }

 int main() {
     // Установка размера и шрифта заголовка *перед* созданием графика
     gStyle->SetTitleX(0.5);
     gStyle->SetTitleY(1.48);
     gStyle->SetTitleFontSize(0.9); // Размер. Настройте по своему вкусу
     gStyle->SetTitleFont(42, "t"); // Шрифт 62 для заголовка

     gStyle->SetLabelFont(42, "x"); // Шрифт для меток оси X (Helvetica Italic)
     gStyle->SetLabelFont(42, "y"); // Шрифт для меток оси Y (Helvetica Italic)


     plotGraphFromFile("Torus_Inner.txt");
     return 0;
 }
