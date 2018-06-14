using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

/**
 * This class serves as c sharp wrapper for all the functions related 
 * with distributions supported within RDAT service DLL.
 */

namespace Prediction_Technologies
{
    class rdatsrvDistWrapper
    {
        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_create", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern int REDCASSRV_DIST_create(Int32 distType, Int32 inputMode, double p1, double p2);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_destory", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern void REDCASSRV_DIST_destory(Int32 iHandle);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_getPDF", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_DIST_getPDF(Int32 iHandle, double x);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_getCDF", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_DIST_getCDF(Int32 iHandle, double x);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_getQuantile", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_DIST_getQuantile(Int32 iHandle, double z);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_getMean", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_DIST_getMean(Int32 iHandle);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_getMedian", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_DIST_getMedian(Int32 iHandle);

        [DllImport("redcassrv.dll", EntryPoint = "REDCASSRV_DIST_getVariance", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        static extern double REDCASSRV_DIST_getVariance(Int32 iHandle);


        /// <summary>
        /// This function is used to create a distribution with two parameters using the RDAT distribution service. 
        /// Please note that the distribution must be released using destroy function after usage. 
        /// 
        /// </summary>
        /// <param name="disttype">The type of the distribution</param>
        /// <param name="inputmode">The input mode for parameters</param>
        /// <param name="p1">First parameter for the distribution</param>
        /// <param name="p2">Second parameter for the distribution</param>
        /// <returns>The handle to the distribution. -1 if the distribution was not created successfully.</returns>
        public int create(Int32 disttype, Int32 inputMode, double p1, double p2)
        {
            return REDCASSRV_DIST_create(disttype, inputMode, p1, p2);
        }

        /// <summary>
        /// This function is used to destroy distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        public void destory(Int32 iHandle)
        {
            REDCASSRV_DIST_destory(iHandle);
        }

        /// <summary>
        /// Returns PDF at x of distribution with given handle.This function is used to draw the PDF curve.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        /// <param name="x">value of x</param>
        /// <returns>PDF value at x of distribution</returns>
        public double getPDF(int iHandle, double x)
        {
            return REDCASSRV_DIST_getPDF(iHandle, x);
        }

        /// <summary>
        /// Returns CDF at x of distribution with given handle.This function is used to draw the CDF curve.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution.</param>
        /// <param name="x">value of x</param>
        /// <returns>CDF value at x of distribution.</returns>
        public double getCDF(int iHandle, double x)
        {
            return REDCASSRV_DIST_getCDF(iHandle, x);
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
            return REDCASSRV_DIST_getQuantile(iHandle, z);
        }

        /// <summary>
        /// Returns mean value of distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution.</param>
        /// <returns>The mean value of distribution with given handle.</returns>
        public double getMean(int iHandle)
        {
            return REDCASSRV_DIST_getMean(iHandle);
        }

        /// <summary>
        /// Returns median value of distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution.</param>
        /// <returns>The median value of distribution with given handle</returns>
        public double getMedian(int iHandle)
        {
            return REDCASSRV_DIST_getMedian(iHandle);
        }

        /// <summary>
        /// Returns variance of distribution with given handle.
        /// </summary>
        /// <param name="iHandle">The handle to the distribution</param>
        /// <returns>The variance of the distribution with given handle.</returns>
        public double getVariance(int iHandle)
        {
            return REDCASSRV_DIST_getVariance(iHandle);
        }

        public System.Collections.ArrayList getXArray(int iHandle, int iPoints)
        {
            double dXMin = 0;                                                   // Minimum value of X
            double dXMax = getQuantile(iHandle, 0.995);    // Maximum value of X
            double dSteps = (dXMax - dXMin) / iPoints;

            System.Collections.ArrayList pdfXArray = new System.Collections.ArrayList(iPoints);
            for (int i = 0; i < iPoints; i++)
            {
                pdfXArray.Add(dXMin + dSteps * (i + 1));
            }

            return pdfXArray;
        }

        public System.Collections.ArrayList getPDFY(int iHandle, int iPoints)
        {
            System.Collections.ArrayList pdfYArray = new System.Collections.ArrayList(iPoints);
            System.Collections.ArrayList pdfXArray = getXArray(iHandle, iPoints);
            for (int i = 0; i < iPoints; i++)
            {
                pdfYArray.Add(getPDF(iHandle, (double)pdfXArray[i]));
            }

            return pdfYArray;
        }

        public System.Collections.ArrayList getCDFY(int iHandle, int iPoints)
        {
            System.Collections.ArrayList pdfYArray = new System.Collections.ArrayList(iPoints);
            System.Collections.ArrayList pdfXArray = getXArray(iHandle, iPoints);
            for (int i = 0; i < iPoints; i++)
            {
                pdfYArray.Add(getCDF(iHandle, (double)pdfXArray[i]));
            }

            return pdfYArray;
        }
    }
}
