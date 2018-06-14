using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

/**
 * This class serves as c sharp wrapper for all homogeneous analysis
 * related functions within RDAT service DLL.
 */
 
namespace Prediction_Technologies
{
    class rdatsrvHomoWrapper
    {
        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_Create", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern int REDCASSRV_HOMO_Create(Int32 priorType, double pp1, double pp2, Int32 likelihoodType, double lp1, double lp2);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_createF", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern int REDCASSRV_HOMO_createF(Int32 priorType, Int32 priorInputMode, double pp1, double pp2, 
            Int32 likelihoodType, Int32 likelihoodInputMode, double lp1, double lp2);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_destory", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern void REDCASSRV_HOMO_destory(Int32 iHandle);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getPDF", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getPDF(Int32 iHandle, double x);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getCDF", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getCDF(Int32 iHandle, double x);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getPDFA", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getPDFA(Int32 iHandle, double[] x, double[] y, int iCount);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getCDFA", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getCDFA(Int32 iHandle, double[] x, double[] y, int iCount);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getQuantile", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getQuantile(Int32 iHandle, double z);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getMean", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getMean(Int32 iHandle);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getMedian", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getMedian(Int32 iHandle);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_HOMO_getVariance", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_HOMO_getVariance(Int32 iHandle);


        /// <summary>
        /// This function is used to create a homogeneous analysis using the RDAT service. Please note that the analysis handle must be
        /// released using destroy function after usage. 
        /// 
        /// The input mode for prior and likelihood function are assumed to be default input mode.
        /// 
        /// </summary>
        /// <param name="priortype">The type of the prior distribution</param>
        /// <param name="pp1">First parameter for the prior distribution</param>
        /// <param name="pp2">Second parameter for the prior distribution</param>
        /// <param name="likelihoodtype">The type of the likelihood distribution</param>
        /// <param name="lp1">first parameter for the likelihood distribution</param>
        /// <param name="lp2">second parameter for the likelihood distribution</param>
        /// 
        /// <returns>The handle to the distribution. -1 if the distribution was not created successfully.</returns>
        public int create(Int32 priortype, double pp1, double pp2, Int32 likelihoodtype, double lp1, double lp2)
        {
            return REDCASSRV_HOMO_Create(priortype, pp1, pp2, likelihoodtype, lp1, lp2);
        }

        /// <summary>
        /// This function is used to create a homogeneous analysis using the RDAT service. Please note that the analysis handle must be
        /// released using destroy function after usage. 
        /// 
        /// The input mode for prior and likelihood function are assumed to be default input mode.
        /// 
        /// </summary>
        /// <param name="priortype">The type of the prior distribution</param>
        /// <param name="pp1">First parameter for the prior distribution</param>
        /// <param name="pp2">Second parameter for the prior distribution</param>
        /// <param name="likelihoodtype">The type of the likelihood distribution</param>
        /// <param name="lp1">first parameter for the likelihood distribution</param>
        /// <param name="lp2">second parameter for the likelihood distribution</param>
        /// 
        /// <returns>The handle to the distribution. -1 if the distribution was not created successfully.</returns>
        public int create(Int32 priortype, Int32 priorInputMode, double pp1, double pp2, Int32 likelihoodtype, Int32 likelihoodInputMode, double lp1, double lp2)
        {
            return REDCASSRV_HOMO_createF(priortype, priorInputMode, pp1, pp2, likelihoodtype, likelihoodInputMode, lp1, lp2);
        }

        /// <summary>
        /// This function is used to destroy distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        public void destory(Int32 iHandle)
        {
            REDCASSRV_HOMO_destory(iHandle);
        }

        /// <summary>
        /// Returns PDF at x of distribution with given handle.This function is used to draw the PDF curve.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        /// <param name="x">value of x</param>
        /// <returns>PDF value at x of distribution</returns>
        public double getPDF(int iHandle, double x)
        {
            return REDCASSRV_HOMO_getPDF(iHandle, x);
        }

        public double getPDFA(int iHandle, double [] x, double [] y, int iCount)
        {
            return REDCASSRV_HOMO_getPDFA(iHandle, x, y, iCount);
        }

        /// <summary>
        /// Returns CDF at x of distribution with given handle.This function is used to draw the CDF curve.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution.</param>
        /// <param name="x">value of x</param>
        /// <returns>CDF value at x of distribution.</returns>
        public double getCDF(int iHandle, double x)
        {
            return REDCASSRV_HOMO_getCDF(iHandle, x);
        }

        public double getCDFA(int iHandle, double [] x, double [] y, int iCount)
        {
            return REDCASSRV_HOMO_getCDFA(iHandle, x, y, iCount);
        }

        /// <summary>
        /// Returns quantile of distribution with given handle.This function is used to get the percentage estimation
        /// of the distribution. It is part of the output required for BLine project. For 95% estimation, the input z value 
        /// should be 0.95.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        /// <param name="z">The percentage value.</param>
        /// <returns>Quantile of distribution within the given handle at the input percentage z.</returns>
        public double getQuantile(int iHandle, double z)
        {
            return REDCASSRV_HOMO_getQuantile(iHandle, z);
        }

        /// <summary>
        /// Returns mean value of distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution.</param>
        /// <returns>The mean value of distribution with given handle.</returns>
        public double getMean(int iHandle)
        {
            return REDCASSRV_HOMO_getMean(iHandle);
        }

        /// <summary>
        /// Returns median value of distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution.</param>
        /// <returns>The median value of distribution with given handle</returns>
        public double getMedian(int iHandle)
        {
            return REDCASSRV_HOMO_getMedian(iHandle);
        }

        /// <summary>
        /// Returns variance of distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        /// <returns>The variance of the distribution with given handle.</returns>
        public double getVariance(int iHandle)
        {
            return REDCASSRV_HOMO_getVariance(iHandle);
        }

        public System.Collections.ArrayList getXArray(int iHandle, int iPoints)
        {
            double dXMin = 0;                                                   // Minimum value of X
            double dXMax = getQuantile(iHandle, 0.995);    // Maximum value of X
            double dSteps = (dXMax - dXMin) / iPoints;  

            System.Collections.ArrayList pdfXArray = new System.Collections.ArrayList(iPoints);
            for (int i = 0; i < iPoints; i++)
            {
                pdfXArray.Add(dXMin + dSteps * (i+1));
            }

            return pdfXArray;
        }

        public System.Collections.ArrayList getPDFY(int iHandle, int iPoints)
        {
            System.Collections.ArrayList pdfYArray = new System.Collections.ArrayList(iPoints);
            System.Collections.ArrayList pdfXArray = getXArray(iHandle, iPoints);
            double[] xInput = new double[iPoints];
            double[] yOutput = new double[iPoints];
            for (int i = 0; i < iPoints; i++)
            {
                xInput[i] = (double)pdfXArray[i];
            }

            getPDFA(iHandle, xInput, yOutput, iPoints);

            for (int i = 0; i < iPoints; i++)
            {
                pdfYArray.Add(yOutput[i]);
            }

            return pdfYArray;
        }

        public System.Collections.ArrayList getCDFY(int iHandle, int iPoints)
        {
            System.Collections.ArrayList pdfYArray = new System.Collections.ArrayList(iPoints);
            System.Collections.ArrayList pdfXArray = getXArray(iHandle, iPoints);

            double[] xInput = new double[iPoints];
            double[] yOutput = new double[iPoints];
            for (int i = 0; i < iPoints; i++)
            {
                xInput[i] = (double)pdfXArray[i];
            }
                
            getCDFA(iHandle, xInput, yOutput, iPoints);

            for (int i = 0; i < iPoints; i++)
            {
                pdfYArray.Add(yOutput[i]);
            }
            return pdfYArray;
        }
    }
}
