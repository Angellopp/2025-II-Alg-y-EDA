#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <iterator>
#include <stdexcept>

// PC1: deben hacer:
//      2 problemas de nivel 2
//      3 problemas de nivel 1
// Cada solucion enviarla como un Pull request

// TODO (Nivel 2): Agregar Traits 

// TODO (Nivel 2): Agregar Iterators (forward, backward) ✅

// TODO (Nivel 1): Agregar Documentacion para generar con doxygen

// TODO  (Nivel 2): Agregar control de concurrencia en todo el vector
template <typename T>
class CVector{
    T      *m_pVect = nullptr;
    size_t  m_count = 0; // How many elements we have now?
    size_t  m_max   = 0; // Max capacity
public:
    using iterator               = T*;
    using const_iterator         = const T*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    // TODO  (Nivel 1) Agregar un constructor por copia ✅
    CVector(const CVector &v);
    CVector(size_t n);
    // TODO  (Nivel 2): Agregar un move constructor ✅
    CVector(CVector &&v);

    // TODO: (Nivel 1) implementar el destructor de forma segura ✅
    virtual ~CVector();

    void insert(T &elem);
    T&   operator[](size_t index);
    size_t size() const { return m_count; }
    // Iteradores forward
    iterator begin() { return m_pVect; }
    iterator end()   { return m_pVect + m_count; }
    const_iterator begin() const { return m_pVect; }
    const_iterator end()   const { return m_pVect + m_count; }
    // Iteradores backward
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }
private:
    void resize();
    void Init(size_t n);
    void Destroy();
};

template <typename T>
CVector<T>::CVector(size_t n){
    Init(n);
}

// Move constructor
template <typename T>
CVector<T>::CVector(CVector &&v)
    : m_pVect(v.m_pVect),
      m_count(v.m_count),
      m_max(v.m_max) {
    v.m_pVect = nullptr;
    v.m_count = 0;
    v.m_max   = 0;
}

template <typename T>
CVector<T>::CVector(const CVector &v)
        : m_pVect(nullptr),
          m_count(v.m_count),
          m_max(v.m_max) {
    if (m_max > 0) {
        // value-init para evitar basura en posiciones no usadas
        m_pVect = new T[m_max]();
        for (size_t i = 0; i < m_count; ++i) {
            m_pVect[i] = v.m_pVect[i];
        }
    }
}



// TODO (Nivel 1): hacer dinamico el delta de crecimiento ✅
template <typename T>
void CVector<T>::resize(){
    size_t newCap;
    if (m_max == 0) newCap = 16;
    else if (m_max < 64) newCap = m_max + 16;
    else newCap = m_max * 2;

    T *pTmp = new T[newCap];
    for(size_t i=0; i < m_count ; ++i)
        pTmp[i] = m_pVect[i];
    delete [] m_pVect;
    m_pVect = pTmp;
    m_max   = newCap;
}

template <typename T>
void CVector<T>::Init(size_t n){
    Destroy();
    if (n != 0) {
        while (m_max < n) resize();
    }
}

template <typename T>
    CVector<T>::~CVector(){
    Destroy();
}

template <typename T>
void CVector<T>::Destroy(){
    if (m_pVect) {
        delete [] m_pVect;
        m_pVect = nullptr;
    }
    m_count = 0;
    m_max   = 0;
}

// TODO (ya está hecha): la funcion insert debe permitir que el vector crezca si ha desbordado
template <typename T>
void CVector<T>::insert(T &elem){
    if(m_count == m_max)
        resize();
    m_pVect[m_count++] = elem;
}

template <typename T>
T& CVector<T>::operator[](size_t index) {
    if (index >= m_count) {
        throw std::out_of_range("Index out of range");
    }
    return m_pVect[index];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, CVector<T>& vec) {
    // os << "[";
    for (size_t i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    // os << "]";
    return os;
}

#endif // __VECTOR_H__