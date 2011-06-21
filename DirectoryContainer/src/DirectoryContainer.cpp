#ifndef __GradSoft_DirectoryContainer_h
#include <GradSoft/DirectoryContainer.h>
#endif
/**
 * DirectoryIterator.cpp
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: DirectoryContainer.cpp,v 1.8 2008-01-08 17:15:00 rssh Exp $
 **/

#include <errno.h>
#include <iostream>
//using namespace std;

namespace GradSoft
{
using namespace std;

// 1. DirectoryContainer's methods:

const DirectoryIterator 
  DirectoryContainer::past_the_end_ = DirectoryIterator();

DirectoryContainer::DirectoryContainer()
{
  path_[0]='.';
  path_[1]=0;
}

DirectoryContainer::DirectoryContainer(const char* path)
{
  strncpy(path_,path,DirectoryEntry::max_path);
}

void DirectoryContainer::setPath(const char* path)
{
  strncpy(path_,path,DirectoryEntry::max_path);
}

DirectoryContainer::DirectoryContainer(const DirectoryContainer& x)
{
  strcpy(path_,x.path_);
}

DirectoryContainer& DirectoryContainer::operator=(const DirectoryContainer& x)
{
  strcpy(path_,x.path_);
  return *this;
}

DirectoryIterator DirectoryContainer::begin() const
{
  return DirectoryIterator(path_);
}

const DirectoryIterator& DirectoryContainer::end() 
{
  return past_the_end_;
}

const DirectoryIterator& DirectoryIterator::end() 
{
  return DirectoryContainer::end();
}

bool DirectoryContainer::shiftToNonTrivial(DirectoryIterator& it) const
{
  // temporary stopping:
  if (it==end()) return false;
#ifdef WIN32
  if (strcmp(it->name(),".")!=0) return true; 
#endif
  return ++(++it)!=end();
}

DirectoryContainer::size_type DirectoryContainer::record_count() const
{
  DirectoryIterator it(path_);
  if (shiftToNonTrivial(it)) {
    DirectoryContainer::size_type retval;

#ifdef DISTANCE_HAVE_2_ARGS
    return distance(it,end());
#else
    distance(it,end(), retval);
    return retval;
#endif

  } else 
    return 0;
}

bool DirectoryContainer::no_records() const
{
  DirectoryIterator it(path_);
  return !shiftToNonTrivial(it);
}

DirectoryContainer::size_type DirectoryContainer::size() const
{
#ifdef DISTANCE_HAVE_2_ARGS
    return distance(begin(),end());
#else
    DirectoryContainer::size_type retval;
    distance(begin(),end(), retval);
    return retval;
#endif
}

DirectoryContainer::size_type DirectoryContainer::max_size() const
{
  return (size_type)-1; // temporary stopping
}

bool DirectoryContainer::empty() const
{
  return begin()==end();
}

void DirectoryContainer::swap(DirectoryContainer& x)
{
  DirectoryContainer temp = *this; 
  *this = x; 
  x = temp; 
}

// 2. DirectoryIterator's methods:


DirectoryIterator::DirectoryIterator():current_(){}

DirectoryIterator::DirectoryIterator(const char* path):current_(path){}

DirectoryIterator::DirectoryIterator(const DirectoryIterator& x):current_()
{
  current_=x.current_;
}

DirectoryIterator& DirectoryIterator::operator=(const DirectoryIterator& x) 
{
  if (*this!=x) current_=x.current_;
  return *this;
}

DirectoryIterator& DirectoryIterator::operator++()
{
  current_.next();
  return *this;
}

DirectoryIterator DirectoryIterator::operator++(int)
{
  DirectoryIterator it=*this;
  current_.next();
  return it;
}

DirectoryEntry& DirectoryIterator::operator*()  
{  return current_; }

const DirectoryEntry& DirectoryIterator::operator*() const 
{  return current_; }

DirectoryEntry* DirectoryIterator::operator->() 
{  return &current_; }

const DirectoryEntry* DirectoryIterator::operator->() const 
{  return &current_; }

bool DirectoryIterator::operator==(const DirectoryIterator& x)
{  return current_==x.current_; }

bool DirectoryIterator::operator!=(const DirectoryIterator& x)
{  
  return !(current_==x.current_); 
}


// 3. DirectoryEntry's methods:


DirectoryEntry::DirectoryEntry():opend_(false){}

DirectoryEntry::DirectoryEntry(const char *path):opend_(false)
{
  char temp[max_path];
#ifdef WIN32
  if (GetCurrentDirectory(max_path,temp)!=0) 
  {
    if (SetCurrentDirectory(path)==TRUE) 
    {
      DWORD pathLength=GetCurrentDirectory(max_path,path_);
      if (pathLength!=0) 
      {
        if (SetCurrentDirectory(temp)==TRUE) 
        {
          if (path_[pathLength-1]!='\\') 
          {
             strcat(path_,"\\");
          }
          open_reset(path_);
          return;
        }
      }
    }
  }
  throwException();
#else
  getcwd(temp,max_path);
  
  if (chdir(path)!=0)  {
          throwException();
  }else{
          getcwd(path_,max_path);
        }

  if (chdir(temp)!=0) {
    throwException();
  }

  if (path_[strlen(path_)-1]!='/') {
    strcat(path_,"/");
  }
  open_reset(path_);
#endif
}

void DirectoryEntry::throwException() const
{
#ifdef WIN32
  DirectoryException ex;
  ex.code=GetLastError();
  char* errorMessage;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    ex.code,
    MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL),
    (LPTSTR) &errorMessage,
    0,
    NULL 
  );
  ex.message.assign(errorMessage);
  LocalFree(errorMessage);
  throw ex;
#else
  DirectoryException ex;
  ex.code=errno;
  ex.message.append(strerror(errno));
  throw ex;
#endif
}

void DirectoryEntry::open_reset(const char *path)
{
  opend_=false;
  strncpy(path_,path,max_path);
#ifdef WIN32
  char temp[max_path];
  strcpy(temp,path_);
  strcat(temp,"*");

  handle_=FindFirstFile(temp,&data_);
  if (handle_==INVALID_HANDLE_VALUE) 
  {
    throwException();
  }else
    opend_=true;
#else
  dirp_=opendir(path_);
  if (dirp_==NULL)
  {
    throwException();
  }else{
    opend_=fillCurrent();
    if (!opend_) throwException();
  }
  just_opened_dir_ = true;
#endif
}

#ifndef WIN32
bool DirectoryEntry::fillCurrent()
{
  dirent* entry_ptr; 
#ifndef HAVE_READDIR_R
  {
    MutexLocker ml(readdirMutex_);
    entry_ptr = readdir(current_.dirp_);
    if (errno) throwException;
    if (entry_ptr==NULL) {
      close(); 
      return false;
    }else{
      entry_ = *entry_ptr;
    }
  }
#else
  if (readdir_r(dirp_,&entry_,&entry_ptr)) 
  {
    throwException();
  }
  if (entry_ptr==NULL) 
  {
    close();
    return false;
  }
#endif

  //      string temp;
  //      temp +=path_;
  //      temp +=entry_.d_name;
  //      stat(temp.c_str(), &str);

  char temp[max_path];
  strcpy(temp,path_);
  strcat(temp,entry_.d_name);
  if (stat(temp,&stat_)==-1) {
     throwException();
  }
  return true;
}
#endif

void DirectoryEntry::ad_exemplum(const DirectoryEntry& pattern)
{
  close();
  if (pattern.opend_)
  {
    open_reset(pattern.path_);
#ifdef WIN32
    find_forward(pattern.data_.cFileName);
#else
    find_forward(pattern.entry_.d_name);
#endif
  }
}

DirectoryEntry::DirectoryEntry(const DirectoryEntry& x):opend_(false)
{
 ad_exemplum(x);
}

DirectoryEntry& DirectoryEntry::operator=(const DirectoryEntry& x)
{
 if (*this!=x) ad_exemplum(x);
 return *this;
}

bool DirectoryEntry::next()
{
#ifdef WIN32
 if (opend_) {
   bool found=(FindNextFile(handle_,&data_)==TRUE);
   if (!found) {
     if (GetLastError()!=ERROR_NO_MORE_FILES) {
        throwException();
     }
     close();
   }
   return found;
 }else{
   return false; 
 }
#else
   if (opend_) {
     if (just_opened_dir_) {
        just_opened_dir_ = false;
        return true;
     } 
     return fillCurrent();
   }
   else return false; 
#endif
}

bool DirectoryEntry::find_forward(const char* name)
{
 assert(name!=NULL && strlen(name)<max_path);

 while(opend_)
 {
#ifdef WIN32
  if (strcmp(data_.cFileName,name)==0) return true;
#else
  if (strcmp(entry_.d_name,name)==0) return true;
#endif
  next(); 
 }
 return false;
}

bool DirectoryEntry::find(const char* name)
{
#ifdef WIN32
  close();
  open_reset(path_);
#else
  if (opend_) rewinddir(dirp_);
#endif
  return find_forward(name);
}

void DirectoryEntry::close()
{
#ifdef WIN32
  if (opend_) 
  {
    if(FindClose(handle_)) opend_=false;
    else throwException();
  }
#else
  if (opend_)
  {
    if (!closedir(dirp_)) opend_=false;
    else throwException();
  }
#endif
}

DirectoryEntry::~DirectoryEntry()
{ 
    close(); 
}

bool DirectoryEntry::operator==(const DirectoryEntry& x) const  
{
 if (opend_) {
   if (x.opend_) {
     if (strcmp(path_,x.path_)==0) {
#ifdef WIN32
        return (strcmp(data_.cFileName,x.data_.cFileName)==0);
#else
  return (strcmp(entry_.d_name,x.entry_.d_name)==0);
#endif
     }
   } 
   return false;
 }else
   return !(x.opend_);
}

//NAME
bool DirectoryEntry::operator!=(const DirectoryEntry& x) const
{
  return !((*this)==x);
}


const char* DirectoryEntry::name() const
{
#ifdef WIN32
  if (handle_!=INVALID_HANDLE_VALUE) return data_.cFileName;
#else
  if (dirp_!=NULL) return entry_.d_name;
#endif
  return NULL;
}

bool DirectoryEntry::is_directory() const
{
#ifdef WIN32
 return (data_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;
#else
 if (!opend_) {
   throwException(); 
 }
 return just_opened_dir_ || ( (S_ISDIR( stat_.st_mode ))!=0 ); 
#endif
}

bool DirectoryEntry::is_hidden() const
{
#ifdef WIN32
  return (data_.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0;
#else
  return false;
#endif
}

int DirectoryEntry::fsize() const
{
#ifdef WIN32
  return ((data_.nFileSizeHigh * MAXDWORD) + data_.nFileSizeLow);
#else
  return stat_.st_size;
#endif
}

}
