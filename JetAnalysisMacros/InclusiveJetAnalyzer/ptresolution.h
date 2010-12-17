#ifndef __ptresolution_h__
#define __ptresolution_h__

#include "TMath.h"
#include "TF1.h"

// Switch MC truth or data resolutions
bool _ismcjer = true;
double pedestal[3] = {9.277,7.698,1.66};
int ycbin = 0;

// Hauke's resolutions
const int _nres = 6;
const int _npar = 4;
float _pres[_nres][_npar] =
  // From CMSSW Spring10_PtResolution_AK5PF
  {{/*0 0.5*/ -0.349206, 0.297831, 0, 0.471121},
   {/*0.5 1*/ -0.499735, 0.336391, 0, 0.430689},
   {/*1 1.5*/ -0.561649, 0.420293, 0, 0.392398},
   {/*1.5 2*/ -1.12329,  0.657891, 0, 0.139595},
   {/*2 2.5*/  1.04792,  0.466763, 0, 0.193137}, 
   {/*2.5 3*/  2.56933,  0.305802, 0, 0.398929}}; // changed since May5?
float _pres_ak5jpt[_nres][_npar] =
  // From CMSSW Spring10_PtResolution_AK5JPT
  {{/*0 0.5*/ 2.51562, 0.316061, 0, 0.445628},
   {/*0.5 1*/ 2.41271, 0.332129, 0, 0.432430},
   {/*1 1.5*/ 2.42631, 0.371536, 0, 0.439477},
   {/*1.5 2*/ 2.67972, 0.511479, 0, 0.257115},
   {/*2 2.5*/ 3.39005, 0.499497, 0, 0.273727},
   {/*2.5 3*/ 2.28498, 0.390866, 0, 0.297617}};
float _pres_ak5calo[_nres][_npar] =
  // From CMSSW Spring10_PtResolution_AK5Calo
  {{/*0 0.5*/ 5.0913, 0.511619, 0, 0.325477},
   {/*0.5 1*/ 4.93688, 0.543349, 0, 0.310638}, 
   {/*1 1.5*/ 4.95729, 0.574202, 0, 0.32344},
   {/*1.5 2*/ 3.37322, 0.96262, 0, 0.0860736},
   {/*2 2.5*/ 4.06368, 0.351561, 0, 0.356167},
   {/*2.5 3*/ 2.59292, 0.357679, 0, 0.321092}};

  // May 5 evening
  /*
  {{-0.34908, 0.29783, 0.00000, 0.47112}, // 0.0-0.5 (331.7/16)
   {-0.49950, 0.33638, 0.00000, 0.43070}, // 0.5-1.0 (181.3/16)
   {-0.56158, 0.42031, 0.00000, 0.39239}, // 1.0-1.5 (267.1/14)
   {-1.12266, 0.65782, 0.00000, 0.13963}, // 1.5-2.0 (315.7/13)
   { 1.04834, 0.46668, 0.00000, 0.19320}, // 2.0-2.5 (89.8/10)
   { 1.89978, 0.33427, 0.00000, 0.36547}};// 2.5-3.0 (28.3/9) 
  */
/* // May 5 afternoon
  {{0.58001, 0.28362, 0.00000, 0.48101}, // 0.0-0.5 (3049.8/16)
   {0.54843, 0.31317, 0.00000, 0.45128}, // 0.5-1.0 (1279.7/16)
   {0.33060, 0.40746, 0.00000, 0.40081}, // 1.0-1.5 (808.5/15)
   {-0.95164, 0.65292, 0.00000, 0.13674}, // 1.5-2.0 (1007.5/13)
   {0., 0.67966, 0.01936, 0.}, // 2.0-2.5, old since new didn't converge
   {1.90946, 0.33008, 0.00000, 0.37072}}; // 2.5-3.0 (61.1/10)
*/
/*
  {{0., 0.67420, 0.04706, 0.}, // 0.0-0.5
   {0., 0.70603, 0.04684, 0.}, // 0.5-1.0
   {0., 0.83819, 0.05009, 0.}, // 1.0-1.5
   {0., 0.81765, 0.02202, 0.}, // 1.5-2.0
   {0., 0.67966, 0.01936, 0.}, // 2.0-2.5
   {0.26792, 0.68888, 0.03052, 0.}}; // 2.5-3.0
*/
double ptresolution(double pt, double eta) {
  
  int ieta = int(fabs(eta) / 0.5);
  float N = _pres[ieta][0];
  float S = _pres[ieta][1];
  float C = _pres[ieta][2];
  float m = _pres[ieta][3];
  double k = (fabs(eta)<1.5 ? 1.1 : 1.2);
  if (_ismcjer) k = 1.;

  double res = k*sqrt(TMath::Sign(N*N,N)/(pt*pt) + S*S*pow(pt,m-1) + C*C);
  //if (ieta==0 && res>0.12) res = 0.12;

  return res;
}

double ptresolution_jpt(double pt, double eta) {
  
  int ieta = int(fabs(eta) / 0.5);
  float N = _pres_ak5jpt[ieta][0];
  float S = _pres_ak5jpt[ieta][1];
  float C = _pres_ak5jpt[ieta][2];
  float m = _pres_ak5jpt[ieta][3];
  double k = (fabs(eta)<1.5 ? 1.1 : 1.2);
  if (_ismcjer) k = 1.;

  double res = k*sqrt(TMath::Sign(N*N,N)/(pt*pt) + S*S*pow(pt,m-1) + C*C);

  return res;
}

double ptresolution_calo(double pt, double eta) {
  
  int ieta = int(fabs(eta) / 0.5);
  float N = _pres_ak5calo[ieta][0];
  float S = _pres_ak5calo[ieta][1];
  float C = _pres_ak5calo[ieta][2];
  float m = _pres_ak5calo[ieta][3];
  double k = (fabs(eta)<1.5 ? 1.1 : 1.2);
  if (_ismcjer) k = 1;

  double res = k*sqrt(TMath::Sign(N*N,N)/(pt*pt) + S*S*pow(pt,m-1) + C*C);
  return res;
}

Double_t res(Double_t *x, Double_t *p) {
  return ptresolution_calo(x[0],0);
}

Double_t res2(Double_t *x, Double_t *p) {
   return sqrt(pow(ptresolution_calo(x[0],0),2)+pow(pedestal[ycbin]/x[0],2));
}


TF1 * fReso = new TF1("fReso","sqrt(pow([0],2)+pow([1]/sqrt(x),2)+pow([2]/x,2))",0,400);
TF1 * fResoNoBkg = new TF1("fResoNoBkg","sqrt(pow(7.7e-02,2)+pow(1.07/sqrt(x),2)+pow(-4.6e-5/x,2))",0,400);
TF1 * fResoWBkg = new TF1("fResoWBkg",Form("sqrt(pow(7.7e-02,2)+pow(1.07/sqrt(x),2)+pow(-4.6e-5/x,2)+pow(%f/x,2))",pedestal[ycbin]),0,400);
TF1 * fResoNoBkgVsCalo = new TF1("fResoNoBkgVsCalo","sqrt(pow(1.04e-01,2)+pow(8.38e-1/sqrt(x),2)+pow(-4.43e-1/x,2))",0,400);
TF1 * fResoWBkgVsCalo = new TF1("fResoWBkgVsCalo",Form("sqrt(pow(1.04e-01,2)+pow(8.38e-1/sqrt(x),2)+pow(-4.43e-1/x,2)+pow(%f/x,2))",pedestal[ycbin]),0,400);

TF1 * fResoWBkgDumb = new TF1("fResoWBkg",Form("sqrt(pow(sqrt(pow(7.7e-02,2)+pow(1.07/sqrt(x),2)+pow(-4.6e-5/x,2))*x,2)+pow(%f,2))/x",pedestal[ycbin]),0,400);
TF1 * fResoWBkgVsCaloDumb = new TF1("fResoWBkgVsCalo",Form("sqrt(pow(sqrt(pow(1.04e-01,2)+pow(8.38e-1/sqrt(x),2)+pow(-4.43e-1/x,2))*x,2)+pow(%f,2))/x",pedestal[ycbin]),0,400);


#endif // __ptresolution_h__
