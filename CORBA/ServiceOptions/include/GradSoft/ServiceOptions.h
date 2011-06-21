#ifndef __GradSoft_ServiceOptions_h
#define __GradSoft_ServiceOptions_h

/*
 * GradSoft/ServiceOptions
 * component for standart set of CORBA service options.
 * (C) GradSoft, 2000, 2001
 * $Id: ServiceOptions.h,v 1.9 2003-03-11 14:24:29 rssh Exp $
 * writeln by: Ruslan Shevchenko, Alik Rivkind
 */

#ifndef __GradSoft_ProgOptions_h
#include <GradSoft/ProgOptions.h>
#endif

#include <string>
#include <stdio.h>

/**
 *
 **/
namespace GradSoft {

/**
 * class for standart options set for CORBA Service.
 * define options:
 *   --with-naming - bind service object via naming service.
 *   --ior-stdout  - output ior of binding objects to stdout
 * and for each object with name name define:
 *  --ior-file-<name> argument -- put ior for object <name> to file <argument>
 *
 **/
class ServiceOptions:public ProgOptions
{
public:

  ///
  ServiceOptions(const char* optPrefix="--", const char* pkgPrefix="" );

  ///
  virtual ~ServiceOptions();

  /**
   * put standart option for object with name <name>,
   * i.e:
   *  --ior-file-<name> argument -- put ior for object <name> to file <argument>
   **/
  void putServiceName(const char* name);

  /**
   * bind service object: all actual actions of service options are evaluated,
   * i.e. :
   *  1. if --with-naming is set, than bind option to naming service.
   *  2. if --ior-file-name is set, outpur iof of object to option argument
   *  3. if --ior-stdout is set, output to stdoout.
   *  4. if ORB propierty mechanizm for settiong URL-style iors is exists,
   * use it.
   **/
  bool bindServiceObject(CORBA::ORB_ptr orb,
                         CORBA::Object_ptr object, 
                         PortableServer::Servant p_servant,
                         const char* name,
                         bool rebind=false);

protected:

  static bool ior_file_callback(const ProgOptions* po, const char* argument, void* data);

  void set_ior_fname(const char* service, const char* fname)
  {
    ServiceRecordIteratorType it = serviceRecords_.find(service);
    (*it).second.set_ior_fname(fname);
  }

  const char* get_ior_fname(const char* service) const
  {
    ServiceRecordConstIteratorType it = serviceRecords_.find(service);
    if (it==serviceRecords_.end()) return NULL;
    else return (*it).second.get_ior_fname();
  }

private:

 class ServiceRecord
 {
   std::string ior_file_option_;
   std::string ior_file_description_;
   std::string ior_fname_;
   public:
     ServiceRecord();

     ServiceRecord(const ServiceRecord& sr)
      :ior_file_option_(sr.ior_file_option_),
       ior_file_description_(sr.ior_file_description_),
       ior_fname_(sr.ior_fname_)
      {}

     ServiceRecord& operator=(const ServiceRecord& sr)
     {
      ior_file_option_=sr.ior_file_option_;
      ior_file_description_=sr.ior_file_description_;
      ior_fname_=sr.ior_fname_;
      return *this;
     }

     void set_ior_file_option(const char* str)
       { ior_file_option_=str; }

     void set_ior_file_description(const char* str)
       { ior_file_description_=str; }

     void set_ior_fname(const char* str)
       { ior_fname_=str; }

     const char* get_ior_fname() const
         { return ior_fname_.c_str(); }

     const char* get_ior_file_description() const
         { return ior_file_description_.c_str(); }

     const char* get_ior_file_option() const
         { return ior_file_option_.c_str(); }


 };

 struct szless: public std::binary_function<const char*,const char*, bool>
 {
   bool operator()(const char* x, const char* y) const
   { return strcmp(x,y)<0; }
 };

 typedef std::map<const char*, ServiceRecord, szless> ServiceRecordStorageType;
 typedef ServiceRecordStorageType::iterator ServiceRecordIteratorType;
 typedef ServiceRecordStorageType::const_iterator ServiceRecordConstIteratorType;
 ServiceRecordStorageType serviceRecords_;


};

}

#endif
