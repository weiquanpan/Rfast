//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include "functions_to_export.h"

using namespace Rcpp;
using namespace std;

//[[Rcpp::export]]
vector<string> add_functions_to_export(const string dir_to_export,const string dir_to_file){
  int which_string_has_export=0,len_which_not_exp=1;
  vector<string> newfiles=readDirectory(dir_to_file),already_exported_files,which_not_exported;
  if(newfiles.empty()){
  	stop("Warning: empty folder.\n");
  }
  vector<string> data_export=readFile(dir_to_export,which_string_has_export);
  if(which_string_has_export==-1){
  	stop("Error. can't find \"export\" function in NAMESPACE file.\n");
  }
  string exported_files=data_export[which_string_has_export];
  exported_files.erase(exported_files.end()-1);
  exported_files.erase(exported_files.begin(),exported_files.begin()+7);
  already_exported_files=split_words(exported_files);
  sort(already_exported_files.begin(),already_exported_files.end());
  if(exported_files.size()==0){
    for(unsigned int i=0;i<newfiles.size();++i){
      if(binary_search(already_exported_files.begin(),already_exported_files.end(),newfiles[i])==false){
        exported_files+=newfiles[i]+',';
        which_not_exported.resize(len_which_not_exp);
        which_not_exported[len_which_not_exp-1]=newfiles[i];
        len_which_not_exp++;
      }
    }
    exported_files[exported_files.size()-1]=')';
  }else{
    for(unsigned int i=0;i<newfiles.size();++i){
      if(binary_search(already_exported_files.begin(),already_exported_files.end(),newfiles[i])==false){
        exported_files+=','+newfiles[i];
        which_not_exported.resize(len_which_not_exp);
        which_not_exported[len_which_not_exp-1]=newfiles[i];
        len_which_not_exp++;
      }
    }
    exported_files+=')';
  }
  data_export[which_string_has_export]="export("+exported_files;
  writeFile(data_export,dir_to_export);
  return which_not_exported;
}

RcppExport SEXP Rfast_add_functions_to_export(SEXP dir_to_exportSEXP,SEXP dir_to_fileSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< const string >::type dir_to_export(dir_to_exportSEXP);
    traits::input_parameter< const string >::type dir_to_file(dir_to_fileSEXP);
    __result = wrap(add_functions_to_export(dir_to_export,dir_to_file));
    return __result;
END_RCPP
}
