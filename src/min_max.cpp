//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include <Rinternals.h>
#include <R.h>
#include "mn.h"

using namespace Rcpp;

//[[Rcpp::export]]
SEXP min_max(SEXP x,bool index=false){
  SEXP F;
  double *xx=REAL(x),*end=xx+LENGTH(x);
  double xxx;
  if(index){
    F = PROTECT(Rf_allocVector(INTSXP, 2));
    int *f=INTEGER(F),min_i=0,max_i=0;
    double *bg=xx;
    for(xx++;xx!=end;++xx){
      xxx=*xx;
      if(xxx>bg[max_i]){
        max_i=xx-bg;
      }else if(xxx<bg[min_i]){
        min_i=xx-bg;
      }
    }
    *f=min_i+1;
    f[1]=max_i+1;
    UNPROTECT(1);
    return F;
  }
  F= PROTECT(Rf_allocVector(REALSXP, 2));
  double *f=REAL(F),min=*xx,max=min;
  for(xx++;xx!=end;++xx){
    xxx=*xx;
    if(xxx>max)
      max=xxx;
    else if(xxx<min)
      min=xxx;
  }
  *f=min;
  f[1]=max;
  UNPROTECT(1);
  return F;
}


RcppExport SEXP Rfast_min_max(SEXP x,SEXP indexSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< bool >::type index(indexSEXP);
    __result = min_max(x,index);
    return __result;
END_RCPP
}


//[[Rcpp::export]]
SEXP min_max_perc(SEXP x){
  const int n=LENGTH(x);
  SEXP f=Rf_allocVector(REALSXP,4);
  double *start=REAL(x),*end=start+n,mx,mn,pos=0,xx,*FF=REAL(f);
  mn=mx=*start;
  for(;start!=end;++start){
    xx=*start;
    if(xx>0) pos++;
    if(mn>xx)
      mn=xx;
    else if(mx<xx)
      mx=xx;
  }
  *FF=mn;
  FF[1]=mx;
  FF[3]=(pos/n)*100.0;
  FF[2]=100.0-FF[3];
  return f;
}

RcppExport SEXP Rfast_min_max_perc(SEXP x) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    __result = min_max_perc(x);
    return __result;
END_RCPP
}
