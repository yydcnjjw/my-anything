// #pragma once

// #include <core/types.hpp>

// namespace my {

// namespace detail {

// template <class T>
// class behavior_observer : public rx::subjects::detail::multicast_observer<T> {
//   typedef behavior_observer<T> self_type;
//   typedef rx::subjects::detail::multicast_observer<T> base_type;

//   class behavior_observer_state
//       : public std::enable_shared_from_this<behavior_observer_state> {
//     mutable std::mutex lock;
//     mutable std::optional<T> _value;

//   public:
//     behavior_observer_state() {}

//     void reset(T v) const {
//       std::unique_lock<std::mutex> guard(lock);
//       _value = std::move(v);
//     }

//     T &get() {
//       std::unique_lock<std::mutex> guard(lock);
//       return _value.value();
//     }

//     bool has_value() const { return _value.has_value(); }
//   };

//   std::shared_ptr<behavior_observer_state> state;

// public:
//   behavior_observer(T f, rx::composite_subscription l)
//       : base_type(l),
//         state(std::make_shared<behavior_observer_state>(std::move(f))) {}

//   rx::subscriber<T> get_subscriber() const {
//     return make_subscriber<T>(this->get_id(), this->get_subscription(),
//                               observer<T, behavior_observer<T>>(*this))
//         .as_dynamic();
//   }

//   T &get_value() { return state->get(); }
//   bool has_value() const { return state->has_value(); }

//   template <class V> void on_next(V v) const {
//     state->reset(v);
//     base_type::on_next(std::move(v));
//   }
// };

// } // namespace detail

// template <class T> class behavior {
//   detail::behavior_observer<T> s;

// public:
//   explicit behavior(
//       T f, rx::composite_subscription cs = rx::composite_subscription())
//       : s(std::move(f), cs) {}

//   bool has_observers() const { return s.has_observers(); }

//   T &get_value() { return s.get_value(); }

//   bool has_value() { return s.has_value(); }

//   rx::subscriber<T> get_subscriber() const { return s.get_subscriber(); }

//   rx::observable<T> get_observable() const {
//     auto keepAlive = s;
//     return make_observable_dynamic<T>([keepAlive, this](rx::subscriber<T> o) {
//       if (keepAlive.get_subscription().is_subscribed()) {
//         o.on_next(get_value());
//       }
//       keepAlive.add(s.get_subscriber(), std::move(o));
//     });
//   }
// };

// template <typename T> class ObValue {
// public:
//   using value_type = T;

//   ObValue() = default;

//   template <typename... Args> void emplace(Args &&...args) {
//     this->_v.get_subscriber().on_next(value_type{std::forward<Args>(args)...});
//   }

//   auto get() const { return this->_v.get_observable(); }

//   bool has_value() const {
//     return this->_v.has_value();
//   }

//   rx::subjects::behavior<value_type> _v;
// };

// } // namespace my
