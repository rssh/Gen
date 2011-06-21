#ifndef __GradSoft_StringPtrWithLen_h
#define __GradSoft_StringPtrWithLen_h

/*
 * part of Grad-Soft C++ ToolBox.
 * (C) Grad-Soft, 2000, Kiev, Ukraine
 * http://www.gradsoft.kiev.ua
 * $Id: StringPtrWithLen.h,v 1.3 2002-01-03 11:32:26 kav Exp $
 */

#ifndef __GradSoft_ProgOptionsPostConfig_h
#include <GradSoft/ProgOptionsPostConfig.h>
#endif

#include <string.h>

namespace GradSoft {

  class StringPtrWithLen
  {
   const char*  key_;
   int  len_;
   bool useLen_;
  public:

   StringPtrWithLen(const char* str, bool useLen)
     :key_(str),
      len_(strlen(str)),
      useLen_(useLen)
   {
   }

   const char* key() const { return key_; }
   const int length() const { return len_; }

   bool operator < (const StringPtrWithLen& x) const;

  };

}

#endif
