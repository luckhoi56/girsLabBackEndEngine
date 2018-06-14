#include "../catch/single_include/catch.hpp"

#include "../homogeneous/SimpleHomogeneous.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

// TEST_CASE("Simple Homogeneous LogNormal - Binomial","[Homogeneous]") {
//    std::ofstream datafile("lnbcases.dat");
//    std::ifstream rsltfile("lnbrslts.dat");
//
//    bool writedata = false;
//    if (datafile.is_open()) writedata = true;
//    bool readdata = false;
//    if (rsltfile.is_open()) readdata = true;
//
//    double x[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    double px[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    double y[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    double v[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    double s[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//
//    int cases = 57;
//    int iterations = 1;
//    double m[21];
//    int samplesize = 20000;
//
//    m[0]=0;
//    m[20]=0;
//
//    // priors number 1 trough 9
//    int prior[57]      = { 1, 5, 9,13,17, 3, 7,11,15, 2, 6,10,14,18, 4, 8,12,16,
//        1,17, 3, 7,11,15, 2, 6,10,14,18, 4, 8,12,16, 1, 5, 9,
//        10,14,18, 4, 8,12,16, 1, 5, 9,13,17, 3, 7,11,15, 2, 6,
//        5, 9,13};
//
//    // likelihoods number 1 through 18
//    int likelihood[57] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6,
//        7, 7, 7, 8, 8, 8, 9, 9, 9,10,10,10,12,12,12,13,13,13,
//        14,14,14,15,15,15,18,18,18,19,19,19,20,20,20,24,24,24,
//        25,25,25};
//
//    double nu,tau,k,N;
//
//    std::cout << "\\chapter{LogNormal-Binomial Test Cases}" << std::endl;
//
//    std::cout.precision(3);
//    std::cout.setf(std::ios::scientific,std::ios::floatfield);
//
//    for (int i = 0 ; i < cases ; i++) {
//
//        switch ((prior[i] - 1) / 6) {
//            case 0 : nu = log(1e-8); break;
//            case 1 : nu = log(1e-5); break;
//            case 2 : nu = log(1e-2); break;
//        }
//
//        switch ((prior[i] - 1) % 6) {
//            case 0 : tau = 0.2; break;
//            case 1 : tau = 0.5; break;
//            case 2 : tau = 1.0; break;
//            case 3 : tau = 2.0; break;
//            case 4 : tau = 3.5; break;
//            case 5 : tau = 5.0; break;
//        }
//
//        switch ((likelihood[i] - 1) % 5) {
//            case 0 : N = 1; break;
//            case 1 : N = 3; break;
//            case 2 : N = 10; break;
//            case 3 : N = 100; break;
//            case 4 : N = 1000; break;
//        }
//
//        switch ((likelihood[i] - 1) / 5) {
//            case 0 : k = 0; break;
//            case 1 : k = 1; break;
//            case 2 : k = 3; break;
//            case 3 : k = 10; break;
//            case 4 : k = 100; break;
//        }
//
//        if (k > N)
//            std::cout << i << ": BAD TEST CASE " << likelihood[i] << "." << prior[i] << std::endl;
//
//        int badresults = 0;
//        bool noresults = false;
//
//        if (readdata) {
//            for (int r = 1 ; r <= 19 ; r++) {
//                noresults = rsltfile.eof();
//                if (noresults)
//                    m[r] = 0;
//                else
//                    rsltfile >> m[r];
//            }
//            noresults = rsltfile.eof();
//            if (!noresults) rsltfile >> badresults;
//        } else {
//            for (int r = 1 ; r <= 19 ; r++) m[r] = 0;
//            noresults = true;
//        }
//
//        std::cout << "\\pagebreak\n\\section{Test Case " << likelihood[i] << "." << prior[i] << "}" << std::endl << std::endl;
//        std::cout << "\\begin{center}" << std::endl;
//        std::cout << "\\begin{tabular}{|r|r|r|r|r|}\\hline" << std::endl;
//        std::cout << "\\multicolumn{5}{|c|}{\\textsc{Input Values}} \\\\ \\hline" << std::endl;
//        std::cout << "\\multicolumn{2}{|c|}{\\textsc{Parameter}} & \\multicolumn{3}{|c|}{\\textsc{Value}} \\\\ \\hline" << std::endl;
//        std::cout << "\\multicolumn{2}{|l|}{Prior.$\\nu$} & \\multicolumn{3}{|r|}{\\texttt{" << nu << "}} \\\\" << std::endl;
//        std::cout << "\\multicolumn{2}{|l|}{Prior.$\\tau$} & \\multicolumn{3}{|r|}{\\texttt{" << tau << "}} \\\\" << std::endl;
//        std::cout << "\\multicolumn{2}{|l|}{Evidence.Events} & \\multicolumn{3}{|r|}{\\texttt{" << k << "}} \\\\" << std::endl;
//        std::cout << "\\multicolumn{2}{|l|}{Evidence.Trials} & \\multicolumn{3}{|r|}{\\texttt{" << N << "}} \\\\" << std::endl;
//        std::cout << "\\multicolumn{2}{|l|}{Test.Iterations} & \\multicolumn{3}{|r|}{\\texttt{" << iterations << "}} \\\\" << std::endl;
//        std::cout << "\\multicolumn{2}{|l|}{Algorithm.Samples} & \\multicolumn{3}{|r|}{\\texttt{" << samplesize << "}} \\\\ \\hline" << std::endl;
//
//        for (int i0 = 0 ; i0 < 21 ; i0++) {
//            x[i0] = 0;
//            px[i0] = 0;
//            s[i0] = 0;
//            v[i0] = 0;
//            y[i0] = 0;
//        }
//
//        double integral,lbmean,lbvar;
//
//        for (int i1 = 0; i1 < iterations ; i1++) {
//            SimpleHomogeneous::updateLogNormalBinomial(nu,tau,k,N,samplesize,y,px,21,integral,lbmean,lbvar);
//            for (int i2 = 0 ; i2 < 21 ; i2++) {
//                x[i2] += y[i2];
//                v[i2] += y[i2] * y[i2];
//            }
//        }
//
//        for (int i3 = 0 ; i3 < 21 ; i3++) {
//            x[i3] /= iterations;
//            s[i3] = iterations == 1 ? 0 : pow((v[i3] - iterations * x[i3] * x[i3])/(iterations - 1),0.5);
//        }
//
//        std::cout << "\\multicolumn{5}{|c|}{\\textsc{Results}} \\ \\hline" << std::endl;
//        std::cout << "\\multicolumn{1}{|c|}{\\textsc{Probability}} & \\multicolumn{1}{|c|}{\\textsc{Mathematica}} & \\multicolumn{1}{|c|}{\\textsc{M-H MN}} & \\multicolumn{1}{|c|}{\\textsc{Ratio}} & \\multicolumn{1}{|c|}{\\textsc{M-H SD/MN}} \\ \\hline" << std::endl;
//
//        for (int j = 0 ; j < 21 ; j++)
//            std::cout << "\\texttt{" << px[j] << "} & \\texttt{" << m[j] << "} & \\texttt{" << x[j] << "} & \\texttt{" << m[j]/x[j] <<  "} & \\texttt{" << s[j]/x[j] <<  "} \\\\" << std::endl;
//        if (noresults) std::cout << "\\hline\\multicolumn{5}{|c|}{Mathematica: no or incomplete results} \\\\" << std::endl;
//        else if (badresults == 1) std::cout << "\\hline\\multicolumn{5}{|c|}{Mathematica: numerical instabilities} \\\\" << std::endl << std::endl;
//        std::cout << "\\hline\\end{tabular}" << std::endl << std::endl;
//        std::cout << "\\end{center}" << std::endl;
//        std::cout << "Integral of the entire distribution: " << integral << std::endl;
//
//        if (writedata) {
//            double min = x[10] / pow(x[10] / x[0],2);
//            double max = x[10] * pow(x[20] / x[10],2);
//            if (max > 1) max = 1;
//            datafile << "lnb" << likelihood[i] << "." << prior[i] << " " << nu << " " << tau << " " << k << " " << N << " " << min << " " << max << " " << x[10] << std::endl;
//        }
//    }
//
//    std::cout << "\\pagebreak\n\\section{Completion of the Test}" << std::endl;
//
//    std::cout.unsetf(std::ios::scientific);
//    std::cout.unsetf(std::ios::floatfield);
//    std::cout.precision(0);
//}
//
TEST_CASE("Homogeneous Normal - Normal","[Homogeneous]")
{
   const int conjugateCases = 30;
   int evidenceCases = 5;
   double nuh[] = {1e-5, 1e-5, 1e-5,-1e2, 1e2 };
   double tau[] = {1e-8,1e-5,1e-2,2,2};
//    std::cout << "\\pagebreak" << std::endl;
//    std::cout << "\\section{Normal-Normal}" << std::endl;
//    std::cout << "{\\small\\begin{center}" << std::endl;
//    std::cout << "\\begin{tabular}{|r|r|r|r|r|r|r|r|}\\hline" << std::endl;
//    std::cout << "\\multicolumn{2}{|c|}{Prior}&\\multicolumn{2}{|c|}{Evidence}&\\multicolumn{2}{|c|}{Result}&\\multicolumn{2}{|c|}{Expected} \\\\ \\hline" << std::endl;
//    std::cout << "$\\nu$ & $\\tau$ & $\\nu$ & $\\tau$ & $\\nu^{*}$ & $\\tau^{*}$ & $\\nu^{x}$ & $\\tau^{x}$  \\\\ \\hline" << std::endl;

   for (int i = 0 ; i < conjugateCases ; i++) {
       double n,t,nx,tx,ne,te,nu,tu;
       int priorCases = 16;
       //Get Normal Parameters AlgorithmTester::getNormalParameters(int index, double &a, double &b)
       int index = i % priorCases;
       double alphas[] = {0,-10,10,1000};
       double betas[] = {1,2,5,10};
       n = alphas[index%4];
       t = betas[index/4];

       ne = nuh[i % evidenceCases];
       te = tau[i % evidenceCases];

       double sigma2 = t * t; // sigma squared
       double tau2 = te * te; // tau squared
       nx = (sigma2 * ne + tau2 * n) / (sigma2 + tau2);
       tx = 1 / sqrt(1 / sigma2 + 1 / tau2);

       SimpleHomogeneous::updateNormalNormal(n,t,ne,te,nu,tu);

//        std::cout << n << "&" << t << "&" << ne << "&" << te << "&" << nu << "&" <<  tu  << "&" << nx << "&" << tx << "\\\\"<< std::endl;

       REQUIRE(nu == Approx(nx));
       REQUIRE(tu == Approx(tx));
   }
   std::cout << "\\hline \\end{tabular} \\end{center} }" << std::endl << std::endl;
}
