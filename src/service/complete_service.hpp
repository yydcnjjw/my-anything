#pragma once

#include <core/core.hpp>

namespace my {

struct CompleteItem {
  SHARED_CLS(CompleteItem)

  virtual ~CompleteItem() = default;
  virtual void exec() = 0;
  virtual std::string const &desc() = 0;
};

using complete_list = std::vector<CompleteItem::ptr_t>;

struct CompleteBackend {
  SHARED_CLS(CompleteBackend)

  virtual ~CompleteBackend() = default;
  virtual std::string const &name() const = 0;
  virtual std::string const &complete_prefix() = 0;
  virtual void complete(std::string const &text, complete_list &list) = 0;
};

class CompleteService {
public:
  UNIQUE_CLS(CompleteService)
  using complete_backend_container_t =
      std::map<std::string, CompleteBackend::ptr_t>;
  CompleteService();

  void disptach(std::string const &text, complete_list &list);

  complete_backend_container_t const &complete_backends() const {
    return this->_complete_backends;
  }

  void add_complete_backend(CompleteBackend::ptr_t const &backend) {
    this->_complete_backends.emplace(backend->name(), backend);
  }

  void remove_complete_backend(CompleteBackend::ptr_t const &backend) {
    this->remove_complete_backend(backend->name());
  }

  void remove_complete_backend(std::string const &name) {
    this->_complete_backends.erase(name);
  }

private:
  complete_backend_container_t _complete_backends;
};

} // namespace my
