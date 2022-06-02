#include "Semaphore.h"

void Semaphore::release()
{
  std::lock_guard<std::mutex> lock(mtx_);
  ++count_;
  cond_var_.notify_one();
}

void Semaphore::acquire()
{
  std::unique_lock<std::mutex> lock(mtx_);
  while (!count_)
  {
    cond_var_.wait(lock);
  }
  --count_;
}

bool Semaphore::try_acquire()
{
  std::lock_guard<std::mutex> lock(mtx_);
  if (count_)
  {
    --count_;
    return true;
  }
  return false;
}