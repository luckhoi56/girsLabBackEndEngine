using System;
using System.Collections.Generic;
using System.Text;

namespace Prediction_Technologies
{
    class rdatsrvConstants
    {
        // Distribution Group
        public const int nRedcassrv_Beta_Distribution = 401;
        public const int nRedcassrv_Binomial_Distribution = 402;
        public const int nRedcassrv_Gamma_Distribution =  403;
        public const int nRedcassrv_Normal_Distribution = 404;
        public const int nRedcassrv_LogNormal_Distribution =  405;
        public const int nRedcassrv_TLogNormal_Distribution = 406;
        public const int nRedcassrv_Uniform_Distribution = 407;
        public const int nRedcassrv_LogUniform_Distribution = 408;
        public const int nRedcassrv_Posisson_Distribution = 409;
        public const int nRedcassrv_Tabular_Distribution = 410;

        // Input Mode related Constants
        public const int nRedcassrv_AlphaBeta_Mode = 500;
        public const int nRedcassrv_MeanSD_Mode = 501;
        public const int nRedcassrv_5th95th_Mode = 502;
        public const int nRedcassrv_AlphaTheta_Mode = 503;
        public const int nRedcassrv_MedianTau_Mode = 504;
        public const int nRedcassrv_MedianEF_Mode = 505;
        public const int nRedcassrv_MeanEF_Mode = 506;
        public const int nRedcassrv_NuTau_Mode = 507;
        public const int nRedcassrv_MinMax_Mode = 508;
        public const int nRedcassrv_EventTime_Mode = 509;
        public const int nRedcassrv_EventDemands_Mode = 510;

        // Non-Homogeneous Analysis related constants
	    /**
	     * Demand based model with Beta variability distribution.
	     */
        public const int nRedcassrv_NonHomo_BETA_BINOMIAL = 601;

	    /**
	     * Time based model with Gamma variability distribution.
	     */
        public const int nRedcassrv_NonHomo_GAMMA_POISSON = 602;


	    /**
	     * Time based model with lognormal variability distribution.
	     */
        public const int nRedcassrv_NonHomo_LOGNORM_POISSON = 603;

	    /**
	     * Demand based model with lognormal variability distribution. <B>Not
	     * implemented.</B>
	     */
        public const int nRedcassrv_NonHomo_LOGNORM_BINOMIAL = 604;
    }
}
