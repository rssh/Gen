#ifndef __GradSoft_ProgOptions_h
#define __GradSoft_ProgOptions_h

/*
 * Part of GradSoft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine.
 * 1999 - 2000
 * http://www.gradsoft.kiev.ua
 * $Id: ProgOptions.h,v 1.14 2003-07-11 20:31:43 rssh Exp $
 */


#ifndef __GradSoft_ProgOptionsPostConfig_h
#include <GradSoft/ProgOptionsPostConfig.h>
#endif

#ifndef __GradSoft_StringPtrWithLen_h
#include <GradSoft/StringPtrWithLen.h>
#endif

#include <iostream>
#include <map>
#include <set>
#include <deque>

/**
 * 
 **/
namespace GradSoft {

 ///

 /**
  * class for parsing argument line
  *
  * 
  **/
 class ProgOptions
 {
  public:
    
    /**
     * constructor of ProgOptions.
     *
     *@invariant
     * caller is responsible for memory allocation for optPrefix and pkgPrefix,
     * i. e. this parameters must be present in memory and be unchanged during
     * lifetime of ProgOptions (string constants are ok).
     *
     *@param optPrefix - option prefix. all options must begin with optPrefix
     *@param pkgPrefix - package prefix. (default -e empty) 
     * ( i. e. option have format <optPrefix><pkgPrefix>XXX )
     *@param allowUnknowOptions - are we exit, when we meet unknown option
     * during parse ?
     **/

    ProgOptions(const char* optPrefix="--",  
                const char* pkgPrefix="", 
                bool allowUnknownOptions=false);

    ///
    virtual ~ProgOptions();

    /**
     * put option to set of parsed options.
     * (i. e. after call of put("OptionName", ... ) parse will understood 
     * construction <optPrefix><pkgPrefix><OptionName>.
     * @param option - option name.
     * @param description - description of option
     * @param haveArgument - is this option have argument ?
     * @param callback - user function, which will be called during parsing
     * of command line, if such option belong to one.
     * @param callbackData - additional data, which will be passed to callback
     * function.
     **/
    void put(const char* option, const char* description,
             bool haveArgument = false,
             bool (*callback)(const ProgOptions*,const char*, void*)=NULL,
             void* callbackData =NULL );
    
    /**
     * The main work of ProfOptions is here. 
     * The function do next:
     *  check: if argv[i] have form <optPrefix><option>,
     *  if yes, and appropriative option have parameter: read parameter,
     *  (which can be in next input word, or in current word after = [i. e.
     *  --x=Y and --x Y are identical]), than call callback with argument,
     * if it was set in put call; than initialize data structure.
     *  if option have form <optPrefix><pkgPrefix>help, than output
     *  help message.
     *@precondition
     * all parsed options must be setted before call of parse.
     *@postcondition
     * is_set(), argument(), argc() and argv() methods being functionally.
     *@param argc, argv -- command line.
     *@return true, if options was parsed successfully, and it was not option 
     * <optPrefix><pkgPrefix>help, otherwise false
     **/
    bool parse(int argc, char** argv);

    /**
     * This method forms argument vector from options set in given file,
     * and parses it like parse(int,char**).
     * 
     *@precondition 
     * The file to parse must be formatted (turn to Programming Guide for details).
     * If the format is not observed,
     * the method ceases from the work, prints message, and returns the 'false'
     * 
     *@see parse(int,char**) 
     *@param fname : the name of file for parsing
     *@return 
     *  true, if operation was successfull
     *  false otherwise
     **/
    bool parseFile(const char* fname, const char* executable="unspecified");

    /**
     * forms argument vector from the string transmitted like to 
     * parseFile(const char*) and parses it like to parse(int,char**)
     * 
     *@see parse(int,char**) 
     *@param str : the string for parsing
     *@return 
     *  true, if operation was successfull
     *  false otherwise
     **/
    bool parseString(const char* str, const char* executable="unspecified");

    /**
     * save option set to file.
     *@precondigion
     * must be called afer parse
     *@param 
     *   fname is a reference to string with name of file
     *@return
     *  true, if operation was successfull
     *  false otherwise
     **/
    bool saveToFile(const char* fname);

    /**
     *@return true, if options was set in parse arguments
     **/
    bool is_set(const char* option)   const;

    /**
     *@return argument of options, if option have argument and it was set
     * in parse argument, otherwise - NULL
     **/
    const char* argument(const char* option) const;

    ///
    int argc() const   
                     { return argc_; }

    ///
    char** argv() const
                     { return argv_; }
                 		
    /**
     *set additional help message, which is printed in outHelp call.
     *(actually 2 messages: one on top of option description, second
     * - on bottom)
     *@param onTop - where to place message ? 
     *@param help  - message to output.
     **/
    void setAdditionalHelp(bool onTop, const char* help);

    /**
     * out help message to stream os.
     */
    void outHelp(std::ostream& os) const;

    /**
     * set property "allow unknown options".
     * if it is set to true, than unknow options are ignored by parse,
     * otherwise it's caused Parser to report error.
     *@param allow - value of property to set.
     **/
    void  allowUnknownOptions(bool allow=true)
     { allowUnknownOptions_ = allow; }

    /**
     * report value of "allow unknown options" property
     */ 
    bool  isUnknownOptionsAllowed() const
     { return allowUnknownOptions_; }

    
    /**
     * put additional arguments, which must be parsed with readed options.
     **/
    void  putArgs(const char* executable, int argc, char** argv);
     

    /**
     * class to hold a copy of argument vector given
     *
     **/
//    friend class ArgsHolder;
    class ArgsHolder
    {
     ArgsHolder(const ArgsHolder& x){}
     ArgsHolder& operator=(const ArgsHolder& x);
    public:
     int argc;
     char** argv;
     ArgsHolder(): argc(0),argv(NULL){}
     ~ArgsHolder();
     void takeArgv(const ProgOptions&);
    };

  private:

    /**
     * makes a new internal buffer enougth to put all existent and 
     * some additional elements, fill new buffer by existent elements and 
     * destructed previous one 
     *
     * @param argc - a number of additional elements 
     * @param executable - name of executable to put in argv_[0] 
     *                     if one is absent. 
     * @return 1, if buffer mentioned has been previously existent 
     *         0 otherwise
     */
    int enlargeArgv(int argc,const char* executable);

    /**
     * eliminates argv_[i] from argv_ with destruction
     */
    void removeIndicated(int i);

    /**
     * eliminates argv_[i] from argv_ without destruction
     */
    void removeWithoutKill(int i);

    void saveArgv(int argc, char** argv);
    void destroyArgv();

    struct str_less: public std::binary_function<char*,char*, bool>
    {
      bool operator()(const char* x, const char* y) const
      { return strcmp(x,y)<0; }
    };
    typedef std::set<char*,str_less> FnamesSet;

    bool parse1(FnamesSet& prevLoaded);
    bool loadFromFile1(const char* fname, const char* executable);

    void prepareMsg(char* msg, char*prefix, char* option, bool haveArg) const;

	class WordSequence; 
	friend class WordSequence;
	class WordSequence 
	{
	private:
		bool inWord_,isGroup_,comments_;
		char *words_,*word_,*currentSymbol_; 
		std::deque<char*> argDeque_;
		ProgOptions *owner_;
	public:
    
		WordSequence(ProgOptions *owner)
			:inWord_(false),isGroup_(false),comments_(false),
			 words_(NULL),word_(NULL),currentSymbol_(NULL),
			 argDeque_(),
			 owner_(owner)
		{}
                ~WordSequence();

		void commonSymbol(char);
		void separator();
		void parseString(const char*);
		bool checkFirstWord();
		void completeArgv(const char* executable);
		int sequenceLength();
	};

    class Element 
    {
      bool have_argument_;   
      const char* description_;
      bool (*callback_)(const ProgOptions*,const char*,void*);
      void* callbackData_;
      bool  is_set_; 
      const char* argument_; 
     public:
      Element(bool have_argument,
              const char* description,
              bool (*callback)(const ProgOptions*,const char*,void*),
              void* callbackData
      );

      Element(const Element& x);

      Element();
      ~Element();

      bool have_argument() const 
           { return have_argument_; }
      const char* description() const 
          { return description_; }

      bool is_set() const 
          { return is_set_; }
      const char* argument() const 
          { return argument_; }

      bool handle(const ProgOptions* progOptions, const char* argument) ;

    };

  typedef std::map< StringPtrWithLen, Element > StorageType;
  typedef StorageType::iterator  IteratorType;
  typedef StorageType::const_iterator  ConstIteratorType;

  StorageType options_;
  const char* optPrefix_;
  int         optPrefixLen_;
  const char* pkgPrefix_;
  int         pkgPrefixLen_;
  bool  allowUnknownOptions_;
  const char* topAdditionalHelp_;
  const char* botAdditionalHelp_;
  int argc_;
  char** argv_;
  int indexToArgv_;
  int maxOptionLength_;
  static const char* szUNKNOWN;

  ConstIteratorType  find(const char* opt, bool isLen) const
  {
   StringPtrWithLen key(opt,isLen);
   return options_.find(key);
  }
  
   // 1. if allowUnknownOptions_ is false, output error message to cerr
   // 2. return allowUnknownOptions_
  bool  unknownOption(const char* option);
  
  ProgOptions(const ProgOptions&);
  ProgOptions& operator=(const ProgOptions&);

 };

}

#endif
