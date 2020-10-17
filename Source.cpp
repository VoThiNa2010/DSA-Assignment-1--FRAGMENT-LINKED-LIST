
#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T removeAt(int index) = 0;
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
    virtual string toString() = 0;
   // virtual void Print_Header();
};

#endif

// STUDENT ANSWER
#ifndef FRAGMENT_LINKED_LIST
#define FRAGMENT_LINKED_LIST
template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;

protected:
    Node** fragmentPointers;
    int fragmentMaxSize;
    int count;
    Node* head = NULL;
    Node* tail = NULL;

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->head = NULL;
        this->tail = NULL;
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node * [2];
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList() {};
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    virtual bool contains(const T& item);
    virtual string toString();
    virtual void Modify();
    virtual void Print_Header();
    Iterator begin(int index = 0);
    Iterator end(int index = -1);

public:
    class Node
    {
    private:
        T data;
        Node* next;
        Node* prev;
        friend class FragmentLinkedList<T>;
        Node* head;
        Node* tail;

    public:
        Node()
        {
            head = 0;
            tail = 0;
            data = 0;
            next = 0;
            prev = 0;
        }
        Node(Node* next, Node* prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node* next, Node* prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T>* pList;
        Node* pNode;

    public:
        Iterator(FragmentLinkedList<T>* pList = 0, bool begin = true);
        Iterator(FragmentLinkedList<T>* pList = 0, int fragmentIndex = 0, bool begin = true);
        Iterator& operator=(const Iterator& iterator);
        T& operator*();
        bool operator!=(const Iterator& iterator);
        void remove();
        void set(const T& element);
        Iterator& operator++();
        Iterator operator++(int);
    };
};
template<class T>
void FragmentLinkedList<T>::Modify()
{   
    int number_fragmentPointers = 0;
    if (count <= fragmentMaxSize) {
        number_fragmentPointers = 2;
        delete[] this->fragmentPointers;
        this->fragmentPointers = new Node * [2];
        this->fragmentPointers[0] = this->head;
        this->fragmentPointers[1] = this->tail;
        return;
    }
    else {
        if (count % fragmentMaxSize == 0) {
            number_fragmentPointers = (this->count / this->fragmentMaxSize) + 1;
        }
        else if (count % fragmentMaxSize != 0) {
            number_fragmentPointers = (this->count / this->fragmentMaxSize) + 2;
        }
        delete[] this->fragmentPointers;
        this->fragmentPointers = new Node * [number_fragmentPointers];
        this->fragmentPointers[0] = this->head;
        for (int i = 1; i < number_fragmentPointers - 1; i++) { 
            int k = 0;
            FragmentLinkedList<T>::Node* p_cur = this->head;
            while (k < fragmentMaxSize * i) {   
                p_cur = p_cur->next;
                k++;
            }
            this->fragmentPointers[i] = p_cur; 
        }
        this->fragmentPointers[number_fragmentPointers - 1] = this->tail;
    }



}
template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    ss << "[";
    Node* ptr = this->fragmentPointers[0];
    if (this->count == 0)
        ss << "]";

    ////
    else
    {
        ptr = this->head;
        while (ptr->next)
        {
            ss << to_string(ptr->data) << ", ";
            ptr = ptr->next;
        }
        if (this->count != 0)
            ss << to_string(ptr->data) << "]";
    }

    ////
    return ss.str();
}

// ADD A NEW ELEMENT INTO THE END OF THE LIST//
template <class T>
void FragmentLinkedList<T>::add(const T& element)
{
    FragmentLinkedList<T>::Node* new_node = new FragmentLinkedList<T>::Node();
    new_node->data = element;
    if (this->head == NULL)
    {
        this->head = new_node;
        tail = new_node;
        this->count++;
    }
    else
    {
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
        this->count++;
    }
    Modify();
    return;
}
// ADD A NEW ELEMENT INTO INDEX INDEX POSITION //
template <class T>
void FragmentLinkedList<T>::add(int index, const T& element)
{
    FragmentLinkedList<T>::Node* new_node = new FragmentLinkedList<T>::Node();
    new_node->data = element;

    FragmentLinkedList<T>::Node* p_cur = this->head;
    if (head == NULL)
    {
        if (index == 0)
        {
            head = new_node;
            tail = new_node;
            this->count++;
        }
        else
        {
            throw std::out_of_range("The index is out of range!");
        }

    }
    else
    {
        if (index == 0)
        {
            head->prev = new_node;
            new_node->next = head;
            head = new_node;
            this->count++;
        }
        else if (index > 0 && index < count)
        {
            int temp = 0;
            while (temp < index)
            {
                p_cur = p_cur->next;
                temp++;
            }
            p_cur->prev->next = new_node;
            new_node->prev = p_cur->prev;
            new_node->next = p_cur;
            p_cur->prev = new_node;
            this->count++;
        }
        else if (index == count || index == count - 1)
        {
            this->add(element);
        }
        else
        {
            throw std::out_of_range("The index is out of range!");
        }
    }
    Modify();
}
// delete the element at index position and return that element//
template <class T>
T FragmentLinkedList<T>::removeAt(int index)
{

    if (this->empty() == true)
    {
        throw std::out_of_range("The index is out of range!");
    }
    else
    {
        if (index == 0)
        {
            T& result = this->head->data;
            this->head = this->head->next;
            this->count--;
            return result;
        }
        else if (index > 0 && index < this->count - 1)
        {
            FragmentLinkedList<T>::Node* p_cur = this->head;
            int temp = 0;
            while (temp < index)
            {
                p_cur = p_cur->next;
                temp++;
            }
            T& result = p_cur->data;
            p_cur->prev->next = p_cur->next;
            p_cur->next->prev = p_cur->prev;
            this->count--;
            free(p_cur);
            return result;
        }
        else if (index == count - 1)
        {
            T& result = this->tail->data;
            this->tail = this->tail->prev;
            this->tail->next = NULL;
            this->count--;
            return result;
        }
        else
        {
            throw std::out_of_range("The index is out of range!");
        }
    }
    Modify();
}
//DELETE THE ELEMENT HAS VALUE ITEM AND RETURN WHETHER THAT ELEMENT CAN BE FOUND//
template <class T>
bool FragmentLinkedList<T>::removeItem(const T& item)
{
    FragmentLinkedList<T>::Node* p_cur = this->head;

    for (int i = 0; i < count; i++, p_cur = p_cur->next)
    {
        if (p_cur->data == item)
        {
            removeAt(i);
            return true;
        }
    }
    Modify();
    return false;
}
//CHECKIN WHETHER THE LIST IS EMPTY
template <class T>
bool FragmentLinkedList<T>::empty()
{
    if (this->count == 0)
    {
        return true;
    }
    else
        return false;
}
//RETURN THE SIZE OF THE CURRENT LIST//
template <class T>
int FragmentLinkedList<T>::size()
{
    return this->count;
}
//DELETE ALL CURRENT ELEMENTS IN THE LIST//
template <class T>
void FragmentLinkedList<T>::clear()
{
    while (this->count != 0)
    {
        this->removeAt(0);
    }
    delete[] this->fragmentPointers;
    this->fragmentPointers = new Node * [2];
    this->fragmentPointers[0] = NULL;
    this->fragmentPointers[1] = NULL;
}
//FIND AND RETURN THE ELEMENT AT INDEX POSITION//
template <class T>
T FragmentLinkedList<T>::get(int index)
{
    if (this->empty())
    {
        throw std::out_of_range("The index is out of range!");
    }
    else
    {
        FragmentLinkedList<T>::Node* p_cur = this->head;
        int temp = 0;

        while (temp < index)
        {
            p_cur = p_cur->next;
            temp++;
        }
        return p_cur->data;
    }
}
//FIND AND RETURN THE POSITION OF THE ELEMENT WHICH HAS VALUE ITEM IN THE LIST//
template <class T>
int FragmentLinkedList<T>::indexOf(const T& item)
{
    if (this->contains(item) == true)
    {
        FragmentLinkedList<T>::Node* p_cur = this->head;
        for (int i = 0; i < count; i++, p_cur = p_cur->next)
        {
            if (p_cur->data == item)
            {
                return i;
            }
        }
        return -1;
    }
    else
    {
        return -1;
    }
}
//CHECK WHETHER THE LIST CONTAINS ELEMENT THAT HAS VALUE ITEM//
template <class T>
bool FragmentLinkedList<T>::contains(const T& item)
{
    FragmentLinkedList<T>::Node* p_cur = this->head;
    for (int i = 0; i < count; i++)
    {
        if (p_cur->data == item)
        {
            return true;
        }
        else
        {
            p_cur = p_cur->next;
        }
    }
    return false;
}
// SET ELEMENT AT INDEX POSITION TO ELEMENT
template <class T>
void FragmentLinkedList<T>::set(int index, const T& element)
{
    if (this->empty()) { return; }
    else
    {
        if (index >= 0 && index < this->count)
        {
            FragmentLinkedList<T>::Node* p_cur = this->head;
            int temp = 0;
            while (temp < index)
            {
                p_cur = p_cur->next;
                temp++;
            }
            p_cur->data = element;
        }
        else
        {
            throw std::out_of_range("Index is out of range");
        }

    }

}

template <class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T>* pList, bool begin) {
    this->pList = pList;
    FragmentLinkedList<T>::Node* p_cur = this->pList->head;
    if (begin == true) {
        this->pNode = /*this->*/pList->head;
    }
    else {
        this->pNode = /*this->*/pList->tail->next;
    }

}
template<class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T>* pList, int index, bool begin) {
   // this->pList = pList;

    if (begin == true) {

        FragmentLinkedList<T>::Node* p_cur = pList->head;
        int temp = index * (pList->fragmentMaxSize);
        int i = 0;
        while (i < temp) {
            p_cur = p_cur->next;
            i++;
        }
        this->pNode = p_cur;
    }
    else {
        FragmentLinkedList<T>::Node* p_cur = pList->head;
        int temp = (pList->count) / (pList->fragmentMaxSize);
        if (index == -1 || index == temp) { this->pNode = (pList->tail)->next; }
        else if (index == 0) {
            int k = pList->fragmentMaxSize - 1;
            int i = 0;
            while (i < k) {
                p_cur = p_cur->next;
                i++;
            }
            this->pNode = p_cur->next;

        }
        else if (index > 0 && index < temp) {
            int m = index * pList->fragmentMaxSize - 1;
            int i = 0;
            while (i < m) {
                p_cur = p_cur->next;
                i++;
            }
            this->pNode = p_cur->next;

        }
    }
}
template<class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::begin(int index) {

    FragmentLinkedList<T>::Iterator it = FragmentLinkedList<T>::Iterator(this, index, true);
    return it;

}
template<class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::end(int index) {

    FragmentLinkedList<T>::Iterator it = FragmentLinkedList<T>::Iterator(this, index, false);
    return it;

}
template<class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T> ::Iterator::operator=(const Iterator& iterator) {
    
    this->pNode = iterator.pNode;
    return *this;
}
template<class T>
T& FragmentLinkedList<T>::Iterator::operator*() {

    T* it = &((this->pNode)->data);
    return *it;
}
template<class T>
bool FragmentLinkedList<T>::Iterator::operator!=(const Iterator& iterator) {
    if ((this->pList == iterator.pList) && (this->pNode == iterator.pNode)) return false;
    else return true;
}
template<class T>
void FragmentLinkedList<T>::Iterator::remove() {

    if (this->pNode == this->pList->head) {
        this->pNode = this->pList->head->prev;
        this->pList->removeAt(0);
    }
    else {
        this->pNode->prev->next = this->pNode->next;
        this->pNode->next->prev = this->pNode->prev;
        this->pNode = this->pNode->prev;
    }

}
template<class T>
void FragmentLinkedList<T>::Iterator::set(const T& element) {
    this->pNode->data = element;
}
template<class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator++() {
    this->pNode = this->pNode->next;
    return *this;
}
template<class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::Iterator::operator++(int) {
    Iterator it = *this;
    ++(*this);
    return it;
}
template<class T>
void FragmentLinkedList<T>::Print_Header()
{
    int number_fragmentPointers = 0;
    if (this->count % this->fragmentMaxSize == 0) {
        number_fragmentPointers = (this->count / this->fragmentMaxSize) + 1;
    }
    else if (this->count % this->fragmentMaxSize != 0) {
        number_fragmentPointers = (this->count / this->fragmentMaxSize) + 2;
    }
    for (int i = 0; i < number_fragmentPointers; i++)
    {
        Node* p = this->fragmentPointers[i];
        cout << "Fragment " << i << " : " << p->data << endl;
    }

}
#endif
// END: STUDENT ANSWER

int main()
{
    FragmentLinkedList<int> test(5);
    for (int i = 0; i < 20; i++)
    {
        test.add(i, i * i + 10);
    }
    cout << test.toString() << endl;
    //FragmentLinkedList<int>::Iterator it = test.begin();
    FragmentLinkedList<int>::Iterator it = test.begin();
    //FragmentLinkedList<int>::Iterator it = test.end(2);//it.remove();
   //FragmentLinkedList<int>::Iterator it = test.end();
     //it++;
     //for ( ; it !=test.end() ; it++){cout << *it << " ";}
    for (it = test.begin(); it != test.end(); it++)
    {
        cout << *it << " ";
    }
    return 0;

}




/**/