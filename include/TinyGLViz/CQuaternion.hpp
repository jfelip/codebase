#ifndef CQUATERNION_H
#define CQUATERNION_H

#include <memory>
#include <cmath>

namespace TinyGLViz {


    template<class T_real=double>
    class CQuaternion {

    public:

        typedef std::shared_ptr<CQuaternion> Ptr;

        typedef std::shared_ptr<CQuaternion> ConstPtr;

        CQuaternion() = default;

        CQuaternion(const T_real *v) { for (uint i = 0; i < 4; ++i) m_data[i] = v[i]; }


        CQuaternion(const CQuaternion<T_real> &rhs) { *this = rhs; }

        CQuaternion(const T_real &x, const T_real &y, const T_real &z, const T_real &w) {
            m_data[0] = x;
            m_data[1] = y;
            m_data[2] = z;
            m_data[3] = w;
        }

        ~CQuaternion() = default;

        void setIdentity();

        void setZero();

        T_real &operator[](const uint &idx) { return m_data[idx]; }

        const T_real &operator[](const uint &idx) const { return m_data[idx]; }

        CQuaternion<T_real> &operator=(const CQuaternion<T_real> &rhs);

        CQuaternion<T_real> operator+(const CQuaternion<T_real> &rhs) const;

        CQuaternion<T_real> operator-(const CQuaternion<T_real> &rhs) const;

        CQuaternion<T_real> operator*(const CQuaternion<T_real> &rhs) const;

        CQuaternion<T_real> operator*(const T_real &rhs) const;

        CQuaternion<T_real> operator/(const CQuaternion<T_real> &rhs) const;

        CQuaternion<T_real> operator/(const T_real &rhs) const;

        void conjugate();

        CQuaternion<T_real> getConjugate() const;

        void inverse();

        CQuaternion<T_real> getInverse() const;

        void normalize();

        T_real norm() const;

        T_real sqNorm() const;

        T_real& x() { return m_data[0]; }
        T_real& y() { return m_data[1]; }
        T_real& z() { return m_data[2]; }
        T_real& w() { return m_data[3]; }

        std::string toString() const;

        template<typename T>
        friend std::ostream &operator<<(std::ostream &os, const CQuaternion<T> &rhs);

        void toRPY( T_real& roll, T_real& pitch, T_real& yaw)
        {
            // roll (x-axis rotation)
            double sinr = +2.0 * (w() * x() + y() * z());
            double cosr = +1.0 - 2.0 * (x() * x() + y() * y());
            roll = atan2(sinr, cosr);

            // pitch (y-axis rotation)
            double sinp = +2.0 * (w() * y() - z() * x());
            if (fabs(sinp) >= 1)
                pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
            else
                pitch = asin(sinp);

            // yaw (z-axis rotation)
            double siny = +2.0 * (w() * z() + x() * y());
            double cosy = +1.0 - 2.0 * (y() * y() + z() * z());
            yaw = atan2(siny, cosy);
        }

    public:
        T_real m_data[4];

    };

	template<class T_real>
	void CQuaternion<T_real>::setIdentity() {
		for (uint i = 0; i < 3; ++i) {
			m_data[i] = T_real(0.0);
		}
		m_data[3] = T_real(1.0);
	}

	template<class T_real>
	void CQuaternion<T_real>::setZero() {
		for (uint i = 0; i < 4; ++i) {
			m_data[i] = T_real(0.0);
		}
	}


	template<class T_real>
	CQuaternion<T_real> &CQuaternion<T_real>::operator=(const CQuaternion<T_real> &rhs) {
		for (uint i = 0; i < 4; ++i) {
			m_data[i] = rhs[i];
		}
		return *this;
	}

	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::operator+(const CQuaternion<T_real> &rhs) const {
		CQuaternion<T_real> res;
		for (uint i = 0; i < 4; ++i) {
			res[i] = (*this)[i] + rhs[i];
		}
		return res;
	}

	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::operator-(const CQuaternion<T_real> &rhs) const {
		CQuaternion<T_real> res;
		for (uint i = 0; i < 4; ++i) {
			res[i] = (*this)[i] - rhs[i];
		}
		return res;
	}

	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::operator*(const CQuaternion<T_real> &rhs) const {
		CQuaternion<T_real> res;
		res[0] = m_data[0] * rhs[3] + m_data[1] * rhs[2] - m_data[2] * rhs[1] + m_data[3] * rhs[0];
		res[1] = -m_data[0] * rhs[2] + m_data[1] * rhs[3] + m_data[2] * rhs[0] + m_data[3] * rhs[1];
		res[2] = m_data[0] * rhs[1] - m_data[1] * rhs[0] + m_data[2] * rhs[3] + m_data[3] * rhs[2];
		res[3] = -m_data[0] * rhs[0] - m_data[1] * rhs[1] - m_data[2] * rhs[2] + m_data[3] * rhs[3];

		return res;
	}

	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::operator*(const T_real &rhs) const {
		CQuaternion<T_real> res;
		for (uint i = 0; i < 4; ++i) {
			res[i] = m_data[i] * rhs;
		}
		return res;
	}

	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::operator/(const T_real &rhs) const {
		CQuaternion<T_real> res;

		for (uint i = 0; i < 4; ++i) {
			res[i] = m_data[i] / rhs;
		}

		return res;
	}

	template<class T_real>
	void CQuaternion<T_real>::conjugate() {
		for (uint i = 0; i < 3; ++i) {
			m_data[i] = -m_data[i];
		}
	}


	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::getConjugate() const {
		CQuaternion<T_real> res;

		for (uint i = 0; i < 3; ++i) {
			res[i] = -m_data[i];
		}
		res[3] = m_data[3];

		return res;
	}

	template<class T_real>
	void CQuaternion<T_real>::inverse() {
		T_real squaredNorm = sqNorm();

		conjugate();
		for (uint i = 0; i < 4; ++i) {
			m_data[i] /= squaredNorm;
		}
	}

	template<class T_real>
	CQuaternion<T_real> CQuaternion<T_real>::getInverse() const {
		CQuaternion<T_real> qInv;
		qInv = getConjugate() / sqNorm();
		return qInv;
	}

	template<class T_real>
	void CQuaternion<T_real>::normalize() {
		T_real n = norm();
		for (uint i = 0; i < 4; ++i) {
			m_data[i] /= n;
		}
	}

	template<class T_real>
	T_real CQuaternion<T_real>::sqNorm() const {
		return m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3];
	}

	template<class T_real>
	T_real CQuaternion<T_real>::norm() const{
		return sqrt(sqNorm());
	}


	template<class T_real>
	std::string CQuaternion<T_real>::toString() const {
		return ("[ " + std::to_string(m_data[0]) + " , " +
				+std::to_string(m_data[1]) + " , " +
				+std::to_string(m_data[2]) + " , " +
				+std::to_string(m_data[3]) + " ] ");
	}


	template<typename T>
	std::ostream &operator<<(std::ostream &os, const CQuaternion<T> &rhs) {
		os << rhs.m_data[0] << " " < rhs.m_data[1] << " " < rhs.m_data[2] << " " < rhs.m_data[3];
		return os;
	}

}


#endif
