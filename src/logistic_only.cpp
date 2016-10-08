//Author: Manos Papadakis

// This file was generated by compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>
#include "mn.h"

using namespace Rcpp;
using namespace std;
using namespace arma;

//[[Rcpp::export]]
vec logistic_only(mat x, colvec y){
  unsigned int j,d=2,n=x.n_rows,i,pcols=x.n_cols;
  char e='e';
  mat z(n,2),inv_L2(d,d),tmp,expyhat,z_tr(2,n),ytr=y.t(),W=zeros(1,n);
  vec p(n),L(pcols);
  colvec b_old(d),b_new(d),L1(d),yhat(n),L2_L1(d);
  double dif,s,t,sw=0.0,szw=0.0,sz2w=0.0,my = mean(y);
  b_old(0)=log(my/(1-my));
  b_old(1)=0;
  z.col(0)=ones(n);
  for(i=0;i<pcols;i++){
    z.col(1)=x.col(i);
    
    for(dif=1.0,s=0.0;dif>0.000000001;){
      sw=szw=sz2w=0.0;
      yhat = z*b_old;
      expyhat=(e^yhat);
      p = expyhat / ( 1 + expyhat );
      for(j=0;j<n;j++){
        t=p(j);
        W(j)=t*(1-t);
        sw+=W(j);
      }
      szw=sum(W*z.col(1));
      sz2w=sum(W*(z.col(1)^2));
      z_tr=z.t();
      L1=z_tr*(y-p);
      t=1.0/(sw*sz2w-szw*szw);
      inv_L2(0,0)=sz2w*t;
      inv_L2(0,1)=inv_L2(1,0)=-szw*t;
      inv_L2(1,1)=sw*t;
      b_new=b_old+inv_L2*L1;
      dif=sum(abs(b_new-b_old));
      b_old=b_new;
    }  
    tmp=ytr*yhat;
    for(j=0;j<n;j++)
      s+=log(1+expyhat(j));
    L(i)=2.0*(s-tmp(0));
  }
  return L;
}

// logistic
RcppExport SEXP Rfast_logistic_only(SEXP xSEXP,SEXP ySEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< mat >::type x(xSEXP);
    traits::input_parameter< colvec >::type y(ySEXP);
    __result = wrap(logistic_only(x,y));
    return __result;
END_RCPP
}