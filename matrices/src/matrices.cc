// Copyright (C) 2015 Tudor Berariu

// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cassert>
#include <iostream>

using namespace std;

template<typename T>
class Matrix;

template<typename T>
ostream& operator<<(ostream&, const Matrix<T>&);

template<typename T>
class Matrix {
public:
  Matrix(size_t, size_t);
  T* operator[](size_t);
  T* at(size_t, size_t);
  T get(size_t, size_t) const;
  Matrix<T> operator*(const Matrix<T>&) const;
  Matrix<T> operator+(const Matrix<T>&) const;
  void operator*=(T x);
  size_t get_rows_no() const;
  size_t get_cols_no() const;

  void magic_init();

  template<typename T_>
  friend ostream& operator<<(ostream&, const Matrix<T_>&);

  Matrix<T>& operator=(const Matrix<T>&);

private:
  size_t rows_no;
  size_t cols_no;

  T* _values;
};

template<typename T, template<typename> class... layers>
struct ForwardComputation;

template<typename T>
struct ForwardComputation<T> {
  inline static Matrix<T> compute(const Matrix<T>& m_in) {
    Matrix<T> m_out = m_in;
    return m_out;
  }
};

template<typename T, template<typename> class CrtLayer,
         template<typename> class... Layers>
struct ForwardComputation<T, CrtLayer, Layers...> {
  using Next = ForwardComputation<T, Layers...>;

  inline static Matrix<T> compute(const Matrix<T>& m_in,
                           const CrtLayer<T>& layer,
                           Layers<T>... other) {
    return Next::compute(layer.compute(m_in), other...);
  }
};

template<typename T>
struct Multiplier {
  Matrix<T> p;
  Multiplier(size_t, size_t);
  Matrix<T> compute(const Matrix<T>&) const;
};

template<typename T>
Multiplier<T>::Multiplier(size_t rows_no, size_t cols_no)
  : p(rows_no, cols_no) {
  p.magic_init();
}

template<typename T>
Matrix<T> Multiplier<T>::compute(const Matrix<T>& m_in) const {
  Matrix<T> m3 = (m_in * p);
  return m3;
}

template<typename T>
struct Adder {
  Matrix<T> p;
  Adder(size_t, size_t);
  Matrix<T> compute(const Matrix<T>&) const;
};

template<typename T>
Adder<T>::Adder(size_t rows_no, size_t cols_no)
  : p(rows_no, cols_no) {
  p.magic_init();
}

template<typename T>
Matrix<T> Adder<T>::compute(const Matrix<T>& m_in) const {
  Matrix<T> m3 = (m_in + p);
  return m3;
}

// Constructor
// Sets cols_no and rows_no
// and allocaties memory
template<typename T>
Matrix<T>::Matrix(size_t rows_no, size_t cols_no)
  : rows_no {rows_no}, cols_no {cols_no} { 
  _values = (T*) malloc(rows_no * cols_no * sizeof(T)); 
}

template<typename T>
size_t Matrix<T>::get_cols_no() const {
  return cols_no;
}

template<typename T>
size_t Matrix<T>::get_rows_no() const {
  return rows_no;
}

template<typename T>
T* Matrix<T>::operator[](size_t idx) {
  assert(idx < rows_no);
  return (_values + idx * cols_no);
}

template<typename T>
T Matrix<T>::get(size_t r, size_t c) const {
  assert(r < rows_no);
  assert(c < cols_no);
  return *(_values + r * cols_no + c);
}

template<typename T>
T* Matrix<T>::at(size_t r, size_t c) {
  assert(r < rows_no);
  assert(c < cols_no);
  return (_values + r * cols_no + c);
}

template<typename T>
void Matrix<T>::magic_init() {
  for (size_t i = 0; i < rows_no; i++)
    for (size_t j = 0; j < cols_no; j++)
      _values[i * cols_no + j] = (i + 1) * 10 + j + 1;
}

template<typename T>
void Matrix<T>::operator*=(T x) {
  for (size_t i = 0 ; i < rows_no * cols_no; i++)
    _values[i] *= x;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& b) const {
  assert(rows_no == b.get_rows_no() && cols_no == b.get_cols_no());
  Matrix<T> c(rows_no, b.get_cols_no());
  for(size_t i = 0; i < rows_no; i++) {
    for(size_t j = 0; j < b.get_cols_no(); j++) {
      c[i][j] = this->get(i,j) + b.get(i,j);
    }
  }
  return c;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& b) const {
  assert(cols_no == b.get_rows_no());
  Matrix<T> c(rows_no, b.get_cols_no());
  for(size_t i = 0; i < rows_no; i++) {
    for(size_t j = 0; j < b.get_cols_no(); j++) {
      c[i][j] = (T)0;
      for(size_t k = 0; k < cols_no; k++)
        c[i][j] += _values[i * cols_no + k] * b.get(k, j);
    }
  }
  return c;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& source) {
  rows_no = source.rows_no;
  cols_no = source.cols_no;
  _values = (T*) malloc(rows_no * cols_no * sizeof(T));
  for(size_t r = 0; r < rows_no; r++) {
    for(size_t c = 0; c < cols_no; c++) {
      _values[r * cols_no + c] = source.get(r, c);
    }
  }
  return *this;
}

template<typename T>
ostream& operator<<(ostream& s, const Matrix<T>& m) {
  for (size_t row = 0; row < m.rows_no; row++) {
    for (size_t col = 0; col < m.cols_no; col++) {
      s << m._values[row * m.cols_no + col] << " ";
    }
    s << endl;
  }
  return s;
}

int main(int argc, char * argv[])
{
  /*
  Matrix<int> m1(4, 6);
  Matrix<int> m2(6, 3);

  m1.magic_init();
  m2.magic_init();

  cout << m1;
  cout << m2;

  Matrix<int> m3 = m1 * m2;

  cout << m3;

  m1 *= 2;

  cout << m1;

  Matrix<int> m4 = m1;

  cout << m4;

  Matrix<int> m5(3, 4);
  Matrix<int> m6(3, 4);

  m5.magic_init();
  m6.magic_init();

  Matrix<int> m7 = (m5 + m6);

  cout << m7;
  */

  Multiplier<int> l1(4, 6);
  Adder<int> l2(3, 6);
  Multiplier<int> l3(6, 2);

  using NN = ForwardComputation<int, Multiplier, Adder, Multiplier>;

  Matrix<int> m_in(3,4);
  m_in.magic_init();

  Matrix<int> m_out = NN::compute(m_in, l1, l2, l3);
  cout << m_out << endl;

  // Testare
  Matrix<int> _m_in(3,4);
  _m_in.magic_init();
  Matrix<int> _p1(4, 6);
  _p1.magic_init();
  Matrix<int> _p2(3, 6);
  _p2.magic_init();
  Matrix<int> _p3(6, 2);
  _p3.magic_init();

  cout << ((_m_in * _p1) + _p2) * _p3;

  return 0;
}
