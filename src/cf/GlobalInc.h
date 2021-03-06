
#ifndef _IM_GLOB_H_
#define _IM_GLOB_H_

#include<cmath>
#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<sstream>

#include<complex>
using namespace std;
typedef complex<float> complex_f;
typedef complex<double> complex_d;

// #include<climits>

#ifndef WINDOWS
#ifndef OSF1
#ifndef HP
#ifndef MACOS
#include <limits.h>
#endif
#endif
#endif
#endif

extern "C"
{
//#include "fitsio2.h"
#undef True
#undef False
}

#define DEFBOOL 1
#ifdef DEFBOOL
#undef False
#undef True
enum Bool {False = 0,True = 1};
#else
#undef False
#undef True
#define True 1
#define False 0
#endif

// output for help  
#define OUTMAN stdout
#define WRITE_PARAM 0
#define MAX_NL 35000
#define MAX_NC 35000

inline void manline()
{
   fprintf(OUTMAN, "\n");
}

#if VMS
inline char *strdup(char *s1)
{
   int T = strlen(s1);
   char *Ret = new char[T];
   strcpy(Ret, s1);
   return(Ret);
}
#endif

#include "SoftInfo.h"
#include "OptMedian.h"
#include "DefMath.h"
#include "Memory.h"
#include "Array.h"
#include "Licence.h"
#include "Usage.h"

int GetOpt(int argc, char **argv, char *opts);

#endif
