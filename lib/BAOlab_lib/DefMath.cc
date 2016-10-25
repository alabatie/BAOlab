/******************************************************************************
**                   Copyright (C) 1998 by CEA
*******************************************************************************
**
**    UNIT
**
**    Version: 1.0
**
**    Author: Jean-Luc Starck
**
**    Date:  22/12/98
**    
**    File:  DefMath.cc
**
*******************************************************************************
**
** double b3_spline (double x)
** 
** Computes the value on a b3-spline 
** we have b3_spline (0) = 2 / 3 
** and if |x| >= 2 => b3_spline (x) = 0
** 
*******************************************************************************
**
**  double xerf (double X)
**
**  compute the inverse function of the repartition function of a Gaussian
**  law. Ref ABRAMOVITZ et STEGUN p. 933
**
*******************************************************************************
**
**  init_random (unsigned int Val)
** 
**  random values generator initialisation from secondes with the
**  unix function drand48
**
*******************************************************************************
**
** float get_random (float Min, float Max)
** 
** return a random value between min and max
**
*****************************************************************************/
 
#include "DefMath.h"
// #include "OptMedian.h"

// #ifndef VMS
// extern "C" {
// extern void srand48(long);
// extern double drand48 ();
// }
// #endif

/***************************************************************************/

double b3_spline (double x)
{
    double A1,A2,A3,A4,A5,Val;

    A1 = ABS ((x - 2) * (x - 2) * (x - 2));
    A2 = ABS ((x - 1) * (x - 1) * (x - 1));
    A3 = ABS (x * x * x);
    A4 = ABS ((x + 1) * (x + 1) * (x + 1));
    A5 = ABS ((x + 2) * (x + 2) * (x + 2));
    Val = 1./12. * (A1 - 4. * A2 + 6. * A3 - 4. * A4 + A5);
    return (Val);
}
    
/***************************************************************************/

double xerf (double X)
/* 
   compute the inverse function of the repartition function of a Gaussian
   law. Ref ABRAMOVITZ et STEGUN p. 933
*/
{
    double Z,A0,A1,B1,B2,Val_Return;
    
    Z = X;
    if (X > 0.5) Z = 1 - X;
    if (Z < FLOAT_EPSILON)
    {
       Val_Return  = 0.;
    }
    else
    {
       Z = sqrt (-2. * log (Z));
       A0 = 2.30753;
       A1 = 0.27061;
       B1 = 0.99229;
       B2 = 0.04481;
       Val_Return = Z - (A0 + A1 * Z) / (1 + ((B1 + B2 * Z) * Z));
       if (X > 0.5) Val_Return = - Val_Return;
    }
    return (Val_Return);
}

/***************************************************************************/

double inverfc(double y)
{
    double s, t, u, w, x, z;

    z = y;
    if (y > 1) {
        z = 2 - y;
    }
    w = 0.916461398268964 - log(z);
    u = sqrt(w);
    s = (log(u) + 0.488826640273108) / w;
    t = 1 / (u + 0.231729200323405);
    x = u * (1 - s * (s * 0.124610454613712 + 0.5)) - 
        ((((-0.0728846765585675 * t + 0.269999308670029) * t + 
        0.150689047360223) * t + 0.116065025341614) * t + 
        0.499999303439796) * t;
    t = 3.97886080735226 / (x + 3.97886080735226);
    u = t - 0.5;
    s = (((((((((0.00112648096188977922 * u + 
        1.05739299623423047e-4) * u - 0.00351287146129100025) * u - 
        7.71708358954120939e-4) * u + 0.00685649426074558612) * u + 
        0.00339721910367775861) * u - 0.011274916933250487) * u - 
        0.0118598117047771104) * u + 0.0142961988697898018) * u + 
        0.0346494207789099922) * u + 0.00220995927012179067;
    s = ((((((((((((s * u - 0.0743424357241784861) * u - 
        0.105872177941595488) * u + 0.0147297938331485121) * u + 
        0.316847638520135944) * u + 0.713657635868730364) * u + 
        1.05375024970847138) * u + 1.21448730779995237) * u + 
        1.16374581931560831) * u + 0.956464974744799006) * u + 
        0.686265948274097816) * u + 0.434397492331430115) * u + 
        0.244044510593190935) * t - 
        z * exp(x * x - 0.120782237635245222);
    x += s * (x * s + 1);
    if (y > 1) {
        x = -x;
    }
    return x;
}

/***************************************************************************/

// double xerf(double y) {return 1. - xerfc(y);}
double xerfc (double F)
{
   double Nu;
   double P = F;
   if (P > 1.) P = 1.;
   else if (P < 0) P = 0.;
   if (P > 0.5) Nu = sqrt(2.)*inverfc((double) (1.-P)/0.5);
   else Nu = - sqrt(2.)*xerfc((double) P/0.5);
   return Nu;
}      

/***************************************************************************/

void init_random (unsigned int Init)
/* 
  random values generator initialisation from secondes with the
  unix function drand48
*/
{    
#ifdef RDNT
     FILE *Fp = popen("date | tr \":\" \"\\12\" | tail -1 | tr \" \" \"\\12\" | head -1","r");
     fscanf (Fp,"%d", &Init);
#endif
     srand(Init);    
}

/***************************************************************************/

float get_random()
{
   return rand()/(RAND_MAX+1.0);
}

/***************************************************************************/

float get_random (float Min, float Max)
{
    float Val;
          
    Val = (float)( get_random() * (Max-Min) + Min);
     // Val = drand48 () * (Max+Min) - Min;
    return(Val);
}

/***************************************************************************/

double entropy (float *Pict, int Size, float StepHisto)
{
    int i,Nbr_Val,ind;
    int *Tab_Histo=NULL;
    double Prob_Ev;
    float Min,Max;
    double Entr;

    Min = Max = Pict [0];
    for (i = 1; i < Size; i++)
    {
        if      (Pict [i] > Max) Max = Pict [i];
        else if (Pict [i] < Min) Min = Pict [i];
    }

    /* Calcul de l'entropie */
    Nbr_Val = (int) ( (Max - Min + 1) / StepHisto);
    Tab_Histo = new int [Nbr_Val];
    
    for (i = 0; i < Nbr_Val; i++) Tab_Histo [i] = 0;
    for (i = 0; i < Size; i++)
    {
        ind =  (int)(((Pict[i] - Min) / StepHisto));
	if ((ind < 0) || (ind >= Nbr_Val))
	{
	    cout << "Error in entropy  function ... " << endl;
	    cout << "Nbr_Val = " << Nbr_Val << " Ind = " << ind << endl;
	    exit(-1);
	}
        Tab_Histo[ind] ++;
    }
    Entr = 0.;
    for (i = 0; i < Nbr_Val; i++)
    {
        if (Tab_Histo [i] > 0)
        {
            Prob_Ev = (double) Tab_Histo [i] / (double) Size;
            Entr += - Prob_Ev * log (Prob_Ev) / log(2.);
        }
    }
    if (Tab_Histo != NULL) delete [] Tab_Histo;
    return (Entr);
}

/***************************************************************************/

// float get_sigma_mad(float *Image, int N)
// {
//     float Noise, Med;
//     int i;
//     fltarray Buff(N);
// 
//     for (i = 0; i < N; i++) Buff(i) = Image[i];
//     Med = get_median(Buff.buffer(), N);
// 
//     for (i = 0; i < N; i++) Buff(i) = ABS(Image[i] - Med);
//     Noise = get_median(Buff.buffer(), N);
//     return (Noise/0.6745);
// }

/***************************************************************************/

float get_sigma_clip (float *Data, int N, int Nit, Bool Average_Non_Null,
            Bool UseBadPixel, float BadPVal)
{
    int It, i;
    double S0,S1,S2,Sm=0,x;
    double Average=0., Sigma=0.;

    for (It = 0; It < Nit; It++)
    {
       S0 = S1 = S2 = 0.;
       for (i = 0; i < N; i++)
        {
           x = Data[i];
           if ((UseBadPixel == False) || (ABS(x-BadPVal) > FLOAT_EPSILON))
           {
	      if ((It == 0) || (ABS(x - Average) < Sm))
	      { 
	         S0 += 1.;
	         S1 += x;
	         S2 += x*x;
	      }
           }
       }
       if (S0 == 0) S0=1;
       if (Average_Non_Null==True)
       {
       	   Average = S1 / S0;
       	   Sigma = S2/S0- Average * Average;
	   // printf("Sigma = %f\n", (float) Sigma);
       	   if (Sigma > 0) Sigma = sqrt(S2/S0- Average * Average);
	   else Sigma = 0.;
       }
       else  Sigma = sqrt(S2/S0);
       Sm = 3. * Sigma;       
    }
    return ((float) Sigma);
}

/***************************************************************************/

float get_sigma_clip_robust (float *Data, int N, int Nit, Bool Average_Non_Null,
            Bool UseBadPixel, float BadPVal)
{
    int It, i;
    double S0,S1,S2,Sm=0,x;
    double Average=0., Sigma=0.;

    for (It = 0; It < Nit; It++)
    {
       S0 = 0.;
       S1 = 0.;
       S2 = 0.;
       for (i = 0; i < N; i++)
       {
           x = Data[i];
           if ((UseBadPixel == False) || (ABS(x-BadPVal) > FLOAT_EPSILON))
           {
	      if ((It == 0) || (ABS(x - Average) < Sm))
	      { 
	         S0 += 1.;
	         S1 += x;
 	      }
           }
       }
       if (S0 == 0) S0=1;
       S1 /= S0;
       for (i = 0; i < N; i++)
       {
           x = Data[i] - S1;
           if ((UseBadPixel == False) || (ABS(x-BadPVal) > FLOAT_EPSILON))
 	      if ((It == 0) || (ABS(x - Average) < Sm)) S2 += x*x;
       }
       Average = S1;
       S2 /= S0;
       // printf("A = %f, S2 = %f\n", Average, S2);
       if (S2 > 0) S2 = sqrt(S2);
       else S2 = 0.;
       Sigma = S2;
       // printf("Sigma(%f) = %f\n", (float) S0, (float) Sigma);
       Sm = 3. * Sigma;       
    }
    return ((float) Sigma);
}

/***************************************************************************/

double skewness(float *Dat, int N)
{
   double Skew;
   double x1,x2,x3,Sigma;
   int i;
   x1=x2=x3=0.;
   for (i=0; i < N; i++)
   {
      x1 += Dat[i];
      x2 +=  pow((double) Dat[i], (double) 2.);
      x3 +=  pow((double) Dat[i], (double) 3.);
   }
   x1 /= (double) N;
   x2 /= (double) N;
   x3 /= (double) N;
   Sigma = x2 - x1*x1;
   if (Sigma > 0.)
   {
      Sigma = sqrt(Sigma);
      Skew = 1. / pow(Sigma,(double) 3.) * (x3 - 3.*x1*x2+2.*x1*x1*x1);
   }
   else Skew = 0.;
   return Skew;
}

/****************************************************************************/

double curtosis(float *Dat, int N)
{
   double Curt;
   double x1,x2,x3,x4,Sigma;
   int i;
   x1=x2=x3=x4=0.;
   for (i=0; i < N; i++)
   {
      x1 += Dat[i];
      x2 +=  pow((double) Dat[i], (double) 2.);
      x3 +=  pow((double) Dat[i], (double) 3.);
      x4 +=  pow((double) Dat[i], (double) 4.);
   }
   x1 /= (double) N;
   x2 /= (double) N;
   x3 /= (double) N;
   x4 /= (double) N;
   Sigma = x2 - x1*x1;
   if (Sigma > 0.)
   { 
      double x1_2 = x1*x1;
      double x1_4 = x1_2*x1_2;
      Sigma = sqrt(Sigma);
      Curt = 1. / pow(Sigma,(double) 4.) * (x4 -4*x1*x3 + 6.*x2*x1_2 -3.*x1_4 ) - 3.;
   }
   else Curt = 0.;
   return Curt; 
}

/****************************************************************************/

void moment4(float *Dat, int N, double &Mean, double &Sigma, 
             double &Skew, double & Curt, float & Min, float & Max)
{
   double x1,x2,x3,x4;
   int i;
   x1=x2=x3=x4=0.;
   Curt=Skew=0;
   Min = Max = Dat[0];
   
   for (i=0; i < N; i++)
   {
      x1 += Dat[i];
      if (Min > Dat[i]) Min = Dat[i];
      if (Max < Dat[i]) Max = Dat[i];
   }
   x1 /= (double) N;
   
   for (i=0; i < N; i++)
   {
      // double Coef=Dat[i];
      x2 +=  pow((double) (Dat[i]-x1), (double) 2.);
      x3 +=  pow((double) (Dat[i]-x1), (double) 3.);
      x4 +=  pow((double) (Dat[i]-x1), (double) 4.);
   }
   
   x2 /= (double) N;
   x3 /= (double) N;
   x4 /= (double) N;
   Sigma = x2;
   if (Sigma > 0.)
   { 
      // double x1_2 = x1*x1;
      // double x1_4 = x1_2*x1_2;
      Sigma = sqrt(Sigma);
      Skew = x3 / pow(Sigma,(double) 3.) ;
      Curt = x4 / pow(Sigma,(double) 4.) - 3.;
   }
   else Sigma = 0.;
   Mean = x1;
}

 
