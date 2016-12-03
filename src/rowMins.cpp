//Author: Manos Papadakis

// This file was generated by compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>
#include <vector>

using namespace Rcpp;
using namespace std;
using namespace arma;

// [[Rcpp::export]]
vector<double> rowMins(NumericMatrix x,bool value=false){
  unsigned int i,p=x.nrow();
  mat X = mat(x.begin(), p, x.ncol(), false); 
  vector<double> F(p);
  vector<double>::iterator FF=F.begin();
  if(value){
  	colvec f=min(X,1);
  	colvec::iterator ff=f.begin();
  	for(;FF!=F.end();++FF,++ff)
      *FF=*ff;
  }else
    for(i=0;i<p;++i,++FF)
      *FF=(X.row(i)).index_min()+1;
  return F;
}

RcppExport SEXP Rfast_rowMins(SEXP xSEXP,SEXP valueSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< NumericMatrix >::type x(xSEXP);
    traits::input_parameter< bool >::type value(valueSEXP);
    __result = wrap(rowMins(x,value));
    return __result;
END_RCPP
}
