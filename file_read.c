/*
 Small IO Test Programm

 GD, Sep 26, 2019
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/param.h>
#include <stdint.h>
#include <string.h>


double deltaT( struct timeval *tv1, struct timeval *tv2)  
{
  /* return time diff between two timevals as double */
  double dt = 1. * ( tv2->tv_sec - tv1->tv_sec ) +
    ( tv2->tv_usec - tv1->tv_usec ) / 1e6 ;

  return dt;
}

double gaus_rnd() 
{
  /* simple gauss approx with sum of 12 rands */
  int i; 
  double r = 0;
  for ( i=0; i<12; i++ ) {
    r += (((double) random() )/ RAND_MAX) - 0.5 ;
  }
  return r ;
}

void usage() 
{
    printf("%s [-B:M:f: -o outfile] file-name\n", "file_read");
    exit(1);
}

int main( int argc, char** argv )
{
  char *file;
  size_t bufsize = 1024L*128L;
  unsigned char * bufptr;
  int c, i, fd, fdo;
  long mr;
  long long ntot = 0, ntoto = 0, maxread = 0;

  ssize_t nin, nout;

  /* for getopt */
  extern char *optarg;
  extern int optind;

  double ttot;

  int output = 0,  debug = 0;
 
  if (argc < 2) {
    usage();
  }

  struct timeval tvs, tv, tvnow;
  struct timezone tz;

  /* for random access read */
  double skip_fraction = 0., frnd;
  struct stat fsb;
  off_t fsize = 0, sret = 0;

  /* rnd gen seed */
  unsigned int seed;
  seed = getpid();
  srandom(seed);

  
  const size_t flenmx = 200;
  char ofile[flenmx];
  ofile[0]='\0';
  
  while( (c = getopt(argc, argv, "Do:hB:M:f:?")) != EOF) {
	
    switch(c) {
    case 'D' :
      debug = 1;
      break;
    case 'B' :
      bufsize = atol(optarg);
      break;
    case 'M': /* max size to read in MB */
      mr = atol(optarg);
      maxread = (uint64_t) mr * 1024L * 1024L;
      break;
    case 'o' :
      output = 1;
      strncpy(ofile,optarg,flenmx);
      break;
    case 'f':
        skip_fraction = atof(optarg);
        if ( skip_fraction < 1. ) {
          skip_fraction = 1./(1.-skip_fraction);
        }  
        break;

    case 'h':
    case '?':
      usage();
      
    }
  }
  
  if ( ( argc - optind) != 1) {
    usage();
  }




      
  file = argv[optind];


  bufptr = malloc( bufsize );

  gettimeofday(&tvs, &tz);

  
  fd = open(file, O_RDONLY);
  if ( fd < 0 ) {
    printf("Error opening file %s \n", file);
    return(2);
  }

  if ( output ) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fdo = open( ofile, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if ( fdo < 0 ) {
      printf("Error opening file %s %d\n", ofile, fdo);
      return(3);
    }
  }

  if ( skip_fraction > 0 ) {
    fstat( fd, &fsb );
    fsize = fsb.st_size;
    if ( debug ) {
      printf ( "Filesize %ld  %s\n", fsize, file );
    }
  }


  
  int nit = 0;
  while ( 1 ) {
    nit ++;
    nin = read( fd, bufptr, bufsize ) ;
    ntot += nin;

    if ( output ) {
      nout = write( fdo, bufptr, nin ) ;
      ntoto += nout;
    }
    
    if ( debug ) printf("%d %ld %lld\n", nit, nin, ntot );
    
    if ( nin < bufsize || (maxread > 0 && ntot > maxread) ) {
      break;
    }

    if ( skip_fraction > 0 ) {
      /* current pos */
      sret = lseek( fd, (off_t) 0, SEEK_CUR );
      frnd = bufsize * skip_fraction * (2. + gaus_rnd() );

      if ( sret + frnd < 0 ) frnd = 0;

      sret = lseek( fd, sret + frnd, SEEK_SET );
      if ( debug ) printf("lseek set to %ld\n", sret );

      if ( sret >= fsize ) {
          break;
      }
    }


    
  }


  close(fd);
  if ( output ) close(fdo);
  
  gettimeofday(&tv, &tz);
  ttot = deltaT ( &tvs, &tv);

  printf("%lld %lld  %8.4f  %8.2f  %ld %s %s\n", ntot, ntoto, ttot, 
	 ntot/(1024.*1024.*ttot), 0L, file, ofile);


  free( bufptr );
  return(0);

  
}
