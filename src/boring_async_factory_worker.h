/*********************************************************************
 * node-dvbtee for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky <https://github.com/mkrufky>
 *
 * MIT License <https://github.com/mkrufky/node-dvbtee/blob/master/LICENSE>
 *
 * See https://github.com/mkrufky/node-dvbtee for more information
 ********************************************************************/

#ifndef SRC_ASYNC_FACTORY_WORKER_H_
#define SRC_ASYNC_FACTORY_WORKER_H_

#include <nan.h>
#include <queue>

namespace Krufky {

namespace Nan {

using namespace ::Nan;

template<class T, typename A, typename B, typename C>
/* abstract */
class AsyncBareFactoryWorker : public AsyncBareProgressWorkerBase {
 public:
  explicit AsyncBareFactoryWorker(Callback *callback_)
      : AsyncBareProgressWorkerBase(callback_) {
  }

  virtual ~AsyncBareFactoryWorker() {
  }

  class ExecutionProgress {
    friend class AsyncBareFactoryWorker;
   public:
    void Construct(A a, B b, C c) const {
        that_->ConstructProgress_(a, b, c);
    }

   private:
    explicit ExecutionProgress(AsyncBareFactoryWorker *that) : that_(that) {}
    NAN_DISALLOW_ASSIGN_COPY_MOVE(ExecutionProgress)
    AsyncBareFactoryWorker* const that_;
  };

  virtual void Execute(const ExecutionProgress& progress) = 0;
  virtual void HandleProgressCallback(const T *data, size_t size) = 0;

 private:
  void Execute() /*final override*/ {
      ExecutionProgress progress(this);
      Execute(progress);
  }

  virtual void ConstructProgress_(A a, B b, C c) = 0;
};

template<class T, typename A, typename B, typename C>
/* abstract */
class AsyncFactoryWorker : public AsyncBareFactoryWorker<T, A, B, C> {
 public:
  explicit AsyncFactoryWorker(Callback *callback_)
      : AsyncBareFactoryWorker<T, A, B, C>(callback_) {
    uv_mutex_init(&async_lock);
  }

  virtual ~AsyncFactoryWorker() {
    uv_mutex_lock(&async_lock);

    while (!asyncdata_.empty()) {
      T *data = asyncdata_.front();

      asyncdata_.pop();

      delete data;
    }

    uv_mutex_unlock(&async_lock);
    uv_mutex_destroy(&async_lock);
  }

  void WorkComplete() {
    WorkProgress();
    AsyncWorker::WorkComplete();
  }

  void WorkProgress() {
    uv_mutex_lock(&async_lock);

    while (!asyncdata_.empty()) {
      T *data = asyncdata_.front();
      asyncdata_.pop();
      uv_mutex_unlock(&async_lock);

      // Don't send progress events after we've already completed.
      if (this->callback) {
          this->HandleProgressCallback(data, 1);
      }

      delete data;

      uv_mutex_lock(&async_lock);
    }

    uv_mutex_unlock(&async_lock);
  }

 private:
  void ConstructProgress_(A a, B b, C c) {
    T *data = new T(a, b, c);

    uv_mutex_lock(&async_lock);
    asyncdata_.push(data);
    uv_mutex_unlock(&async_lock);

    uv_async_send(&this->async);
  }

  uv_mutex_t async_lock;
  std::queue<T*> asyncdata_;
};

}  // namespace Nan

}  // namespace Krufky

#endif  // SRC_ASYNC_FACTORY_WORKER_H_
