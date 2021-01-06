#pragma once

#ifndef YTC_MATRIX_HPP
#define YTC_MATRIX_HPP

#include "YtcVector.hpp"

namespace YtcGE
{
    template<typename T, int Row, int Col>
    class Matrix final
    {
        template<typename U, int Row, int Col>
        friend class Matrix;
    public:
        using RowVector_T = Vector<T, Col>;
        using ColVector_T = Vector<T, Row>;
    private:
        using InternalStruct = std::array<RowVector_T, Row>;
    public:
        using Matrix_T = Matrix<T, Row, Col>;
        using value_type = typename InternalStruct::value_type;
        using difference_type = typename InternalStruct::difference_type;
        using pointer = typename InternalStruct::pointer;
        using reference = typename InternalStruct::reference;
        using const_reference = typename InternalStruct::const_reference;
        using iterator = typename InternalStruct::iterator;
        using const_iterator = typename InternalStruct::const_iterator;
        using reverse_iterator = typename InternalStruct::reverse_iterator;
        using const_reverse_iterator = typename InternalStruct::const_reverse_iterator;

        static const Matrix_T& Zero() noexcept
        {
            static Matrix_T zero(0);
            return zero;
        }

        constexpr Matrix() noexcept = default;

        constexpr Matrix(T val) noexcept
        {
            Fill(val);
        }

        constexpr Matrix(std::initializer_list<T> initialVals) 
        {
            *this = std::move(initialVals);
        }

        constexpr Matrix(const Matrix_T& other) noexcept : data_(other.data_)
        {
        }

        template<typename U>
        constexpr Matrix(const Matrix<U, Row, Col>& other) noexcept
        {
            *this = other;
        }

        constexpr Matrix(Matrix_T&& other) noexcept : data_(std::move(other.data_))
        {
        }

        constexpr Matrix(std::initializer_list<std::initializer_list<T>> initialVals)
        {
            *this = std::move(initialVals);
        }

        Matrix_T& operator=(std::initializer_list<std::initializer_list<T>> initialVals)
        {
            std::copy(initialVals.begin(), initialVals.end(), begin());
            return *this;
        }

        Matrix_T& operator=(std::initializer_list<T> initialVals)
        {
            auto iter = initialVals.begin();
            for (int i = 0; i < Row; ++i)
            {
                for (int j = 0; j < Col && iter != initialVals.end(); ++j)
                {
                    data_[i][j] = *iter++;
                }
            }
            return *this;
        }

        Matrix_T& operator=(Matrix_T&& other) noexcept
        {
            if (this != &other)
            {
                data_ = std::move(other.data_);
            }
            return *this;
        }

        Matrix_T& operator=(const Matrix_T& other) noexcept
        {
            if (this != &other)
            {
                std::copy(other.begin(), other.end(), begin());
            }
            return *this;
        }

        template<typename U>
        Matrix_T& operator=(const Matrix<U, Row, Col>& other) noexcept
        {
            for (int i = 0; i < Row; ++i)
            {
                data_[i] = other.data_[i];
            }
            return *this;
        }

        iterator begin() noexcept
        {
            return data_.begin();
        }

        const_iterator begin() const noexcept
        {
            return data_.begin();
        }

        const_iterator cbegin() const noexcept
        {
            return begin();
        }

        iterator end() noexcept
        {
            return data_.end();
        }

        const_iterator end() const noexcept
        {
            return data_.end();
        }

        const_iterator cend() const noexcept
        {
            return end();
        }

        reverse_iterator rbegin() noexcept
        {
            return data_.rbegin();
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return data_.rbegin();
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }

        reverse_iterator rend() noexcept
        {
            return data_.rend();
        }

        const_reverse_iterator rend() const noexcept
        {
            return data_.rend();
        }

        const_reverse_iterator crend() const noexcept
        {
            return rend();
        }

        constexpr int Rows() const noexcept
        {
            return Row;
        }

        constexpr int Cols() const noexcept
        {
            return Col;
        }

        ColVector_T ColVector(int colIndex) const noexcept
        {
            ColVector_T vec;
            for (int i = 0; i < Row; ++i)
            {
                vec[i] = (*this)[i][colIndex];
            }
            return vec;
        }

        constexpr RowVector_T RowVector(int rowIndex) const noexcept
        {
            return data_[rowIndex];
        }

        RowVector_T& operator[](int index)
        {
            return data_[index];
        }

        const RowVector_T& operator[](int index) const 
        {
            return data_[index];
        }

        void Fill(T val) noexcept
        {
            std::for_each(data_.begin(), data_.end(), [=](RowVector_T& row) { row.Fill(val); });
        }

        void Swap(const Matrix_T& other) noexcept
        {
            data_.swap(other.data_);
        }

        template<typename U>
        constexpr bool NearlyEqualTo(const Matrix<U, Row, Col>& other) const noexcept
        {
            return std::equal(other.cbegin(), other.cend(), begin(), end(), [](const RowVector_T& rowL, const RowVector_T& rowR)
            {
                return rowL.NearlyEqualTo(rowR);
            });
        }

        template<typename U> Matrix_T& operator+=(const Matrix<U, Row, Col>& other) noexcept;
        template<typename U> Matrix_T& operator-=(const Matrix<U, Row, Col>& other) noexcept;
        template<typename U> Matrix_T& operator*=(U scale) noexcept;
        template<typename U> Matrix_T& operator/=(U divisor) noexcept;
    private:
        InternalStruct data_;
    };

    template<typename T, int R, int C>
    inline bool operator==(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) noexcept
    {
        return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }
    
    template<typename T, int R, int C>
    inline bool operator!=(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template<typename A, typename B, typename T, int R, int C>
    inline void Add(const Matrix<A, R, C>& lhs, const Matrix<B, R, C>& rhs, Matrix<T, R, C>& result) noexcept
    {
        for (int i = 0; i < R; ++i)
        {
            Add(lhs[i], rhs[i], result[i]);
        }
    }

    template<typename A, typename B, typename T, int R, int C>
    inline void Sub(const Matrix<A, R, C>& lhs, const Matrix<B, R, C>& rhs, Matrix<T, R, C>& result) noexcept
    {
        for (int i = 0; i < R; ++i)
        {
            Sub(lhs[i], rhs[i], result[i]);
        }
    }

    template<typename A, typename B, typename T, int R, int C>
    inline void Mult(const Matrix<A, R, C>& lhs, B scale, Matrix<T, R, C>& result) noexcept
    {
        for (int i = 0; i < R; ++i)
        {
            Mult(lhs[i], scale, result[i]);
        }
    }

    template<typename A, typename B, typename T, int R, int C>
    inline void Div(const Matrix<A, R, C>& lhs, B divisor, Matrix<T, R, C>& result) noexcept
    {
        for (int i = 0; i < R; ++i)
        {
            Div(lhs[i], divisor, result[i]);
        }
    }

    template<typename A, typename B, int R, int C>
    inline Matrix<std::common_type_t<A, B>, R, C> operator+(const Matrix<A, R, C>& lhs, const Matrix<B, R, C>& rhs) noexcept
    {
        using FinalType = Matrix<std::common_type_t<A, B>, R, C>;
        FinalType result;
        Add(lhs, rhs, result);
        return result;
    }

    template<typename A, typename B, int R, int C>
    inline Matrix<std::common_type_t<A, B>, R, C> operator-(const Matrix<A, R, C>& lhs, const Matrix<B, R, C>& rhs) noexcept
    {
        using FinalType = Matrix<std::common_type_t<A, B>, R, C>;
        FinalType result;
        Sub(lhs, rhs, result);
        return result;
    }

    template<typename A, typename B, int R, int C>
    inline Matrix<A, R, C> operator*(const Matrix<A, R, C>& lhs, B scale) noexcept
    {
        using FinalType = Matrix<A, R, C>;
        FinalType result;
        Mult(lhs, scale, result);
        return result;
    }

    template<typename A, typename B, int R, int C>
    inline Matrix<A, R, C> operator*(B scale, const Matrix<A, R, C>& rhs) noexcept
    {
        return rhs * scale;
    }

    template<typename A, typename B, int R, int C>
    inline Matrix<A, R, C> operator/(const Matrix<A, R, C>& lhs, B divisor) noexcept
    {
        Matrix<A, R, C> m;
        Div(lhs, divisor, m);
        return m;
    }

    template<typename T, int R, int C>
    template<typename U>
    inline typename Matrix<T, R, C>::Matrix_T& Matrix<T, R, C>::operator+=(const Matrix<U, R, C>& rhs) noexcept
    {
        Add(*this, rhs, *this);
        return *this;
    }

    template<typename T, int R, int C>
    template<typename U>
    inline typename Matrix<T, R, C>::Matrix_T& Matrix<T, R, C>::operator-=(const Matrix<U, R, C>& rhs) noexcept
    {
        Sub(*this, rhs, *this);
        return *this;
    }

    template<typename T, int R, int C>
    template<typename U>
    inline typename Matrix<T, R, C>::Matrix_T& Matrix<T, R, C>::operator*=(U scale) noexcept
    {
        Mult(*this, scale, *this);
        return *this;
    }

    template<typename T, int R, int C>
    template<typename U>
    inline typename Matrix<T, R, C>::Matrix_T& Matrix<T, R, C>::operator/=(U divisor) noexcept
    {
        Div(*this, divisor, *this);
        return *this;
    }

    template<typename T, typename U, int R, int C, int CR>
    inline Matrix<std::common_type_t<T, U>, R, CR> operator*(const Matrix<T, R, C>& lhs, const Matrix<U, C, CR>& rhs) noexcept
    {
        using FinalType = Matrix<std::common_type_t<T, U>, R, CR>;
        FinalType result;
        for (int i = 0; i < R; ++i)
        {
            auto left = lhs.RowVector(i);
            for (int j = 0; j < CR; ++j)
            {
                result[i][j] = Dot(left, rhs.ColVector(j));
            }
        }
        return result;
    }

    template<typename T, typename U, int N, int C>
    inline Vector<std::common_type_t<T, U>, N> operator*(const Vector<T, N>& v, const Matrix<U, N, C>& m) noexcept
    {
        using FinalType = Vector<std::common_type_t<T, U>, C>;
        FinalType result;
        for (int i = 0; i < C; ++i)
        {
            result[i] = Dot(v, m.ColVector(i));
        }
        return result;
    }

    template<typename T, int N>
    struct SquareMatrixHelper
    {
        using Matrix_T = YtcGE::Matrix<T, N, N>;

        static const Matrix_T Identity() noexcept
        {
            Matrix_T m(0);
            for (int i = 0; i < N; ++i)
            {
                m[i][i] = T(1);
            }
            return m;
        }
    };

    using Mat22f = Matrix<float, 2, 2>;
    using Mat33f = Matrix<float, 3, 3>;
    using Mat44f = Matrix<float, 4, 4>;
    using Mat22i = Matrix<int, 2, 2>;
    using Mat33i = Matrix<int, 3, 3>;
    using Mat44i = Matrix<int, 4, 4>;
    using SquareMatrixHelper44f = SquareMatrixHelper<float, 4>;
    using SquareMatrixHelper33f = SquareMatrixHelper<float, 3>;
    using SquareMatrixHelper22f = SquareMatrixHelper<float, 2>;

    template<typename T>
    struct _3DTransformHelper
    {
        using Mat = Matrix<T, 4, 4>;

        static constexpr Mat Translation(T x, T y, T z) noexcept
        {
            return
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1,
            };
        }

        static constexpr Mat Scaling(T nx, T ny, T nz) noexcept
        {
            return 
            {
                nx, 0, 0, 0,
                0, ny, 0, 0,
                0, 0, nz, 0,
                0, 0, 0, 1,
            }
        }

        static constexpr Mat Rotatation(const Vector<T, 4>& direction, float theta) noexcept
        {
            return Rotatation(direction[0], direction[1], direction[2], theta);
        }

        static constexpr Mat Rotatation(T x, T y, T z, float theta) noexcept
        {
            auto cosTheta = std::cos(theta);
            auto sinTheta = std::sin(theta);
            auto squareX = x * x;
            auto squareY = y * y;
            auto squareZ = z * z;
            auto xy = x * y;
            auto xz = x * z;
            auto yz = y * z;
            auto oneMinusCosTheta = 1.0f- cosTheta;
            auto xSinTheta = x * sinTheta;
            auto ySinTheta = y * sinTheta;
            auto zSinTheta = z * sinTheta;
            return 
            {
                cosTheta + oneMinusCosTheta * squareX, oneMinusCosTheta * xy - zSinTheta, oneMinusCosTheta * xz + ySinTheta, 0,
                oneMinusCosTheta * xy + zSinTheta, cosTheta + oneMinusCosTheta * squareY, oneMinusCosTheta * yz - xSinTheta, 0,
                oneMinusCosTheta * xz + ySinTheta, oneMinusCosTheta * yz + xSinTheta, cosTheta + oneMinusCosTheta * squareZ, 0,
                0, 0, 0, 1
            };
        }
    };

}

#endif