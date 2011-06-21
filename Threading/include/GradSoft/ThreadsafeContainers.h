#ifndef __GradSoft_ThreadsafeContainers_h
#define __GradSoft_ThreadsafeContainers_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, Kiev, Ukraine, 2000-2001
 * http://www.gradsoft.com.ua/eng/
 * $Id: ThreadsafeContainers.h,v 1.14 2004-06-12 10:53:01 rin Exp $
 */

#ifndef __GradSoft_RWLock_h
#include <GradSoft/RWLock.h>
#endif

#include <map>

namespace GradSoft {

/**
 * threadsafe wrapper arround back insertion sequence.
 **/
template<class container>
class threadsafe_biseq: public RWLocked<container>
{
public:

  ///
  typedef threadsafe_biseq self_type;
  ///
  typedef ReadLocker rlocker;
  ///
  typedef WriteLocker wlocker;

  ///
  typedef container container_type;

  ///
  typedef typename container::value_type value_type;
  ///
  typedef typename container::reference reference;
  ///
  typedef typename container::const_reference const_reference;
  ///
  //typedef typename container::pointer pointer;

  ///
  typedef typename container::iterator iterator;
  ///
  typedef typename container::const_iterator const_iterator;

  ///
  typedef typename container::reverse_iterator reverse_iterator;
  ///
  typedef typename container::const_reverse_iterator const_reverse_iterator;

  ///
  typedef typename container::difference_type difference_type;
  ///
  typedef typename container::size_type size_type;

public:

  ///
  threadsafe_biseq()
   :RWLocked<container>()
  {
  }

  ///
  threadsafe_biseq(const threadsafe_biseq& x)
  {
   rlocker(x.rwlock_);
   threadsafe_biseq::v_ = x.v_;
  }

  ///
  threadsafe_biseq(iterator beg, iterator end)
  {
    insert(begin_(),beg,end);
  }

  ///
  threadsafe_biseq(size_type n)
  {
   threadsafe_biseq::v_ = container(n);
  }

  ///
  void swap(const threadsafe_biseq& x)
  {
   if (&x==this) return;
   threadsafe_biseq::write_lock(); x.write_lock();
   threadsafe_biseq::v_.swap(x.v_);
   threadsafe_biseq::write_unlock(); x.write_unlock();
  }

  ///
  void swap_(const threadsafe_biseq& x)
  {
   if (&x==this) return;
   threadsafe_biseq::v_.swap(x.v_);
  }

  ///
  bool operator<(const threadsafe_biseq& x)
  {
    if (&x==this) return false;
    rlocker rl1(threadsafe_biseq::rwlock_); rlocker rl2(x.rwlock_);
    return ( threadsafe_biseq::v_ < x.v_);
  }
  
  ///
  bool operator<=(const threadsafe_biseq& x)
  {
    if (&x==this) return true;
    rlocker rl1(threadsafe_biseq::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_biseq::v_<=x.v_);
  }

  ///
  bool operator>(const threadsafe_biseq& x)
  {
    if (&x==this) return false;
    rlocker rl1(threadsafe_biseq::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_biseq::v_>x.v_);
  }

  ///
  bool operator>=(const threadsafe_biseq& x)
  {
    if (&x==this) return true;
    rlocker rl1(threadsafe_biseq::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_biseq::v_>=x.v_);
  }

  /**
   * obtain size of container in thread safe way
   * container must be not locked.
   **/
  size_type size() const { rlocker rl(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.size(); }

  /**
   * obtain size of underlaying container in not thread-safe way
   * container must be locked before using this function. 
   **/
  size_type size_() const { return threadsafe_biseq::v_.size(); }

  ///
  size_type max_size()  const { rlocker rl(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.max_size(); }

  ///
  size_type max_size_()  const { return threadsafe_biseq::v_.max_size(); }

  ///
  bool      empty() const { rlocker rl(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.empty(); }

  ///
  bool      empty_() const { return threadsafe_biseq::v_.empty(); }

  ///
  iterator begin_()
     { return threadsafe_biseq::v_.begin(); }
  ///
  const_iterator begin_() const
     { return threadsafe_biseq::v_.begin(); }

  ///
  iterator end_()
     { return threadsafe_biseq::v_.end(); }
  ///
  const_iterator end_() const
     { return threadsafe_biseq::v_.end(); }

  ///
  reverse_iterator rbegin_()
     {  return threadsafe_biseq::v_.rbegin(); }
  ///
  const_reverse_iterator rbegin_() const
     {  return threadsafe_biseq::v_.rbegin(); }

  ///
  reverse_iterator rend_()
     {  return threadsafe_biseq::v_.rend(); }
  ///
  const_reverse_iterator rend_() const
     {  return threadsafe_biseq::v_.rend(); }

  ///
  reference front() 
    { rlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.front(); }
  ///
  reference front_() 
    { return threadsafe_biseq::v_.front(); }
  ///
  const_reference front() const
    { rlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.front(); }
  ///
  const_reference front_() const
    { return threadsafe_biseq::v_.front(); }

  ///
  reference back() 
    { rlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.back(); }
  ///
  reference back_() 
    { return threadsafe_biseq::v_.back(); }
  ///
  const_reference back() const
    { rlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.back(); }
  ///
  const_reference back_() const
    { return threadsafe_biseq::v_.back(); }

  ///
  void push_back(const value_type& v)
    { wlocker l(threadsafe_biseq::rwlock_); threadsafe_biseq::v_.push_back(v); }
  ///
  void push_back_(const value_type& v)
    { threadsafe_biseq::v_.push_back(v); }

  ///
  void pop_back(void)
    { wlocker l(threadsafe_biseq::rwlock_); threadsafe_biseq::v_.pop_back(); }
  ///
  void pop_back_(void)
    { threadsafe_biseq::v_.pop_back(); }

  ///
  iterator insert(iterator it, const value_type& v)
    { wlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.insert(it,v); }
  ///
  iterator insert_(iterator it, const value_type& v)
    { return threadsafe_biseq::v_.insert(it,v); }
  ///
  iterator insert(iterator it, size_type n, const value_type& v)
    { wlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.insert(it,n,v); }
  ///
  iterator insert_(iterator it, size_type n, const value_type& v)
    { return threadsafe_biseq::v_.insert(it,n,v); }

  ///
  iterator insert(iterator it, iterator p, iterator q)
    { wlocker l(this.rwlock); return threadsafe_biseq::v_.insert(it,p,q); }
  
  ///
  iterator insert_(iterator it, iterator p, iterator q)
    { return threadsafe_biseq::v_.insert(it,p,q); }

  ///
  iterator erase(iterator p)
    { wlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.erase(p); }
  ///
  iterator erase_(iterator p)
    { return threadsafe_biseq::v_.erase(p); }

  ///
  iterator erase(iterator p, iterator q)
    { wlocker l(threadsafe_biseq::rwlock_); return threadsafe_biseq::v_.erase(p,q); }
  ///
  iterator erase_(iterator p, iterator q)
    { return threadsafe_biseq::v_.erase(p,q); }
  
  ///
  void  clear()
    { wlocker l(threadsafe_biseq::rwlock_);  threadsafe_biseq::v_.clear(); }
  ///
  void  clear_()
    { threadsafe_biseq::v_.clear(); }

  ///
  void  resize(size_type n, const value_type* v)
    { wlocker l(threadsafe_biseq::rwlock_);  threadsafe_biseq::v_.resize(n,v); }

  ///
  void  resize_(size_type n, const value_type* v)
    { threadsafe_biseq::v_.resize(n,v); }


////

  ///
  const container& get_container_() const
  { return threadsafe_biseq::v_; }

  ///
  container& get_container_() 
  { return threadsafe_biseq::v_; }

////

};

/**
 * threadsafe wrapper for unique associative container
 **/
template<class container>
class threadsafe_uac: public RWLocked<container>
{
public:

  typedef threadsafe_uac self_type;
  typedef ReadLocker rlocker;
  typedef WriteLocker wlocker;

  ///
  typedef typename container::value_type value_type;
  ///
  typedef typename container::key_type key_type;
  ///
  typedef typename container::reference reference;
  ///
  typedef typename container::const_reference const_reference;

  ///
  typedef typename container::iterator iterator;
  ///
  typedef typename container::const_iterator const_iterator;

  ///
  typedef typename container::reverse_iterator reverse_iterator;
  ///
  typedef typename container::const_reverse_iterator const_reverse_iterator;

  ///
  typedef typename container::difference_type difference_type;
  ///
  typedef typename container::size_type size_type;

  typedef std::pair<iterator,bool> pair_iterator_bool;
  typedef std::pair<iterator,iterator> pair_iterator_iterator;
  typedef std::pair<const_iterator,const_iterator> pair_const_iterator_const_iterator;

public:

  ///
  threadsafe_uac()
   :RWLocked<container>()
  {
  }
 
  ///
  threadsafe_uac(const threadsafe_uac& x)
  {
   rlocker xl(x.rwlock_);
   threadsafe_uac::v_ = x.v_;
  }

  /**
   * lock <code> (*this) </coden> and <code> x </code>
   * and then swap them.
   **/
  void swap(const threadsafe_uac& x)
  {
   if (&x==this) return;
   threadsafe_uac::write_lock(); x.write_lock();
   threadsafe_uac::v_.swap(x.v_);
   threadsafe_uac::write_unlock(); x.write_unlock();
  }

  /**
   * swap underlying containers, when they are already locked.
   **/
  void swap_(const threadsafe_uac& x)
  {
   threadsafe_uac::v_.swap(x.v_);
  }

  ///
  bool operator<(const threadsafe_uac& x)
  {
    if (&x==this) return false;
    rlocker rl1(threadsafe_uac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_uac::v_<x.v_);
  }
  
  ///
  bool operator<=(const threadsafe_uac& x)
  {
    if (&x==this) return true;
    rlocker rl1(threadsafe_uac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_uac::v_<=x.v_);
  }

  ///
  bool operator>(const threadsafe_uac& x)
  {
    if (&x==this) return false;
    rlocker rl1(threadsafe_uac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_uac::v_>x.v_);
  }

  ///
  bool operator>=(const threadsafe_uac& x) const
  {
    if (&x==this) return true;
    rlocker rl1(threadsafe_uac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_uac::v_>=x.v_);
  }

  ///
  size_type size() const { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.size(); }
  ///
  size_type size_() const { return threadsafe_uac::v_.size(); }

  ///
  size_type max_size() const { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.max_size(); }

  ///
  size_type max_size_() const { return threadsafe_uac::v_.max_size(); }

  ///
  bool      empty() const { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.empty(); }
  ///
  bool      empty_()  const { return threadsafe_uac::v_.empty(); }


  ///
  iterator begin_()
     { return threadsafe_uac::v_.begin(); }
  ///
  const_iterator begin_() const
     { return threadsafe_uac::v_.begin(); }

  ///
  iterator end_()
     { return threadsafe_uac::v_.end(); }
  ///
  const_iterator end_() const
     { return threadsafe_uac::v_.end(); }

  ///
  reverse_iterator rbegin_()
     {  return threadsafe_uac::v_.rbegin(); }
  ///
  const_reverse_iterator rbegin_() const
     {  return threadsafe_uac::v_.rbegin(); }

  ///
  reverse_iterator rend_()
     {  return threadsafe_uac::v_.rend(); }
  ///
  const_reverse_iterator rend_() const
     {  return threadsafe_uac::v_.rend(); }

  ///
  pair_iterator_bool insert(const value_type& v)
    { wlocker l(threadsafe_uac::rwlock_); return threadsafe_uac::v_.insert(v); }

  ///
  pair_iterator_bool insert_(const value_type& v)
    { return threadsafe_uac::v_.insert(v); }

  ///
  iterator insert(iterator it, const value_type& v)
    { wlocker wl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.insert(it,v); }

  ///
  iterator insert_(iterator it, const value_type& v)
    { return threadsafe_uac::v_.insert(it,v); }


  ///
  void insert(iterator p, iterator q)
    { wlocker l(threadsafe_uac::rwlock_); return threadsafe_uac::v_.insert(p,q); }
  ///
  void insert_(iterator p, iterator q)
    { return threadsafe_uac::v_.insert(p,q); }

  ///
  size_t erase(const key_type & key)
    { wlocker l(threadsafe_uac::rwlock_); return threadsafe_uac::v_.erase(key); }
  ///
  size_t erase_(const key_type& key)
    { return threadsafe_uac::v_.erase(key); }

  ///
  void erase(iterator p)
    { wlocker l(threadsafe_uac::rwlock_); threadsafe_uac::v_.erase(p); }
  ///
  void erase_(iterator p)
    { threadsafe_uac::v_.erase(p); }

  ///
  void erase(iterator p, iterator q)
    { wlocker l(threadsafe_uac::rwlock_); return threadsafe_uac::v_.erase(p,q); }
  ///
  void erase_(iterator p, iterator q)
    { return threadsafe_uac::v_.erase(p,q); }
  
  ///
  void  clear()
    { wlocker l(threadsafe_uac::rwlock_);  threadsafe_uac::v_.clear(); }
  ///
  void  clear_()
    { threadsafe_uac::v_.clear(); }

  ///
  iterator find(const key_type& k)
    { rlocker l(threadsafe_uac::rwlock_); return threadsafe_uac::v_.find(k); }
  ///
  iterator find_(const key_type& k)
    { return threadsafe_uac::v_.find(k); }
  ///
  const_iterator find(const key_type& k) const
    { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.find(k); }
  ///
  const_iterator find_(const key_type& k) const
    { return threadsafe_uac::v_.find(k); }

  ///
  size_type  count(const key_type& k) const
    { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.count(k); } 
  ///
  size_type  count_(const key_type& k) const
    { return threadsafe_uac::v_.count(k); } 

  ///
  pair_iterator_iterator equal_range(const key_type& k)
    { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.equal_range(k); }
  ///
  pair_const_iterator_const_iterator equal_range(const key_type& k) const
    { rlocker rl(threadsafe_uac::rwlock_); return threadsafe_uac::v_.equal_range(k); }
  ///
  pair_iterator_iterator equal_range_(const key_type& k)
    { return threadsafe_uac::v_.equal_range(k); }
  ///
  pair_const_iterator_const_iterator equal_range_(const key_type& k) const
    { return threadsafe_uac::v_.equal_range(k); }

////

  ///
  const container& get_container_() const
  { return threadsafe_uac::v_; }

  ///
  container& get_container_() 
  { return threadsafe_uac::v_; }

////

};

/**
 * Threadsafe adapter for Multiple associative Container
 **/
template<class container>
class threadsafe_mac: public RWLocked<container>
{
public:

  typedef threadsafe_mac self_type;
  typedef ReadLocker rlocker;
  typedef WriteLocker wlocker;

  ///
  typedef typename container::value_type value_type;
  ///
  typedef typename container::key_type key_type;

  ///
  typedef typename container::reference reference;
  ///
  typedef typename container::const_reference const_reference;

  ///
  typedef typename container::iterator iterator;
  ///
  typedef typename container::const_iterator const_iterator;

  ///
  typedef typename container::reverse_iterator reverse_iterator;
  ///
  typedef typename container::const_reverse_iterator const_reverse_iterator;

  ///
  typedef typename container::difference_type difference_type;
  ///
  typedef typename container::size_type size_type;

  typedef std::pair<iterator,iterator> pair_iterator_iterator;
  typedef std::pair<const_iterator,const_iterator> pair_const_iterator_const_iterator;

public:

  ///
  threadsafe_mac()
   :RWLocked<container>()
  {
  }
 
  ///
  threadsafe_mac(const threadsafe_mac& x)
   :RWLocked<container>(x)
  {
  }

  ///
  void swap(const threadsafe_mac& x)
  {
   if (&x==this)return;
   threadsafe_mac::write_lock(); x.write_lock();
   threadsafe_mac::v_.swap(x.v_);
   threadsafe_mac::write_unlock(); x.write_unlock();
  }

  ///
  void swap_(const threadsafe_mac& x)
  {
   if (&x==this)return;
   threadsafe_mac::v_.swap(x.v_);
  }

  ///
  bool operator<(const threadsafe_mac& x)
  {
    if (&x==this)return false;
    rlocker rl1(threadsafe_mac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_mac::v_<x.v_);
  }
  
  ///
  bool operator<=(const threadsafe_mac& x)
  {
    if (&x==this)return true;
    rlocker rl1(threadsafe_mac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_mac::v_<=x.v_);
  }

  ///
  bool operator>(const threadsafe_mac& x)
  {
    if (&x==this)return false;
    rlocker rl1(threadsafe_mac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_mac::v_>x.v_);
  }

  ///
  bool operator>=(const threadsafe_mac& x)
  {
    if (&x==this)return true;
    rlocker rl1(threadsafe_mac::rwlock_); rlocker rl2(x.rwlock_);
    return (threadsafe_mac::v_>=x.v_);
  }

  ///
  size_type size() const { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.size(); }

  ///
  size_type size_() const { return threadsafe_mac::v_.size(); }

  ///
  size_type max_size() const { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.max_size(); }
  ///
  size_type max_size_() const { return threadsafe_mac::v_.max_size(); }

  ///
  bool      empty()  const { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.empty(); }
  ///
  bool      empty_()  const { return threadsafe_mac::v_.empty(); }

  ///
  iterator begin_()
     { return threadsafe_mac::v_.begin(); }
  ///
  const_iterator begin_() const
     { return threadsafe_mac::v_.begin(); }

  ///
  iterator end_()
     { return threadsafe_mac::v_.end(); }
  ///
  const_iterator end_() const
     { return threadsafe_mac::v_.end(); }

  ///
  reverse_iterator rbegin_()
     {  return threadsafe_mac::v_.rbegin(); }
  ///
  const_reverse_iterator rbegin_() const
     {  return threadsafe_mac::v_.rbegin(); }

  ///
  reverse_iterator rend_()
     {  return threadsafe_mac::v_.rend(); }
  ///
  const_reverse_iterator rend_() const
     {  return threadsafe_mac::v_.rend(); }

  ///
  iterator insert(const value_type& v)
    { wlocker l(threadsafe_mac::rwlock_); return threadsafe_mac::v_.insert(v); }
  ///
  iterator insert_(const value_type& v)
    { return threadsafe_mac::v_.insert(v); }

  ///
  void insert(iterator p, iterator q)
    { wlocker l(threadsafe_mac::rwlock_); threadsafe_mac::v_.insert(p,q); }
  ///
  void insert_(iterator p, iterator q)
    { threadsafe_mac::v_.insert(p,q); }

  ///
  size_t erase(const key_type & key)
    { wlocker l(threadsafe_mac::rwlock_); return threadsafe_mac::v_.erase(key); }
  ///
  size_t erase_(const key_type& key)
    { return threadsafe_mac::v_.erase(key); }

  ///
  void erase(iterator p)
    { wlocker l(threadsafe_mac::rwlock_); threadsafe_mac::v_.erase(p); }
  ///
  void erase_(iterator p)
    { threadsafe_mac::v_.erase(p); }

  ///
  void erase(iterator p, iterator q)
    { wlocker l(threadsafe_mac::rwlock_); return threadsafe_mac::v_.erase(p,q); }
  ///
  void erase_(iterator p, iterator q)
    { return threadsafe_mac::v_.erase(p,q); }
  
  ///
  void  clear()
    { wlocker l(threadsafe_mac::rwlock_);  threadsafe_mac::v_.clear(); }
  ///
  void  clear_()
    { threadsafe_mac::v_.clear(); }

  ///
  iterator find(const key_type& k)
    { rlocker l(threadsafe_mac::rwlock_); return threadsafe_mac::v_.find(k); }
  ///
  iterator find_(const key_type& k)
    { return threadsafe_mac::v_.find(k); }
  ///
  const_iterator find(const key_type& k) const
    { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.find(); }
  ///
  const_iterator find_(const key_type& k) const
    { return threadsafe_mac::v_.find(); }

  ///
  size_type  count(const key_type& k) const
    { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.count(k); } 
  ///
  size_type  count_(const key_type& k) const
    { return threadsafe_mac::v_.count(k); } 

  ///
  pair_iterator_iterator equal_range(const key_type& k)
    { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.equal_range(k); }
  ///
  pair_const_iterator_const_iterator equal_range(const key_type& k) const
    { rlocker rl(threadsafe_mac::rwlock_); return threadsafe_mac::v_.equal_range(k); }
  ///
  pair_iterator_iterator equal_range_(const key_type& k)
    { return threadsafe_mac::v_.equal_range(k); }
  ///
  pair_const_iterator_const_iterator equal_range_(const key_type& k) const
    { return threadsafe_mac::v_.equal_range(k); }

////

  ///
  const container& get_container_() const
  { return threadsafe_mac::v_; }

  ///
  container& get_container_() 
  { return threadsafe_mac::v_; }

////

};

}//namespace GradSoft

#endif
