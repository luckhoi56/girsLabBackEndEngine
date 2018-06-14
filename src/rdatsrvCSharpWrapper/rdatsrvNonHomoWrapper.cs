using System;
using System.Collections.Generic;
using System.Text;

/**
 * This class serves as c sharp wrapper for all non-homogeneous analysis
 * related functions within RDAT service DLL.
 */

namespace Prediction_Technologies
{
    class rdatsrvNonHomoWrapper
    {
        /// <summary>
        ///  Create a new session for non-homogeneous analysis and return the session
        /// id. If the session was not created successfully, the return value will be 
        /// -1.
        /// 
        /// Please note that all session needs to be destroyed by user using function
        /// destroySession once it is no longer needed.
        /// </summary>
        /// <returns>the non-homogeneous analysis session id; -1 if the session cannot
        /// be created.</returns>
        public int createNonHomogeneousSession()
        {

        }

        /// <summary>
        /// Destroy the non-homogeneous analysis session with input session id.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        public void destroySession(int iSession)
        {

        }

        /// <summary>
        /// Clear analysis with input session id. This function is used to clear
        /// all data loaded into analysis engine for the current analysis session.
        /// 
        /// It should be used when user restarts the analysis.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        public void clearAnalysis(int iSession)
        {

        }

        /// <summary>
        /// Set the type of non-homogeneous analysis.
        /// The input type must be one of the four types defined in class rdatsrvConstant
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="iType">the non-homogeneous analysis type. The types supported so far include:
        /// nRedcassrv_NonHomo_BETA_BINOMIAL, nRedcassrv_NonHomo_GAMMA_POISSON,
        /// nRedcassrv_NonHomo_LOGNORM_POISSON, nRedcassrv_NonHomo_LOGNORM_BINOMIAL</param>
        public void setModelType(int iSessionID, int iType)
        {

        }

        /// <summary>
        /// Returns the mean of population variance mean.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the mean of population variance mean</returns>
        public double getPopVarMeanMean(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Returns the p percentile of the population variance mean
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="p">percentage; the value should be between 0 and 1</param>
        /// <returns>the population variance mean percentile.</returns>
        public double getPopVarMeanPercentile(int iSessionID, double p)
        {
            return NativeNonHomog2.getMeasureN(_sessionID, PARAMETER_DEFAULT, 0, MEASURE_MEAN,
                    p);
        }

        /// <summary>
        /// Returns the mean of the popvar variance.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the mean of popvar variance </returns>
        public double getPopVarVarianceMean(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Returns the p percentile of the popvar variance.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="p"></param>
        /// <returns></returns>
        public double getPopVarVariancePercentile(int iSessionID, double p)
        {
            return 0;
        }

        /// <summary>
        /// Returns the mean estimate of the popvar z percentile. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="z">the z value</param>
        /// <returns>the mean estimate of the popvar z percentile</returns>
        public double getPopVarPercentileMean(int iSessionID, double z)
        {
            return 0;
        }

        /// <summary>
        /// Returns the p percentile of the popvar z percentile. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="z">z value</param>
        /// <param name="p"></param>
        /// <returns> the p percentile of the popvar z percentile. </returns>
        public double getPopVarPercentilePercentile(int iSessionID, double z, double p)
        {
            return 0;
        }

        /// <summary>
        /// Returns the p percentile of the popvar CDF at x. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="x">x value</param>
        /// <param name="p">percentile value</param>
        /// <returns>the p percentile of the popvar CDF at x. </returns>
        public double getPopVarCDF(int iSessionID, double x, double p)
        {
            return 0;
        }

        /// <summary>
        /// Returns the p percentile of the popvar PDF at x. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="x">x value</param>
        /// <param name="p">percentile value</param>
        /// <returns>the p percentile of the popvar PDF at x</returns>
        public double getPopVarPDF(int iSessionID, double x, double p)
        {
            return 0;
        }

        /// <summary>
        /// Returns the mean of the popvar cdf at x. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="x">x value</param>
        /// <returns>the mean of the popvar cdf at x. </returns>
        public double getPopVarCDFMean(int iSessionID, double x)
        {
            return 0;
        }

        /// <summary>
        /// Returns the mean of the popvar pdf at x. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="x">x value</param>
        /// <returns>the mean of the popvar pdf at x. </returns>
        public double getPopVarPDFMean(int iSessionID, double x)
        {
            return 0;
        }

        /// <summary>
        /// Returns the recommended upper bound for the variable. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the recommended upper bound for the variable. </returns>
        public double getPopVarUpperBound(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Returns the recommended lower bound for the variable. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the recommended lower bound for the variable. </returns>
        public double getPopVarLowerBound(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Adds a new data point (evidence) to the session. If the data contains
        /// only the estimate or the event/exposure type data, set the other fields
        /// to 0. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="x_star">median (estimate)</param>
        /// <param name="tau">sigma of lognormal</param>
        /// <param name="events">number of events</param>
        /// <param name="exposure">number of demands / total run time.</param>
        public void addDataPoint(int iSessionID, double x_star, double tau, double events,
                double exposure)
        {
        }

        /// <summary>
        /// Sets the number of samples to be used in the sampling of the posterior 
        /// distribution.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="samples">number of samples</param>
        public void setSamples(int iSessionID, int samples)
        {
        }

        /// <summary>
        /// Executes the analysis.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the execution status.</returns>
        public int executeAnalysis(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Constructs a binned version of the posterior distribution. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="size">size of the bin</param>
        /// <param name="logScale">use logscale or not</param>
        /// <returns>true if the bin has been constructed successfully; otherwise return false.</returns>
        public boolean constructBins(int iSessionID, int size, boolean logScale)
        {
            return true;
        }

        /// <summary>
        /// Returns the value of the first parameter at the specified grid point. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="index">index of the grid point</param>
        /// <returns>the value of the first parameter at the specified grid point. </returns>
        public double getBinnedAlpha(int iSessionID, int index)
        {
            return 0;
        }

        /// <summary>
        /// Returns the value of the second parameter at the specified grid point. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="index">index of the grid point</param>
        /// <returns>the value of the second parameter at the specified grid point.</returns>
        public double getBinnedBeta(int iSessionID, int index)
        {
            return 0;
        }

        /// <summary>
        /// Returns the density at the specified grid point. Adjust for the
        /// logarithmic or linear scaling applied during binning. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="alpha">index for alpha</param>
        /// <param name="beta">index for beta</param>
        /// <returns></returns>
        public double getBinnedDensity(int iSessionID, int alpha, int beta)
        {
            return 0;
        }

        /// <summary>
        /// Returns the marginal distribution of the first parameter. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="size">size of alpha</param>
        /// <param name="mode">mode value</param>
        /// <returns>marginal alpha structure</returns>
        public DPD getMarginalAlpha(int iSessionID, int size, int mode)
        {
            return null
        }

        /// <summary>
        /// Returns the marginal distribution of the second parameter. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="size">size of beta</param>
        /// <param name="mode">mode value</param>
        /// <returns>marginal beta structure</returns>
        public DPD getMarginalBeta(int iSessionID, int size, int mode)
        {
            return null
        }

        /// <summary>
        /// Returns the number of samples generated during the sampling procedure. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the number of samples generated during the sampling procedure. </returns>
        public int getSampleCount(int iSessionID)
        {
            return 1;
        }

        /// <summary>
        /// Returns the value of the first parameter for the given sample point. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="index">index of alpha sample point</param>
        /// <returns>the value of the first parameter for the given sample point. </returns>
        public double getSampleAlpha(int iSessionID, int index)
        {
            return 0;
        }

        /// <summary>
        /// Returns the value of the second parameter for the given sample point. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="index">index of beta sample point</param>
        /// <returns>the value of the second parameter for the given sample point. </returns>
        public double getSampleBeta(int iSessionID, int index)
        {
            return 0;
        }

        /// <summary>
        /// Returns the probability for the given sample point. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="index">index of the sample point</param>
        /// <returns>the probability for the given sample point. </returns>
        
        public double getSampleProbability(int iSessionID, int index)
        {
            return 0;
        }

        /// <summary>
        /// Adds a sample to the DLL. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="alpha">alpha value</param>
        /// <param name="beta">beta value</param>
        /// <param name="prob">probability value</param>
        public void addSample(int iSessionID, double alpha, double beta, double prob)
        {
        }

        /// <summary>
        /// Returns a string describing the current status. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>a string describing the current status. </returns>
        public String getStatus(int iSessionID)
        {
            return "";
        }

        /// <summary>
        /// Returns the progress, a value between 0 and 100. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>the progress value between 0 and 100. </returns>
        public int getProgress(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Returns a value indicating the status of the DLL 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns> a value indicating the status of the DLL </returns>
        public int getStatusCode(int iSessionID)
        {
            return 0;
        }

        /// <summary>
        /// Requests that the current operation be canceled. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        public void requestCancel(int iSessionID)
        {
        }

        /// <summary>
        /// Clear the status
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        public void clearStatus(int iSessionID)
        {
        }

        /// <summary>
        /// Returns true if it was requested that the operation be canceled. 
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <returns>true if the cancel request has been sent for input session id.</returns>
        public boolean isCancelRequested(int iSessionID)
        {
            return false;
        }

        /// <summary>
        /// Set log normal prior for homogeneous analysis with input session id.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="code"> identifier for parameter combination</param>
        /// <param name="nu1">ln(median) of first parameter lognormal distribution</param>
        /// <param name="tau1">tau (sigma) of first parameter lognormal distribution</param>
        /// <param name="nu2">ln(median) of second parameter lognormal distribution</param>
        /// <param name="tau2">tau (sigma) of second parameter lognormal distribution</param>
        public void setLogNormalPrior(int iSessionID, int code, double nu1, double tau1,
                double nu2, double tau2)
        {
        }

        /// <summary>
        /// Returns the natural log of the likelihood at (p1,p2). This value is
        /// obtained separate from any set of samples already computed.
        /// </summary>
        /// <param name="iSessionID">the non-homogeneous analysis session id</param>
        /// <param name="p1">likelihood probability 1</param>
        /// <param name="p2">likelihood probability 2</param>
        /// <returns>the log likelihood function between two probability</returns>
        public double getLnLikelihood(int iSessionID, double p1, double p2)
        {
            return 0;
        }

    }
}
