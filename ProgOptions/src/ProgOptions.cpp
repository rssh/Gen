#ifndef __GradSoft_ProgOptions_h
#include <GradSoft/ProgOptions.h>
#endif

/*
 * part of GradSoft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine.
 * http://www.gradsoft.kiev.ua
 * $Id: ProgOptions.cpp,v 1.59 2003-11-13 18:41:47 rin Exp $
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include <fstream>


namespace GradSoft {

using namespace std;

#define ARGUMENT " <argument>  "
const char* ProgOptions::szUNKNOWN = "unknown";

ProgOptions::ProgOptions(const char* optPrefix,
                         const char* pkgPrefix, bool allowUnknownOptions)
{
 optPrefix_ = optPrefix;
 optPrefixLen_ = strlen(optPrefix);
 pkgPrefix_ = pkgPrefix;
 pkgPrefixLen_ = strlen(pkgPrefix);
 allowUnknownOptions_=allowUnknownOptions;
 topAdditionalHelp_=NULL;
 botAdditionalHelp_=NULL;
 argc_=0;
 argv_=NULL;
 indexToArgv_=0;
 maxOptionLength_=pkgPrefixLen_+strlen("config")+strlen(ARGUMENT);
}

ProgOptions::~ProgOptions()
{
 destroyArgv();
}

void ProgOptions::destroyArgv()
{
 for(int i=0; i<argc_; ++i) {
   delete[] argv_[i];
 }
 if (argv_) delete[] argv_;
 argc_ = 0;
 argv_ = NULL;
}

void ProgOptions::saveArgv(int argc, char** argv)
{
 int prevArgc=argc_;
 enlargeArgv(argc,NULL);
 argc_=prevArgc+argc;
 int i,j;
 for(i=prevArgc,j=0; i<argc_; ++i,++j) {
  argv_[i]=new char[strlen(argv[j])+1];
  strcpy(argv_[i],argv[j]);
 }
}


void ProgOptions::put(const char* option, const char* description,
                 bool haveArgument,
                 bool (*callback)(const ProgOptions*,const char*, void*),
                 void* callbackData )
{
#define ARGUMENT " <argument>  "
  int tmp=strlen(option);
  if (haveArgument) tmp += strlen(ARGUMENT);
  if (maxOptionLength_<tmp) maxOptionLength_=tmp;

  StringPtrWithLen key(option,true);
  options_[key]=Element(haveArgument, description, callback, callbackData);
}

bool ProgOptions::unknownOption(const char* option)
{
 if (!allowUnknownOptions_) {
    if (argv_) {
       cerr << argv_[0] << ": unknown option: " << option << endl;
    }else{
       cerr << "(no name)" << ": unknown option: " << option << endl;
    }
 }
 return allowUnknownOptions_;
}


int ProgOptions::enlargeArgv(int argc, const char* executable)
{
        if (argc) 
        {
                if (argc_) 
                {
                        char** newArgv_ = new char*[argc_+argc];
                        for(int i=0; i<argc_; ++i) 
                        {
                                newArgv_[i] = argv_[i];
                        }
                        delete[] argv_;
                        argv_ = newArgv_;
                        indexToArgv_=argc_;
                        argv_[argc_+argc-1]=NULL;
                        return 1;
                } else {
                    argv_ = new char*[argc+1];
                    indexToArgv_=1;
                    if (executable==NULL) {
                        argv_[0] = new char[strlen(szUNKNOWN)+1];
                        strcpy(argv_[0],szUNKNOWN);
                    }else{
                        argv_[0] = new char[strlen(executable)+1];
                        strcpy(argv_[0],executable);
                    }
                    argv_[argc]=NULL;
                    return 0;
                }
        } 
        return 0;
}

void ProgOptions::removeWithoutKill(int i)
{
  for (int j=i;j<argc_-1;j++) argv_[j]=argv_[j+1];
  argv_[--argc_]=NULL;
}

void ProgOptions::removeIndicated(int i)
{
        delete[] argv_[i];
        removeWithoutKill(i);
}

void ProgOptions::putArgs(const char* executable, int argc, char** argv)
{
 if (argc_==0) {
   enlargeArgv(argc+1, executable);
   argc_=1;
 }else{
   enlargeArgv(argc,executable);
 }
 for(int i=0; i<argc; ++i){
   argv_[argc_] = new char[strlen(argv[i])+1];
   strcpy(argv_[argc_++],argv[i]);
 }
}

bool ProgOptions::parse(int argc, char** argv)
{
        if (argc) 
        {                
          for(int i=enlargeArgv(argc,NULL); i<argc; ++i) {
               argv_[argc_] = new char[strlen(argv[i])+1];
               strcpy(argv_[argc_++],argv[i]);
          }
          bool argv0IsUnknown = false;
          if (argv_[0]==NULL || 
                (argv0IsUnknown=(strcmp(argv_[0],szUNKNOWN)==0)) ) {
               if (argv0IsUnknown) {
                   delete[] argv_[0];
               } 
               argv_[0]=new char[strlen(argv[0]+1)];
               strcpy(argv_[0],argv[0]);
          }
        }
        if (argc_) {
          FnamesSet prevLoaded;
          bool retval = parse1(prevLoaded);
          return retval; 
        }else{
          return false;
        }
}


bool ProgOptions::parse1(FnamesSet& prevLoaded)
{
 bool retval=true;
 for(int i=indexToArgv_; i<argc_ && retval; ++i) {
    char* arg = argv_[i];
    char* argument=NULL;
    if (strncmp(arg,optPrefix_,optPrefixLen_)!=0){
      retval=unknownOption(arg);
      continue;
    }
    StringPtrWithLen key(arg+optPrefixLen_,false);
    IteratorType it = options_.find(key);
    if (it!=options_.end()) {
                if ((*it).second.have_argument()) {
         if (key.length() > (*it).first.length()) {
           if (arg[optPrefixLen_+(*it).first.length()]=='=') {
             argument=arg+optPrefixLen_+(*it).first.length()+1;
           }else{
             // it's not 'Opt=XXX' but longer option
             goto NotFoundLabel;
           }
         }else{
           if (i==argc_-1) {
              cerr << argv_[0] << ": option " << argv_[i] << " must have argument" << endl;
              retval=false;
              break;
           }else{
              ++i;
              argument=argv_[i];
           } 
         } 
       }
    }else{
        
NotFoundLabel:

       if (key.length()==pkgPrefixLen_+strlen("help")) {
         if (
           !strncmp(arg+optPrefixLen_,pkgPrefix_,pkgPrefixLen_)&&
           !strcmp(arg+optPrefixLen_+pkgPrefixLen_,"help")){
          outHelp(cerr); 
          retval=false;
         }
       }else if(key.length()==pkgPrefixLen_ + strlen("config")) {
         if (
           !strncmp(arg+optPrefixLen_,pkgPrefix_,pkgPrefixLen_)&&
           !strcmp(arg+optPrefixLen_+pkgPrefixLen_,"config"))
                 {
          removeIndicated(i);
          if (i==argc_) {
            cerr << argv_[0] << ": config file isn't specified" << endl;
            retval=false;
          }else{
            char*& fname = argv_[i];
            FnamesSet::iterator fnames_it = prevLoaded.find(fname);
            if (fnames_it!=prevLoaded.end()) { 
              cerr << argv_[0] 
                   << ":recurring reference to " 
                   << fname << " (ignored)" << endl;
            }else{
              prevLoaded.insert(fname);
              retval = loadFromFile1(fname,argv_[0]);
              if (!retval) {
                cerr << argv_[0] << ": failed to read options from " << fname << endl;
                if (errno!=0) perror(argv_[0]);
              }
            }
            removeWithoutKill(i--);
            continue;
          }
         }
       }else{
         retval=unknownOption(arg);
       }
       continue;
    }
    retval=(*it).second.handle(this,argument);
 }
 for(FnamesSet::iterator it = prevLoaded.begin(); it!=prevLoaded.end();) 
 {
    FnamesSet::iterator tmp = it;
    ++it;
    delete[] *tmp;
    prevLoaded.erase(tmp);
 }
 return retval;
}


void ProgOptions::WordSequence::commonSymbol(char ch) 
{
    if (!inWord_) {
        inWord_=true;
        word_=currentSymbol_;
    }
    *currentSymbol_++=ch;
}

void ProgOptions::WordSequence::separator() 
{
    if (inWord_) {
        inWord_=false;
        *currentSymbol_++=0;
        argDeque_.push_back(strcpy(new char[currentSymbol_-word_],word_));
    }
}

void ProgOptions::WordSequence::parseString(const char* str) 
{
    int length=strlen(str);
    if (length>0) 
    {
        words_= new char[length+1];
        currentSymbol_=words_;

        for (int i=0; str[i]; ++i) 
        {
            if (!comments_) 
            {
                if (!isGroup_)
                {
                    switch (str[i]) 
                    {
                    case '"':   
                        isGroup_=true;
                        continue;
                    case '/':
                        switch (str[i+1]) 
                        {
                        case '/':
                            goto A;
                        case '*':
                            comments_=true;
                            ++i;
                            continue;
                        }
                        break;
                    case '\t':
                    case ' ':
                        separator();
                        continue;
                    }
                }
                else
                {
                    if (str[i]=='"')
                    {
                        isGroup_=false;
                        continue;
                    }
                }

                if (str[i]=='\\') if (str[i+1]=='"') ++i;
                commonSymbol(str[i]);
                continue;
                
            } else if(str[i]=='/') if(i>0) if(str[i-1]=='*') comments_=false;
        }
    A:  isGroup_=false;
        separator();
        delete[] words_;
    }
}

#define MARKER "@ProgOptions config file"
#define MARKER1 "@\"ProgOptions config file\""

ProgOptions::WordSequence::~WordSequence()
{
}

bool ProgOptions::WordSequence::checkFirstWord()
{
        if (argDeque_.size()) {

                char* firstWord=argDeque_[0];
                if (!strcmp(firstWord,MARKER))
                {
                        argDeque_.pop_front();
                        return true;
                } 
        }
        return false;
}

void ProgOptions::WordSequence::completeArgv(const char* executable)
{
  int argc = argDeque_.size();

  owner_->enlargeArgv(argc,executable);

  if (owner_->argc_==0) {
     owner_->argv_[0] = new char[strlen(executable)+1];
     strcpy(owner_->argv_[0],executable);
     ++owner_->argc_;
  }

  for(int i=0; i<argc; ++i) {
    owner_->argv_[owner_->argc_++]=argDeque_[i];
  }
}

int ProgOptions::WordSequence::sequenceLength()
{
        return argDeque_.size();
}

bool  ProgOptions::parseString(const char* str, const char* exe)
{
        WordSequence words(this);
        words.parseString(str);
        if (words.sequenceLength()) 
        {
                words.completeArgv(exe);
                FnamesSet prevLoaded;
                return parse1(prevLoaded);
        }
        return false;
}

bool  ProgOptions::parseFile(const char* fname,const char* executable)
{
        if (loadFromFile1(fname,executable)) {

                char* filename = new char[strlen(fname)+1];
                strcpy(filename,fname);
                FnamesSet prevLoaded;
                prevLoaded.insert(filename);

                FnamesSet::iterator it=prevLoaded.begin();

                bool retval = parse1(prevLoaded);
                return retval;
        }
        if (argc_) cerr << argv_[0];
        else cerr << executable;

        cerr << ": failed to read options from " << fname <<  endl;
        if (errno!=0) {
          if (argc_) perror(argv_[0]);
          else perror(executable);
        }
        
        return false;
}

bool  ProgOptions::loadFromFile1(const char* fname, const char* executable)
{
 ifstream ifs;

#ifdef IOS_HAVE_NOCREATE
 ifs.open(fname, ios::in | ios::nocreate);
#else
 ifs.open(fname, ios::in);                
#endif

#ifdef IFSTREAM_HAVE_ISOPEN
 if(ifs.is_open()==0) return false;
#endif
 if (ifs.bad()) return false;

 WordSequence words(this); 
 
 char buff[10000];

 ifs.getline(buff,10000);
 words.parseString(buff);
 if (!words.checkFirstWord())
 {
        cerr << fname << ": is not configuration file resolvable by ProgOptions (token not found)" << endl;
        return false;
 }

 while(!ifs.eof()) {

    ifs.getline(buff,10000);
    if (buff[0]=='#') {
        continue;
    }
    words.parseString(buff);
 } 
 ifs.close();
 words.completeArgv(executable);
 return true;
}

bool  ProgOptions::saveToFile(const char* fname)
{
 ofstream of;
 of.open(fname);
 if (of.bad()) return false;

 of << MARKER1 << endl;
 of << endl;
 of << "# Automatically generated by: " << endl; 
 of << "\"" << argv_[0] << "\"" << endl;
 of << endl;
 of << "# The list of options: " << endl;

 int i;
 for(i=1; i<argc_; ++i) 
 {
        of << "\t"; 
        of << "\"";
        char* arg = argv_[i];
        char* word=new char[2*strlen(arg)+1];
        int j;
        for (j=0; *arg;) {
                if (*arg=='"') word[j++]='\\';
                word[j++] = *arg++;
        }
        word[j]=0; of << word; delete[] word;
        of << "\"";
        of << endl;
 }
 of.close();
 return true;
}

bool  ProgOptions::is_set(const char* option)   const
{
ConstIteratorType it = find(option, true);
return it!=options_.end() && (*it).second.is_set();
}

const char* ProgOptions::argument(const char* option) const
{
 ConstIteratorType it = find(option, false);
 return it==options_.end() ? NULL : (*it).second.argument();
}

void ProgOptions::setAdditionalHelp(bool onTop, const char* help)
{
  if (onTop) 
    topAdditionalHelp_ = help;
  else
    botAdditionalHelp_ = help;
}

void ProgOptions::prepareMsg(char *msg, char*prefix, char*option, bool haveArg) const
{ 
  char* argument=ARGUMENT;
  int i;
  for(i=0;i<maxOptionLength_;i++){
    if (*prefix) msg[i]=*prefix++;
    else if (*option) msg[i]=*option++;
	     else if (haveArg && *argument) msg[i]=*argument++;
	          else msg[i]=' ';
	}
  msg[i]=0;
}

void ProgOptions::outHelp(ostream& os) const
{
  if (topAdditionalHelp_) os << topAdditionalHelp_ << endl;
  char* msg=new char[maxOptionLength_+1];
  ConstIteratorType it;
  for(it=options_.begin(); it!=options_.end();++it){
    prepareMsg(msg,"",(char*)(*it).first.key(),
                       (*it).second.have_argument());
    os << "  " << optPrefix_ << msg;
    os << (*it).second.description() << endl;
  }
  if ((it=find("help", true))==options_.end()) {
    prepareMsg(msg,(char*)pkgPrefix_,"help",false);
	os << "  " << optPrefix_ << msg;
    os << "show this help message" << endl;
  }
  if ((it=find("config", true))==options_.end()) {
    prepareMsg(msg,(char*)pkgPrefix_,"config",true);
	os << "  " << optPrefix_ << msg;
    os << "read options from file specified" << endl;
  }
  delete[] msg;
  if (botAdditionalHelp_) os << botAdditionalHelp_ << endl;
}

ProgOptions::Element::Element(bool have_argument,
                          const char* description,
                       bool (*callback)(const ProgOptions*,const char*,void*),
                              void* callbackData
                                  )
  :have_argument_(have_argument),
   description_(description),
   callback_(callback),
   callbackData_(callbackData),
   is_set_(false),
   argument_(NULL)
{} 

ProgOptions::Element::Element(const ProgOptions::Element& x)
{
  have_argument_=x.have_argument_;
  description_=x.description_;
  callback_=x.callback_;
  callbackData_=x.callbackData_;
  is_set_=x.is_set_;
  argument_=x.argument_;
}

ProgOptions::Element::Element()
{
  have_argument_=0;
  description_="not initialized";
  callback_=NULL;
  callbackData_=NULL;
  is_set_=false;
  argument_=NULL;
}

ProgOptions::Element::~Element()
{
}


bool ProgOptions::Element::handle(const ProgOptions* progOptions, 
                                  const char* argument) 
{
  is_set_=true;
  argument_=argument;
  bool retval = true;
  if (callback_) retval=callback_(progOptions,argument,callbackData_);
  return retval;
}

void ProgOptions::ArgsHolder::takeArgv(const ProgOptions& x)
{
 // Free memory first:
 while(argc) delete[] argv[--argc]; // note, that we assume that delete[](NULL)
                                    // is legal.
 if (argv) delete[] argv;
 // Now take the arguments:
 argc=x.argc();
 argv=new char*[argc+1];
 for(int i=0; i<argc; ++i)
 {
   argv[i]=new char[strlen(x.argv()[i])+1];
   strcpy(argv[i],x.argv()[i]);
 }
 argv[argc]=NULL;
}

ProgOptions::ArgsHolder::~ArgsHolder()
{
 while(argc) delete[] argv[--argc]; // note, that we assume that delete[](NULL)
                                    // is legal.
 if (argv) delete[] argv;
 argv = NULL;
}


}

