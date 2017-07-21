//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include <R.h>
#include <Rinternals.h>

using namespace Rcpp;
using namespace std;
using namespace arma;

// [[Rcpp::export]]
NumericVector row_max_indices(NumericMatrix x){
  const unsigned int p=x.nrow();
  mat X = mat(x.begin(), p, x.ncol(), false); 
  NumericVector F(p);
  NumericVector::iterator FF=F.begin();
  for(unsigned int i=0;i<p;++i,++FF)
      *FF=(X.row(i)).index_max()+1;
  return F;
}

//[[Rcpp::export]]
SEXP row_max(SEXP x){
  int ncol=Rf_ncols(x),nrow=Rf_nrows(x);
  SEXP F;
  F=PROTECT(Rf_allocVector(REALSXP,nrow));
  double *xx=REAL(x),*end=xx+ncol*nrow,*f=REAL(F),*x3,*ff;
  const double *endf=f+LENGTH(F);
  for(ff=f;ff!=endf;++ff,++xx)
    *ff=*xx;
  for(;xx!=end;)
    for(ff=f,x3=xx,xx+=nrow;x3!=xx;++ff,++x3){
      *ff=std::max(*ff,*x3);
    }
    UNPROTECT(1);
  return F;
}

RcppExport SEXP Rfast_row_max_indices(SEXP xSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< NumericMatrix >::type x(xSEXP);
    __result = wrap(row_max_indices(x));
    return __result;
END_RCPP
}

RcppExport SEXP Rfast_row_max(SEXP x) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    __result = row_max(x);
    return __result;
END_RCPP
}