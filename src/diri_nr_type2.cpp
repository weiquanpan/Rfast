//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include "mn.h"

using namespace Rcpp;
using namespace std;

//[[Rcpp::export]]
colvec diri_nr_type2(colvec a1,colvec a2,colvec ma,int p,const double tol){
  mat f2(p,p),slv;
  double sa;
  colvec f;
  while ( sum( abs( a2 - a1 ) ) > tol ) {
    a1 = a2;
    sa = sum(a1);
    f = ma - Digamma_v(a1,p) + digamma(sa);
    fill_with_value(f2.memptr(),f2.memptr()+p*p,trigamma(sa));
    f2.diag() = f2.diag() - Trigamma_v(a1,p);
    slv = solve(f2, f);
    a2 = a1 - slv.each_col();
  }
  return a2;
}

// part of the Compositional::diri.nr
RcppExport SEXP Rfast_diri_nr_type2(SEXP a1SEXP,SEXP a2SEXP,SEXP maSEXP,SEXP pSEXP,SEXP tolSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< colvec >::type a1(a1SEXP);
    traits::input_parameter< colvec >::type a2(a2SEXP);
    traits::input_parameter< colvec >::type ma(maSEXP);
    traits::input_parameter< int >::type p(pSEXP);
    traits::input_parameter< const double >::type tol(tolSEXP);
    __result = wrap(diri_nr_type2(a1,a2,ma,p,tol));
    return __result;
END_RCPP
}
