//Author: Manos Papadakis

// This file was generated by compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rinternals.h>
#include <R.h>
#include <cmath>

using namespace Rcpp;

//[[Rcpp::export]]
SEXP Log(SEXP x){
  const int nrow=Rf_nrows(x),ncol=Rf_ncols(x),n=nrow*ncol;
  SEXP f;
  switch(TYPEOF(x)){
    case REALSXP:{
      f=PROTECT(Rf_allocMatrix(REALSXP,nrow,ncol));
      double *start_f=REAL(f),*start_x=REAL(x),*end_x=start_x+n;
      for(;start_x!=end_x;++start_x,++start_f)
        *start_f=std::log(*start_x);
      break;
    }
    default:{
      f=PROTECT(Rf_allocMatrix(INTSXP,nrow,ncol));
      int *start_f=INTEGER(f),*start_x=INTEGER(x),*end_x=start_x+n;
      for(;start_x!=end_x;++start_x,++start_f)
        *start_f=std::log(*start_x);
      break;
    }
  }
  UNPROTECT(1);
  return f;
}

RcppExport SEXP Rfast_Log(SEXP x) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    __result = Log(x);
    return __result;
END_RCPP
}