// $Header: C:/cvsrepo/ptdev/native/redcas/testsuite/AlgorithmTester.cpp,v 1.2 2001/07/24 21:31:42 fgroen Exp $

// AlgorithmTester.cpp: implementation of the AlgorithmTester class.
//
//////////////////////////////////////////////////////////////////////


#include "AlgorithmTester.h"
#include "Distribution.h"
#include "SimpleHomogeneous.h"

#include <sys/types.h>
#include <sys/timeb.h>

const int conjugateCases = 30;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AlgorithmTester::AlgorithmTester()
{
	datafile.open("data.dat");
	rsltfile.open("rslt.dat",ios::nocreate,filebuf::openprot);
	if (datafile) {
		datafile.precision(9);
//		datafile.setf(ios::scientific,ios::floatfield);
	}
	texfile.open("report.tex");
	writedata = datafile != 0;
	readdata = rsltfile != 0;
}

AlgorithmTester::~AlgorithmTester()
{
	if (datafile.is_open()) datafile.close();
	if (rsltfile.is_open()) rsltfile.close();
	if (texfile.is_open()) texfile.close();
}

void AlgorithmTester::startTestReport()
{
	char tmpbuf[127];
	time_t ltime;
	time( &ltime );
	struct tm *today = localtime( &ltime );
	strftime( tmpbuf, 128, "DLL Test Report - (%m/%d/%y)", today );
	
 
	texfile.precision(3);
	texfile.setf(ios::scientific,ios::floatfield);
	texfile << "\\documentstyle{report}" << endl;
	texfile << "\\setlength{\\oddsidemargin}{0in}" << endl;
	texfile << "\\setlength{\\textwidth}{6.5in}" << endl;
	texfile << "\\markboth{" << tmpbuf << "}{" << tmpbuf << "}" << endl;
	texfile << "\\pagestyle{myheadings}" << endl;
	texfile << "\\begin{document}" << endl;
}

void AlgorithmTester::endTestReport()
{
	texfile << "\\end{document}" << endl;
}

void AlgorithmTester::testParametricDistributions()
{
	texfile << "\\input{pcqintro.tex}" << endl;
	testGammaPDFCDF();
	testBetaPDFCDF();
	testNormalPDFCDF();
	testLogNormalPDFCDF();
	testTrLogNormalPDFCDF();
}

void AlgorithmTester::testHomogeneous()
{
	texfile << "\\chapter{Homogeneous Updating}" << endl;
	testHGammaPoisson();
	testHBetaBinomial();
	testHNormalNormal();
	testHLogLog();
	writeHTabularCase(0,0,0,0,0,0,0,0);
	testHTabularPoisson();
	testHTabularBinomial();
	testHTabularNormal();
	testHTabularLogNormal();
}

////////////////////////////////////////////////////////////////////////////

void AlgorithmTester::testHNormalNormal()
{
	int evidenceCases = 5;
	double nuh[] = {1e-5, 1e-5, 1e-5,-1e2, 1e2 };
	double tau[] = {1e-8,1e-5,1e-2,2,2};
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Normal-Normal}" << endl;
	texfile << "{\\small\\begin{center}" << endl;
	texfile << "\\begin{tabular}{|r|r|r|r|r|r|r|r|}\\hline" << endl;
	texfile << "\\multicolumn{2}{|c|}{Prior}&\\multicolumn{2}{|c|}{Evidence}&\\multicolumn{2}{|c|}{Result}&\\multicolumn{2}{|c|}{Expected} \\\\ \\hline" << endl;
	texfile << "$\\nu$ & $\\tau$ & $\\nu$ & $\\tau$ & $\\nu^{*}$ & $\\tau^{*}$ & $\\nu^{x}$ & $\\tau^{x}$  \\\\ \\hline" << endl;
	for (int i = 0 ; i < conjugateCases ; i++) {
		double n,t,nx,tx,ne,te,nu,tu;
		int priorCases = getNormalParameterCount();
		getNormalParameters(i % priorCases,n,t);

		ne = nuh[i % evidenceCases];
		te = tau[i % evidenceCases];

		double sigma2 = t * t; // sigma squared
		double tau2 = te * te; // tau squared
		nx = (sigma2 * ne + tau2 * n) / (sigma2 + tau2);
		tx = 1 / sqrt(1 / sigma2 + 1 / tau2);

		SimpleHomogeneous::updateNormalNormal(n,t,ne,te,nu,tu);

		texfile << n << "&" << t << "&" << ne << "&" << te << "&" << nu << "&" <<  tu  << "&" << nx << "&" << tx << "\\\\"<< endl;
	}
	texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
}


void AlgorithmTester::testHLogLog()
{
	int evidenceCases = 5;
	double nuh[] = {-5, -5, -5,-1, 2 };
	double tau[] = {0.2,1,5,0.5, 2};
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Lognormal-Lognormal}" << endl;
	texfile << "{\\small\\begin{center}" << endl;
	texfile << "\\begin{tabular}{|r|r|r|r|r|r|r|r|}\\hline" << endl;
	texfile << "\\multicolumn{2}{|c|}{Prior}&\\multicolumn{2}{|c|}{Evidence}&\\multicolumn{2}{|c|}{Result}&\\multicolumn{2}{|c|}{Expected} \\\\ \\hline" << endl;
	texfile << "$\\nu$&$\\tau$&$\\nu$&$\\tau$&$\\nu^{*}$&$\\tau^{*}$&$\\nu^{x}$&$\\tau^{x}$\\\\ \\hline" << endl;
	for (int i = 0 ; i < conjugateCases ; i++) {
		double n,t,nx,tx,ne,te,nu,tu;
		int priorCases = getLogNormalParameterCount();
		getLogNormalParameters(i % priorCases,n,t);
		ne = nuh[i % evidenceCases];
		te = tau[i % evidenceCases];
		double sigma2 = t * t; // sigma squared
		double tau2 = te * te; // tau squared
		double wnu = sigma2 / (tau2 + sigma2);
		nx = ne * wnu + n * (1 - wnu);
		tx = 1.0 / sqrt(1 / sigma2 + 1 / tau2);
		SimpleHomogeneous::updateLogNormalLogNormal(n,t,ne,te,nu,tu);
		texfile << n << "&" << t << "&" << ne << "&" << te << "&" << nu << "&" <<  tu  << "&" << nx << "&" << tx << "\\\\"<< endl;
	}
	texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
}


void AlgorithmTester::testHGammaPoisson()
{
	int evidenceCases = 5;
	double k[] = { 0, 1, 5,50, 100 };
	double t[] = {10,20,30,40, 50};
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Gamma-Poisson}" << endl;
	texfile << "{\\small\\begin{center}" << endl;
	texfile << "\\begin{tabular}{|r|r|r|r|r|r|r|r|}\\hline" << endl;
	texfile << "\\multicolumn{2}{|c|}{Prior}&\\multicolumn{2}{|c|}{Evidence}&\\multicolumn{2}{|c|}{Result}&\\multicolumn{2}{|c|}{Expected} \\\\ \\hline" << endl;
	texfile << "$\\alpha$ & $\\lambda$ & $k$ & $T$ & $\\alpha^{*}$ & $\\lambda^{*}$ & $\\alpha^{x}$ & $\\lambda^{x}$  \\\\ \\hline" << endl;
	for (int i = 0 ; i < conjugateCases ; i++) {
		double a,b,l,ax,lx,ke,te,au,lu;
		int priorCases = getGammaParameterCount();
		getGammaParameters(i % priorCases,a,b);
		l = 1/b;
		ke = k[i % evidenceCases];
		te = t[i % evidenceCases];
		ax = a + ke;
		lx = l + te;
		SimpleHomogeneous::updateGammaPoisson(a,l,ke,te,au,lu);
		texfile << a << "&" << l << "&" << ke << "&" << te << "&" << au << "&" <<  lu  << "&" << ax << "&" << lx << "\\\\"<< endl;
	}
	texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
}

void AlgorithmTester::testHBetaBinomial()
{
	int evidenceCases = 6;
	double k[] = {  0,  1, 50,100,   1,  4};
	double n[] = {100,100,100,100,1500,6000};
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Beta-Binomial}" << endl;
	texfile << "{\\small\\begin{center}" << endl;
	texfile << "\\begin{tabular}{|r|r|r|r|r|r|r|r|}\\hline" << endl;
	texfile << "\\multicolumn{2}{|c|}{Prior}&\\multicolumn{2}{|c|}{Evidence}&\\multicolumn{2}{|c|}{Result}&\\multicolumn{2}{|c|}{Expected} \\\\ \\hline" << endl;
	texfile << "$\\alpha$ & $\\beta$ & $k$ & $N$ & $\\alpha^{*}$ & $\\beta^{*}$ & $\\alpha^{x}$ & $\\beta^{x}$  \\\\ \\hline" << endl;
	for (int i = 0 ; i < conjugateCases ; i++) {
		double a,b,ax,bx,ke,ne,au,bu;
		int priorCases = getBetaParameterCount();
		getBetaParameters(i % priorCases,a,b);

		ke = k[i % evidenceCases];
		ne = n[i % evidenceCases];

		ax = a + ke;
		bx = b + ne - ke;
		SimpleHomogeneous::updateBetaBinomial(a,b,ke,ne,au,bu);
		texfile << a << "&" << b << "&" << ke << "&" << ne << "&" << au << "&" <<  bu  << "&" << ax << "&" << bx << "\\\\"<< endl;
	}
	texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
}

void AlgorithmTester::testHTabularPoisson()
{
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Tabular-Poisson}" << endl;

	const double karr [] = {0,1,5,10,100};
	const double tarr [] = {10,1000,10000};

	int tabSize;
	double tabx[100];
	double tabc[100];

	const int posSize = 41;
	double posx[posSize];
	double posc[posSize];
	double expc[posSize];

	for (int i = 0 ; i < 15 ; i++) {

		for (int a = 0 ; a < posSize ; a++) expc[a] = 0;

		getTabularPrior(i % getTabularPriorCount(),tabSize,tabx,tabc);

		double k = karr[i % 5];
		double T = tarr[i / 5];

		int retval = SimpleHomogeneous::updateTabularPoisson(tabx,tabc,tabSize,k,T,-1,posx,posc,posSize);

		if (!retval) {
			writeHTabularCase(tabSize,tabx,tabc,posSize,posx,0,k,T);
			readHTabularCase(posSize, expc);
		} else texfile << "ALGORITHM RETURNED ERROR " << retval << endl;
	
		texfile << "\\pagebreak" << endl;	
		texfile << "\\subsection{Case Tabular-Poisson " << i + 1 << "}";

		texfile << "\\begin{itemize}" << endl;	
		texfile << "\\item Tabular prior: " << i % getTabularPriorCount() << endl;	
		texfile << "\\item Evidence: $k=" << k << "$, $T=" << T << "$" << endl;
		texfile << "\\end{itemize}" << endl;	

		texfile << "{\\small\\begin{center}" << endl;
		texfile << "\\begin{tabular}{||r|r|r||r|r|r||}\\hline" << endl;
		texfile << "$x$&cdf$(x)$&cdf$_{exp}(x)$&$x$&cdf$(x)$&cdf$_{exp}(x)$ \\\\ \\hline" << endl;

		int mp = posSize / 2;
		int lp = posSize - 1;

		for (int j=0;j<posSize/2;j++) {
			texfile << posx[j]    << "&" << posc[j]    << "&" << expc[j]    << "&";
			texfile << posx[j+mp] << "&" << posc[j+mp] << "&" << expc[j+mp] << "\\\\" << endl;
		}

		if (posSize % 2) {
			texfile << "&&&" << posx[lp] << "&" << posc[lp] << "&" << expc[lp] << "\\\\" << endl;
		}
		texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
	}
}

void AlgorithmTester::testHTabularNormal()
{
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Tabular-Normal}" << endl;

	const double karr [] = {0,1e-6,1e-4,1e-2,1};
	const double narr [] = {0.0001,0.001,0.01};

	int tabSize;
	double tabx[100];
	double tabc[100];

	const int posSize = 41;
	double posx[posSize];
	double posc[posSize];
	double expc[posSize];

	for (int i = 0 ; i < 15 ; i++) {

		for (int a = 0 ; a < posSize ; a++) expc[a] = 0;

		getTabularPrior(i % getTabularPriorCount(),tabSize,tabx,tabc);

		double k = karr[i % 5];
		double N = narr[i / 5];

		int retval = SimpleHomogeneous::updateTabularNormal(tabx,tabc,tabSize,k,N,-1,posx,posc,posSize);
		if (!retval) {
			writeHTabularCase(tabSize,tabx,tabc,posSize,posx,1,k,N);
			readHTabularCase(posSize, expc);
		} else texfile << "ALGORITHM RETURNED ERROR " << retval << endl;

		texfile << "\\pagebreak" << endl;	
		texfile << "\\subsection{Case Tabular-Normal " << i + 1 << "}";

		texfile << "\\begin{itemize}" << endl;	
		texfile << "\\item Tabular prior: " << i % getTabularPriorCount() << endl;	
		texfile << "\\item Evidence: $\\mu=" << k << "$, $\\sigma=" << N << "$" << endl;
		texfile << "\\end{itemize}" << endl;	

		texfile << "{\\small\\begin{center}" << endl;
		texfile << "\\begin{tabular}{||r|r|r||r|r|r||}\\hline" << endl;
		texfile << "$x$&cdf$(x)$&cdf$_{exp}(x)$&$x$&cdf$(x)$&cdf$_{exp}(x)$ \\\\ \\hline" << endl;

		int mp = posSize / 2;
		int lp = posSize - 1;

		for (int j=0;j<posSize/2;j++) {
			texfile << posx[j]    << "&" << posc[j]    << "&" << expc[j]    << "&";
			texfile << posx[j+mp] << "&" << posc[j+mp] << "&" << expc[j+mp] << "\\\\" << endl;
		}

		if (posSize % 2) {
			texfile << "&&&" << posx[lp] << "&" << posc[lp] << "&" << expc[lp] << "\\\\" << endl;
		}
		texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
	}
}


void AlgorithmTester::testHTabularLogNormal()
{
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Tabular-Lognormal}" << endl;

	const double karr [] = {-12,-9,-6,-3,0};
	const double narr [] = {0.2,1,5};

	int tabSize;
	double tabx[100];
	double tabc[100];

	const int posSize = 41;
	double posx[posSize];
	double posc[posSize];
	double expc[posSize];

	for (int i = 0 ; i < 15 ; i++) {

		for (int a = 0 ; a < posSize ; a++) expc[a] = 0;

		getTabularPrior(i % getTabularPriorCount(),tabSize,tabx,tabc);

		double k = karr[i % 5];
		double N = narr[i / 5];

		int retval = SimpleHomogeneous::updateTabularLogNormal(tabx,tabc,tabSize,k,N,-1,posx,posc,posSize);
		if (!retval) {
			writeHTabularCase(tabSize,tabx,tabc,posSize,posx,2,k,N);
			readHTabularCase(posSize, expc);
		} else texfile << "ALGORITHM RETURNED ERROR " << retval << endl;
	
		texfile << "\\pagebreak" << endl;	
		texfile << "\\subsection{Case Tabular-LogNormal " << i + 1 << "}";

		texfile << "\\begin{itemize}" << endl;	
		texfile << "\\item Tabular prior: " << i % getTabularPriorCount() << endl;	
		texfile << "\\item Evidence: $\\nu=" << k << "$, $\\tau=" << N << "$" << endl;
		texfile << "\\end{itemize}" << endl;	

		texfile << "{\\small\\begin{center}" << endl;
		texfile << "\\begin{tabular}{||r|r|r||r|r|r||}\\hline" << endl;
		texfile << "$x$&cdf$(x)$&cdf$_{exp}(x)$&$x$&cdf$(x)$&cdf$_{exp}(x)$ \\\\ \\hline" << endl;

		int mp = posSize / 2;
		int lp = posSize - 1;

		for (int j=0;j<posSize/2;j++) {
			texfile << posx[j]    << "&" << posc[j]    << "&" << expc[j]    << "&";
			texfile << posx[j+mp] << "&" << posc[j+mp] << "&" << expc[j+mp] << "\\\\" << endl;
		}

		if (posSize % 2) {
			texfile << "&&&" << posx[lp] << "&" << posc[lp] << "&" << expc[lp] << "\\\\" << endl;
		}
		texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
	}
}

void AlgorithmTester::testHTabularBinomial()
{
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Tabular-Binomial}" << endl;

	const double karr [] = {0,1,2,5,10,0,1,5,50,250,0,1,5,25,100};
	const double narr [] = {10,1000,10000};

	int tabSize;
	double tabx[100];
	double tabc[100];

	const int posSize = 41;
	double posx[posSize];
	double posc[posSize];
	double expc[posSize];

	for (int i = 0 ; i < 15 ; i++) {

		for (int a = 0 ; a < posSize ; a++) expc[a] = 0;

		getTabularPrior(i % getTabularPriorCount(),tabSize,tabx,tabc);

		double k = karr[i];
		double N = narr[i / 5];

		int retval = SimpleHomogeneous::updateTabularBinomial(tabx,tabc,tabSize,k,N,-1,posx,posc,posSize);

		texfile << "\\pagebreak" << endl;	
		texfile << "\\subsection{Case Tabular-Binomial " << i + 1 << "}";

		texfile << "\\begin{itemize}" << endl;	
		texfile << "\\item Tabular prior: " << i % getTabularPriorCount() << endl;	
		texfile << "\\item Evidence: $k=" << k << "$, $N=" << N << "$" << endl;
		texfile << "\\end{itemize}" << endl;	

		texfile << "{\\small\\begin{center}" << endl;
		texfile << "\\begin{tabular}{||r|r|r||r|r|r||}\\hline" << endl;
		texfile << "$x$&cdf$(x)$&cdf$_{exp}(x)$&$x$&cdf$(x)$&cdf$_{exp}(x)$ \\\\ \\hline" << endl;

		if (!retval) {
			writeHTabularCase(tabSize,tabx,tabc,posSize,posx,3,k,N);
			readHTabularCase(posSize, expc);
		} else texfile << "ALGORITHM RETURNED ERROR " << retval << endl;

		int mp = posSize / 2;
		int lp = posSize - 1;

		for (int j=0;j<posSize/2;j++) {
			texfile << posx[j]    << "&" << posc[j]    << "&" << expc[j]    << "&";
			texfile << posx[j+mp] << "&" << posc[j+mp] << "&" << expc[j+mp] << "\\\\" << endl;
		}

		if (posSize % 2) {
			texfile << "&&&" << posx[lp] << "&" << posc[lp] << "&" << expc[lp] << "\\\\" << endl;
		}
		texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
	}
}


int AlgorithmTester::getTabularPriorCount() {return 3;}

void AlgorithmTester::getTabularPrior(int i,int &tabsize, double * tabx,double * tabc)
{
	const double tabx1[] = {1e-6,1e-5,1e-4,1e-3,1e-2};
	const double tabc1[] = {0,0.01,0.1,0.7,1};
	const int	tabs1 = 5;	

	const double tabx2[] = {1e-4,1e-2};
	const double tabc2[] = {0,1};
	const double tabs2 = 2;

	const double tabx3[] = {1e-4,2e-4,3e-4,4e-4,5e-4};
	const double tabc3[] = {   0, 0.1, 0.4, 0.8,   1};
	const double tabs3 = 5;

	int i1,i2,i3;

	switch(i) {

	case 0:
		tabsize = tabs1;
		for (i1 = 0 ; i1 < tabsize ; i1++) {
			tabx[i1] = tabx1[i1];
			tabc[i1] = tabc1[i1];
		}
		break;
	case 1:
		tabsize = tabs2;
		for (i2 = 0 ; i2 < tabsize ; i2++) {
			tabx[i2] = tabx2[i2];
			tabc[i2] = tabc2[i2];
		}
		break;
	case 2:
		tabsize = tabs3;
		for (i3 = 0 ; i3 < tabsize ; i3++) {
			tabx[i3] = tabx3[i3];
			tabc[i3] = tabc3[i3];
		}
		break;
	}
}

/* Writes a tabular homogeneous case to the mathematica file.
 * likelihoods: poisson[k,T] (0), normal[m,s] (1), lognormal[n,t] (2), binomial[k,N] (3)
 */
void AlgorithmTester::writeHTabularCase(int tabSize, double * tabx, double * tabc, int posSize, double * posx, int likelihood, double p1, double p2)
{
	char b1[100],b2[100],b3[100];

	if (writedata && tabSize == 0) {
		datafile << "hresults = {};" << endl;
	} else if (writedata) {
		datafile << "prior = Which[";
		for (int i = 1 ; i < tabSize ; i++) {
			double pdf = (tabc[i] - tabc[i - 1]) / (tabx[i] - tabx[i - 1]);
			datafile << "x >= " << mx(tabx[i - 1],b1) << " 0 && x < " << mx(tabx[i],b2) << "," << mx(pdf,b3) << "," << endl;
		}
		datafile << "True, 0];" << endl;

		switch(likelihood) {
		// poisson
		case 0: 
			datafile << "like = PDF[GammaDistribution[" << mx(p1+1,b1) << "," << mx(1/p2,b2) << "], x];" << endl; 
			break;

		// normal
		case 1: 
			datafile << "like = PDF[NormalDistribution[" << mx(p1,b1) << "," << mx(p2,b2) << "], x];" << endl;
			break;

		// lognormal
		case 2: 
			datafile << "like = PDF[LogNormalDistribution[" << mx(p1,b1) << "," << mx(p2,b2) << "], x];" << endl;
			break;

		// binomial
		case 3: 
			datafile << "like = PDF[BetaDistribution[" << mx(p1 + 1,b1) << "," << mx(p2 - p1 + 1,b2) << "], x];" << endl; 
			break;
		}

		datafile << "integral[y_] := NIntegrate[prior*like, {x," << mx(tabx[0],b1) << ", y}];" << endl;
		datafile << "normfact = N[integral[" << mx(tabx[tabSize - 1],b1) << "]];" << endl;

		datafile << "hresults = Join[hresults,{";
		for (int j = 0 ; j < posSize; j++) 
			datafile << "integral[" << mx(posx[j],b1) << "]/normfact" << (j < posSize - 1 ? "," : "}];");
	}
}

void AlgorithmTester::readHTabularCase(int posSize, double * expc)
{
	if (readdata) {
		for (int i = 0 ; i < posSize ; i++) rsltfile >> expc[i];
	} else {
		for (int i = 0 ; i < posSize ; i++) expc[i] = 0;
	}
}

////////////////////////////////////////////////////////////////////////////

void AlgorithmTester::testNormalPDFCDF()
{
	double a, b;
	int cases = getNormalParameterCount();
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Normal PDF, CDF, and Percentiles}" << endl;
	for (int i = 0 ; i < cases ; i++) {
		getNormalParameters(i,a,b);
		texfile << "\\pagebreak" << endl;
		texfile << "\\subsection{Normal(" << a << "," << b << ")}" << endl;
		constructPDFCDFTable(1,a,b);
		constructSampleTable(1,a,b);
	}
}

void AlgorithmTester::testLogNormalPDFCDF()
{
	double a, b;
	int cases = getLogNormalParameterCount();
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{LogNormal PDF, CDF, and Percentiles}" << endl;
	for (int i = 0 ; i < cases ; i++) {
		getLogNormalParameters(i,a,b);
		texfile << "\\pagebreak" << endl;
		texfile << "\\subsection{LogNormal(" << a << "," << b << ")}" << endl;
		constructPDFCDFTable(2,a,b);
		constructSampleTable(2,a,b);
	}
}

void AlgorithmTester::testTrLogNormalPDFCDF()
{
	double a, b;
	int cases = getTrLogNormalParameterCount();
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Truncated LogNormal PDF, CDF, and Percentiles}" << endl;
	for (int i = 0 ; i < cases ; i++) {
		getTrLogNormalParameters(i,a,b);
		texfile << "\\pagebreak" << endl;
		texfile << "\\subsection{TRLN(" << a << "," << b << ")}" << endl;
		constructPDFCDFTable(4,a,b);
		constructSampleTable(4,a,b);
	}
}

void AlgorithmTester::testGammaPDFCDF()
{
	double a, b;
	int cases = getGammaParameterCount();
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Gamma PDF, CDF, and Percentiles}" << endl;
	for (int i = 0 ; i < cases ; i++) {
		getGammaParameters(i,a,b);
		texfile << "\\pagebreak" << endl;
		texfile << "\\subsection{Gamma(" << a << "," << b << ")}" << endl;
		constructPDFCDFTable(0,a,b);
		constructSampleTable(0,a,b);
	}
}

void AlgorithmTester::testBetaPDFCDF()
{
	double a, b;
	int cases = getBetaParameterCount();
	texfile << "\\pagebreak" << endl;
	texfile << "\\section{Beta PDF, CDF, and Percentiles}" << endl;
	for (int i = 0 ; i < cases ; i++) {
		getBetaParameters(i,a,b);
		texfile << "\\pagebreak" << endl;
		texfile << "\\subsection{Beta(" << a << "," << b << ")}" << endl;
		constructPDFCDFTable(3,a,b);
		constructSampleTable(3,a,b);
	}
}

void AlgorithmTester::constructSampleTable(int distribution, double a, double b)
{

	double x[] = {0.001, 0.005, 0.01, 0.025, 0.05, 0.10, 0.2,.3,.4,.5,.6,.7,.8,.9,.95,.975,.99,.995,.999};

	double sampledv[19][20];
	double samplemn[19][3];
	double samplesd[19][3];

	Distribution dist;

	for (int cs = 0 ; cs < 3 ; cs++) {

		int samples;
		switch(cs) {
			case 0: samples = 2000; break;
			case 1: samples = 2000; break;
			case 2: samples = 2000; break;
		}

		for (int sset = 0 ; sset < 20 ; sset++) {

			dist.clear(samples);
			for (int i = 0 ; i < samples ; i++) {
				double value;
				switch(distribution) {
					case 0: value = Java_redcas_numerical_NativeGamma_getSample(0,0,b,a); break;
					case 1: value = Java_redcas_numerical_NativeNormal_getSample(0,0,a,b); break;
					case 2: value = Java_redcas_numerical_NativeLogNormal_getSample(0,0,a,b); break;
					case 3: value = Java_redcas_numerical_NativeBeta_getSample(0,0,a,b); break;
					case 4: value = Java_redcas_numerical_NativeTrLogNormal_getSample(0,0,a,b); break;
				}
				dist.add(value,1./samples);
			}

			for (int j = 0 ; j < 19 ; j++) sampledv[j][sset] = dist.getPercentile(x[j]);
		}

		for (int stat = 0 ; stat < 19 ; stat++) {
		   samplemn[stat][cs] = 0;
		   samplesd[stat][cs] = 0;
		   for (sset = 0 ; sset < 20 ; sset++) samplemn[stat][cs] += sampledv[stat][sset];
		   samplemn[stat][cs] /= 20;
		   for (sset = 0 ; sset < 20 ; sset++) samplesd[stat][cs] += (sampledv[stat][sset] - samplemn[stat][cs]) * (sampledv[stat][sset] - samplemn[stat][cs]);
		   samplesd[stat][cs] /= 19;
		   samplesd[stat][cs] = pow(samplesd[stat][cs],0.5);
		}
	}

	texfile << "{\\small \\begin{center}" << endl;
	texfile << "\\begin{tabular}{|r|r|r|r|r|r|r|r|}\\hline" << endl;
	texfile << "$p$ & $x$ & $x_{2K}$ & $sd_{2K}$ & $x_{2K}$ & $sd_{2K}$ & $x_{2K}$ & $sd_{2K}$ \\\\ \\hline" << endl;

	for (int stat = 0 ; stat < 19 ; stat++) {

		double xref;
		switch (distribution) {
		case 0: xref = Java_redcas_numerical_NativeGamma_getQuantile(0, 0, b, a, x[stat]); break;
		case 1: xref = Java_redcas_numerical_NativeNormal_getQuantile(0, 0, a, b, x[stat]); break;
		case 2: xref = Java_redcas_numerical_NativeLogNormal_getQuantile(0, 0, a, b, x[stat]);	break;
		case 3:	xref = Java_redcas_numerical_NativeBeta_getQuantile(0, 0, a, b, x[stat]); break;
		case 4:	xref = Java_redcas_numerical_NativeTrLogNormal_getQuantile(0, 0, a, b, x[stat]); break;
		}

		texfile << "\\texttt{" << x[stat] << "}&";
		texfile << "\\texttt{" << xref << "}&";
		texfile << "\\texttt{" << samplemn[stat][0] << "}&";
		texfile << "\\texttt{" << samplesd[stat][0] << "}&";
		texfile << "\\texttt{" << samplemn[stat][1] << "}&";
		texfile << "\\texttt{" << samplesd[stat][1] << "}&";
		texfile << "\\texttt{" << samplemn[stat][2] << "}&";
		texfile << "\\texttt{" << samplesd[stat][2] << "}\\\\" << endl;
	}
	
	texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;
}

void AlgorithmTester::constructPDFCDFTable(int distribution, double a, double b)
{
	double x[] = {0.001, 0.01, 0.05, 0.10, 0.25, 0.50, 0.75, 0.90, 0.95, 0.99, 0.999};
	
	char b1[100],b2[100],b3[100];

	double xval[100];
	double pdf[100];
	double cdf[100];
	double exppdf[100];
	double expcdf[100];
	double expprc[100];
	double mn, expmn, var, expvar;
	char * label, * parameter1, * parameter2;

	switch (distribution) {

	case 0:
		mn = Java_redcas_numerical_NativeGamma_getMean(0, 0, b, a);
		var = Java_redcas_numerical_NativeGamma_getVariance(0, 0, b, a);
		label = "GammaDistribution"; parameter1 = "shape"; parameter2 = "scale ($\\beta$)";
		break;
	case 1:
		mn = Java_redcas_numerical_NativeNormal_getMean(0, 0, a, b);
		var = Java_redcas_numerical_NativeNormal_getVariance(0, 0, a, b);
		label = "NormalDistribution"; parameter1 = "$\\mu$"; parameter2 = "$\\sigma$";
		break;
	case 2:
		mn = Java_redcas_numerical_NativeLogNormal_getMean(0, 0, a, b);
		var = Java_redcas_numerical_NativeLogNormal_getVariance(0, 0, a, b);
		label = "LogNormalDistribution"; parameter1 = "$\\nu$"; parameter2 = "$\\tau$";
		break;
	case 4:
		mn = Java_redcas_numerical_NativeTrLogNormal_getMean(0, 0, a, b);
		var = Java_redcas_numerical_NativeTrLogNormal_getVariance(0, 0, a, b);
		label = "LogNormalDistribution"; parameter1 = "$\\nu$"; parameter2 = "$\\tau$";
		break;
	case 3:
		mn = Java_redcas_numerical_NativeBeta_getMean(0, 0, a, b);
		var = Java_redcas_numerical_NativeBeta_getVariance(0, 0, a, b);
		label = "BetaDistribution"; parameter1 = "$\\alpha$"; parameter2 = "$\\beta$";
		break;
	}

	if (readdata) rsltfile >> expmn >> expvar;
	else {
		expmn = expvar = 0;
	}

	if (writedata) {
		datafile << "Mean[" << label << "[" << mx(a,b1) << "," << mx(b,b2) << "]]," << endl;
		datafile << "Variance[" << label << "[" << mx(a,b1) << "," << mx(b,b2) << "]]," << endl;
	}

	for (int i = 0 ; i < 11 ; i++) {

		switch (distribution) {

		case 0:
			xval[i] = Java_redcas_numerical_NativeGamma_getQuantile(0, 0, b, a, x[i]);
			pdf[i] = Java_redcas_numerical_NativeGamma_getPDF(0, 0, b, a, xval[i]);
			cdf[i] = Java_redcas_numerical_NativeGamma_getCDF(0, 0, b, a, xval[i]);
			break;
		case 1:
			xval[i] = Java_redcas_numerical_NativeNormal_getQuantile(0, 0, a, b, x[i]);
			pdf[i] = Java_redcas_numerical_NativeNormal_getPDF(0, 0, a, b, xval[i]);
			cdf[i] = Java_redcas_numerical_NativeNormal_getCDF(0, 0, a, b, xval[i]);
			break;
		case 2:
			xval[i] = Java_redcas_numerical_NativeLogNormal_getQuantile(0, 0, a, b, x[i]);
			pdf[i] = Java_redcas_numerical_NativeLogNormal_getPDF(0, 0, a, b, xval[i]);
			cdf[i] = Java_redcas_numerical_NativeLogNormal_getCDF(0, 0, a, b, xval[i]);
			break;
		case 3:
			xval[i] = Java_redcas_numerical_NativeBeta_getQuantile(0, 0, a, b, x[i]);
			pdf[i] = Java_redcas_numerical_NativeBeta_getPDF(0, 0, a, b, xval[i]);
			cdf[i] = Java_redcas_numerical_NativeBeta_getCDF(0, 0, a, b, xval[i]);
			break;
		case 4:
			xval[i] = Java_redcas_numerical_NativeTrLogNormal_getQuantile(0, 0, a, b, x[i]);
			pdf[i] = Java_redcas_numerical_NativeTrLogNormal_getPDF(0, 0, a, b, xval[i]);
			cdf[i] = Java_redcas_numerical_NativeTrLogNormal_getCDF(0, 0, a, b, xval[i]);
			break;
		}

		if (readdata) rsltfile >> exppdf[i] >> expcdf[i] >> expprc[i];
		else exppdf[i] = expcdf[i] = expprc[i] = 0;
		if (writedata) {
			datafile << "PDF[" << label << "[" << mx(a,b1) << "," << mx(b,b2) << "]," << mx(xval[i],b3) << "]," << endl;
			datafile << "CDF[" << label << "[" << mx(a,b1) << "," << mx(b,b2) << "]," << mx(xval[i],b3) << "]," << endl;
			datafile << "Quantile[" << label << "[" << mx(a,b1) << "," << mx(b,b2) << "]," << mx(x[i],b3) << "]," << endl;
		}
	}

	texfile << "{\\small \\begin{center}" << endl;
	texfile << "\\begin{tabular}{|r|r|r|r|r|r|}\\hline" << endl;
	texfile << "\\multicolumn{2}{|l|}{Distribution} & \\multicolumn{4}{|r|}{\\texttt{" << label << "}} \\\\" << endl;
	texfile << "\\multicolumn{2}{|l|}{" << parameter1 << "} & \\multicolumn{4}{|r|}{\\texttt{" << a << "}} \\\\" << endl;
	texfile << "\\multicolumn{2}{|l|}{" << parameter2 << "} & \\multicolumn{4}{|r|}{\\texttt{" << b << "}} \\\\" << endl;
	texfile << "\\multicolumn{2}{|l|}{mean} & \\multicolumn{4}{|r|}{\\texttt{" << mn << "}} \\\\" << endl;
	texfile << "\\multicolumn{2}{|l|}{exp. mean} & \\multicolumn{4}{|r|}{\\texttt{" << expmn << "}} \\\\" << endl;
	texfile << "\\multicolumn{2}{|l|}{variance} & \\multicolumn{4}{|r|}{\\texttt{" << var << "}} \\\\" << endl;
	texfile << "\\multicolumn{2}{|l|}{exp. variance} & \\multicolumn{4}{|r|}{\\texttt{" << expvar << "}} \\\\ \\hline" << endl;
	texfile << "$x$ & exp & CDF & exp & PDF & exp \\\\ \\hline" << endl;

	for (int j = 0 ; j < 11 ; j++) {
		texfile << "\\texttt{" << xval[j] << "}&\\texttt{" << expprc[j] << "}&\\texttt{" << cdf[j] << "}&\\texttt{" << expcdf[j] << "}&\\texttt{" << pdf[j] << "}&\\texttt{" << exppdf[j] << "}\\\\" << endl;
	}

	texfile << "\\hline \\end{tabular} \\end{center} }" << endl << endl;

}

// writes x to the text buffer in format #.###########*10^###
char * AlgorithmTester::mx(double x, char *b)
{
	int z = (x != 0) ? (int)floor(log(fabs(x))/log(10)) : 0;
	double y = x / pow(10,z);
	sprintf(b,"%1.9f*10^%d",y,z);
	return b;
}

int AlgorithmTester::getBetaParameterCount() {return 16;}
void AlgorithmTester::getBetaParameters(int index, double &a, double &b)
{
	double alphas[] = {0.1,10,1000,100000};
	double betas[] =  {1,100,10000,1000000};
	a = alphas[index % 4];
	b = betas[index  / 4];
}

int AlgorithmTester::getGammaParameterCount() {return 36;}
void AlgorithmTester::getGammaParameters(int index, double &a, double &b)
{
	double alphas[] = {0.2,0.5,1,5,25,100};
	double betas[] = {1e-8,1e-6,1e-4,1e-2,1,1e2};
	a = alphas[index % 6];
	b = betas[index / 6];
}

int AlgorithmTester::getNormalParameterCount() {return 16;}
void AlgorithmTester::getNormalParameters(int index, double &a, double &b)
{
	double alphas[] = {0,-10,10,1000};
	double betas[] = {1,2,5,10};
	a = alphas[index%4];
	b = betas[index/4];
}

int AlgorithmTester::getLogNormalParameterCount() {return 36;}
void AlgorithmTester::getLogNormalParameters(int index, double &a, double &b)
{
	double alphas[] = {2,0,-2,-6,-12,-18};
	double betas[] =  {0.1,0.2,0.5,1,2,4};
	a = alphas[index%6];
	b = betas[index/6];
}

int AlgorithmTester::getTrLogNormalParameterCount() {return 36;}
void AlgorithmTester::getTrLogNormalParameters(int index, double &a, double &b)
{
	double alphas[] = {2,0,-2,-6,-12,-18};
	double betas[] =  {0.1,0.2,0.5,1,2,4};
	a = alphas[index%6];
	b = betas[index/6];
}
