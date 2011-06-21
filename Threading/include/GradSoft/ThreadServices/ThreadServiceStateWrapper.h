

class __dll_threading ThreadServiceStateWrapper: public ThreadService
{
private:

  auto_ptr<ThreadService>   delegated_;

public:

  ThreadServiceStateWrapper(ThreadService* delegated)
    :delegated_(delegated)
  {}


  void process(Runnable* runnable)
  {
    delegated_->process(runnable);
  }

  void activate()
  {
    ThreadService::activate();
    delegated_->activate(); 
  }

  void  deactivate()
  {
    ThreadService::deactivate();
    delegated_->deactivate(); 
  }

protected:

  void  mark_deactivate()
  {
    ThreadService::mark_deactivate();
    delegated_->mark_deactivate(); 
  }

};
