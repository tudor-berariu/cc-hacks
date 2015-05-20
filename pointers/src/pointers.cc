// Copyright (C) 2015 Tudor Berariu

#include <cstdlib>
#include <cassert>

#include <iostream>

using namespace std;

#define ROWS 4
#define COLS 5

template<typename T>
struct vector {
  size_t capacity;
  size_t size;
  T* _v;

  vector() {
    capacity = 4;
    _v = (T*) malloc(capacity * sizeof(T));
    size = 0;
  }

  ~vector() {

    cout << "Eliberez memoria pulii" << endl;
    free(_v);
  }

  size_t get_size() const {
    return size;
  }

  void push_back(T x) {
    if (size == capacity) {
      capacity *= 2;
      _v = (T*) realloc(_v, capacity * sizeof(T));
    }
    _v[size] = x;
    size++;
  }

  T operator[](size_t idx) {
    return *(_v + idx);
  }
};

void f() {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  cout << "Al treilea eleemnt este: " << v[2] << endl;
  cout << v.get_size() << endl;
}

template<typename T>
struct Matrix {
  size_t rows_no;
  size_t cols_no;
  T* _m;

  Matrix(size_t rows_no, size_t cols_no): rows_no{rows_no}, cols_no{cols_no} {
    _m = (T*) malloc(rows_no * cols_no * sizeof(T));
  }

  ~Matrix() {
    free(_m);
  }

  T* operator[](size_t row) {
    assert(row < rows_no);
    return _m + (row * cols_no);
  }
};

int main(int , char *[])
{
  int** m;

  m = (int**) malloc(ROWS * sizeof(int*));
  for (size_t i = 0; i < ROWS; i++) {
    m[i] = (int*) malloc(COLS * sizeof(int));
    for (size_t j = 0; j < COLS; j++) {
      m[i][j] = (i + 1) *10 + j;
    }
  }

  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++)
      cout << m[i][j] << " @ " << &(m[i][j]) << "\t";
    cout << endl;
  }

  cout << &(m[2][2]) << "\t" << (*(m+2) + 2) << endl;

  int *n;
  n = (int*) malloc(ROWS * COLS * sizeof(int));
  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++)
      n[i*COLS+j] = (i + 1) * 10 + j;
  }
  
  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++)
      cout << n[i * COLS + j] << " @ " << &(n[i * COLS + j]) << "\t";
    cout << endl;
  }

  using M = Matrix<int>;
  M m2(4, 5);
  m2[2][5] = 2333;
  cout << m2[3][0] << endl;

  cout << " Before " << endl;
  f();
  cout << " After " << endl;

  // l-a măscărit pe m

  for (size_t i = 0; i < ROWS; i++) {
    free(m[i]);
  }
  free(m);

  return 0;
}
