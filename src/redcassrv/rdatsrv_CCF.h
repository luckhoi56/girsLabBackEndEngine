#ifndef RDATSRV_CCF_H__
#define RDATSRV_CCF_H__

#ifdef REDCASSRV_EXPORTS
#define REDCASSRV_API __declspec(dllexport)
#else
#define REDCASSRV_API __declspec(dllimport)
#endif

class CRedcassrvCCF {
public:
	CRedcassrvCCF(void);
	static CRedcassrvCCF * getInstance() { return &instance; }

	int createCCFSession();
	void destroyCCFSession(int iHandle);
	int initialize(int iHandle, int type, int size);
	int setPrior(int iHandle, int iInputMode, int iCCFImpactLevel, double para1,
		double para2);
	int setLikelihood(int iHandle, int iCCFImpactLevel, int events);
	int runAnalysis(int iHandle);
	int getPosteriorDistHandle(int iHandle, int iImpactLevel);

private:
	static CRedcassrvCCF instance;
};

#endif