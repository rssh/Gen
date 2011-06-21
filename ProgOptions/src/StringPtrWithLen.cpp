#include <GradSoft/StringPtrWithLen.h>

namespace GradSoft {

bool StringPtrWithLen::operator < (const StringPtrWithLen& x) const
{
 if (useLen_ && x.useLen_) {
   int maxLen_ = ( (len_ < x.len_ ) ? x.len_ : len_ );
   return (strncmp(key_,x.key_,maxLen_) < 0);
 }else if (useLen_ && !x.useLen_){
   int cmp = strncmp(key_,x.key_,len_);
   if (cmp==0 && x.key_[len_]!='=' && x.key_[len_]!='\0') return true;
   return (cmp < 0);
 }else if (!useLen_ && x.useLen_) {
   int cmp = strncmp(key_,x.key_,x.len_);
   if (cmp==0 && key_[x.len_]!='=' && key_[x.len_]!='\0') return true;
   return (cmp < 0);
 }else /* if (!useLen && !x.useLen) */
   return (strcmp(key_,x.key_) < 0);
}

}

