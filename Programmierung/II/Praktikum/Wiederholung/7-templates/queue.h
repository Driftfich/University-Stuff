#ifndef _queue_H_
#define _queue_H_

#include <iostream>
#include <vector>

#define BUF_ENT_MAX 10

using namespace std;

template <class T>
class Queue {
    int idx;
    T* arr;

    public:
        Queue() {this->arr = new T[BUF_ENT_MAX]; this->idx = 0;};
        Queue(const Queue<T>& other);
        ~Queue() {delete[] arr;};

        void push(T val);
        int find(T val);
        void print(ostream& os) const;
        T& operator[](int idx) const;
        Queue<T>& operator=(const Queue<T>& other);
        template <class U>
        friend ostream& operator<<(ostream& os, const Queue<U>& other);
};

template <class T>
Queue<T>::Queue(const Queue<T>& other) {
    this->arr = new T[BUF_ENT_MAX];
    this->idx = 0;
    for (int i=0; i<BUF_ENT_MAX; i++) {
        push(other[i]);
    }
    this->idx = 0;
}

template <class T>
void Queue<T>::push(T val) {
    arr[idx] = val;
    idx = (idx + 1) % BUF_ENT_MAX;
}

template <class T>
int Queue<T>::find(T val) {
    for (int i=0; i < BUF_ENT_MAX; i++) {
        if (arr[i] == val) return i;
    }
    return -1;
}

template <class T>
T& Queue<T>::operator[](int idx) const {
    if (idx<0 || idx >= BUF_ENT_MAX) throw "Index out of bounds.\n";
    return arr[idx];
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other) {
    for (int i=0; i<BUF_ENT_MAX; i++) {
        this->arr[i] = other.arr[i];
    }
    this->idx = other.idx;

    return *this;
}

template <class T>
void Queue<T>::print(ostream& os) const {
    os << "Queue([";
    for (int i=0; i<BUF_ENT_MAX; i++) {
        os << arr[i] << ", ";
        // if (i % 5 == 0) os << endl;
    }
    os << ")]" << endl;
}

template <class T>
ostream& operator<<(ostream& os, const Queue<T>& other) {
    other.print(os);
    return os;
}

#endif