//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include <algorithm>
#include "mn.h"

using namespace Rcpp;
using namespace std;

//[[Rcpp::export]]
NumericMatrix stable_sort_col(NumericMatrix x,const bool descend){
  int n=x.nrow(),p=x.ncol(),i;
  NumericMatrix::iterator start=x.begin(),end=start+n;
  if(descend)  
    for(i=0;i<p;++i){
      stable_sort(start,end,descending_double);
      start=end;
      end+=n;
    }
  else
    for(i=0;i<p;++i){
      stable_sort(start,end);
      start=end;
      end+=n;
    }  
  return x;
}

//[[Rcpp::export]]
NumericMatrix stable_sort_row(NumericMatrix x,const bool descend){
  const int sz=x.ncol(),p=x.nrow();
  NumericVector rowi(sz);
  if(descend)
    for(int i=0;i<p;++i){
      rowi=x.row(i);
      stable_sort(rowi.begin(),rowi.end(),descending_double);
      x.row(i)=rowi;
    }
  else
    for(int i=0;i<p;++i){
      rowi=x.row(i);
      stable_sort(rowi.begin(),rowi.end());
      x.row(i)=rowi;
    } 
  return x;
}

// sort_col
RcppExport SEXP Rfast_stable_sort_col(SEXP xSEXP,SEXP descendSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< NumericMatrix >::type x(xSEXP);
    traits::input_parameter< const bool >::type descend(descendSEXP);
    __result = wrap(stable_sort_col(x,descend));
    return __result;
END_RCPP
}

// sort_row
RcppExport SEXP Rfast_stable_sort_row(SEXP xSEXP,SEXP descendSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< NumericMatrix >::type x(xSEXP);
    traits::input_parameter< const bool >::type descend(descendSEXP);
    __result = wrap(stable_sort_row(x,descend));
    return __result;
END_RCPP
}
