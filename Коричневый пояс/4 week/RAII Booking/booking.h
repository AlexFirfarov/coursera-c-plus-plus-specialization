#pragma once

namespace RAII {

template <typename Provider>
class Booking {
public:
  Booking(Provider* provider, int booking_id)
    : provider_(provider),
      booking_id_(booking_id) {}

  Booking(const Booking&) = delete;

  Booking(Booking&& other)
    : provider_(other.provider_) {
    other.provider_ = nullptr;
  };

  Booking& operator=(const Booking&) = delete;

  Booking& operator=(Booking&& other) {
    provider_ = other.provider_;
    other.provider_ = nullptr;
    return *this;
  };

  ~Booking() {
    if (provider_) {
      --provider_->counter;
    }
  }

private:
  Provider* provider_;
  int booking_id_;
};

}