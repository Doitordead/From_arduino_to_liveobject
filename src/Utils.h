#pragma once
#include <Wire.h>

/*******************************************************************************
 * 
 *                  Linked List
 * 
 * ****************************************************************************/
typedef uint8_t byte;

template<typename T>
class ListNode
{
  public:
    ListNode(T* el, ListNode* p) : element(el),prev(p){};
    ~ListNode(){delete element;}
  public:
    T* element;
    ListNode* prev;
};

template<typename T>
class LinkedList
{
  public:
    LinkedList(): m_nSize(0), head(nullptr), tail(nullptr){}
    ~LinkedList();
  public:
    bool push(T* element);
    int size();
    int find(T* element);
    T* operator[](size_t index);
  private:
    ListNode<T>* head;
    ListNode<T>* tail;
    int m_nSize;
};

template<typename T>
LinkedList<T>::~LinkedList()
{
  ListNode<T> * tmp = tail;
  for(int i=0;i<m_nSize;++i)
  {
    tmp = tail->prev;
    delete tail;
    if(i<m_nSize-1)tail = tmp;
  }

}

template<typename T>
int LinkedList<T>::size()
{
  return m_nSize;
}

template<typename T>
bool LinkedList<T>::push(T* element)
{
  if(find(element) == -1)
  {
    tail = new ListNode<T>(element, tail);
    m_nSize++;
    return true;
  }
  return false;
}

template<typename T>
T* LinkedList<T>::operator[](size_t index)
{
  ListNode<T>* tmp=tail;
  for(int i=0;i<index;++i)
  {
    tmp = tmp->prev;
  }
  return tmp->element;
}

template<typename T>
int LinkedList<T>::find(T* element)
{
  ListNode<T>* tmp=tail;
  for(int i=0;i<m_nSize;++i)
  {
    if(*(tmp->element)==*element) return i;
    tmp = tmp->prev;
  }
  return -1;


}



/*******************************************************************************
 * 
 *                  Hex converter
 * 
 * ****************************************************************************/
template<typename T>
String ToHexT(T val)
{
  union
  {
    T input;
    long long output;
  } data;

  data.output =0;
  data.input = val;
  String ret;
  char buff[5];
  for(int i=0,e=sizeof(data.input);i<e;++i)
  {
    uint8_t x = (uint8_t)((data.output>>((e-1-i)*8)));
    if(x<16) ret+='0';
    memset(buff,'\0',5);
    itoa(x,buff,16);
    ret+=buff;
  }
  return ret;
}

template<typename T>
String ToHexTU(T val)
{
  union
  {
    T input;
    unsigned long long output;
  } data;

  data.output =0;
  data.input = val;
  String ret;
  char buff[5];
  for(int i=0,e=sizeof(data.input);i<e;++i)
  {
    uint8_t x = (uint8_t)((data.output>>((e-1-i)*8)));
    if(x<16) ret+='0';
    memset(buff,'\0',5);
    itoa(x,buff,16);
    ret+=buff;
  }
  return ret;
}

String ToHex(String x);
String ToHex(int x);
String ToHex(long x);
String ToHex(float x);
String ToHex(double x);
String ToHex(long long x);
String ToHex(int8_t x);

String ToHex(unsigned int x);
String ToHex(unsigned long x);
String ToHex(unsigned long long x);
String ToHex(uint8_t x);


/*******************************************************************************
 * 
 *                  PMIC CONSTANTS
 * 
 * ****************************************************************************/

#define PMIC_ADDRESS 0x6B
#define SYSTEM_STATUS_REGISTER 0x08
#define PMIC_VERSION_REGISTER 0x0A

byte readRegister(byte address);
void batteryBegin();