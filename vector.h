#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iterator>
#include <cstddef>
#include <stdexcept>
#include <ostream>

/**
 * @file vector.h
 * @brief Implementación de @c CVector, un contenedor dinámico minimalista.
 *
 * ## Resumen
 * - Crecimiento dinámico amortizado.
 * - Acceso por índice con @c operator[] (solo en rango).
 * - Semánticas de copia y movimiento.
 * - Iteradores forward y backward (vía @c std::reverse_iterator).
 *
 * @note Este archivo está documentado para su uso en Doxygen.
 */

// PC1: deben hacer:
//      2 problemas de nivel 2
//      3 problemas de nivel 1
// Cada solucion enviarla como un Pull request

// TODO (Nivel 2): Agregar Traits 

// TODO (Nivel 2): Agregar Iterators (forward, backward) ✅

// TODO (Nivel 1): Agregar Documentacion para generar con doxygen

template <typename T>
class CVector{
    T      *m_pVect = nullptr;
    size_t  m_count = 0; // How many elements we have now?
    size_t  m_max   = 0; // Max capacity
public:
    /**
     * @name Tipos de iterador
     * @{
     */
    using iterator               = T*;                        //!< Iterador mutable (forward)
    using const_iterator         = const T*;                  //!< Iterador constante (forward)
    using reverse_iterator       = std::reverse_iterator<iterator>;        //!< Iterador mutable (backward)
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;  //!< Iterador constante (backward)
    /** @} */

    // --- Constructores / Destructor ---

    /**
     * @brief Construye con tamaño inicial @p n (inicializa a T()).
     *
     * @details Este constructor deja el tamaño lógico en @p n para permitir
     * operaciones como @c v[3]=8 inmediatamente si @p n>=4.
     */
    explicit CVector(size_t n = 0) { Init(n); }

    /**
     * @brief Constructor por copia.
     * @param v Vector origen a copiar.
     */
    CVector(const CVector& v)
        : m_pVect(nullptr), m_count(v.m_count), m_max(v.m_max) {
        if (m_max > 0) {
            m_pVect = new T[m_max]();              // value-init
            for (size_t i = 0; i < m_count; ++i)   // copia los usados
                m_pVect[i] = v.m_pVect[i];
        }
    }

    /**
     * @brief Constructor por movimiento.
     * @param v Vector origen; tras el movimiento queda vacío/seguro.
     * @post @c v.size()==0
     */
    CVector(CVector&& v) noexcept
        : m_pVect(v.m_pVect), m_count(v.m_count), m_max(v.m_max) {
        v.m_pVect = nullptr; v.m_count = 0; v.m_max = 0;
    }

    /**
     * @brief Destructor seguro (idempotente).
     */
    ~CVector() { Destroy(); }

    // --- Operaciones básicas ---

    /**
     * @brief Inserta un elemento al final (crece si es necesario).
     * @param elem Elemento a insertar.
     */
    void insert(T& elem) {
        if (m_count == m_max) resize();
        m_pVect[m_count++] = elem;
    }

    /**
     * @brief Acceso por índice (solo en rango).
     * @param index Índice [0, size()).
     * @throw std::out_of_range si @p index >= size().
     * @return Referencia al elemento en @p index.
     */
    T& operator[](size_t index) {
        if (index >= m_count) throw std::out_of_range("Index out of range");
        return m_pVect[index];
    }

    /**
     * @brief Número de elementos lógicos.
     */
    size_t size() const { return m_count; }

    // --- Iteradores forward ---
    /// @brief Primer elemento (mutable).
    iterator begin() { return m_pVect; }
    /// @brief Uno-pasado-el-último (mutable).
    iterator end()   { return m_pVect + m_count; }
    /// @brief Primer elemento (const).
    const_iterator begin() const { return m_pVect; }
    /// @brief Uno-pasado-el-último (const).
    const_iterator end()   const { return m_pVect + m_count; }
    /// @brief Primer elemento (const) explícito.
    const_iterator cbegin() const { return m_pVect; }
    /// @brief Uno-pasado-el-último (const) explícito.
    const_iterator cend()   const { return m_pVect + m_count; }

    // --- Iteradores backward ---
    /// @brief Inicio invertido (mutable).
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    /// @brief Fin invertido (mutable).
    reverse_iterator rend()   { return reverse_iterator(begin()); }
    /// @brief Inicio invertido (const).
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    /// @brief Fin invertido (const).
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }
    /// @brief Inicio invertido (const) explícito.
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    /// @brief Fin invertido (const) explícito.
    const_reverse_iterator crend()   const { return const_reverse_iterator(begin()); }

private:
    // Política de crecimiento dinámico (amortizada)
    void resize() {
        size_t newCap;
        if (m_max == 0) newCap = 16;
        else if (m_max < 64) newCap = m_max + 16;
        else newCap = m_max * 2;

        T* pTmp = new T[newCap];
        for (size_t i = 0; i < m_count; ++i)
            pTmp[i] = m_pVect[i];
        delete [] m_pVect;
        m_pVect = pTmp;
        m_max   = newCap;
    }

    // Inicializa con tamaño n (size = n, capacity = n, value-init)
    void Init(size_t n) {
        Destroy();
        if (n > 0) {
            m_pVect = new T[n]();
            m_max   = n;
            m_count = n;       
        }
    }

    void Destroy() {
        if (m_pVect) { delete [] m_pVect; m_pVect = nullptr; }
        m_count = 0;
        m_max   = 0;
    }
};

// Operador de impresión
/**
 * @brief Imprime el contenido del vector en un flujo de salida.
 * @tparam T Tipo de los elementos.
 * @param os Flujo de salida (p. ej., std::cout).
 * @param vec Vector a imprimir.
 * @return Referencia al flujo @p os.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const CVector<T>& vec) {
    // os << "[";
    for (size_t i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    // os << "]";
    return os;
}

#endif // __VECTOR_H__
