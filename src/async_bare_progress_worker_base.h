/*********************************************************************
 * node-dvbtee for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky <https://github.com/mkrufky>
 *
 * MIT License <https://github.com/mkrufky/node-dvbtee/blob/master/LICENSE>
 *
 * See https://github.com/mkrufky/node-dvbtee for more information
 ********************************************************************/

#ifndef SRC_ASYNC_BARE_PROGRESS_WORKER_BASE_H_
#define SRC_ASYNC_BARE_PROGRESS_WORKER_BASE_H_

#include <nan.h>

namespace Krufky {

namespace Nan {

using namespace ::Nan;

/* abstract */ class AsyncBareProgressWorkerBase : public AsyncWorker {
 public:
  explicit AsyncBareProgressWorkerBase(Callback *callback_)
      : AsyncWorker(callback_) {
    uv_async_init(
        uv_default_loop()
      , &async
      , AsyncProgress_
    );
    async.data = this;
  }

  virtual ~AsyncBareProgressWorkerBase() {
  }

  virtual void WorkProgress() = 0;

  virtual void Destroy() {
      uv_close(reinterpret_cast<uv_handle_t*>(&async), AsyncClose_);
  }

 private:
  inline static NAUV_WORK_CB(AsyncProgress_) {
    AsyncBareProgressWorkerBase *worker =
            static_cast<AsyncBareProgressWorkerBase*>(async->data);
    worker->WorkProgress();
  }

  inline static void AsyncClose_(uv_handle_t* handle) {
    AsyncBareProgressWorkerBase *worker =
            static_cast<AsyncBareProgressWorkerBase*>(handle->data);
    delete worker;
  }

 protected:
  uv_async_t async;
};

}  // namespace Nan

}  // namespace Krufky

#endif  // SRC_ASYNC_BARE_PROGRESS_WORKER_BASE_H_
