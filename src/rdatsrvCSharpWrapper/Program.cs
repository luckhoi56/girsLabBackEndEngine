using System;
using System.Collections.Generic;
using System.Text;
using Prediction_Technologies;

/**
 * This is the example class designed to demonstrate how to load RDatService 
 * Wrapper in C# environment. 
 *
 * The procedure to load the functions are the same for both 32 bit and 64 bit 
 * environment 
 */

namespace rdatsrvCSharpWrapper
{
    class Program
    {
        rdatsrvHomoWrapper rdatHomoAnalysisService = new rdatsrvHomoWrapper();
        rdatsrvDistWrapper rdatDistService = new rdatsrvDistWrapper();

        static void Main(string[] args)
        {
            Program test = new Program();
            test.testDistributionService();
            test.testHomogeneousAnalysis();
        }

        /**
         * This function is used to demonstrate how to use the distribution
         * service inside RDAT Service DLL 
         */
        void testDistributionService()
        {
            testDistLognormal();    // Status OK
            testDistBeta();         // Status OK
            testDistGamma();        // Status OK
            testDistNormal();       // Status OK
            testDistTLogNormal();   // Status OK
            testDistUniform();      // Status OK
            testDistLogUniform();
        }

        void testDistLognormal()
        {
            double dMean, dMedian, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a LogNormal distribution with 
            // input mode 5th_95th estimation:
            //
            //   5th:   0.00002
            //  95th:   0.0008
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_LogNormal_Distribution, rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 2.372E-4
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve median value for prior distribution
                // The expected median value is 1.265E-4
                dMedian = rdatDistService.getMedian(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 1.416E-7
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 0.00002 and 0.0008
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // To draw the curve for prior distribution, please use function getPDF and getCDF to 
                // retrieve the value at given x value
                //
                // Normally we use 0 as the minimum value of X, and 99.5% upper bound estimation as 
                // the maximum value of X; user can determine the steps based on the number of points
                // needed to draw the curve

                // The following is an example to draw the CDF curve with 100 data points
                double dXMin = 0;                                                   // Minimum value of X
                double dXMax = rdatDistService.getQuantile(iPriorHandle, 0.995);    // Maximum value of X
                int iPointCount = 100;                                              // Number of points needed for the curve; please modify this 
                // value to the actual number of points needed
                double dSteps = (dXMax - dXMin) / iPointCount;                      // Steps between two points
                for (int i = 1; i <= iPointCount; i++)
                {
                    // The following two variables refer to the X and Y value in the curve
                    // It can be saved in an array. 
                    //
                    // Please note that we always starts from step 1 since 0 is invalid input
                    // for a lot of distributions
                    double dXValue = dXMin + dSteps * i;                            // the X value at step i
                    double dYValue = rdatDistService.getCDF(iPriorHandle, dXValue); // the Y value at step i
                }

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        // Test beta distribution
        void testDistBeta()
        {
            double dMean, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a Beta distribution with 
            // input mode alpha/beta
            //
            //  Alpha: 10
            //  Beta:  5
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_Beta_Distribution,
                rdatsrvConstants.nRedcassrv_AlphaBeta_Mode, 10, 5); 

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 0.6667
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 0.0139
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 0.46 and 0.8473
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);
             
                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        // Test Gamma distribution
        void testDistGamma()
        {
            double dMean, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a Gamma distribution with 
            // input mode alpha/beta
            //
            //  Alpha: 10
            //  Theta:  5
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_Gamma_Distribution,
                rdatsrvConstants.nRedcassrv_AlphaTheta_Mode, 10, 5);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 2.0
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 0.4
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 1.0851 and 3.141
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }

            // Test input mode Mean/Std
            //
            //  Mean: 5
            //  Std:  0.7
            iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_Gamma_Distribution,
                rdatsrvConstants.nRedcassrv_MeanSD_Mode, 5, 0.7);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 5
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 0.49
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 3.9069 and 6.2044
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        void testDistNormal()
        {
            double dMean, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a Normal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_Normal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.0002, 0.0008);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 0.0005
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 3.326e-8
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 0.0002 and 0.0008
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        void testDistTLogNormal()
        {
            double dMean, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a Truncated LogNormal distribution with 
            // input mode Median/Error Factor
            //
            //  Median: 0.0008
            //  Error Factor: 10
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_TLogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_MedianEF_Mode, 0.0008, 10);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 2.131e-3
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 2.734e-5
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 8.0e-5 and 8.0e-3
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        void  testDistUniform()
        {
            double dMean, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a Uniform distribution with 
            // input mode Min Max
            //
            //  Min 0.02
            //  Max 0.08
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_Uniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.02, 0.08);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 0.05
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 3.0e-4
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 0.023 and 0.077
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        void testDistLogUniform()
        {
            double dMean, dVar;
            double d5thEst, d95thEst;

            // In this example, the prior distribution is a Log-Uniform distribution with 
            // input mode Min Max
            //
            //  Min 0.02
            //  Max 0.08
            int iPriorHandle = rdatDistService.create(rdatsrvConstants.nRedcassrv_LogUniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.02, 0.08);

            if (iPriorHandle != -1)
            {
                // Retrieve mean value for prior distribution
                // The expected mean value is 0.0433
                dMean = rdatDistService.getMean(iPriorHandle);

                // Retrieve standard variance for prior distribution
                // The expected var is 2.908e-4
                dVar = rdatDistService.getVariance(iPriorHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // the expected result is 0.0214 and 0.0746
                // For all other estimation, change the second parameter to the value needed
                d5thEst = rdatDistService.getQuantile(iPriorHandle, 0.05);
                d95thEst = rdatDistService.getQuantile(iPriorHandle, 0.95);

                // It is very important to release the resource from RDAT service after retrieving all
                // results.
                rdatDistService.destory(iPriorHandle);
            }
        }

        /**
         * This function is used to demonstrate how to use the homogeneous analysis
         * engine inside RDAT service DLL
         */
        void testHomogeneousAnalysis()
        {
            testHomo_BetaLognormal();           // Status OK
            testHomo_BetaBinomial();            // Status OK
            testHomo_GammaLognormal();          // Status OK
            testHomo_GammaPoisson();            // Status OK
            testHomo_NormalNormal();            // Status OK
            testHomo_LogNormalLogNormal();      // Status OK
            testHomo_LogNormalBinomial();       // Status OK
            testHomo_LogNormalPossion();        // Status OK
            testHomo_TLogNormalLognormal();
            testHomo_TLogNormalBinomial();
            testHomo_UniformNormal();           // Status OK
            testHomo_UniformLogNormal();        // Status OK
            testHomo_UniformBinomial();         // Status OK
            testHomo_UniformPossion();          // Status OK
            testHomo_LogUniformLogNormal();     // Status OK
            testHomo_LogUniformBinomial();      // Status OK
            testHomo_LogUniformPossion();       // Status OK  
        }

        void testHomo_BetaLognormal()
        {
            // In this example, the prior distribution is a Beta distribution with 
            // input mode alpha/beta
            //
            //  Alpha: 10
            //  Beta:  5

            // In this example, the likelihood function used is a Lognormal distribution with the following parameters:
            //    5th: 0.00002
            //    95th : 0.0008
            // This indicates that there is no failure found in the whole testing period.
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Beta_Distribution,
                rdatsrvConstants.nRedcassrv_AlphaBeta_Mode, 10, 5, rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 0.424
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 0.0226
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 0.19
                // The expected value for 9th upper bound is 0.6852
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                System.Collections.ArrayList testX = rdatHomoAnalysisService.getXArray(iAnalysisHandle, 100);
                System.Collections.ArrayList testY = rdatHomoAnalysisService.getPDFY(iAnalysisHandle, 100);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_BetaBinomial()
        {
            // In this example, the prior distribution is a Beta distribution with 
            // input mode alpha/beta
            //
            //  Alpha: 10
            //  Beta:  5

            // In this example, the likelihood function used is a binomial distribution with the following parameters:
            //    event: 0
            //    Demand : 5000
            // This indicates that there is no failure found in the whole testing period.
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Beta_Distribution,
                rdatsrvConstants.nRedcassrv_AlphaBeta_Mode, 10, 5, rdatsrvConstants.nRedcassrv_Binomial_Distribution, 
                rdatsrvConstants.nRedcassrv_EventDemands_Mode, 0, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 1.994E-3
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 3.967E-7
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 1.082e-3
                // The expected value for 9th upper bound is 3.13e-3
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
           }
        }

        void testHomo_GammaLognormal()
        {
            // In this example, the prior distribution is a Gamma distribution with 
            // input mode alpha/Theta
            //
            //  Alpha: 10
            //  Theta:  5

            // In this example, the likelihood distribution is a LogNormal distribution with 
            // input mode 5th_95th estimation:
            //
            //   5th:   0.00002
            //  95th:   0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Gamma_Distribution,
                rdatsrvConstants.nRedcassrv_AlphaTheta_Mode, 10, 5, rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 0.6585
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 0.1066
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 0.2372
                // The expected value for 9th upper bound is 1.2746
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_GammaPoisson()
        {
            // In this example, the prior distribution is a Gamma distribution with 
            // input mode alpha/theta
            //
            //  Alpha: 10
            //  Theta:  5

            // In this example, the likelihood function used is a Possison distribution with the following parameters:
            //    event: 0
            //    Time : 100
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Gamma_Distribution,
                rdatsrvConstants.nRedcassrv_AlphaTheta_Mode, 10, 5, rdatsrvConstants.nRedcassrv_Posisson_Distribution,
                rdatsrvConstants.nRedcassrv_EventTime_Mode, 0, 100);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 0.0952
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 9.07e-4
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 0.0517
                // The expected value for 9th upper bound is 0.1496
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_NormalNormal()
        {
            // In this example, the prior distribution is a Normal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008

            // In this example, the likelihood distribution is a Normal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Normal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Normal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 4.1e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.811e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 1.342e-4
                // The expected value for 9th upper bound is 6.858e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_LogNormalLogNormal()
        {
            // In this example, the prior distribution is a LogNormal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008

            // In this example, the likelihood distribution is a LogNormal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 1.732e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.626e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 3.433e-5
                // The expected value for 9th upper bound is 4.661e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_LogNormalBinomial()
        {
            // In this example, the prior distribution is a LogNormal distribution with 
            // the following parameters:
            //   5th:   0.00002
            //  95th:   0.0008

            // In this example, the likelihood function used is a Binomial distribution with the following parameters:
            //    event: 1
            //    Demands : 5000
            // This indicates that there is no failure found in the whole testing period.
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Binomial_Distribution,
                rdatsrvConstants.nRedcassrv_EventDemands_Mode, 1, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 1.849E-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.094e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 3.51e-5
                // The expected value for 9th upper bound is 4.677e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_LogNormalPossion()
        {
            // In this example, the prior distribution is a LogNormal distribution with 
            // the following parameters:
            //   5th:   0.00002
            //  95th:   0.0008

            // In this example, the likelihood function used is a Poisson distribution with the following parameters:
            //    event: 0
            //    Time : 5000
            // This indicates that there is no failure found in the whole testing period.
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Posisson_Distribution, 
                rdatsrvConstants.nRedcassrv_EventTime_Mode, 0, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 9.878E-5
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 8.51E-9
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 1.437e-5
                // The expected value for 9th upper bound is 2.777e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                // To draw the curve for prior distribution, please use function getPDF and getCDF to 
                // retrieve the value at given x value
                // The following is an example to calculate the CDF when x=0.00002
                // The expected value is 0.09375
                // Please refer to testDistributionService code to get instruction about
                // how to draw CDF/PDF Curve.
                double dValue = rdatHomoAnalysisService.getCDF(iAnalysisHandle, 0.00002);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
           }
       }

        void testHomo_TLogNormalLognormal()
        {
            // In this example, the prior distribution is a Tr-LogNormal distribution with 
            // input mode Median/Tau
            //
            //  Median: 0.005
            //  Tau: 0.03

            // In this example, the likelihood distribution is a LogNormal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.00002
            //  95th: 0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_TLogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_MedianTau_Mode, 0.005, 0.03, rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 4.989e-3
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.24e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 4.747e-3
                // The expected value for 9th upper bound is 5.239e-3
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_TLogNormalBinomial()
        {
            // In this example, the prior distribution is a Tr-LogNormal distribution with 
            // input mode Median/Tau
            //
            //  Median: 0.005
            //  Tau: 0.03

            // In this example, the likelihood distribution is a Binomial distribution with 
            // input mode Event/Demands
            //
            //  Event: 5
            //  Demands: 5000
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_TLogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_MedianTau_Mode, 0.005, 0.03, rdatsrvConstants.nRedcassrv_Binomial_Distribution,
                rdatsrvConstants.nRedcassrv_EventDemands_Mode, 5, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 4.914e-3
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.127e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 4.678e-3
                // The expected value for 9th upper bound is 5.158e-3
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }
        
        void testHomo_UniformNormal()
        {
            // In this example, the prior distribution is a Uniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood distribution is a Normal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Uniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Normal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 4.1e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 3.502e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 9.926e-5
                // The expected value for 9th upper bound is 7.207e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_UniformLogNormal()
        {
            // In this example, the prior distribution is a Uniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood distribution is a LogNormal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Uniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 3.29e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 4.193e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 6.055e-5
                // The expected value for 9th upper bound is 7.169e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_UniformBinomial()
        {
            // In this example, the prior distribution is a Uniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood distribution is a Binomial distribution with 
            // input mode Event/Demand
            //
            //  Event: 1
            //  Demands: 5000
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Uniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Binomial_Distribution,
                rdatsrvConstants.nRedcassrv_EventDemands_Mode, 1, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 3.371e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 3.677e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 7.096e-5
                // The expected value for 9th upper bound is 6.983e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_UniformPossion()
        {
            // In this example, the prior distribution is a Uniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood function used is a Poisson distribution with the following parameters:
            //    event: 0
            //    Time : 5000
            // This indicates that there is no failure found in the whole testing period.
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_Uniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Posisson_Distribution,
                rdatsrvConstants.nRedcassrv_EventTime_Mode, 0, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 2.038e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.717E-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 3.005e-5
                // The expected value for 9th upper bound is 5.541e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_LogUniformLogNormal()
        {
            // In this example, the prior distribution is a LogUniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood distribution is a LogNormal distribution with 
            // input mode 5th/95th
            //
            //  5th: 0.0002
            //  95th: 0.0008
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_LogUniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_LogNormal_Distribution,
                rdatsrvConstants.nRedcassrv_5th95th_Mode, 0.00002, 0.0008);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 1.837e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.67e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 2.91e-5
                // The expected value for 9th upper bound is 5.499e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_LogUniformBinomial()
        {
            // In this example, the prior distribution is a LogUniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood distribution is a Binomial distribution with 
            // input mode Event/Demand
            //
            //  Event: 1
            //  Demands: 5000
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_LogUniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Binomial_Distribution,
                rdatsrvConstants.nRedcassrv_EventDemands_Mode, 1, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 2.039e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 2.716e-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 3.005e-5
                // The expected value for 9th upper bound is 5.54e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }

        void testHomo_LogUniformPossion()
        {
            // In this example, the prior distribution is a LogUniform distribution with 
            // the following parameters:
            //  Min:   0.00002
            //  Max:   0.0008

            // In this example, the likelihood function used is a Poisson distribution with the following parameters:
            //    event: 0
            //    Time : 5000
            // This indicates that there is no failure found in the whole testing period.
            int iAnalysisHandle = rdatHomoAnalysisService.create(rdatsrvConstants.nRedcassrv_LogUniform_Distribution,
                rdatsrvConstants.nRedcassrv_MinMax_Mode, 0.00002, 0.0008, rdatsrvConstants.nRedcassrv_Posisson_Distribution,
                rdatsrvConstants.nRedcassrv_EventTime_Mode, 0, 5000);

            if (iAnalysisHandle != -1)
            {
                // Get the mean value for posterior distribution
                // The expected result is 9.747e-4
                double dPosteriorMean = rdatHomoAnalysisService.getMean(iAnalysisHandle);

                // Get the variance for posterior distribution
                // The expected result is 1.037E-8
                double dPosteriorVar = rdatHomoAnalysisService.getVariance(iAnalysisHandle);

                // Retrieve the 5th and 95th upper bound estimation
                // For all other estimation, change the second parameter to the value needed
                // 
                // The expected value for 5th upper bound is 2.213e-5
                // The expected value for 9th upper bound is 3.073e-4
                double d5thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.05);
                double d95thEst = rdatHomoAnalysisService.getQuantile(iAnalysisHandle, 0.95);

                rdatHomoAnalysisService.destory(iAnalysisHandle);
            }
        }
    }
}
