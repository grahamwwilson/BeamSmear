//
// ROOT macro for producing Guinea-PIG input beam files
// with variables subject to uncorrelated Gaussian smearing.
//
// The output file is unsorted in z
// A subsequent shell based sort will be used to do the necessary sorting in z
//
// The Guinea-PIG input beam files are used for the macroparticles
// in the GP simulations. There must be at least as many beam particles as 
// specified macroparticles.
//
// The files consist of these 6 variables per beam:
// 1. Ebeam (GeV)
// 2. x (microns)
// 3. y (microns)
// 4. z (microns)
// 5. x' (micro-radians)
// 6. y' (micro-radians)
//
// So far the assumption in the files in common use seems to be that there are 
// no correlations amongst the variables. This assumption should be 
// double-checked as older studies indicated that there should at least be 
// some E-z correlations, and this is a parameter that can change the 
// center-of-mass energy spread.
//
// This implementation is not time-critical. The relatively slower Ranlux 
// random number generator is used.
//
//                           Graham W. Wilson,   04-NOV-2021
//

void beamsmearZ(string bfile="electronZ.ini",unsigned int seed=13,
               double Espread=0.00300, int N=80000, double Emean=45.6, 
               double truncate=4.0,
               double sigmaZ=410.0, double betaX=18.0, 
               double betaY=0.39, double emittX=6.2, double emittY=0.0485){

// Open output file
   ofstream beamfile;
   beamfile.open(bfile);
   
// Initialize 6 independent random number generators for each variable
   TRandom *r1 = new TRandom1(seed);
   TRandom *r2 = new TRandom1(seed+1);
   TRandom *r3 = new TRandom1(seed+2);
   TRandom *r4 = new TRandom1(seed+3);
   TRandom *r5 = new TRandom1(seed+4);
   TRandom *r6 = new TRandom1(seed+5);
   
   bool update=true;
   
   double x1,x2,x3,x4,x5,x6;
   const double m=0.5109989461e-3;
   double gamma = Emean/m; // Question - should this be calculated per beam 
                           // (as done here) or per particle after smearing?
   double sigmaE  = Espread*Emean;
// Use the transverse beam emittance and beta functions to calculate 
// transverse beam-size and divergence assuming no correlations 
//
// Input normalized emittances and beta parameters are in the same units as Guinea-PIG input, 
// namely emittance (10^-6 m. rad) and beta (mm)
//  
// Convert to SI units of m.rad and m
   double bX = betaX*1.0e-3;
   double bY = betaY*1.0e-3;
   double eX = emittX*1.0e-6;
   double eY = emittY*1.0e-6;
// Conversion factor from meters to microns and and radians to micro-radians
   const double CONV=1.0e6;
   double sigmaX  = CONV*sqrt(eX*bX/gamma);
   double sigmaY  = CONV*sqrt(eY*bY/gamma);
   double sigmaXP = CONV*sqrt(eX/(gamma*bX));
   double sigmaYP = CONV*sqrt(eY/(gamma*bY));
   
// Check parameters
   cout << "bfile          " << bfile << endl;
   cout << "N              " << N << endl;
   cout << "seed           " << seed << endl;
   cout << "Ebeam  (GeV) = " << Emean << endl;
   cout << "gamma        = " << gamma << endl;
   cout << "sE/E         = " << Espread << endl;
   cout << "sigmaE (GeV) = " << sigmaE << endl;
   cout << "sigmaX  (um) = " << sigmaX << endl;
   cout << "sigmaY  (um) = " << sigmaY << endl;
   cout << "sigmaZ  (um) = " << sigmaZ << endl;
   cout << "sigmaXP (um) = " << sigmaXP << endl;
   cout << "sigmaYP (um) = " << sigmaYP << endl;
   cout << "truncate at +- " << truncate << " sigma" << endl;
   cout << "update flag    " << update << endl; 
   
// Simulate N particles for this beam   
   for (int i=0;i<N;i++){
   
// In addition to smearing the parameters we also truncate the 
// gaussian to the specified smaller range of eg +- 4 sigma.
// To effectively turn off set truncate to a large value like 10.
       int inrange=0;
       while (inrange < 6){
          inrange = 0;
          x1 = r1->Gaus(Emean,sigmaE);
// recalculate beam-size and divergence for single particle using actual gamma
          if(update){
              gamma = x1/m;
              sigmaX  = CONV*sqrt(eX*bX/gamma);
              sigmaY  = CONV*sqrt(eY*bY/gamma);
              sigmaXP = CONV*sqrt(eX/(gamma*bX));
              sigmaYP = CONV*sqrt(eY/(gamma*bY));
          }          
          x2 = r2->Gaus(0.0,sigmaX);
          x3 = r3->Gaus(0.0,sigmaY);
          x4 = r4->Gaus(0.0,sigmaZ);     // This should also be a function of the relative deviation from nominal?
          x5 = r5->Gaus(0.0,sigmaXP);
          x6 = r6->Gaus(0.0,sigmaYP);
// Check that all 6 variables are within the specified tolerance 
// in standard deviations          
          if(abs((x1-Emean)/sigmaE)<truncate)inrange++;
          if(abs(x2/sigmaX)<truncate)inrange++;
          if(abs(x3/sigmaY)<truncate)inrange++;
          if(abs(x4/sigmaZ)<truncate)inrange++;
          if(abs(x5/sigmaXP)<truncate)inrange++;
          if(abs(x6/sigmaYP)<truncate)inrange++;
//          if(inrange < 6)cout << "Some elements out of specified range " 
//                              << inrange << endl;                          
       }
       
// Choose same formatting as files used by ILD       
       beamfile << setprecision(7) << std::scientific
                << setw(16) << x1
                << setw(16) << x2 
                << setw(16) << x3 
                << setw(16) << x4 
                << setw(16) << x5 
                << setw(16) << x6 << endl;
   }
   
   beamfile.close();

}
