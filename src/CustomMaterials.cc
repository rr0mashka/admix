
#include "CustomMaterials.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4MaterialTable.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4NistManager.hh"

using namespace std;

CustomMaterials* CustomMaterials::instancePtr = nullptr;
mutex CustomMaterials::mtx;


CustomMaterials::CustomMaterials()
{
  DefineMaterials();
}

CustomMaterials::~CustomMaterials()
{;}

void CustomMaterials::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();
  // Define required materials

  G4double A;  // atomic mass
  G4double Z;  // atomic number
  G4double d;  // density

  // General elements

  A = 1.01*g/mole;
  G4Element* elH = new G4Element ("Hydrogen","H",Z = 1.,A);

  A = 12.011*g/mole;
  G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);

  A = 14.01*g/mole;
  G4Element* elN = new G4Element("Nitrogen","N",Z = 7.,A);

  A = 16.00*g/mole;
  G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);

  A = 22.99*g/mole;
  G4Element* elNa = new G4Element("Sodium","Na",Z = 11.,A);

  A = 24.305*g/mole;
  G4Element* elMg = new G4Element("Magnesium","Mg",Z = 12.,A);

  A = 30.974*g/mole;
  G4Element* elP = new G4Element("Phosphorus","P",Z = 15.,A);

  A = 32.064*g/mole;
  G4Element* elS = new G4Element("Sulfur","S",Z = 16.,A);

  A = 35.453*g/mole;
  G4Element* elCl = new G4Element("Chlorine","Cl",Z = 17.,A);

  A = 39.098*g/mole;
  G4Element* elK = new G4Element("Potassium","K",Z = 19.,A);

  A = 40.08*g/mole;
  G4Element* elCa = new G4Element("Calcium","Ca",Z = 20.,A);

  A = 55.85*g/mole;
  G4Element* elFe  = new G4Element("Iron","Fe",Z = 26.,A);

  A = 65.38*g/mole;
  G4Element* elZn = new G4Element("Zinc","Zn",Z = 30.,A);

  A = 85.47 *g/mole;
  G4Element* elRb = new G4Element("Rb","Rb",Z = 37.,A);

  A = 87.62 *g/mole;
  G4Element* elSr = new G4Element("Sr","Sr",Z = 38.,A);

  A = 91.22 *g/mole;
  G4Element* elZr = new G4Element("Zr","Zr",Z = 40.,A);

  A = 207.19 *g/mole;
  G4Element* elPb = new G4Element("Lead","Pb", Z = 82.,A);

  // Water
  d = 1.000*g/cm3;
  matH2O = new G4Material("Water",d,2);
  matH2O->AddElement(elH,2);
  matH2O->AddElement(elO,1);
  matH2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);


  // Lipid
  d = 0.900*g/cm3;
  G4Material* lipid = new G4Material("Lipid",d,3);
  lipid->AddElement(elC, 3);
  lipid->AddElement(elH, 5);
  lipid->AddElement(elO, 1);

  // Protein
  d = 1.200*g/cm3;
  G4Material* protein = new G4Material("Protein",d,4);
  protein->AddElement(elC, 4);
  protein->AddElement(elH, 7);
  protein->AddElement(elO, 2);
  protein->AddElement(elN, 1);

  // MIRD soft tissue
  d = 0.9869 *g/cm3;
  soft = new G4Material("soft_tissue",d,16);
  soft->AddElement(elH,0.1047);
  soft->AddElement(elC,0.2302);
  soft->AddElement(elN,0.0234);
  soft->AddElement(elO,0.6321);
  soft->AddElement(elNa,0.0013);
  soft->AddElement(elMg,0.00015);
  soft->AddElement(elP,0.0024);
  soft->AddElement(elS,0.0022);
  soft->AddElement(elCl,0.0014);
  soft->AddElement(elK,0.0021);
  soft->AddElement(elFe,0.000063);
  soft->AddElement(elZn,0.000032);
  soft->AddElement(elRb,0.0000057);
  soft->AddElement(elSr,0.00000034);
  soft->AddElement(elZr,0.000008);
  soft->AddElement(elPb,0.00000016);

  // MIRD Skeleton
  d = 1.4862*g/cm3;
  skeleton = new G4Material("skeleton",d,15);
  skeleton -> AddElement(elH,0.0704);
  skeleton -> AddElement(elC,0.2279);
  skeleton -> AddElement(elN,0.0387);
  skeleton -> AddElement(elO,0.4856);
  skeleton -> AddElement(elNa,0.0032);
  skeleton -> AddElement(elMg,0.0011);
  skeleton -> AddElement(elP,0.0694);
  skeleton -> AddElement(elS,0.0017);
  skeleton -> AddElement(elCl,0.0014);
  skeleton -> AddElement(elK,0.0015);
  skeleton -> AddElement(elCa,0.0991);
  skeleton -> AddElement(elFe,0.00008);
  skeleton -> AddElement(elZn,0.000048);
  skeleton -> AddElement(elSr,0.000032);
  skeleton -> AddElement(elPb,0.000011);

  // MIRD lung material
  d = 0.2958 *g/cm3;
  lung = new G4Material("lung_material", d,16);
  lung -> AddElement(elH, 0.1021);
  lung -> AddElement(elC, 0.1001);
  lung -> AddElement(elN,0.028);
  lung -> AddElement(elO,0.7596);
  lung -> AddElement(elNa,0.0019);
  lung -> AddElement(elMg,0.000074);
  lung -> AddElement(elP,0.00081);
  lung -> AddElement(elS,0.0023);
  lung -> AddElement(elCl,0.0027);
  lung -> AddElement(elK,0.0020);
  lung -> AddElement(elCa,0.00007);
  lung -> AddElement(elFe,0.00037);
  lung -> AddElement(elZn,0.000011);
  lung -> AddElement(elRb,0.0000037);
  lung -> AddElement(elSr,0.000000059);
  lung -> AddElement(elPb,0.00000041);

  G4double density_adipose = 0.93 *g/cm3;
  adipose = new G4Material("adipose", density_adipose,8);
  adipose -> AddElement(elH, 0.112);
  adipose -> AddElement(elC, 0.619);
  adipose -> AddElement(elN, 0.017);
  adipose -> AddElement(elO, 0.251);
  adipose -> AddElement(elS, 0.00025);
  adipose -> AddElement(elP, 0.00025);
  adipose -> AddElement(elK, 0.00025);
  adipose -> AddElement(elCa,0.00025);

  G4double density_glandular = 1.04 * g/cm3;
  glandular = new G4Material("glandular", density_glandular,8);
  glandular -> AddElement(elH, 0.1);
  glandular -> AddElement(elC,0.184);
  glandular -> AddElement(elN, 0.032);
  glandular -> AddElement(elO, 0.679);
  glandular -> AddElement(elS, 0.00125);
  glandular -> AddElement(elP, 0.00125);
  glandular -> AddElement(elK, 0.00125);
  glandular -> AddElement(elCa,0.00125);


  d = (density_adipose * 0.5) + (density_glandular * 0.5);
  adipose_glandular = new G4Material("adipose_glandular", d, 2);
  adipose_glandular -> AddMaterial(adipose, 0.5);
  adipose_glandular -> AddMaterial(glandular, 0.5);

  // Air
  d = 1.290*mg/cm3;
  G4Material* matAir = new G4Material("Air",d,2);
  matAir->AddElement(elN,0.7);
  matAir->AddElement(elO,0.3);



  //========AMG6 material =======================

  G4double AMG6_density =  2.640*g/cm3;
  G4int ncomp = 6;

  G4Material* matAMG6 = new G4Material("AMG6", AMG6_density, ncomp);
  G4Material* pAl = nist->FindOrBuildMaterial("G4_Al");
  G4Material* pMg = nist->FindOrBuildMaterial("G4_Mg");
  G4Material* pMn = nist->FindOrBuildMaterial("G4_Mn");
  G4Material* pTi = nist->FindOrBuildMaterial("G4_Ti");
  G4Material* pNa = nist->FindOrBuildMaterial("G4_Na");
  G4Material* pCu = nist->FindOrBuildMaterial("G4_Cu");

  G4double frac_Cu = 0.001;
  G4double frac_Na = 0.0001;
  G4double frac_Ti = 0.0006;
  G4double frac_Mn = 0.007;
  G4double frac_Mg = 0.062;
  G4double frac_Al = 1 - frac_Mg - frac_Mn - frac_Ti - frac_Na  - frac_Cu;

  matAMG6->AddMaterial(pAl, frac_Al);
  matAMG6->AddMaterial(pMg, frac_Mg);
  matAMG6->AddMaterial(pMn, frac_Mn);
  matAMG6->AddMaterial(pTi, frac_Ti);
  matAMG6->AddMaterial(pNa, frac_Na);
  matAMG6->AddMaterial(pCu, frac_Cu);

  //============================================================================

  //=============================== PolyBiz ====================================

  G4double PolyBiz_density =  2.920*g/cm3;
  G4int ncomponents = 3;

  G4Material* matPolyBiz = new G4Material("PolyBiz", PolyBiz_density, ncomponents);
  G4Material* pH = nist->FindOrBuildMaterial("G4_H");
  G4Material* pC = nist->FindOrBuildMaterial("G4_C");
  G4Material* pBi = nist->FindOrBuildMaterial("G4_Bi");

  G4double frac_H = 0.03;
  G4double frac_C = 0.20;
  G4double frac_Bi = 0.77;

  matPolyBiz->AddMaterial(pH, frac_H);
  matPolyBiz->AddMaterial(pC, frac_C);
  matPolyBiz->AddMaterial(pBi, frac_Bi);

  //============================================================================

  //========================== CatBrain ================================
  G4Material* CatBrain = new G4Material("CatBrain", 1.05 * g / cm3, 3);
  CatBrain->AddMaterial(matH2O, 0.80); // 80% воды
  CatBrain->AddMaterial(lipid, 0.10);  // 10% жиров
  CatBrain->AddMaterial(protein, 0.10); // 10% белков
  //============================================================================

  //======== soft tissue with boron mixture ==================================


  G4double  rho_tissue= 0.9869*g/cm3; //soft tissue
  G4double  rho_admix = 2.37 *g/cm3; // density of boron with A = 10.8 !!!! i.e. natural boron !!!!!!!!!!!!!!!!
  G4double admix_c = 400*mg/L; //concetration in  mg per Litre ++

  G4double density =  admix_c + (rho_tissue/rho_admix)*(rho_admix - admix_c);
  G4double admix_fr = admix_c /density;// mass fractions

  G4Material* TissueWithAdmixture = new G4Material("SoftTissueWithBoron", density, 2);
  G4Material* Admixture = nist->FindOrBuildMaterial("G4_B");
  TissueWithAdmixture->AddMaterial(soft, 1 - admix_fr);
  TissueWithAdmixture->AddMaterial(Admixture, admix_fr);


// oak wood

  G4Material* WoodDiskMaterial = new G4Material("OakWood", density, 3);
  WoodDiskMaterial->AddElement(nist->FindOrBuildElement("C"), 50.0 * perCent);
  WoodDiskMaterial->AddElement(nist->FindOrBuildElement("H"), 6.0 * perCent);
  WoodDiskMaterial->AddElement(nist->FindOrBuildElement("O"), 44.0 * perCent);

  //============================================================================


}

G4Material* CustomMaterials::GetMaterial(G4String material)
{
  // Returns a material
  G4Material* pttoMaterial = G4Material::GetMaterial(material);
  return pttoMaterial;
}

CustomMaterials* CustomMaterials::Instance() {
    if (CustomMaterials::instancePtr == nullptr) {
        lock_guard<mutex> lock(CustomMaterials::mtx);
        if (CustomMaterials::instancePtr == nullptr) {
            CustomMaterials::instancePtr = new CustomMaterials();
        }
    }
    return CustomMaterials::instancePtr;
}
