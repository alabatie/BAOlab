/******************************************************************************
**                   Copyright (C) 1995 by CEA
*******************************************************************************
**
**    UNIT
**
**    Version: 3.3
**
**    Author: Jean-Luc Starck
**
**    Date:  96/05/07 
**    
**    File:  IM_IOTools.cc
**
**    Modification History:
**    R Gastaud 16 March 1998 v3.3 re write fitsaddhist_com and fitswritehist
**              18 March 1998  v3.4 initialize to zero : change malloc in calloc
**
*******************************************************************************
**
**    DESCRIPTION  
**    ----------- 
**                 
**    PARAMETRES    
**    ----------    
** 
**    RESULTS      
**    -------  
**
**
*******************************************************************************
**
** void	swapbytes(void *ptr, int nb, int n)
**
** Swap bytes for doubles, longs and shorts (for DEC machines or PC for inst.).
**
*******************************************************************************
**
** int fitsreadhist(char *fitshead, int headersize, char *keyword, char **add_ptr)
** 
** PURPOSE: read recursively the value of the input keyword 'keyword' 
**          in a fits header, hold in the string 'fitshead' 
**          and write it in the string referred by 'add_ptr'. 
**          it is useful for HISTORY & COMMENT field for instance.
**
** INPUTS: fitshead --- STRING containing the FITS header
**         keyword  --- STRING containing the name of the keyword
**                      (HISTORY or COMMENT for example)
** OUTPUT: **add_ptr --- POINTEUR on the address of the resulting string
** RETURN: INT, the number of bytes used for memory allocation of the 
**              string referred by add_ptr.
**
*******************************************************************************
**
** int fitsfind(char *fitsbuf, char *keyword)
** Search for a FITS keyword in a FITS header. 
**
*******************************************************************************
**
** char *fitsnfind(char *fitsbuf, char *str, int nblock)
** search for a FITS keyword in a fits header of nblock blocks. 
**
*******************************************************************************
**
** int fitsread(char *fitsbuf, char *keyword, void *ptr, 
**              h_type type,type_data t_type)
**
** read a FITS keyword in a fits header. 
**
*******************************************************************************
**
** int fitsaddhist_com(fitsstruct *pfitsbuf, char *comment, char *type_com)
**  
** PURPOSE: Add a comment in the field 'history' or 'comment' 
**         of the structure FITSSTRUCT. 
** 
** INPUT:  comment --- STRING, value of HISTORY or COMMENT field;
**         type_com --- STRING, "HISTORY" or "COMMENT" to specify in 
**                      which field the comment must be appended.
** OUTPUT: pfitsbuf --- FITSSTRUCT *, address of the structure 
**                      FITSSTRUCT holding the FITS Header;
** RETURN: INT, the length of the added comment.
**
*******************************************************************************
**
** int fitsadd(char **add_fitsbuf, char *keyword, char *comment, 
**              int *pfitsheadsize)
**
** PURPOSE: If the keyword 'keyword' doesn't exist, a fits line 
**         containing this keyword is created & added to
**         the STRING referred by 'add_fitsbuf'. The header size 
**         referred by pfitsheadsize
** 
** INPUTS: keyword --- STRING, the name of the keyword to update or to add
**         comment --- STRING, value to put in the field 'keyword'
** 
** OUTPUTS: add_fitsbuf   --- STRING POINTER, the address of the fits
**                            header string to update
**         pfitsheadsize --- POINTER, address of the header size
** RETURN: INT, the line number in the fits header where the keyword 
**         has been added.
** 
** CAUTION: doesn't work for creating multiple fits lines field 
**         like HISTORY or COMMENT fields.
**
*******************************************************************************
**
** int fitswrite(char *fitsbuf, char *keyword, void *ptr, h_type type, 
**               type_data t_type, int *pfitsheadsize)
**
** PURPOSE: Update the value (enclosed in 'ptr') of the keyword 'keyword' 
**          in the STRING referred by 'fitsbuf'.
**          If the keyword 'keyword' doesn't exist, a fits line containing this 
**          keyword is created & added  the STRING referred by 'fitsbuf'. 
**          The header size referred by pfitsheadsize
** 
** INPUTS: keyword --- STRING, the name of the keyword to update or to add
**         ptr     --- POINTER, address of the value to put in the 
**                              field 'keyword'
**         type    --- H_TYPE, type of the value referred by 'ptr'
**         t_type  --- TYPE_DATA, secondary type of the value referred by 'ptr'
** OUTPUTS: fitsbuf --- STRING, the fits header to update
**         pfitsheadsize --- POINTER, address of the header size
** RETURN: INT, RETURN_OK if no problems, RETURN_ERROR if an error occures.
**
** CAUTION: doesn't work for updating or creating multiple fits lines
**          field like HISTORY or COMMENT fields.
**
*******************************************************************************
**
** int fitswritehist(fitsstruct *pfitsbuf)
**               
** PURPOSE: add in the field 'fitshead' the value contained in the 
**          fields 'history' & 'comment'.
**          The header size referred by 'pfitsbuf->fitsheadsize' is updated.
**
** INPUTS: pfitsbuf --- POINTER to FITSSTRUCT, the fits header to update
** RETURN: INT, the size of 'fitshead'.
** 
*******************************************************************************
**
**  char  *readfitshead(FILE *file, char *filename, int *nblock)
**               
** read data from the FITS-file header
** 
*******************************************************************************
**
**  void readimagehead(fitsstruct *field)
**               
** extract some data from the FITS-file header 
** 
*******************************************************************************
** 
**  void readdataf(FILE *file, char *filename, int bitpix, int Nelem, 
**                  float *ptr, float bscale, float bzero)
**
**  read and convert input data stream in float. 
**
*******************************************************************************
**
** void  readdataf(fitsstruct *field, float *ptr)
**  read and convert input data stream in float. 
**
*******************************************************************************
**
** void  writedataf(FILE *file, char *filename, int bitpix, int Nelem, 
**                  float *ptr, float bscale, float bzero)
** read and convert input data stream in float. 
**
*******************************************************************************
**
** void  writedataf(fitsstruct *field, float *ptr)
**
** rite float data to a file. 
** 
*******************************************************************************
**
** void  writedataf(fitsstruct *field, float *ptr)
** 
** write float data to a file.
**
*******************************************************************************
**
** char *creafitsheader()
** create a fits header
**
*******************************************************************************
**
** void initfield(fitsstruct *field)
**
** PURPOSE: initialize to "0" the fields of a FITSSTRUCT structure
**
** INPUT: *field --- FITSSTRUCT, fits header structure.
**
*******************************************************************************
**
** void updatefitsheader(fitsstruct *field)
**
** PURPOSE: Update the fields 'fitshead' and 'fitsheadsize' of a 
**          FITSSTRUCT structure from the values 
**          of other fields of the FITSSTRUCT structure.
**
** INPUT: *field --- FITSSTRUCT, a fits header structure
**
*******************************************************************************
**
** void writeimagehead(fitsstruct *Header)
**
** PURPOSE: Write down on the disk the FITS header enclosed in the field
**          fitshead' of a FITSSTRUCT structure. 
**          Before writin gon the disk, one must update the header
**           from the fields of the FITSSTRUCT structure.
**
** INPUT: *Header --- FITSSTRUCT, fits header structure
**
*******************************************************************************
**
** void makehistory(char *mystring, char *myproc, char *myalgo, char *myargs)
**
** PURPOSE:  add in mystring:  the date, the machine, the user name
**                     procedure = myproc
**                     algorithm = myalgo
**                     myargs = myargs
**
******************************************************************************/

#include"GlobalInc.h"
#include"IM_IO.h"
#include<ctype.h>
#include<time.h>

int DEBUH = 0; 
// softinfo Soft;
type_data TypeInputData = UNKNOWN;

#include "fitsio2.h"

/*--------------------------------------------------------------------------*/
void ffswap2(short *svalues,  /* IO - pointer to shorts to be swapped       */
             long nvals)      /* I  - number of shorts to be swapped        */
/*
  swap the bytes in the input short integers: ( 0 1 -> 1 0 )
*/
{
    register char *cvalues;
    register long ii;

    union u_tag {
        char cvals[2];   /* equivalence an array of 4 bytes with */
        short sval;      /* a short */
    } u;

    cvalues = (char *) svalues;      /* copy the initial pointer value */

    for (ii = 0; ii < nvals;)
    {
        u.sval = svalues[ii++];  /* copy next short to temporary buffer */

        *cvalues++ = u.cvals[1]; /* copy the 2 bytes to output in turn */
        *cvalues++ = u.cvals[0];
    }
    return;
}
/*--------------------------------------------------------------------------*/
void ffswap4(INT32BIT *ivalues,  /* IO - pointer to floats to be swapped    */
                 long nvals)     /* I  - number of floats to be swapped     */
/*
  swap the bytes in the input 4-byte integer: ( 0 1 2 3 -> 3 2 1 0 )
*/
{
    register char *cvalues;
    register long ii;

    union u_tag {
        char cvals[4];      /* equivalence an array of 4 bytes with */
        INT32BIT ival;      /* a float */
    } u;

    cvalues = (char *) ivalues;   /* copy the initial pointer value */

    for (ii = 0; ii < nvals;)
    {
        u.ival = ivalues[ii++];  /* copy next float to buffer */

        *cvalues++ = u.cvals[3]; /* copy the 4 bytes in turn */
        *cvalues++ = u.cvals[2];
        *cvalues++ = u.cvals[1];
        *cvalues++ = u.cvals[0]; 
    }
    return;
}
/*--------------------------------------------------------------------------*/
void ffswap8(double *dvalues,  /* IO - pointer to doubles to be swapped     */
             long nvals)       /* I  - number of doubles to be swapped      */
/*
  swap the bytes in the input doubles: ( 01234567  -> 76543210 )
*/
{
    register char *cvalues;
    register long ii;

    union u_tag {
        char cvals[8];   /* equivalence an array of 8 bytes with */
        double dval;      /* a double */
    } u;

    cvalues = (char *) dvalues;      /* copy the pointer value */

    for (ii = 0; ii < nvals;)
    {
        u.dval = dvalues[ii++];  /* copy next double to buffer */

        *cvalues++ = u.cvals[7]; /* copy the 8 bytes in turn */
        *cvalues++ = u.cvals[6];
        *cvalues++ = u.cvals[5];
        *cvalues++ = u.cvals[4];
        *cvalues++ = u.cvals[3];
        *cvalues++ = u.cvals[2];
        *cvalues++ = u.cvals[1];
        *cvalues++ = u.cvals[0];
    }
    return;
}




/************************************************************************/
char *fitsname(char * NameStep)
{
    char Sreturn[256];
    char Name[256];
    char *ValRet;

    strcpy(Name, NameStep);
    if (strstr(Name, ".fit") != NULL)  strcpy(Sreturn, Name);
    else sprintf(Sreturn, "%s.%s", Name, "fits");
    ValRet = strdup(Sreturn);
    return (ValRet);
}

/************************************************************************/
Bool std_inout(char *Filename) {return False;}

FILE *fits_file_des_in(char *fname)
{
   FILE *fp;
   
   if (std_inout(fname) == True)  fp = stdin;
   else 
   {
      fp = fopen(fitsname(fname), "rb");
      if (!fp) 
      {
        cerr << "Unable to open file: " <<  fname  << endl;
        exit(-1);
      }
   }
   return fp;
}
FILE *fits_file_des_out(char *fname)
{
   FILE *fp;
   
   if (std_inout(fname) == True) fp = stdout;
   else 
   {
      fp = fopen(fitsname(fname), "wb");
      if (!fp) 
      {
        cerr << "Unable to open file: " <<  fname  << endl;
        exit(-1);
      }
   }
   return fp;
} 
/*****************************************************************/

void    error(int num, char *msg1, char *msg2)
  {
  fprintf(stderr, "\n> %s%s\n\n",msg1,msg2);
  exit(num);
  }

/******************************* swapbytes **********************************/

void	swapbytes(void *ptr, int nb, int n)
/*
Swap bytes for doubles, longs and shorts (for DEC machines or PC for inst.).
*/
  {
   char	*cp;
   int	j;

  cp = (char *)ptr;

  if (nb&4)
    {
    for (j=0; j<n; j++)
      {
      cp[0] ^= (cp[3]^=(cp[0]^=cp[3]));
      cp[1] ^= (cp[2]^=(cp[1]^=cp[2]));
      cp +=4;
      }
    return;
    }
  if (nb&2)
    {
    for (j=0; j<n; j++)
      {
      cp[0] ^= (cp[1]^=(cp[0]^=cp[1]));
      cp += 2;
      }
    return;
    }
  if (nb&1)
    return;

  if (nb&8)
    {
    for (j=0; j<n; j++)
      {
      cp[0] ^= (cp[7]^=(cp[0]^=cp[7]));
      cp[1] ^= (cp[6]^=(cp[1]^=cp[6]));
      cp[2] ^= (cp[5]^=(cp[2]^=cp[5]));
      cp[3] ^= (cp[4]^=(cp[3]^=cp[4]));
      cp += 8;
      }
    return;
    }

  error(EXIT_FAILURE, (char*) "*Internal Error*: Unknown size in ", (char*) "swapbytes()");

  return;
  }

/******************************* fitsfind ***********************************/

int fitsfind(char *fitsbuf, char *keyword)
/* Search for a FITS keyword in a FITS header. 
   return the first line containing the keyword: keyword */
  {
   char	*ptr;
   int	i=0, len;

  len = strlen(keyword);

  ptr= (char *) &(fitsbuf[80*i]);
  // RG printf(" %80s\n", ptr);
  for (i=0; strncmp(ptr, "END     ", 8); i++)
  {
    if (!strncmp(ptr, keyword, len))
    {
       // printf(" we got it %d\n",i);
       return i;
    }
    ptr= (char *) &(fitsbuf[80*(i+1)]);
  }
  if (strncmp(keyword, "END     ", 8))
  {
    return RETURN_ERROR;
  }
  else
  {
    return i;
  }
}
/******************************* fitsfind2 ***********************************/

int fitsfind2(char *fitsbuf, int headersize, char *keyword)
/* Search for a FITS keyword in a FITS header. 
   return the first line containing the keyword: keyword */
  {
   char	*ptr;
   int	i, len;

  len = strlen(keyword);
  //total_len = strlen(fitsbuf);
  //printf(" headersize=%d \n", headersize);
  ptr= (char *) &(fitsbuf[0]);
  //printf(" %80s\n", ptr);
  for (i=0; strncmp(ptr, "END     ",8) && (i*80 < headersize); i++)
  {
    if (!strncmp(ptr, keyword, len))
    {
       // printf(" we got it %d\n",i);
       return i;
    }
    ptr= (char *) &(fitsbuf[80*(i+1)]);
  }
  if (!strncmp(keyword, "END     ", 8))
  {
    return RETURN_ERROR;
  }
  else
  {
   // printf(" keyword %s not found  \n", keyword);
   return -1;
  }
  
}
/******************************** fitsnfind **********************************/

char *fitsnfind(char *fitsbuf, char *str, int nblock)
/* search for a FITS keyword in a fits header of nblock blocks. */
{
  int i;

  for (i=0;i<36*nblock;i++)
      if (!strncmp(&fitsbuf[80*i], str, strlen(str)))
              return &fitsbuf[80*i];

  return (char *)NULL;
}


/********************************* fitsread **********************************/

int fitsread(char *fitsbuf,char *keyword,void *ptr,h_type type,type_data t_type)
/* read a FITS keyword in a fits header. */
  {
   int		pos;
   static char	s[80];
   char		*str, *st, *c;

  if ((pos = fitsfind(fitsbuf, keyword)) < 0)
    return RETURN_ERROR;

  str = &fitsbuf[80*pos+10];

  switch(type)
    {
    case H_INT:		if (t_type == T_SHORT)
			  sscanf(str, "    %hd", (short *)ptr);
			else
			  sscanf(str, "    %ld", (long *)ptr);
			break;

    case H_FLOAT:
    case H_EXPO:	if (t_type == T_DOUBLE)
			  sscanf(str, "    %lf", (double *)ptr);
			else
			  sscanf(str, "    %f", (float *)ptr);
			break;

    case H_BOOL:	sscanf(str, "%1s", s);
			*(int *)ptr = ((int)s[0] == 'T') ? 1 : 0;
			break;

    case H_STRING:	c = strncpy((char *) ptr, str, 70); 
                       /* column 11 to 80 included (FITS convention)  */
                       st = strchr( (char *) ptr, '/');
		       if (st != NULL) *st = (char) '\0';
		       break;

    case H_COMMENT:	sscanf(str, "%s", (char *)ptr);
			break;

    default:		error(EXIT_FAILURE,
				(char*) "*Internal Error*: Unknown FITS type in ",
				(char*) "fitsread()");
			break;
    }

  return RETURN_OK;
  }

/******************* fitsreadhist ****************************/

int fitsreadhist(char *fitshead, int headersize, char *keyword, char **add_ptr)
/* 
 * PURPOSE: read recursively the value of the input keyword 'keyword' in a fits header, hold in the string 'fitshead' 
 * and write it in the string referred by 'add_ptr'. 
 *   it is useful for HISTORY & COMMENT field for instance.
 *
 * INPUTS: fitshead --- STRING containing the FITS header
 *         keyword  --- STRING containing the name of the keyword (HISTORY or COMMENT for example)
 * OUTPUT: **add_ptr --- POINTEUR on the address of the resulting string
 * RETURN: INT, the number of bytes used for memory allocation of the string referred by add_ptr.
 *
 */ 
{
   int		pos, nb_lines, size;
   char		*inptr, *outptr,  *c;

	inptr = fitshead;
   *add_ptr = (char *) calloc(80*10*+1, sizeof(char));  /* v 3.4 */
   **add_ptr='\0';
	size = 80*10;
   outptr = *add_ptr;
   nb_lines = 0;

   DEBUH = 0; // RG 
	
     while ((pos = fitsfind2(inptr, headersize, keyword)) >= 0) 
     {
        inptr += 80*pos + 8 ;
	  c = strncpy( outptr, inptr, 72);
	  if (DEBUH) cout << "inptr = " << inptr << endl;
	  inptr += 72;
	  
	  outptr += 72;
     nb_lines++;
	  *outptr='\0';
     if( (nb_lines%10)== 0){
		 *add_ptr = (char *) realloc(*add_ptr, (nb_lines+10)*80+1);
		 size = (nb_lines+10)*80;
       outptr = *add_ptr + nb_lines*72;
	  }
	}
 // version 3.4 put to zero
 pos = strlen(*add_ptr);
 memset(*add_ptr+pos,'\0', size-pos);
 if(DEBUH) printf(" readhist keyword=%s pos=%d size=%d \n", keyword, pos, size);
 if(DEBUH) printf("%s\n", *add_ptr);
  return size;
}

int fitsaddhist_com(fitsstruct *pfitsbuf, char *comment, char *type_com)
/* 
 * PURPOSE: Add a comment in the field 'history' or 'comment' of the structure 
 *    FITSSTRUCT. It begins at new line of 72 characters.
 *
 * INPUT:  comment --- STRING, value of HISTORY or COMMENT field;
 *         type_com --- STRING, "HISTORY" or "COMMENT" to specify in which field the comment
 *                      must be appended.
 * OUTPUT: pfitsbuf --- FITSSTRUCT *, address of the structure FITSSTRUCT holding the FITS Header;
 * RETURN: INT, the length of the added comment.
 *
 *  16-MAR-1998 
 */
{
  int add_len = 0;  /* Length of the comment or history to be added */
  int nb_chars = 0; /* number of characters stored in out */
  int out_nblines = 0;  /* number of lines of the output */
  int add_nblines = 0;  /* number of lines to be added */
  char *out_ptr=NULL; /* pointer to the output */
  int out_size=0;   /* the allocated size, not to be confused with the length! */
  int out_len ;  /* the actual size */
  int status = 0;
  char *Newptr;
  int i;
   
    /* Length of the comment to be added */
  if (comment != NULL) add_len = strlen(comment); 
                 else return(0); /* it is finished nothing to add !!! */


    /*--- TYPE of the comment ---*/
  if ( strcmp(type_com,"HISTORY") == 0 ) 
      {
      out_ptr        = pfitsbuf->history;
      out_size = pfitsbuf->hist_size; 
      status = 1;
      }

  if ( strcmp(type_com,"COMMENT") == 0 ) 
      {
      out_ptr        = pfitsbuf->comment;
      out_size = pfitsbuf->com_size; 
      status = 1;
      }
   
  if ( status == 0 ) 
      {
      printf(" ERROR in fitsaddhist_com: the type of the comment is  %s!\n\n",
               type_com);
      exit(-1);
      }

  /* find out the length of the field */
  if (out_ptr == NULL) out_len = 0; else out_len = strlen(out_ptr);
  
//  nb_chars = out_len + add_len;
  out_nblines = out_len / 72;
  if( (out_len % 72) > 0) out_nblines ++;
  nb_chars = (out_len % 72) ;
  add_nblines = add_len / 72;
  if( (add_len % 72) > 0) add_nblines ++;
  out_nblines += add_nblines;

 /* Memory reallocation if there is not enough memory for adding the comment */
  if (out_nblines*72 +1  > out_size)
      {
     //  Newptr = new char [out_nblines*72 +1  ];
      Newptr = (char*) malloc( (out_nblines*72 +1  ) *sizeof(char));
      if (Newptr == NULL)
          {
          cerr << "Error: unable to allocate memory : "  << endl;
          exit(-1);
          }
      memset(Newptr,'\0', (out_nblines*72 +1));
      strncpy(Newptr, out_ptr, out_size);
      // Newptr[out_size] = '\0';
      free ((char *) out_ptr);
      out_ptr = Newptr;
      out_size = out_nblines*72 +1;
      if (DEBUH) cout << "Memory reallocation = " << out_size << endl;
      if (DEBUH) cout << " out_ptr = " << out_ptr <<"X" << endl;
      }
 if (DEBUH) cout << " nb_chars="  << nb_chars << endl;
 if (nb_chars > 0) for(i = nb_chars; i < 72;i++) strcat(out_ptr, " ");
 /* Now add the comment */
  strcat(out_ptr, comment);
  

  if (DEBUH) cout << " out_ptr = " << out_ptr  << endl;

  /* update of the input structure */
  if ( strcmp(type_com,"COMMENT") == 0 ) 
      {
      pfitsbuf->comment = out_ptr ;
      pfitsbuf->com_size = out_size; 
      }
  else
      {
      pfitsbuf->history = out_ptr ;
      pfitsbuf->hist_size = out_size; 
      }
  return add_len;

}





/********************************* fitsadd *********************************/

int fitsadd(char **add_fitsbuf, char *keyword, char *comment, int *pfitsheadsize)
	  /* 
		* PURPOSE: If the keyword 'keyword' doesn't exist, a fits line containing this keyword is created & added to
		*          the STRING referred by 'add_fitsbuf'. The header size referred by pfitsheadsize
		*
		* INPUTS: keyword --- STRING, the name of the keyword to update or to add
		*         comment --- STRING, value to put in the field 'keyword'
		*
		* OUTPUTS: add_fitsbuf   --- STRING POINTER, the address of the fits header string to update
		*          pfitsheadsize --- POINTER, address of the header size
		*
		* RETURN: INT, the line number in the fits header where the keyword has been added.
		*
		* CAUTION: doesn't work for creating multiple fits lines field like HISTORY or COMMENT fields.
		*
		*/
{
  char    *key_ptr;
  int     headpos, nkey;

	

  if ((headpos = fitsfind(*add_fitsbuf, keyword)) < 0) {

    headpos = fitsfind(*add_fitsbuf, (char*) "END     ");
	 nkey = *pfitsheadsize / 80;
	 if (nkey - headpos < 4) {
       *pfitsheadsize += FBSIZE;
       *add_fitsbuf = (char *) realloc((char *) *add_fitsbuf, (size_t) *pfitsheadsize );
	memset(*add_fitsbuf+*pfitsheadsize-FBSIZE,'\0', FBSIZE);  // v3.4

	 }
	 
	 // Go to the 'END' line and copy it to the next line
    key_ptr = *add_fitsbuf + 80*headpos;
	 
    sprintf(key_ptr, "%-8.8s=                      / %-47.47s",
				keyword, comment?comment:" ");
	 sprintf(key_ptr + 80, "%-80s","END");

  }

  return headpos;
}

/********************************* fitswrite *********************************/

int fitswrite(char *fitsbuf, char *keyword, void *ptr, h_type type, type_data t_type, int *pfitsheadsize)
	  /* 
		* PURPOSE: Update the value (enclosed in 'ptr') of the keyword 'keyword' in the STRING referred by 'fitsbuf'.
		*          If the keyword 'keyword' doesn't exist, a fits line containing this keyword is created & added to
		*          the STRING referred by 'fitsbuf'. The header size referred by pfitsheadsize
		*
		* INPUTS: keyword --- STRING, the name of the keyword to update or to add
		*         ptr     --- POINTER, address of the value to put in the field 'keyword'
		*         type    --- H_TYPE, type of the value referred by 'ptr'
		*         t_type  --- TYPE_DATA, secondary type of the value referred by 'ptr'
		*
		* OUTPUTS: fitsbuf --- STRING, the fits header to update
		*          pfitsheadsize --- POINTER, address of the header size
		*
		* RETURN: INT, RETURN_OK if no problems, RETURN_ERROR if an error occures.
		*
		* CAUTION: doesn't work for updating or creating multiple fits lines field like HISTORY or COMMENT fields.
		*
		*/ 
{
  int i, l, pos;
  char str[70];
  char *pc = (char *) ptr;

  if ((pos = fitsfind(fitsbuf, keyword)) < 0)
	 fitsadd(&fitsbuf,keyword, NULL, pfitsheadsize);


  switch(type)
    {
    case H_INT:	sprintf(str, "%20d", (t_type==T_SHORT)?
				*(short *)ptr: *(int *)ptr);
			break;

    case H_FLOAT:	sprintf(str, "        %12.4f", (t_type==T_DOUBLE)?
				*(double *)ptr: *(float *)ptr);
			break;

    case H_EXPO:	sprintf(str, "    %16.9e", (t_type==T_DOUBLE)?
				*(double *)ptr: *(float *)ptr);
			break;
    case H_BOOL:	if (*(int *)ptr)
			  sprintf(str, "                   T");
			else
			  sprintf(str, "                   F");
			break;
    case H_STRING:	l = strlen(pc);
			for (i=0; i < l; i++) str[i] = pc[i];
			for (i=l; i < 69; i++) str[i] = ' ';
			str[69] = '\0';
			break;
    case H_COMMENT:	sprintf(str, "%69s", (char *)ptr);
			break;
    default:		error(EXIT_FAILURE,
				(char*) "*FATAL ERROR*: Unknown FITS type in ",
				(char*) "fitswrite()");
			break;
    }

  if ((pos = fitsfind(fitsbuf, keyword)) < 0)
  {
  return RETURN_ERROR;
  }
  else 
  {

     fitsbuf += 80*pos+10;
      i = 0;
     while (str[i] != '\0') 
     {
        fitsbuf[i] = str[i];
        i++;
     }
  }

  return RETURN_OK;
}




/************************** fitswritehist *********************************/

int fitswritehist(fitsstruct *pfitsbuf)
	  /* 
		* PURPOSE: add in the field 'fitshead' the value contained in the fields 'history' & 'comment'.
		*          The header size referred by 'pfitsbuf->fitsheadsize' is updated.
		*
		* INPUTS: pfitsbuf --- POINTER to FITSSTRUCT, the fits header to update
		*
		*
		* RETURN: INT, the size of 'fitshead'.
		*
		*/ 
{
  char    *outptr, *ptr, *curptr;
  int     len, lenhist, lencom,size,i;

	

	size = pfitsbuf->fitsheadsize;
	outptr = (char *) calloc(size+1 ,sizeof(char)); // v3.4

	/* copy all the fields, except HISTORY & COMMENT fields, of the fits header in the new string */
	ptr = pfitsbuf->fitshead;
	curptr = outptr;
	while (strncmp(ptr,"END",3) != 0) {
	  if ((strncmp(ptr,"HISTORY",7) != 0) && (strncmp(ptr,"COMMENT",7) != 0)){
		 strncpy(curptr,ptr,80);
		 curptr +=80;
	  }
	  ptr +=80;
	}
	*curptr='\0';


	/* is there enough memory for adding 'history' & 'comment' fields of the FITSSTRUCT structure? */
	lenhist = (pfitsbuf->history != NULL?strlen(pfitsbuf->history):0);
	lencom = (pfitsbuf->comment != NULL? strlen(pfitsbuf->comment) :0);
	len =  lenhist + ((lenhist / 72)+1)*8 + lencom + ((lencom / 72)+1)*8 + strlen(outptr) + 80*sizeof(char);
	if (len >= FBSIZE) {
 	  size = FBSIZE*(len/FBSIZE + 1);
	  outptr = (char *) realloc( (void *) outptr, (size_t) sizeof(char)*(size+1) );
	  curptr = outptr + strlen(outptr);
	}
	

	/* Add in 'fitshead' field the 'history' & 'comment' fields of the FITSSTRUCT structure */
	len = (pfitsbuf->history != NULL?strlen(pfitsbuf->history):0);
	ptr = pfitsbuf->history;
	i=0;
	while(i<len){
	  if( ptr + 72 > pfitsbuf->history + pfitsbuf->hist_size ) 
		printf(" warning fitswritehist hist_size=%d \n",pfitsbuf->hist_size );
	  sprintf(curptr,"%-8.8s%-72.72s","HISTORY ",ptr);
	  i += 72;
	  ptr += 72;
	  curptr += 80;
	}


	len = (pfitsbuf->comment != NULL? strlen(pfitsbuf->comment) :0);
	ptr = pfitsbuf->comment;
	i=0;
	// printf(" pfitsbuf->com_size=%d len=%d\n", pfitsbuf->com_size, len);
	while(i<len){
	  // printf(" strlen=%d \n", strlen(ptr));
	  if( ptr +72 > pfitsbuf->comment + pfitsbuf->com_size ) 
		printf(" warning fitswritehist com_size=%d \n",pfitsbuf->com_size );
	  sprintf(curptr,"%-8.8s%-72.72s","COMMENT ",ptr);
	  // left centered, 8 is the minimum number of caracters
	  //  .8 is the maximum number of carac.
	  
	  i += 72;
	  ptr += 72;
	  curptr += 80;
	}

	/* Add the 'END' keyword */
	sprintf(curptr,"%-80s","END");
	len = (size - strlen(outptr))/80;
	
	for (i=0, curptr += 80;i<len;i++,curptr += 80) 
	  sprintf(curptr,"%-80s","  ");



	/* Update the  header and its size */
	pfitsbuf->fitshead = outptr;
	pfitsbuf->fitsheadsize = size;
	
			
	return size; 
  
}





/****************************** readfitshead ********************************/

char  *readfitshead(FILE *file, char *filename, int *nblock)
	  /* 
		* read data from the FITS-file header 
		*
		*/
{
   int     n;
   char    *buf;

  if (!(buf= new char[FBSIZE]))
    error(EXIT_FAILURE, (char*) "*Error*: Not enough memory in ", (char*) "readfitshead()");

  /*Find the number of FITS blocks of the header while reading it */
  QFREAD(buf, FBSIZE, file, filename);

  if ( strncmp(buf, "SIMPLE  ", 8) && strncmp(buf, "XTENSION", 8) )
    error(EXIT_FAILURE, filename, (char*) " is NOT a FITS file!");

  for (n=1; !fitsnfind(buf,(char*) "END     ", n); n++)
    {
    if (!(buf=(char *)realloc(buf, (size_t)(FBSIZE*(n+1)))))
      error(EXIT_FAILURE, (char*) "*Error*: Not enough memory in ",(char*) "readfitshead()");
    QFREAD(&buf[FBSIZE*n], FBSIZE, file, filename);
    }
  *nblock = n;
  return  buf;
}


/****************************** readimagehead *******************************/

void readimagehead(fitsstruct *field)
/* extract some data from the FITS-file header */
{
  int		i, n;
  char		*buf, st[80], *point;

  buf = readfitshead(field->file, field->filename, &n);
  field->naxis = FITSTOI((char*) "NAXIS   ", 0);
  if(field->naxis < 1)
    error(EXIT_FAILURE, field->filename,(char*) " NAXIS is not equal to 1,2, or 3 !");

  field->bitpix = FITSTOI((char*) "BITPIX  ", 0);
  if (field->bitpix != BP_BYTE
	&& field->bitpix != BP_SHORT
	&& field->bitpix != BP_INT
	&& field->bitpix != BP_FLOAT
	&& field->bitpix != BP_DOUBLE)
    error(EXIT_FAILURE,(char*) "Sorry, I don't know that kind of data.",(char*) "");

  field->bitpix = FITSTOI((char*) "BITPIX  ", 0);
  if (field->bitpix != BP_BYTE
	&& field->bitpix != BP_SHORT
	&& field->bitpix != BP_INT
	&& field->bitpix != BP_FLOAT
	&& field->bitpix != BP_DOUBLE)
    error(EXIT_FAILURE,(char*) "Sorry, I don't know that kind of data.",(char*) "");

  field->bytepix = (field->bitpix>0?field->bitpix:-field->bitpix)>>3;
  if (field->naxis > 0)
  {
     field->width =  FITSTOI((char*) "NAXIS1  ", 0);
     field->crpixx = FITSTOF((char*) "CRPIX1  ", 0.);
     field->crvalx = FITSTOF((char*) "CRVAL1  ", 0.0);
     field->cdeltx = FITSTOF((char*) "CDELT1  ", 0.0);
     (field->TabAxis)[0] = field->width;
     (field->TabRef)[0] = field->crpixx;
     (field->TabValRef)[0] = field->crvalx;
     (field->TabStep)[0] = field->cdeltx;
     field->npix = field->width;
     field->crotax = FITSTOF((char*) "CROTA1  ", 0.0);
     FITSTOS((char*) "CTYPE1 ", field->ctypex, "       ");
  }
  if (field->naxis > 1)
  {
     field->height = FITSTOI((char*) "NAXIS2  ", 0);
     field->crpixy = FITSTOF((char*) "CRPIX2  ", 0.);
     field->crvaly = FITSTOF((char*) "CRVAL2  ", 0.0);
     field->cdelty = FITSTOF((char*) "CDELT2  ", 0.0);
     (field->TabAxis)[1] = field->height;
     (field->TabRef)[1] = field->crpixy;
     (field->TabValRef)[1] = field->crvaly;
     (field->TabStep)[1] = field->cdelty;
     field->npix = field->width*field->height;
     field->crotay = FITSTOF((char*) "CROTA2  ", 0.0);
     FITSTOS((char*) "CTYPE2 ", field->ctypey, "       ");
  }
  if (field->naxis > 2)
  {
     (field->TabAxis)[2] = FITSTOI((char*) "NAXIS3  ", 0);
     (field->TabRef)[2] = FITSTOF((char*) "CRPIX3  ", 0.);
     (field->TabValRef)[2] = FITSTOF((char*) "CRVAL3  ", 0.0);
     (field->TabStep)[2] = FITSTOF((char*) "CDELT3  ", 0.0);
     field->npix = field->npix*(field->TabAxis)[2];
  }

  field->bscale = FITSTOF((char*) "BSCALE  ", 1.0);
  field->bzero = FITSTOF((char*) "BZERO   ", 0.0);
  field->epoch = FITSTOF((char*) "EPOCH   ", 0.0);

  //FITSTOS("OBJECT  ", field->ident, "Unknown");

  field->fitshead = buf;
  field->fitsheadsize = n*FBSIZE;
  field->origin = (char*) "\0";
  strcpy(field->rident, (char*) "");

  /* HISTORY & COMMENT fields */
  field->hist_size = fitsreadhist(field->fitshead,field->fitsheadsize,(char*) "HISTORY",&field->history);
  field->com_size = fitsreadhist(field->fitshead,field->fitsheadsize,(char*) "COMMENT",&field->comment);

  }



/**********************************************************************/

void  readdatai(FILE *file, char *filename, int bitpix, int Nelem, int *ptr, float bscale, float bzero)
/* read and convert input data stream in int. */
  {
  int i,size=Nelem;
  char *bufdata;
  long NbrByte = size * ABS(bitpix) / 8;

    //if (!(bufdata=new char [NbrByte]))
    //   error(EXIT_FAILURE, "*Error*: Not enough memory in ",  "readdata()");
    bufdata = alloc_buffer((size_t) NbrByte);
    QFREAD(bufdata, NbrByte, file, filename);
/*  
#if BYTESWAPPED == 1
cout << "MACHINE = " << MACHINE <<  endl;
cout << "BYTESWAPPED = True" << endl;
#endif*/

    switch(bitpix)
      {
      case BP_BYTE: 
                    for (i=0; i< size; i++) ptr[i] = (int) (((unsigned char *)bufdata)[i]*bscale+bzero);
                    break;

      case BP_SHORT:    
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap2((short int *)bufdata, size);    /* reverse order of bytes in each bufdata */
#endif
      
			for (i=0; i< size; i++)
			ptr[i] = (int) (((short int *)bufdata)[i]*bscale+bzero);
			break;

      case BP_INT:      
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
			for (i=0; i<size; i++)
			 ptr[i] = (int) (((long int *)bufdata)[i]*bscale+bzero);
			break;
			
      case BP_FLOAT:
#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size);
#endif
			for (i=0; i<size; i++)
			{
			   ptr[i] = (int) (((float *) bufdata)[i]*bscale+bzero);
			}
			break;
      case BP_DOUBLE:
#if BYTESWAPPED == 1
    ffswap8((double *)bufdata, size);   /* reverse order of bytes in each value */
#endif
			for (i=0; i<size; i++)
			  ptr[i] = (int) (((double *)bufdata)[i]*bscale+bzero);
			break;

      default:		error(EXIT_FAILURE,
				(char*) "*FATAL ERROR*: unknown BITPIX type in ",
				(char*) "readdata()");
			break;
      }
  // delete bufdata;
  free_buffer((char *) bufdata);
  return;
  }


/**********************************************************************/

void  readdataf(FILE *file, char *filename, int bitpix, int Nelem, float *ptr, float bscale, float bzero)
/* read and convert input data stream in float. */
  {
  int i,size=Nelem;
  char *bufdata;
  // float *ptrf;
  long NbrByte = size * ABS(bitpix) / 8;

  //if (!(bufdata=new char [NbrByte]))
  //     error(EXIT_FAILURE, "*Error*: Not enough memory in ",  "readdata()");
    if (bitpix != BP_FLOAT)
               bufdata = alloc_buffer((size_t) NbrByte);
    else bufdata = (char *) ptr;
    QFREAD(bufdata, NbrByte, file, filename);
 
/*    
#if BYTESWAPPED == 1
cout << "MACHINE = " << MACHINE <<  endl;
cout << "BYTESWAPPED = True" << endl;
#endif
*/
    switch(bitpix)
      {
      case BP_BYTE: 
                    for (i=0; i< size; i++) ptr[i] = ((unsigned char *)bufdata)[i]*bscale+bzero;
                    break;

      case BP_SHORT:    
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap2((short int *)bufdata, size);    /* reverse order of bytes in each bufdata */
#endif
      
			for (i=0; i< size; i++)
			  ptr[i] = ((short int *)bufdata)[i]*bscale+bzero;
			break;

      case BP_INT:      
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size);  /* reverse order of bytes in each value */
#endif
			for (i=0; i<size; i++)
			  ptr[i] = ((long int *)bufdata)[i]*bscale+bzero;
			break;
      case BP_FLOAT:
#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
			for (i=0; i<size; i++)
			{
			   ptr[i] = ((float *) bufdata)[i]*bscale+bzero;
			}
			break;
      case BP_DOUBLE:
#if BYTESWAPPED == 1
    ffswap8((double *)bufdata, size);   /* reverse order of bytes in each value */
#endif
			for (i=0; i<size; i++)
			  ptr[i] = ((double *)bufdata)[i]*bscale+bzero;
			break;

      default:		error(EXIT_FAILURE,
				(char*) "*FATAL ERROR*: unknown BITPIX type in ",
				(char*) "readdata()");
			break;
      }
  if (bitpix != BP_FLOAT) free_buffer((char *) bufdata);
  // delete bufdata;
  return;
  }


/**********************************************************************/

void  readdatai(fitsstruct *field, int *ptr)
{
    readdatai(field->file, field->filename, field->bitpix,
              field->npix, ptr, field->bscale, field->bzero);
}

/**********************************************************************/

void  readdataf(fitsstruct *field, float *ptr)
{
    readdataf(field->file, field->filename, field->bitpix,
              field->npix, ptr, field->bscale, field->bzero);
}

/**********************************************************************/

void  writedatai(FILE *file, char *filename, int bitpix, int Nelem, 
                      int *ptr, float bscale, float bzero)
{
  /* read and convert input data stream in float. */

  int i,size=Nelem;
  char *bufdata;
  short *bufs;
  int *bufi;
  float *buff;
  double *bufd;
  float Val;
  long NbrByte = size * ABS(bitpix) / 8;

   //if (!(bufdata=new char [NbrByte])) 
   //     error(EXIT_FAILURE, "*Error*: Not enough memory in ",  "writedataf()");
#if BYTESWAPPED == 1
    bufdata = alloc_buffer((size_t) NbrByte);
#else
    if (bitpix != BP_INT)
               bufdata = alloc_buffer((size_t) NbrByte);
    else bufdata = (char *) ptr;
#endif

    bufs = (short *) bufdata;
    bufi = (int *) bufdata;
    buff = (float *) bufdata;
    bufd = (double *) bufdata;

    // cout << "writedatai" << endl << "BITPIX = " << bitpix  << endl;

    switch(bitpix)
      {
      case BP_BYTE:	for (i=0; i< size; i++)
			{
                           Val = (ptr[i] - bzero) / bscale;
                           //if (Val > 127) Val = 127;
                           //if (Val < -128) Val = -128;
                           bufdata[i] = (char) (Val+0.5);
			}
			break;
      case BP_SHORT:
			for (i=0; i< size; i++)
			{
                           Val = (ptr[i] - bzero) / bscale;
                           //if (Val > 32767) Val = 32767;
                           //if (Val < -32768) Val = -32768;
                           if (Val > 0) bufs[i] = (short) (Val+0.5);
                           else  bufs[i] = (short) (Val-0.5);
			}
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap2((short int *)bufdata, size); /* reverse order of bytes in each value */
#endif
 			break;
      case BP_INT:
			for (i=0; i< size; i++)
			{
                           Val = (int)((double)(ptr[i] - bzero) / bscale);
                           if (Val > 0) bufi[i] = (int) (Val+0.5);
                           else  bufi[i] = (int) (Val-0.5);
                        }
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
			break;
      case BP_FLOAT:
			for (i=0; i< size; i++)
                           buff[i] = (ptr[i] - bzero) / bscale;
#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size);
#endif
			break;
      case BP_DOUBLE:
			for (i=0; i< size; i++)
                           bufd[i] = (double)((ptr[i] - bzero) / bscale);
#if BYTESWAPPED == 1
    ffswap8((double *)bufdata, size); /* reverse order of bytes in each value */
#endif
			break;
      default:		error(EXIT_FAILURE,
				(char*) "*FATAL ERROR*: unknown BITPIX type in ",
				(char*) "writedata()");
			break;
      }
// cout << "Nelem = " << Nelem << endl;
// cout << "bitpix = " << bitpix << endl;
// cout << "NbrByte = " << NbrByte << endl;

  QFWRITE(bufdata, NbrByte,file, filename);
  // delete bufdata;
#if BYTESWAPPED == 1
  free_buffer((char *) bufdata);
#else
  if (bitpix != BP_INT) free_buffer((char *) bufdata);
#endif
}


/**********************************************************************/

void  writedataf(FILE *file, char *filename, int bitpix, int Nelem, 
                      float *ptr, float bscale, float bzero)
{
  /* read and convert input data stream in float. */

  int i,size=Nelem;
  char *bufdata;
  short *bufs;
  int *bufi;
  float *buff;
  double *bufd;
  float Val;
  long NbrByte = size * ABS(bitpix) / 8;
  long FitsNbrByte = FBSIZE - (int) (NbrByte % FBSIZE);
 
// cout << " NbrByte = " << NbrByte << " FitsNbrByte = " << FitsNbrByte << endl;
  
   // if (!(bufdata=new char [FitsNbrByte])) 
   //     error(EXIT_FAILURE, "*Error*: Not enough memory in ",  "writedataf()");
#if BYTESWAPPED == 1
    bufdata = alloc_buffer((size_t) NbrByte);
#else
    if (bitpix != BP_FLOAT)
               bufdata = alloc_buffer((size_t) NbrByte);
    else bufdata = (char*) ptr;
#endif
 
    char *FitsBit = new char [FitsNbrByte];
    for (i = 0; i < FitsNbrByte; i++) FitsBit[i] = 0;
     	       
    bufs = (short *) bufdata;
    bufi = (int *) bufdata;
    buff = (float *) bufdata;
    bufd = (double *) bufdata;

// cout << "writedataf" << endl << "BITPIX = " << bitpix  << endl;
 
    switch(bitpix)
      {
      case BP_BYTE:	for (i=0; i< size; i++)
			{
                           Val = (ptr[i] - bzero) / bscale;
                           //if (Val > 127) Val = 127;
                           //if (Val < -128) Val = -128;
                           bufdata[i] = (char) Val;
			}
			break;
      case BP_SHORT:
			for (i=0; i< size; i++)
			{
                           Val = (ptr[i] - bzero) / bscale;
                           if (Val > 0) bufs[i] = (int) (Val+0.5);
                           else  bufs[i] = (int) (Val-0.5);
 			}
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
    ffswap2((short int *)bufdata, size); /* reverse order of bytes in each value */
#endif
 			break;
      case BP_INT:
			for (i=0; i< size; i++)
			{
                           Val = (int)((double)(ptr[i] - bzero) / bscale);
                           if (Val > 0) bufi[i] = (int) (Val+0.5);
                           else  bufi[i] = (int) (Val-0.5);
                        }
/* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
     ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
			break;
      case BP_FLOAT:
			for (i=0; i< size; i++)
			  {
			    buff[i] = (float) (ptr[i] - bzero) / bscale;
			  }
			

#if BYTESWAPPED == 1
    ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
			break;
      case BP_DOUBLE:
			for (i=0; i< size; i++)
                           bufd[i] = (double)((ptr[i] - bzero) / bscale);
			
#if BYTESWAPPED == 1
    ffswap8((double *)bufdata, size); /* reverse order of bytes in each value */
#endif
			break;
      default:		error(EXIT_FAILURE,
				(char*) "*FATAL ERROR*: unknown BITPIX type in ",
				(char*) "writedata()");
			break;
      }
//  cout << "Nelem = " << Nelem << endl;
//  cout << "bitpix = " << bitpix << endl;
//  cout << "NbrByte = " << NbrByte << endl;

  QFWRITE(bufdata, NbrByte,file, filename);
  QFWRITE(FitsBit, FitsNbrByte,file, filename);

#if BYTESWAPPED == 1
  free_buffer((char *) bufdata);
#else
  if (bitpix != BP_FLOAT) free_buffer((char *) bufdata);
#endif

  delete [] FitsBit;
  // delete bufdata;
}

/**********************************************************************/

void  writedatai(fitsstruct *field, int *ptr)
{
    writedatai(field->file, field->filename, field->bitpix,
              field->npix, ptr, field->bscale, field->bzero);
}


/**********************************************************************/

void  writedataf(fitsstruct *field, float *ptr)
{

// cout << "filename = " << field->filename << endl;
// cout << "npix = " << field->npix << endl;
// cout << "bscale = " << field->bscale << endl;
// cout << "bzero = " << field->bzero << endl;
// cout << "bitpix = " << field->bitpix << endl;

    writedataf(field->file, field->filename, field->bitpix,
              field->npix, ptr, field->bscale, field->bzero);
}



/**************************************/

char *creafitsheader()
{
   char *Buf;
   int i,k;
   static char	bufmodel[4][80] = {
	"SIMPLE  =                    T / THIS IS REGULAR FITS",
	"BITPIX  =                    0 /",
	"NAXIS   =                    0 /",
	"END"};

   if (!(Buf = (char *) malloc (FBSIZE*sizeof(char))))
      error(EXIT_FAILURE, (char*) "*Error*: Not enough memory in ", (char*) "creafitsheader()");

   for (k = 0; k < 4; k++)
   {
       for (i = 0; i < (int) strlen (bufmodel[k]); i++) Buf[k*80+i] = bufmodel[k][i];
       for (i = strlen (bufmodel[k]); i < 80; i++) Buf[k*80+i] = ' ';
       
   }
   for (k = 4; k < 36; k++) for (i = 0; i < 80; i++) Buf[k*80+i] = ' ';
   return (Buf);
}




/******************* initfield ******************************************************************/

void initfield(fitsstruct *field)
/* 
 * PURPOSE: initialize to "0" the fields of a FITSSTRUCT structure
 *
 * INPUT: *field --- FITSSTRUCT, fits header structure.
 *
 */
{
  int i;

  field->file=NULL;	
  field->fitsheadsize= 2880;
  field->bitpix = 0;
  field->bytepix = 0;
  field->width = 0;
  field->height = 0;
  field->npix = 0;
  field->bscale = 1.;
  field->bzero = 0.;
  field->crpixx = 0.;
  field->crpixy = 0.;
  field->crvalx = 0.;
  field->crvaly = 0.;
  field->cdeltx= 0.;
  field->cdelty = 0.;
  field->ngamma=0.;
  field->pixscale=0.;
  field->nlevels = 0;
  field->pixmin  = 0.;
  field->pixmax  = 0.;
  field->epoch = 0.;
  field->crotax = 0.;
  field->crotay = 0.;
  field->fitshead = creafitsheader();
  field->origin = (char*) "";
  strcpy(field->ctypex, "");
  strcpy(field->ctypey, "");
  strcpy(field->rident, "");

  /* HISTORY & COMMENT fields */
  field->history = NULL;
  field->hist_size = 0;
  field->comment = NULL;
  field->com_size = 0;

  field->naxis=0;
  for (i=0; i < MAX_NBR_AXIS;i++)
  {
     (field->TabAxis)[i]=0;
     (field->TabStep)[i]=0.;
     (field->TabRef)[i]=0.;
     (field->TabValRef)[i]=0.;
  }
}	

/******************* updatefitsheader ******************************************************/

void updatefitsheader(fitsstruct *field)
/* 
 * PURPOSE: Update the fields 'fitshead' and 'fitsheadsize' of a FITSSTRUCT structure from the values 
 * of other fields of the FITSSTRUCT structure.
 *
 * INPUT: *field --- FITSSTRUCT, a fits header structure
 *
 */
{
   int status;
   // extern softinfo Soft;
   
   if (DEBUH) cout << "updatefitsheader entry " << endl;
   
   if (field->fitsheadsize < FBSIZE) field->fitsheadsize=FBSIZE;
   if (field->fitshead == NULL)
   {
       field->fitshead = creafitsheader();
       field->fitsheadsize=FBSIZE;
   }
    fitswrite(field->fitshead, (char*) "NAXIS   ", &(field->naxis), H_INT, T_INT,&(field->fitsheadsize));
	

   if (field->naxis > 0)
     fitswrite(field->fitshead, (char*) "NAXIS1  ", &(field->width), H_INT, T_INT,&(field->fitsheadsize));

   if (field->naxis > 1)
     fitswrite(field->fitshead, (char*) "NAXIS2  ", &(field->height), H_INT, T_INT,&(field->fitsheadsize));

   if (field->naxis > 2) 
     fitswrite(field->fitshead, (char*) "NAXIS3  ", &((field->TabAxis)[2]), H_INT, T_INT,&(field->fitsheadsize));
   fitswrite(field->fitshead, (char*) "BITPIX  ", &(field->bitpix), H_INT, T_INT,&(field->fitsheadsize));
   if (((field->bzero != 0.) || (field->bscale != 1.)) && (field->bscale != 0.))
   {
       fitswrite(field->fitshead, (char*) "BSCALE  ",&(field->bscale), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   }
   if (ABS(field->bzero) > FLOAT_EPSILON)
       fitswrite(field->fitshead, (char*) "BZERO  ", &(field->bzero), H_EXPO, T_DOUBLE,&(field->fitsheadsize));

   if (ABS(field->crpixx)  > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CRPIX1  ", &(field->crpixx), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (ABS(field->crpixy) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CRPIX2  ", &(field->crpixy), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (ABS((*field).TabRef[2]) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CRPIX3  ", &((field->TabRef)[2]), H_EXPO, T_DOUBLE,&(field->fitsheadsize));

   if (ABS(field->crvalx) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CRVAL1  ", &(field->crvalx), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (ABS(field->crvaly) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CRVAL2  ", &(field->crvaly), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (ABS((*field).TabValRef[2]) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CRVAL3  ", &((field->TabValRef)[2]), H_EXPO, T_DOUBLE,&(field->fitsheadsize));

   if (ABS(field->cdeltx) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CDELT1  ", &(field->cdeltx), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (ABS(field->cdelty) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CDELT2  ", &(field->cdelty), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (ABS((*field).TabStep[2]) > FLOAT_EPSILON)
     fitswrite(field->fitshead, (char*) "CDELT3  ", &((field->TabStep)[2]), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
 
   if ((strlen(field->ctypex) > 1) && (strlen(field->ctypey) > 1))
   {
       fitswrite(field->fitshead, (char*) "CTYPE1  ", 
                               (void*)  "          ", H_STRING, T_INT,&(field->fitsheadsize));
       fitswrite(field->fitshead, (char*) "CTYPE1  ", 
                                 field->ctypex, H_STRING, T_INT,&(field->fitsheadsize));
       fitswrite(field->fitshead, (char*) "CTYPE2  ", 
                               (void*)  "          ", H_STRING, T_INT,&(field->fitsheadsize));
       fitswrite(field->fitshead, (char*) "CTYPE2  ", 
                                 field->ctypey, H_STRING, T_INT,&(field->fitsheadsize));
   }

//if (DEBUH) cout <<  Soft.banner() << endl;
	/* Add the soft copyright in the history field of the FITSSTRUCT structure */
//	status = fitsaddhist_com(field,Soft.banner(),"HISTORY");

if (DEBUH) cout << " field->origin = " << field->origin << endl;
	if (strlen(field->origin)  > 1) 
	{
	   status = fitsaddhist_com(field,field->origin,(char*) "HISTORY"); 
	   field->origin = (char*) "";
	}

if (DEBUH) cout << (char*) " field->rident = " << field->rident << endl;

	if (strlen(field->rident)  > 1) 
	   status = fitsaddhist_com(field,field->rident,(char*) "HISTORY"); 
if (DEBUH) cout << "5" << endl;

	status = fitswritehist(field);

   field->bytepix = ABS(field->bitpix) / 8;
if (DEBUH) cout << "END" << endl;


}

/****************************** writeimagehead *******************************************************************/
 
void writeimagehead(fitsstruct *Header)
	  /*
		* PURPOSE: Write down on the disk the FITS header enclosed in the field 'fitshead' of a FITSSTRUCT
		*    structure. Before writing down on the disk, one must update the header from the fields of the FITSSTRUCT
		*    structure.
		*
		* INPUT: *Header --- FITSSTRUCT, fits header structure
		*
		*/
{

  /* Update the header hold in the field 'fitshead' */
  updatefitsheader(Header);


  /* Write down on the disk */
  QFWRITE(Header->fitshead,Header->fitsheadsize,Header->file,Header->filename);
}

/****************************************************************/

void makehistory(char *mystring, char *myproc, char *myalgo, char *myargs)
/* add in mystring:  the date, the machine, the user name
                     procedure = myproc
                     algorithm = myalgo
                     myargs = myargs
*/

{
  time_t *ptp, tp;
  struct tm var;
  char *ptr;

  ptp = &tp;
  tp = time(ptp);
  if (tp == -1){
	 printf("ERROR in calling time function \n");
	 exit(-1);
  }

  var = *gmtime(&tp);
  strftime(mystring,26,"date=%d-%b-%Y %H:%M:%S ",&var);

  ptr = getenv("HOST");
  if (ptr != NULL)
  {
     strcat(mystring," node=");
     strcat(mystring, ptr);
  }

  ptr = getenv("USER");
  if (ptr != NULL)
  {
     strcat(mystring,"  user=");
     strcat(mystring, ptr);
  }
  sprintf(mystring, "%-72s",mystring);

  if (myproc != NULL)
  {
     strcat(mystring,"  procedure=");
     strcat(mystring, myproc);
  }

  if (myalgo != NULL)
  {
     strcat(mystring,"  algorithm=");
     strcat(mystring, myalgo);
  }

  sprintf(mystring + 72, "%-72s",mystring+72);

  if (myargs != NULL)
  {
     strcat(mystring, myargs);
  }
  sprintf(mystring + 2*72, "%-72s",mystring+2*72);
}

/*************************************/

void fits_write_fltarr(char *File_Name, fltarray &Mat)
{
   fitsstruct HD;

   initfield(&HD);
   HD.bitpix = -32;
   HD.width =  Mat.nx();
   HD.height = Mat.ny();
   HD.naxis = Mat.naxis();
   HD.npix = Mat.n_elem();
   for (int i=0; i < HD.naxis; i++) HD.TabAxis[i] = Mat.axis(i+1);
   fits_write_fltarr(File_Name, Mat, &HD);
}

/************************************/

void io_write_ima_float(char *File_Name, Ifloat &Mat)
{
   fitsstruct HD;

   initfield(&HD);
   HD.bitpix = -32;
   HD.width =  Mat.nc();
   HD.height = Mat.nl();
   HD.naxis = Mat.naxis();
   HD.npix = Mat.n_elem();
   for (int i=0; i < HD.naxis; i++) HD.TabAxis[i] = Mat.axis(i+1);
   io_write_ima_float(File_Name, Mat, &HD);
}

/************************************/

void io_write_ima_float(char *File_Name, Ifloat &Mat, fitsstruct *FitsHeader)
{
    FitsHeader->filename = fitsname(File_Name);
    FitsHeader->file = fits_file_des_out(File_Name);
    writeimagehead(FitsHeader);
    writedataf(FitsHeader, Mat.buffer());
    fclose(FitsHeader->file);
}

/************************************/

void fits_write_fltarr(char *File_Name, fltarray &Mat, fitsstruct *FitsHeader)
{
    FitsHeader->filename = fitsname(File_Name);
//     if (!(FitsHeader->file = fopen(FitsHeader->filename, "wb")))
//     {
//         fprintf(stderr,"Error writing on file %s\n",File_Name);
//         exit (-1);
//     }
    FitsHeader->file = fits_file_des_out(File_Name);
    writeimagehead(FitsHeader);
    writedataf(FitsHeader, Mat.buffer());
    fclose(FitsHeader->file);
}

/************************************/

void fits_read_fltarr(char *File_Name, fltarray & Data)
{
   fitsstruct FitsHeader;
   fits_read_fltarr(File_Name, Data, &FitsHeader, 1);
}

/************************************/

void io_read_ima_float(char *File_Name, Ifloat & Data)
{
   fitsstruct FitsHeader;
   io_read_ima_float(File_Name, Data, &FitsHeader);
}

/************************************/

void fits_read_fltarr(char *File_Name, fltarray & Data, fitsstruct *FitsHeader)
{
   fits_read_fltarr(File_Name, Data, FitsHeader, 1);
}

/************************************/

void fits_read_fltarr(char *File_Name, fltarray & Data, fitsstruct *FitsHeader,
 int openflag)
{
    // initfield(FitsHeader);
    FitsHeader->filename = strdup(fitsname(File_Name));
    FitsHeader->file = fits_file_des_in(File_Name);
//     if (!(FitsHeader->file = fopen(FitsHeader->filename, "rb")))
//     {
//         fprintf(stderr,"Error reading file %s\n",File_Name);
//         exit (-1);
//     }
    readimagehead(FitsHeader);

    switch(FitsHeader->naxis)
    {
       case 0: cerr << "Error: bad NAXIS keywords in " << File_Name << endl;
               exit(0);
               break;       
       case 1: Data.alloc((FitsHeader->TabAxis)[0]);
               break;       
       case 2: Data.alloc((FitsHeader->TabAxis)[0],
                               (FitsHeader->TabAxis)[1]);
               break;    
       case 3: Data.alloc((FitsHeader->TabAxis)[0],
                               (FitsHeader->TabAxis)[1],
                               (FitsHeader->TabAxis)[2]);
               break;    
        default: cerr << "Error: bad NAXIS keywords in " << File_Name << endl;
                 cerr << "       NAXIS must between 1 and 3" << endl;
               break;    
    }

   readdataf(FitsHeader->file,File_Name, FitsHeader->bitpix, Data.n_elem(),
              Data.buffer(), FitsHeader->bscale, FitsHeader->bzero);
   if( openflag) fclose(FitsHeader->file);
}

/************************************/

/************************************/

void io_read_ima_float(char *File_Name, Ifloat & Data, fitsstruct *FitsHeader)
{
    FitsHeader->filename = strdup(fitsname(File_Name));
    FitsHeader->file = fits_file_des_in(File_Name);
    readimagehead(FitsHeader);

    switch(FitsHeader->naxis)
    {
       case 0: cerr << "Error: bad NAXIS keywords in " << File_Name << endl; exit(0);
               break;       
       case 2: Data.alloc((FitsHeader->TabAxis)[0], (FitsHeader->TabAxis)[1]);
               break;    
        default: cerr << "Error: bad NAXIS keywords in " << File_Name << endl;
                 cerr << "       NAXIS must be equals to 2." << endl;
               break;    
    }

   readdataf(FitsHeader->file, File_Name, FitsHeader->bitpix, Data.n_elem(),
             Data.buffer(), FitsHeader->bscale, FitsHeader->bzero);
   fclose(FitsHeader->file);
}

/************************************/


/*
main ()
{
  float *data;
  int N,i;
  float Min=10000,Max=-1000000;

  field->filename = "153349o.fits";
  readimagehead();		
  printf("Nl = %d\n", field->height);
  printf("Nc = %d\n", field->width);
  printf("bitpix = %d\n", field->bitpix);
  printf("bscale = %f\n", field->bscale);
  printf("zero = %f\n", field->bzero);

  N = field->height*field->width;
  data = new float [N];

  readdata(data, N);
  for (i=0; i < N; i++)
  {
     if (Min > data[i]) Min = data[i];
     if (Max < data[i]) Max = data[i];
  }
  printf ("Min = %f, Max = %f\n", Min, Max);
  printf("Header = \n %s \n", field->fitshead);
  initfield();
  field->bitpix = 32;
  field->width = 5;
  field->height = 3;
  updatefitsheader();
  printf("Header = \n %s \n", field->fitshead);
}
*/

