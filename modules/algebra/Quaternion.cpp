
module;

#include <cstddef>
#include <array>

export module algebra:quaternion;

import :vector;

namespace tire {

export {
    template <typename T>
    struct quaternion_base {
            quaternion_base() = default;

            explicit quaternion_base(const quaternion_base& rhs) : data_{ rhs.data_ } {
            }

            explicit quaternion_base(const std::array<T, 4>& rhs) {
                data_[0] = rhs[0];
                data_[1] = rhs[1];
                data_[2] = rhs[2];
                data_[3] = rhs[3];
            }

            quaternion_base(T x, T y, T z, T w) {
                data_[0] = x;
                data_[1] = y;
                data_[2] = z;
                data_[3] = w;
            }

            quaternion_base(const algebra::vector3_base<T>& rhs) {
                data_[0] = rhs.data_[0];
                data_[1] = rhs.data_[1];
                data_[2] = rhs.data_[2];
                data_[3] = T{};
            }

            explicit quaternion_base(const algebra::vector4_base<T>& rhs) {
                data_[0] = rhs.data_[0];
                data_[1] = rhs.data_[1];
                data_[2] = rhs.data_[2];
                data_[3] = rhs.data_[3];
            }

            T operator[](size_t id) {
                return data_[id];
            }

            const T operator[](size_t id) const {
                return data_[id];
            }

            T x() const {
                return data_[0];
            }

            T y() const {
                return data_[1];
            }

            T z() const {
                return data_[2];
            }

            T w() const {
                return data_[3];
            }

            quaternion_base& operator=(const quaternion_base& rhs) {
                if (this != &rhs) {
                    data_[0] = rhs.data_[0];
                    data_[1] = rhs.data_[1];
                    data_[2] = rhs.data_[2];
                    data_[3] = rhs.data_[3];
                }

                return *this;
            };

            quaternion_base operator+(const quaternion_base& b) {
                return quaternion_base{
                    data_[0] + b[0], data_[1] + b[1], data_[2] + b[2], data_[3] + b[3]
                };
            }

            quaternion_base operator+=(const quaternion_base& b) {
                return this + b;
            }

            quaternion_base operator-(const quaternion_base& b) {
                return quaternion_base{
                    data_[0] - b[0], data_[1] - b[1], data_[2] - b[2], data_[3] - b[3]
                };
            }

            quaternion_base operator-=(const quaternion_base& b) {
                return this - b;
            }

            quaternion_base& operator=(quaternion_base&& rhs) = delete;

            ~quaternion_base() = default;

            /*
             * Test purpose members
             */

            [[nodiscard]] T* data() {
                return data_.data();
            };

        private:
            std::array<T, 4> data_{};
    };
}
}  // namespace toy::algebra
