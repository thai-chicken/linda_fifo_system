#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

class Semaphore
{
  private:
  std::mutex mtx_;
  std::condition_variable cond_var_;
  unsigned long count_ = 0; // number of threads waiting on the semaphore

  public:
  void release();
  void acquire();
  bool try_acquire();
};

#endif