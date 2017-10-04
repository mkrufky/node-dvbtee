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

template<class T, typename... Targs>
/* abstract */ class AsyncBareFactoryWorker : public AsyncWorker {
 public:
  explicit AsyncBareFactoryWorker(Callback *callback_)
      : AsyncWorker(callback_) {
    async = new uv_async_t;
    uv_async_init(
        uv_default_loop()
      , async
      , AsyncProgress_
    );
    async->data = this;
  }

  virtual ~AsyncBareFactoryWorker() {
  }

  virtual void WorkProgress() = 0;

  class ExecutionProgress {
    friend class AsyncBareFactoryWorker;
   public:
    void Signal() const {
        uv_async_send(that_->async);
    }

    void Construct(Targs... Fargs) const {
        that_->ConstructProgress_(Fargs...);
    }

   private:
    explicit ExecutionProgress(AsyncBareFactoryWorker *that) : that_(that) {}
    NAN_DISALLOW_ASSIGN_COPY_MOVE(ExecutionProgress)
    AsyncBareFactoryWorker* const that_;
  };

  virtual void Execute(const ExecutionProgress& progress) = 0;
  virtual void HandleProgressCallback(const T *data, size_t size) = 0;

  virtual void Destroy() {
      uv_close(reinterpret_cast<uv_handle_t*>(async), AsyncClose_);
  }

 private:
  void Execute() /*final override*/ {
      ExecutionProgress progress(this);
      Execute(progress);
  }

  virtual void ConstructProgress_(Targs... Fargs) = 0;

  inline static NAUV_WORK_CB(AsyncProgress_) {
    AsyncBareFactoryWorker *worker =
            static_cast<AsyncBareFactoryWorker*>(async->data);
    worker->WorkProgress();
  }

  inline static void AsyncClose_(uv_handle_t* handle) {
    AsyncBareFactoryWorker *worker =
            static_cast<AsyncBareFactoryWorker*>(handle->data);
    delete reinterpret_cast<uv_async_t*>(handle);
    delete worker;
  }

 protected:
  uv_async_t *async;
};

template<class T, typename... Targs>
/* abstract */
class AsyncFactoryWorker : public AsyncBareFactoryWorker<T, Targs...> {
 public:
  explicit AsyncFactoryWorker(Callback *callback_)
      : AsyncBareFactoryWorker<T, Targs...>(callback_) {
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
  void ConstructProgress_(Targs... Fargs) {
    T *data = new T(Fargs...);

    uv_mutex_lock(&async_lock);
    asyncdata_.push(data);
    uv_mutex_unlock(&async_lock);

    uv_async_send(this->async);
  }

  uv_mutex_t async_lock;
  std::queue<T*> asyncdata_;
};

}  // namespace Nan

}  // namespace Krufky

#endif  // SRC_ASYNC_FACTORY_WORKER_H_
