#pragma once

#include <memory>
#define SELF_T(cls) using self_t = cls;
#define SHARED_PTR_T(cls)                                                      \
  SELF_T(cls)                                                                  \
  using ptr_t = std::shared_ptr<self_t>;

#define UNIQUE_PTR_T(cls)                                                      \
  SELF_T(cls)                                                                  \
  using ptr_t = std::unique_ptr<self_t>;

#define SHARED_CLS(cls)                                                        \
  SHARED_PTR_T(cls)                                                            \
  template <typename... Args> static ptr_t make(Args &&...args) {              \
    return std::make_shared<self_t>(std::forward<Args>(args)...);              \
  }

#define UNIQUE_CLS(cls)                                                        \
  UNIQUE_PTR_T(cls)                                                            \
  template <typename... Args> static ptr_t create(Args &&...args) {            \
    return std::make_unique<self_t>(std::forward<Args>(args)...);              \
  }
