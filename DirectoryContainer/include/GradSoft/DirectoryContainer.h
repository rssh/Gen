#ifndef __GradSoft_DirectoryContainer_h
#define __GradSoft_DirectoryContainer_h

/*
 * DirectoryContainer: a set of means to handle file directory
 * (C) GradSoft, 2000, 2001, 2007
 * http://www.gradsoft.com.ua
 * $Id: DirectoryContainer.h,v 1.14 2008-01-08 17:14:59 rssh Exp $
 */

#ifndef __GradSoft_DirectoryContainerPostConfig_h
#include <GradSoft/DirectoryContainerPostConfig.h>
#endif

#ifndef WIN32
#include <GradSoft/Threading.h>
#endif

#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifndef WIN32
# include <dirent.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
#else
# include <wtypes.h>
# include <winbase.h>
#endif

#include <iterator>
#include <string>

/**
 *
 **/
namespace GradSoft
{

 ///

/**
 * This is exception which will be thrown if any kind of problem occur:
 **/
struct DirectoryException
{ 
  int code;
  std::string message;
  ///
  const char* what() const { return message.c_str(); }
};

/**
 * This is "unit of issue":
 **/
class DirectoryEntry
{
  friend class DirectoryIterator;
  friend class DirectoryContainer;

private:

#ifdef MAX_PATH
  enum {max_path=MAX_PATH};
#else
#ifdef PATH_MAX
  enum {max_path=PATH_MAX};
#else
  enum {max_path=256}; // posix
#endif
#endif

#ifdef WIN32
  HANDLE handle_;
  WIN32_FIND_DATA data_;
#else
  DIR*   dirp_;
  struct dirent entry_;
  struct stat stat_; 
  bool   just_opened_dir_;
#endif
  char path_[max_path];
  bool opend_;

  DirectoryEntry();

#ifndef WIN32
  bool fillCurrent();
#endif
  void throwException() const;
  void open_reset(const char* path);
  void ad_exemplum(const DirectoryEntry& pattern);
  bool find_forward(const char* name);

public:

 /*
  * Create directory stream and position object 
  * at the first entry present
  */
  DirectoryEntry(const char* path);

        ///
  DirectoryEntry(const DirectoryEntry& x);

        ///
  ~DirectoryEntry();

        /// 
  DirectoryEntry& operator=(const DirectoryEntry& x);

        ///
  bool operator==(const DirectoryEntry& x) const;

        ///
  bool operator!=(const DirectoryEntry& x) const;

 /**
  * position object at the enrty regular;
  * return true, if the entry matched is found, 
  * and false otherwise
  **/
  bool next();

 /**
  * position object at the entry specified by name
  * return true, if the entry matched is found,
  * and false otherwise
  **/
  bool find(const char* name);

 /**
  * close directory stream 
  **/
  void close();

  /**
   * return the name of file in this directory entry, 
   * related to this directory
   **/
  const char* name() const;

  /**
   * return true if current name is directory
   **/
  bool  is_directory() const;

  /**
   *  return true if current directory entry is hidden
   **/
  bool is_hidden() const;

  /**
   *  return size of current file 
   **/
  int fsize() const;
};

/*
 * This is forward_iterator compatible class:
 */

#if defined(_MSC_VER) || defined(__SUNPRO_CC)
#define DirectoryIteratorBaseClass std::iterator<std::forward_iterator_tag,DirectoryEntry,long>
#else
#if defined(__GNUC__) && (__GNUC__ < 3 )
#define DirectoryIteratorBaseClass std::forward_iterator<DirectoryEntry,long>
#else
#define DirectoryIteratorBaseClass std::iterator<std::forward_iterator_tag,DirectoryEntry,long,DirectoryEntry*,DirectoryEntry&>
#endif
#endif


class DirectoryIterator : public DirectoryIteratorBaseClass
{
 friend class DirectoryContainer;

 DirectoryEntry current_;

#ifndef WIN32
#ifndef HAVE_READDIR_R
 static Mutex readdirMutex_;
#endif
#endif

public:


  ///
  DirectoryIterator();

  ///
  DirectoryIterator(const char* path);

  ///
  DirectoryIterator(const DirectoryIterator& );

  ///
  DirectoryIterator& operator=(const DirectoryIterator& );

  ///
  DirectoryIterator& operator++();

  ///
  DirectoryIterator operator++(int);

  /**
   * Return current DirectoryEntry
   **/
  DirectoryEntry& operator*();

  /**
   * Return const DirectoryEntry
   **/
  const DirectoryEntry& operator*() const;

  /**
   * Return pointer on DirectoryEntry
   */
  DirectoryEntry* operator->();

  /**
   * Return const pointer on DirectoryEntry
   **/
  const DirectoryEntry* operator->() const;

  /**
   * equality operation.
   **/
  bool operator==(const DirectoryIterator& x);

  /**
   * unequality operation.
   **/
  bool operator!=(const DirectoryIterator& x);

  /**
   *  end() of any directory.
   **/
  static const DirectoryIterator& end(); 

};

/**
 * This is "container":
 */
class DirectoryContainer 
{
  typedef DirectoryIterator I; 

  static const I past_the_end_; 
  char path_[DirectoryEntry::max_path];
  bool shiftToNonTrivial(DirectoryIterator& it) const; 

public:

  typedef I::value_type value_type;
  typedef I iterator; 
  typedef I const_iterator;
  typedef I::value_type& reference;
  typedef const I::value_type& const_reference;
  typedef I::value_type* pointer; 

#ifdef ITERATOR_HAVE_DISTANCE_TYPE
  typedef I::distance_type difference_type;
#else
  typedef long difference_type;
#endif

  typedef long size_type;

 /*
  * Create DirectoryContainer instance for curent disk directory 
  */
  DirectoryContainer();

 /*
  * Create DirectoryContainer instance for directory specified
  */
  DirectoryContainer(const char *path);

        ///
  DirectoryContainer(const DirectoryContainer& x);

        ///
  DirectoryContainer& operator=(const DirectoryContainer& x);

        ///
  DirectoryIterator begin() const;

        ///
  static const DirectoryIterator& end(); 

        ///
  size_type size() const; 

        ///
  size_type max_size() const;

        ///
  bool empty() const;

        ///
  void swap(DirectoryContainer& x);

 /*
  * connect DirectoryContainer with directory specified
  */
  void setPath(const char *path);

 /*
  * returns a record count in the directory connected
  * without record "." and ".." (if present)
  */
  size_type record_count() const;

 /*
  * returns true, if either record count is null 
  * or "." and ".." are present only, and false otherwise
  */
  bool no_records() const;
};

}

#endif
