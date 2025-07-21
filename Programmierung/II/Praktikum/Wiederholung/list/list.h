#ifndef _LIST_H_
#define _LIST_H_

template <class T>
class element {
    T* dta;
    element<T>* nxt;

    public:
        element(T* data) {
            this->dta = data;
            this->nxt = nullptr;
        }
        ~element() {
            
        }

        T* getData() {return this->dta;}
        // void setData(T* data) {this->dta = data;}
        element<T>* getNext() {return this->nxt;}
        void setNext(element<T>* nxt) {this->nxt = nxt;}
};

template <class T>
class simpleList {
    element<T>* head;
    element<T>* tail;

    public:
        simpleList() : head(nullptr), tail(nullptr) {}
        ~simpleList() {
            // for (element* e = head; e != NULL; e = e->getNext()) {
            //     delete e->getData();
            //     delete e;
            // }
            element<T>* cur = head;
            element<T>* tmp;
            while (cur != nullptr) {
                tmp = cur;
                cur = cur->getNext();

                delete tmp->getData();
                delete tmp;
            }
        }

        int append(T* data) {
            element<T>* e = new element<T>(data);

            // inital fill
            if (head == nullptr) {
                head = e;
                tail = e;
                return 0;
            }

            // else add it to the end
            tail.setNext(e);
            tail = e;
            return 0;
        }

        T* getAndRemove() {
            if (head == nullptr) return nullptr;

            T* data = head->getData();
            element<T>* tmp = head;
            head = head->getNext();
            delete tmp;
            return data;
        }
};

#endif
