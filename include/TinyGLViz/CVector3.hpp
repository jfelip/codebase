#ifndef _CODEBASE_CVECTOR3_H_
#define _CODEBASE_CVECTOR3_H_

#include <ostream>
#include <cmath>

namespace TinyGLViz {

    namespace vec3 {

        template<class T_Real=double>
        class Vector3 {
        public:

            Vector3() : m_data{0.0, 0.0, 0.0} {}

            ~Vector3() = default;

            Vector3(T_Real x, T_Real y, T_Real z) : m_data{x, y, z} {}

            Vector3(const Vector3 &right) : m_data{right(0), right(1), right(2)} {}

            Vector3(Vector3 &&right) noexcept : m_data{std::move(right[0]), std::move(right[1]), std::move(right[2])} {}

            T_Real x() const { return m_data[0]; }

            T_Real y() const { return m_data[1]; }

            T_Real z() const { return m_data[2]; }

            T_Real &operator[](unsigned int i) { return m_data[i]; }

            T_Real operator()(unsigned int i) const { return m_data[i]; }

            bool operator==(const Vector3 &rhs) const {
                return m_data[0] == rhs.m_data[0] &&
                        m_data[1] == rhs.m_data[1] &&
                        m_data[2] == rhs.m_data[2];
            }

            bool operator!=(const Vector3 &rhs) const {
                return !(rhs == *this);
            }

            Vector3 &operator=(const Vector3 &right) {
                m_data[0] = right(0);
                m_data[1] = right(1);
                m_data[2] = right(2);
                return *this;
            }

            Vector3 &operator=(Vector3 &&right) noexcept {
                m_data[0] = std::move(right[0]);
                m_data[1] = std::move(right[1]);
                m_data[2] = std::move(right[2]);
                return *this;
            }

            Vector3 &operator=(const T_Real &right) {
                m_data[0] = right;
                m_data[1] = right;
                m_data[2] = right;
                return *this;
            }


            Vector3 &operator*=(const Vector3 &right) {
                m_data[0] = m_data[0] * right(0);
                m_data[1] = m_data[1] * right(1);
                m_data[2] = m_data[2] * right(2);
                return *this;
            }

            Vector3 &operator*=(const T_Real &right) {
                m_data[0] = m_data[0] * right;
                m_data[1] = m_data[1] * right;
                m_data[2] = m_data[2] * right;
                return *this;
            }

            Vector3 &operator/=(const Vector3 &right) {
                m_data[0] = m_data[0] / right(0);
                m_data[1] = m_data[1] / right(1);
                m_data[2] = m_data[2] / right(2);
                return *this;
            }

            Vector3 &operator/=(const T_Real &right) {
                m_data[0] = m_data[0] / right;
                m_data[1] = m_data[1] / right;
                m_data[2] = m_data[2] / right;
                return *this;
            }

            Vector3 &operator+=(const Vector3 &right) {
                m_data[0] = m_data[0] + right(0);
                m_data[1] = m_data[1] + right(1);
                m_data[2] = m_data[2] + right(2);
                return *this;
            }

            Vector3 &operator+=(const T_Real &right) {
                m_data[0] = m_data[0] + right;
                m_data[1] = m_data[1] + right;
                m_data[2] = m_data[2] + right;
                return *this;
            }


            Vector3 &operator-=(const Vector3 &right) {
                m_data[0] = m_data[0] - right(0);
                m_data[1] = m_data[1] - right(1);
                m_data[2] = m_data[2] - right(2);
                return *this;
            }

            Vector3 &operator-=(const T_Real &right) {
                m_data[0] = m_data[0] - right;
                m_data[1] = m_data[1] - right;
                m_data[2] = m_data[2] - right;
                return *this;
            }


            T_Real norm() {
                return std::sqrt(squared_norm());
            }

            T_Real squared_norm() {
                return (m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2]);
            }

            void normalize() {
                T_Real n = norm();
                m_data[0] /= n;
                m_data[1] /= n;
                m_data[2] /= n;
            }

        protected:
            T_Real m_data[3];
        };


        template<typename T>
        Vector3<T> operator*(const Vector3<T> &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht(0) * rht(0), lht(1) * rht(1), lht(2) * rht(2));
        }

        template<typename T>
        Vector3<T> operator*(const Vector3<T> &lht, const T &rht) {
            return Vector3<T>(lht(0) * rht, lht(1) * rht, lht(2) * rht);
        }

        template<typename T>
        Vector3<T> operator*(const T &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht * rht(0), lht * rht(1), lht * rht(2));
        }

        template<typename T>
        Vector3<T> operator/(const Vector3<T> &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht(0) / rht(0), lht(1) / rht(1), lht(2) / rht(2));
        }

        template<typename T>
        Vector3<T> operator/(const Vector3<T> &lht, const T &rht) {
            return Vector3<T>(lht(0) / rht, lht(1) / rht, lht(2) / rht);
        }

        template<typename T>
        Vector3<T> operator/(const T &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht / rht(0), lht / rht(1), lht / rht(2));
        }

        template<typename T>
        Vector3<T> operator+(const Vector3<T> &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht(0) + rht(0), lht(1) + rht(1), lht(2) + rht(2));
        }

        template<typename T>
        Vector3<T> operator+(const Vector3<T> &lht, const T &rht) {
            return Vector3<T>(lht(0) + rht, lht(1) + rht, lht(2) + rht);
        }

        template<typename T>
        Vector3<T> operator+(const T &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht + rht(0), lht + rht(1), lht + rht(2));
        }

        template<typename T>
        Vector3<T> operator-(const Vector3<T> &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht(0) - rht(0), lht(1) - rht(1), lht(2) - rht(2));
        }

        template<typename T>
        Vector3<T> operator-(const Vector3<T> &lht, const T &rht) {
            return Vector3<T>(lht(0) - rht, lht(1) - rht, lht(2) - rht);
        }

        template<typename T>
        Vector3<T> operator-(const T &lht, const Vector3<T> &rht) {
            return Vector3<T>(lht - rht(0), lht - rht(1), lht - rht(2));
        }

        template<typename T>
        std::ostream &operator<<(std::ostream &os, const Vector3<T> &t) {
            return (os << t(0) << " " << t(1) << " " << t(2));
        }

        template<typename T>
        T dot(const Vector3<T> &left, const Vector3<T> &right) {
            return (left[0] * right(0) + left[1] * right(1) + left[2] * right(2));
        }

        template<typename T>
        void cross(const Vector3<T> &left, const Vector3<T> &right, Vector3<T> &result) {
            result[0] = left(1) * right(2) - left(2) * right(1);
            result[1] = left(2) * right(0) - left(0) * right(2);
            result[2] = left(0) * right(1) - left(1) * right(0);
        }


    }
}
#endif
