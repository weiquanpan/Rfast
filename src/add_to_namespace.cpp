//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include "system_files.h"

using namespace Rcpp;


using std::vector;
using std::string;
using std::binary_search;


//[[Rcpp::export]]
vector<string> add_to_namespace(const string dir_to_export,const string dir_to_file,const bool sorting){
  int which_string_has_export=0,len_which_not_exp=1;
  vector<string> newfiles=read_functions_and_signatures(dir_to_file)["export"],already_exported_files,which_not_exported;
  if(newfiles.empty()){
  	stop("Warning: empty folder.\n");
  }
  vector<string> data_export=readFile(dir_to_export,which_string_has_export);
  if(which_string_has_export==-1){
  	stop("Error. can't find \"export\" function in NAMESPACE file with path \"%s\".\n",dir_to_export);
  }
  string exported_files=data_export[which_string_has_export];
  exported_files.erase(exported_files.end()-1);
  exported_files.erase(exported_files.begin(),exported_files.begin()+7);
  if(exported_files.size()==0 || sorting){
    if(sorting){
      exported_files.clear();
    }
    for(unsigned int i=0;i<newfiles.size();++i){
        exported_files+=newfiles[i]+',';
    }
    which_not_exported=newfiles;
    exported_files[exported_files.size()-1]=')';
  }else{
    already_exported_files=split_words(exported_files);
    sort(already_exported_files.begin(),already_exported_files.end());
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

RcppExport SEXP Rfast_add_to_namespace(SEXP dir_to_exportSEXP,SEXP dir_to_fileSEXP,SEXP sortingSEXP,SEXP no_readSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< const string >::type dir_to_export(dir_to_exportSEXP);
    traits::input_parameter< const string >::type dir_to_file(dir_to_fileSEXP);
    traits::input_parameter< const bool >::type sorting(sortingSEXP);
    __result = add_to_namespace(dir_to_export,dir_to_file,sorting);
    return __result;
END_RCPP
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//[[Rcpp::export]]
vector<string> remove_from_namespace(const string dir_to_export,vector<string> files_to_remove){
  int which_string_has_export=0;
  vector<string> data_export=readFile(dir_to_export,which_string_has_export);
  if(which_string_has_export==-1){
  	stop("Error. can't find \"export\" function in NAMESPACE file with path \"%s\".\n",dir_to_export);
  }
  vector<string> unknown_files;
  string exported_files=data_export[which_string_has_export],which_export;
  exported_files.erase(exported_files.end()-1);
  exported_files.erase(exported_files.begin(),exported_files.begin()+7);
  if(exported_files.size()==0){
    stop("Error. NAMESPACE file doesn't have any export function.\n");
  }else{
  	int len_unknown_files=1;
    vector<string> already_exported_files=split_words(exported_files);
    sort(files_to_remove.begin(),files_to_remove.end());
    for(unsigned int i=0;i<already_exported_files.size();++i){
      if(binary_search(files_to_remove.begin(),files_to_remove.end(),already_exported_files[i])==false){
        which_export+=already_exported_files[i]+",";
      }else{
      	unknown_files.resize(len_unknown_files);
        unknown_files[len_unknown_files++-1]=already_exported_files[i];
      }
    }
  }
  which_export[which_export.size()-1]=')';
  data_export[which_string_has_export]="export("+which_export;
  writeFile(data_export,dir_to_export);
  return unknown_files;
}

RcppExport SEXP Rfast_remove_from_namespace(SEXP dir_to_exportSEXP,SEXP files_to_removeSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< const string >::type dir_to_export(dir_to_exportSEXP);
    traits::input_parameter< vector<string> >::type files_to_remove(files_to_removeSEXP);
    __result = remove_from_namespace(dir_to_export,files_to_remove);
    return __result;
END_RCPP
}
