
//Author: Manos Papadakis

#include <RcppArmadillo.h>

using namespace Rcpp;
using namespace arma;

//[[Rcpp::export]]
NumericMatrix canberra2_dist(NumericMatrix x){
  const int ncl=x.ncol(),nrw=x.nrow();
  mat xx(x.begin(),nrw,ncl,false);
  NumericMatrix f(ncl,ncl);
  colvec xv(nrw),yv(nrw);
  register double a;
  int i,j;
  for(i=0;i<ncl-1;++i){
    xv=xx.col(i);
    for(j=i+1;j<ncl;++j){
      yv=xx.col(j);
      a=sum(abs(xv-yv)/(abs(xv)-abs(yv)));
      f(i,j)=a;
      f(j,i)=a;
    }
  }
  return f;
}

RcppExport SEXP Rfast_canberra2_dist(SEXP xSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< NumericMatrix >::type x(xSEXP);
    __result = wrap(canberra2_dist(x));
    return __result;
END_RCPP
}