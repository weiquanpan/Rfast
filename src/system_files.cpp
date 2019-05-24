//Author: Manos Papadakis

#include <RcppArmadillo.h>
#include "system_files.h"
#include <algorithm>

using namespace std;
using namespace Rcpp;
using std::vector;
using std::string;
using std::count;
using std::ofstream;
using std::remove;
using std::strtok;
using std::ifstream;
using std::sort;
using std::lower_bound;
using std::getline;
using std::binary_search;
using std::endl;

void print_error(){}

void reset_file(ifstream& file){
  file.clear();
  file.seekg(0,ios::beg);
}

vector<string> split_words(string x){
  x.erase(remove(x.begin(),x.end(), ' '),x.end());
  int n=count(x.begin(),x.end(),',')+1;
  vector<string> y(n);
  x+=",";
  int i=0;
  const char *split=",";
  char *token = strtok(&x[0], split);
  while (token != NULL) {
    y[i++]=token;
    token = strtok(NULL, split);
  }
  return y;
}

void writeFile(vector<string> f,string path){
  ofstream oput(path.c_str());
  if(!oput.is_open()){
    Rcpp::stop("can't open file\n");
  }
  for(unsigned int i=0;i<f.size();++i){
    oput<<f[i]<<endl;
  }
}

vector<string> readFile(string path,int& which_string_has_export){
  ifstream input(path.c_str());
  string s,export_word="export";
  vector<string> f;
  which_string_has_export=-1;
  bool found_export=false;
  while(getline(input,s)){
    if(find_export(s,export_word) && !found_export){ // oso briskei to export kai den to exei ksanavrei
      which_string_has_export=f.size();
        found_export=true;
    }
    f.push_back(s);
  }  
  return f;
}

bool find_export(string x,string y){
  unsigned int leny=y.size();
  if(x.size()<leny){
    return false;
  }
  unsigned int i=0;
  for(i=0;i<y.size();++i){
    if(x[i]!=y[i])
      return false;
  }
  return true;
}

vector<string> readDirectory(const string path,const int n){
  DIR *dir=NULL;
  struct dirent *ent;
  vector<string> files;
  string textf;
  if((dir = opendir(path.c_str())) != NULL) {
    readdir(dir);
    readdir(dir);
    while((ent = readdir(dir)) != NULL) {
      textf=ent->d_name;
      textf.erase(textf.end()-n,textf.end());
      files.push_back(textf);
    }
    closedir(dir);
  }else{
    Rcpp::stop("Error: Could not open directory with path \""+path+"\"");
  }
  return files;
}

vector<string> find_which(vector<string> big,vector<string> small){
  vector<string> f;
  for(unsigned int i=0;i<big.size();++i){
    if(binary_search(small.begin(),small.end(),big[i])==false){
      f.push_back(big[i]);
    }
  }
  return f;
}

vector<string> find_duplis(vector<string> x){
  x.push_back("@");
  vector<string>::iterator a=x.begin(),b=a+1;
  vector<string> f;
  int s=0;
  for(;b!=x.end();++b){
    if(*a!=*b){
      if(s){
        f.push_back(*a);
      }
      a=b;
      s=0;
    }else{
      ++s;
    }
  }
  return f;
}

bool binary_help(vector<string>::iterator first,vector<string>::iterator last,string& val,vector<string>::iterator& res){
  vector<string>::iterator t=lower_bound(first,last,val);
  int tt=t-first+1;
  bool found=false;
  if(tt!=last-first && val>=*first){
    res=t;
    found=true;
  }
  return  found;
}

void dont_read_man(vector<string>& all_rd_files,vector<string>& no_read){
  if(no_read[0]!=""){
    sort(all_rd_files.begin(),all_rd_files.end());
    vector<string>::iterator fv;
    for(unsigned int i=0;i<no_read.size();++i){
      if(binary_help(all_rd_files.begin(),all_rd_files.end(),no_read[i],fv)){
        all_rd_files.erase(fv);
      }
    }
  }
}

bool is_alias(string& s){
  return (s.size()>5 && s[0]=='\\' && s[1]=='a' && s[2]=='l' 
            && s[3]=='i' && s[4]=='a' && s[5]=='s');
}

bool is_title(string& s){
  return (s.size()>5 && s[0]=='\\' && s[1]=='t' && s[2]=='i' 
            && s[3]=='t' && s[4]=='l' && s[5]=='e');
}

void remove_alias_and_spaces(string &s){
  DEBUG("Start remove_alias_and_spaces");
  s.erase(s.end()-1);
  s.erase(s.begin(),s.begin()+7);
  remove_spaces(s);
  DEBUG("End remove_alias_and_spaces");
}

vector<string> read_aliases(ifstream &file){
  DEBUG("Start read_aliases");
  reset_file(file);
  vector<string> als;
  string s;
  do{
    getline(file,s);
    if(is_alias(s)){
      remove_alias_and_spaces(s);
      DEBUG(s);
      als.push_back(s);
    }
  }while(!is_title(s));
  DEBUG("End read_aliases");
  return als;
}

bool is_example(const char *s,int len){
  return (len>7 && s[0]=='\\' && s[1]=='e' && s[2]=='x' 
            && s[3]=='a' && s[4]=='m' && s[5]=='p' 
            && s[6]=='l' && s[7]=='e' && s[8]=='s');
}

bool get_example(ifstream &file,string &res){
  string s;
  getline(file,s);
  bool is_e=is_example(s.c_str(),s.size());
  res = is_e ? s : "";
  return is_e;
}

string read_example(ifstream &file,int& long_lines){
    string als;
    string s;
    unsigned int count_curly_bracket=1;/*at least there will be an empty example section*/
    while(!get_example(file,s));
    getline(file,s);
    while(count_curly_bracket>0){/* check for {} and extract the example correct*/
        if(s.size()>99){ // 100 max lines
            ++long_lines;
        }
        for(auto& symbol : s){
            if(symbol=='{')
                ++count_curly_bracket;
            else if(symbol=='}')
                --count_curly_bracket;
        }
        s+="\n";
        als+=s;
        getline(file,s);
    }
    als[als.size()-2]='\n'; // replace } with new line
    als.erase(als.end()-1); // remove extra new line
    return als;
}

bool is_usage(string &s){
  return (s.size()>5 && s[0]=='\\' && s[1]=='u' && s[2]=='s' 
            && s[3]=='a' && s[4]=='g' && s[5]=='e');
}

bool get_usage(ifstream &file,string &res){
  //DEBUG("Start get_usage");
  string s;
  getline(file,s);
  bool is_e=is_usage(s);
  res = is_e ? s : "";
  //DEBUG("End get_usage");
  return is_e;
}

vector<string> read_usage(ifstream &file){
  DEBUG("START read_usage");
  vector<string> usg;
  string s;
  bool sinexeia_apo_kato_grammi=false;
  reset_file(file);
  while(!get_usage(file,s));
  do{
    getline(file,s);
    remove_spaces(s);
    if(s!="" && sinexeia_apo_kato_grammi){
      DEBUG("ektelesi tin sinexeia stin apo kato grammi");
      sinexeia_apo_kato_grammi=false;
      usg[usg.size()-1]+=s;
    }else if(s!="}" && s[s.size()-1]!='}' && s!=""){ //  keni grammi, sketo "}", "sinartisi}"
      usg.push_back(s);
    }
    if(s!="" && s[s.size()-1]!=')'){ //  periptosi pou i sinartisi paei kai stin kato grammi
      DEBUG("BBrike sinexeia stin apo kato grammi");
      sinexeia_apo_kato_grammi=true;
    }
  }while(s[s.size()-1]!='}');
  if(s.size()>1 && s[s.size()-1]=='}'){ //  periptosi "sinartisi}"
    s.erase(s.end()-1);
    usg.push_back(s);
  }
  //for(auto& v : usg)
  //  name_of_functions_in_usage.push_back(v.substr(0,v.find(')')));
  DEBUG("END read_usage");
  return usg;
}

void remove_spaces(string& s){
  s.erase(remove_if(s.begin(),s.end(),[&](char& x){return isspace(x);}),s.end());
}


string read_function_from_r_file(ifstream &file){
  string func;
  string s;
  size_t bg;
  reset_file(file);
  DEBUG("START read_function_from_r_file");
  do{
    getline(file,s);
  }while(s[0]=='#'); // oso briskei sxolia
  DEBUG(s);
  remove_spaces(s);
  func=s;
  if(!find_string(s,"){")){ // periptosi pou paei kai se alli grammi i sinartisi
    do{
      getline(file,s);
      remove_spaces(s);
      func+=s;
    } while (!find_string(s,"{")); 
  }
  DEBUG("function: ",func);
  string keyword_function1 = "<-function";
  string keyword_function2 = "=function";
  int keyword_function_size = keyword_function1.size();
  bg=func.find(keyword_function1);
  DEBUG(bg);
  if(bg==string::npos){
      DEBUG("trying operator =.");
      bg=func.find(keyword_function2);
      keyword_function_size = keyword_function2.size(); 
  }
  func.erase(func.begin()+bg,func.begin()+bg+keyword_function_size);
  DEBUG(func);
  func.erase(func.end()-1);
  DEBUG(func);
  DEBUG("END read_function_from_r_file");
  return func;
}

bool check_read_file(ifstream& file,char attr){
    DEBUG("Start checking file");
    string s;
    bool ret=true;
    while(getline(file,s)){
        if(is_dont_read(s,attr)){
            ret=false;
            break;
        }else if(!std::isspace(s[0])){
            break;
        }
    }
    DEBUG("End checking file");
    return ret;
}

bool is_dont_read(string& s,char attr){
    return s[0]==attr and s.size()>=sizeof("[dont read]") and //sizeof("[dont read]") 11 + \0
    s[1]=='[' and s[2]=='d' and s[3]=='o' and s[4]=='n' and s[5]=='t' and s[6]==' ' and s[7]=='r' and 
    s[8]=='e' and s[9]=='a' and s[10]=='d' and s[11]==']';
}

bool is_export(string& s){
    return s[0]=='#' and s.size()>=sizeof("[export]") and //sizeof("[export]") 8 + \0
           s[1]=='[' and s[2]=='e' and s[3]=='x' and s[4]=='p' and 
           s[5]=='o' and s[6]=='r' and s[7]=='t' and s[8]==']';
}


string read_current_signature_function_from_r_file(string& line,string keyword_function,ifstream &file,const int position_of_function_key){
    string func=line;
    DEBUG("START read_function_from_r_file");
    remove_spaces(line);
    if(!find_string(line,"){")){ // periptosi pou paei kai se alli grammi i sinartisi
        do{
            getline(file,line);
            remove_spaces(line);
            func+=line;
        } while (!find_string(line,"{")); 
    }
    DEBUG("function: ",func);
    func.erase(func.begin()+position_of_function_key,func.begin()+position_of_function_key+keyword_function.size());
    DEBUG(func);
    func.erase(func.end()-1);
    DEBUG(func);
    DEBUG("END read_function_from_r_file");
    return func;
}

//proipothesi na einai dilomeno h sinartisi tou stil a<-function h a=function
void read_functions_from_r_file(const string filename,vector<string> &exported_functions_names,vector<string> &not_exported_functions_names,List& signatures,bool& found_dont_read){
    
    ifstream file(filename.c_str());
    size_t position_of_function_key1=0,position_of_function_key2=0;
    string line;
    int depth_scope=0,number_of_line=0,number_of_export_line=0;
    bool found_export=false;
    
    while(getline(file,line) and isspace(line[0])){
        ++number_of_line;
    }
    
    if(is_dont_read(line,'#')){
        found_dont_read=true;
        DEBUG("found dont read: "+number_of_line);
    }else{    
        auto read_name_from_function = [&](string& line){
            string function_name;
            DEBUG(line);
            
            for(unsigned int i=0;i<line.size();++i){
                char ch=line[i];
                
                if(std::isspace(ch))
                    continue;
                else if(ch=='{'){
                    ++depth_scope;
                    DEBUG("depth: "+to_string(depth_scope));
                    continue;
                }else if(ch=='}'){
                    --depth_scope;
                    DEBUG("depth: "+to_string(depth_scope));
                    continue;
                }
                if(ch=='<' and i+9<line.size()){//an bro < kai exo akoma 9 theseis na psakso
                    position_of_function_key1=line.find("<-function");
                    // an brika to function kai eimai sto global scope
                    if(position_of_function_key1==i and depth_scope==0){
                        if(found_export){
                            DEBUG("<-function export: "+function_name);
                            exported_functions_names.push_back(function_name);
                            signatures[function_name]=List::create(_["signature"]=read_current_signature_function_from_r_file(line,"<-function",file,position_of_function_key1),_["filename"]=filename);
                        }else{
                            DEBUG("<-function: "+function_name);
                            not_exported_functions_names.push_back(function_name);
                        }
                        found_export=false;
                    }
                }else if(ch=='=' and i+8<line.size()){//an bro = kai exo akoma 8 theseis na psakso
                    position_of_function_key2=line.find("=function");
                    // an brika to function kai eimai sto global scope
                    if(position_of_function_key1==i and depth_scope==0){
                        DEBUG("=function: "+function_name);
                        if(found_export){
                            DEBUG("<-function export: "+function_name);
                            exported_functions_names.push_back(function_name);
                            signatures[function_name]=List::create(_["signature"]=read_current_signature_function_from_r_file(line,"=function",file,position_of_function_key2),_["filename"]=filename);
                        }else{
                            DEBUG("<-function: "+function_name);
                            not_exported_functions_names.push_back(function_name);
                        }
                        found_export=false;
                    }
                }
                function_name+=ch;
            }
            if(found_export and number_of_export_line==number_of_line-1){//an exo vrei export  stin porigoumeni grammi kai h epomeni einai space
                Rcout<<"Warning: In file '"<<filename<<"' unused [export] attribute in line "<<number_of_export_line<<".\n";
            }
            found_export=false;
        };
        
        do{
            ++number_of_line;
            remove_spaces(line);
            if(is_export(line) and depth_scope==0){ //mono oi sinartiseis poy einai sto global scope epitrepontai na einai export
                DEBUG("found export: "+line);
                number_of_export_line=number_of_line;
                found_export=true;
                continue;
            }else if(line[0]=='#'){ // pass comments
                DEBUG("found comments: "+line);
                continue;
            }
            read_name_from_function(line);
        }while(getline(file,line));
    }
}

List read_functions_and_signatures(string path){
    std::vector<string> exported_functions_names,not_exported_functions_names,files=read_directory(path),dont_read;
    exported_functions_names.reserve(500);
    not_exported_functions_names.reserve(500);
    bool found_dont_read=false;
    List signatures;
    for(auto& file : files){
        read_functions_from_r_file(path+"\\"+file,exported_functions_names,not_exported_functions_names,signatures,found_dont_read);
        if(found_dont_read){
            found_dont_read=false;
            dont_read.push_back(file);
        }
    }
    return List::create(_["dont read"]=dont_read,_["export"]=exported_functions_names,_["without export"]=not_exported_functions_names,_["signatures"]=signatures);
}
