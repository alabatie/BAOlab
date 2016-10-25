/*******************************************************************************
**
**    UNIT
**
**    Version: 3.3
**
**    Author: Jean-Luc Starck
**
**    Date:  96/06/13 
**    
**    File:  IM_IO.h
**
*******************************************************************************
**
**    DESCRIPTION  FITS Include
**    ----------- 
**                 
******************************************************************************/

#ifndef _IM_IO_H_
#define _IM_IO_H_

#include"GlobalInc.h"
#include"Array.h"

 
#define DEFAULT_FORMAT_IMAGE  F_FITS
#define MAXCHAR 256
#define RETURN_OK 0
#define RETURN_ERROR (-1)
#define RETURN_FATAL_ERROR (-2)
#ifdef  NOSMALLHUGE
#define BIG 1e+30   /* a huge number */
#else
#define BIG HUGE_VAL
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE -1
#endif

/*------------------- a few definitions to read FITS parameters ------------*/

#define FBSIZE  2880L   /* size (in bytes) of one FITS block */

#define FITSTOF(k, def) \
                        ((point = fitsnfind(buf, k, n))? \
                                 atof(strncpy(st, &point[10], 70)) \
                                :(def))

#define FITSTOI(k, def) \
                        ((point = fitsnfind(buf, k, n))? \
                                 atoi(strncpy(st, &point[10], 70)) \
                                :(def))
#define FITSTOS(k, str, def) \
                        { point = fitsnfind(buf, k, n); \
                          if (point != NULL) \
                                { \
                                for (i=0,point+=11; (*point)!='\'' && i < 69;) \
                                        (str)[i++] = *(point++); \
                                (str)[i] = '\0'; \
                                } \
                          else\
                                strcpy(str, def); \
                        }
#define QFREAD(ptr, size, file, fname) \
                if (fread(ptr, (size_t)(size), (size_t)1, file)!=1) \
                  error(EXIT_FAILURE, (char*) "*Error* while reading ", (char*) fname)

#define QFWRITE(ptr, size, file, fname) \
                if (fwrite(ptr, (size_t)(size), (size_t)1, file)!=1) \
                  error(EXIT_FAILURE, (char*) "*Error* while writing ", (char*) fname)

#define QFSEEK(file, offset, pos, fname) \
                if (fseek(file, (offset), pos)) \
                  error(EXIT_FAILURE,"*Error*: file positioning failed in ", \
                        fname)
#define QFTELL(pos, file, fname) \
                if ((pos=ftell(file))==-1) \
                  error(EXIT_FAILURE,"*Error*: file position unknown in ", \
                        fname)

/* int     t_size[] = {1, 2, 4, 4, 8}; */
typedef enum {H_INT, H_FLOAT, H_EXPO, H_BOOL, H_STRING, H_COMMENT,
                        H_KEY}  h_type;         /* type of FITS-header data */


extern void swapbytes(void *ptr, int nb, int n);
extern void    error(int num, char *msg1, char *msg2);


enum type_data {T_BYTE, T_SHORT, T_INT, T_FLOAT, T_DOUBLE,
                     T_COMPLEX_F, T_COMPLEX_D, UNKNOWN};

typedef unsigned char byte;
typedef unsigned long u_long;

 /*--------------------------- FITS BitPix coding ----------------------------*/

#define         BP_BYTE         8
#define         BP_SHORT        16
#define         BP_INT          32
#define         BP_FLOAT        (-32)
#define         BP_DOUBLE       (-64)


/*----------------------------- Fits image parameters ----------------------------*/

// typedef struct
class fitsstruct {
 void fitsinit()
 {
    file=NULL;	
    fitsheadsize= 2880;
    bitpix = 0;
    bytepix = 0;
    width = 0;
    height = 0;
    npix = 0;
    bscale = 1.;
    bzero = 0.;
    crpixx = 0.;
    crpixy = 0.;
    crvalx = 0.;
    crvaly = 0.;
    cdeltx= 0.;
    cdelty = 0.;
    ngamma=0.;
    pixscale=0.;
    nlevels = 0;
    pixmin  = 0.;
    pixmax  = 0.;
    epoch = 0.;
    crotax = 0.;
    crotay = 0.;
    fitshead = NULL;
    origin = (char*) "";
    strcpy(ctypex, "");
    strcpy(ctypey, "");
    strcpy(rident, "");

    /* HISTORY & COMMENT fields */
    history = NULL;
    hist_size = 0;
    comment = NULL;
    com_size = 0;

    naxis=0;
    for (int i=0; i < MAX_NBR_AXIS;i++)
    {
       TabAxis[i]=0;
       TabStep[i]=0.;
       TabRef[i]=0.;
       TabValRef[i]=0.;
    }
    filename = NULL;
    origin = NULL;
    fitshead = NULL;
   }
 public:
  
  fitsstruct ()  
  { 
     fitsinit();
  }
  void hd_fltarray(fltarray &Mat, char *History=NULL)
  {
     char *creafitsheader();

     fitshead = creafitsheader();
     bitpix = -32;
     width =  Mat.nx();
     height = Mat.ny();
     naxis = Mat.naxis();
     npix = Mat.n_elem();
     for (int i=0; i < naxis; i++) TabAxis[i] = Mat.axis(i+1);
     if (History != NULL) origin = History;
  }
  ~fitsstruct()  
  { 
      if (filename != NULL) free (filename);
      // if (fitshead != NULL) free ((char *) fitshead);
      if (history != NULL)  free (history);
      if (comment != NULL)  free (comment);
     fitsinit();
  }
  
  char		*filename;		/* pointer to the image filename */
  char          *origin;                /* pointer to the origin */
  char		ident[512];		/* field identifier (read from FITS)*/
  char		rident[512];	        /* field identifier (relative) */
  FILE		*file;			/* pointer the image file structure */
  char		*fitshead;		/* pointer to the FITS header */
  int		fitsheadsize;		/* FITS header size */
/* ---- main image parameters */
  int		bitpix, bytepix;	/* nb of bits and bytes per pixel */
  int		width, height;		/* x,y size of the field */
  int		npix;			/* total number of pixels */
  double	bscale, bzero;		/* FITS scale and offset */
  double	ngamma;			/* normalized photo gamma */
  int		nlevels;		/* nb of quantification levels */
  float		pixmin, pixmax;		/* min and max values in frame */
/* ---- basic astrometric parameters */
  double	epoch;			/* epoch for coordinates */
  double	pixscale;		/* pixel size in arcsec.pix-1 */
					/* */
/* ---- astrometric parameters */
  double	crpixx,crpixy;		/* FITS CRPIXn */
  double	crvalx,crvaly;		/* FITS CRVALn */
  double	cdeltx,cdelty;		/* FITS CDELTn */
  double	crotax,crotay;		/* FITS CROTAn */
  char          ctypex[256];            /* FITS CTYPE1 */
  char          ctypey[256];            /* FITS CTYPE2 */
  char          CoordType[256];         
  
/* ---- HISTORY & COMMENT parameters --- */
	 char *history;
	 int hist_size;
	 char *comment;
	 int com_size;

/* ---- for non image use */
  int naxis;
  int TabAxis[MAX_NBR_AXIS];
  double TabStep[MAX_NBR_AXIS];
  double TabRef[MAX_NBR_AXIS];
  double TabValRef[MAX_NBR_AXIS];
  };

FILE *fits_file_des_in(char *fname);
FILE *fits_file_des_out(char *fname);
Bool std_inout(char *Filename);

void fits_read_header(char *File_Name, fitsstruct *Header);
void fits_read_fltarr(char *File_Name, fltarray &Mat);
void fits_read_fltarr(char *File_Name, fltarray &Mat, fitsstruct * FitsHeader);
void fits_read_fltarr(char *File_Name, fltarray &Mat, fitsstruct * FitsHeader,
                      int openflag);
void fits_write_header(char *File_Name, fitsstruct *Header);

void fits_write_fltarr(char *File_Name, fltarray &Mat);
void fits_write_fltarr(char *File_Name, fltarray &Mat, fitsstruct *FitsHeader);

void makehistory(char *mystring, char *myproc, char *myalgo, char *myargs);
int fitsaddhist_com(fitsstruct *pfitsbuf, char *comment, char *type_com);
int fitsread(char *fitsbuf, char *keyword, void *ptr,
             h_type type, type_data t_type);
char *readfitshead(FILE *file, char *filename, int *nblock);
char *creafitsheader();
void initfield(fitsstruct *Header); /* initialize the structure FITSSTRUCT */
void init_fits_struct(fitsstruct *Ptr, int Nl, int Nc);

void io_write_ima_float(char *File_Name, Ifloat &Mat);
void io_write_ima_float(char *File_Name, Ifloat &Mat, fitsstruct *FitsHeader);
void io_read_ima_float(char *File_Name, fltarray & Data);
void io_read_ima_float(char *File_Name, Ifloat & Data, fitsstruct *FitsHeader);

//  Gif format
# define PARM(a) a
# define PIC8  0
# define PIC24 1
# define F_FULLCOLOR 0




/* info structure filled in by the LoadXXX() image reading routines */
typedef struct { byte *pic;                  /* image data */
	         int   w, h;                 /* pic size */
		 int   type;                 /* PIC8 or PIC24 */

		 byte  r[256],g[256],b[256];
		                             /* colormap, if PIC8 */

		 int   normw, normh;         /* 'normal size' of image file
					        (normally eq. w,h, except when
						doing 'quick' load for icons */

		 int   frmType;              /* def. Format type to save in */
		 int   colType;              /* def. Color type to save in  */
                    /* also called colorType value F_FULLCOLOR, F_GREYSCALE, */
		 char  fullInfo[128];        /* Format: field in info box */
		 char  shrtInfo[128];        /* short format info */
		 char *comment;              /* comment text */

		 int   numpages;             /* # of page files, if >1 */
		 char  pagebname[64];        /* basename of page files */
	       } PICINFO;


#define xvbzero(s,size) memset(s,0,size)

// Gif format
inline byte float_to_byte(float V)
{
   byte Vb;
   if (V > 255) Vb = 255;
   else if (V < 0) Vb = 0;
   else Vb = (byte) V; 
   return Vb;
}
inline byte int_to_byte(int V)
{
   byte Vb;
   if (V > 255) Vb = 255;
   else if (V < 0) Vb = 0;
   else Vb = (byte) V; 
   return Vb;
} 


#define F_FULLCOLOR 0
#define F_BWDITHER  2
#define F_GREYSCALE 1
#define MONO(rd,gn,bl) ( ((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5)

int io_read3d_tiff(char *name, fltarray & Data);
int io_write3d_tiff(  char *name, fltarray & Data);

typedef unsigned short u_short;
typedef unsigned char  u_char;
typedef unsigned int   u_int;

#endif
