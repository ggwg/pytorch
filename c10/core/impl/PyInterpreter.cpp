#include <c10/core/SymIntArrayRef.h>
#include <c10/core/TensorImpl.h>
#include <c10/core/impl/PyInterpreter.h>

namespace c10 {
namespace impl {

struct NoopPyInterpreterVTable final : public PyInterpreterVTable {
  std::string name() const override {
    return "<unloaded interpreter>";
  }

  void decref(PyObject* pyobj, bool is_tensor) const override {} // do nothing

#define PANIC(m)              \
  TORCH_INTERNAL_ASSERT(      \
      0,                      \
      "attempted to call " #m \
      " on a Tensor with nontrivial PyObject after corresponding interpreter died")

  c10::intrusive_ptr<TensorImpl> detach(const TensorImpl* self) const override {
    PANIC(detach);
  }

  void dispatch(const c10::OperatorHandle& op, torch::jit::Stack* stack)
      const override {
    PANIC(dispatch);
  }

  bool is_contiguous(const TensorImpl* self) const override {
    PANIC(is_contiguous);
  }
  c10::Device device(const TensorImpl* self) const override {
    PANIC(device);
  }
  int64_t dim(const TensorImpl* self) const override {
    PANIC(dim);
  }
  c10::IntArrayRef strides(const TensorImpl* self) const override {
    PANIC(strides);
  }
  c10::IntArrayRef sizes(const TensorImpl* self) const override {
    PANIC(sizes);
  }
  c10::SymIntArrayRef sym_sizes(const TensorImpl* self) const override {
    PANIC(sym_sizes);
  }
  c10::Layout layout(const TensorImpl* self) const override {
    PANIC(layout);
  }
  c10::SymInt sym_numel(const TensorImpl* self) const override {
    PANIC(sym_numel);
  }
  c10::SymIntArrayRef sym_strides(const TensorImpl* self) const override {
    PANIC(sym_strides);
  }

  // Just swallow the event, don't do anything
  void trace_gpu_event_creation(uintptr_t event) const override {}
  void trace_gpu_event_deletion(uintptr_t event) const override {}
  void trace_gpu_event_record(uintptr_t event, uintptr_t stream)
      const override {}
  void trace_gpu_event_wait(uintptr_t event, uintptr_t stream) const override {}
  void trace_gpu_memory_allocation(uintptr_t ptr) const override {}
  void trace_gpu_memory_deallocation(uintptr_t ptr) const override {}
  void trace_gpu_stream_creation(uintptr_t stream) const override {}
};

void PyInterpreter::disarm() noexcept {
  static NoopPyInterpreterVTable noop_vtable;
  vtable_ = &noop_vtable;
}

} // namespace impl
} // namespace c10
