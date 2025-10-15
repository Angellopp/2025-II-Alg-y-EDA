#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include <fstream>
#include "types.h"
//#include "util.h"
#include "general_iterator.h"
using namespace std;

template <typename Traits>
class CBinaryTree;

template <typename Traits>
class CBinaryTreeNode{
    friend class CBinaryTree<Traits>;
public:
  using value_type = typename Traits::T;
  using Node       = CBinaryTreeNode<Traits>;
protected:
    value_type     m_data;
    Node          *m_pParent = nullptr;
    Ref            m_ref;
    vector<Node *> m_pChild  = {nullptr, nullptr}; // 2 hijos inicializados en nullptr

public:
    CBinaryTreeNode(Node* pParent, value_type data, Ref ref, Node* p0 = nullptr, Node* p1 = nullptr)
        : m_data(data), m_pParent(pParent), m_ref(ref)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
    }
    ~CBinaryTreeNode(){
        m_pChild[0]; m_pChild[0] = nullptr;
        m_pChild[1] = nullptr;
    }
    value_type  getData()                {   return m_data;    }
    value_type &getDataRef()             {   return m_data;    }
    Ref     getRef()                     {   return m_ref;     }
    Node* getpNext() {
        Node* current = this;

        if (current->m_pChild[1]) {
            current = current->m_pChild[1];
            while (current->m_pChild[0]) {
                current = current->m_pChild[0];
            }
            return current;
        }

        Node* parent = current->m_pParent;
        while (parent && current == parent->m_pChild[1]) {
            current = parent;

            parent = parent->m_pParent;
        }

        return parent;
    }
    Node* getpPrev() {
    Node* current = this;

    if (current->m_pChild[0]) {
        current = current->m_pChild[0];
        while (current->m_pChild[1]) {
            current = current->m_pChild[1];
        }
        return current;
    }

    Node* parent = current->m_pParent;
    while (parent && current == parent->m_pChild[0]) {
        current = parent;
        parent  = parent->m_pParent;
    }
    return parent;
}

private: // TODO: Add this class as friend of the BinaryTree
        // and make these methods private
    void      setChild(Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }
};
template <typename Container>
class binary_tree_iterator : public general_iterator<Container,  class binary_tree_iterator<Container> > // 
{  
public:
    using Parent    = class general_iterator<Container, binary_tree_iterator<Container> >;     \
    using Node      = typename Container::Node;

  public:
    binary_tree_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    binary_tree_iterator(Container &other)  : Parent (other) {}
    binary_tree_iterator(Container &&other) : Parent(other) {} // Move constructor C++11 en adelante
public:
    // TODO: Revisar el avance de un iterator ✅
    binary_tree_iterator operator++() {
        Parent::m_pNode = Parent::m_pNode ? (Node*)Parent::m_pNode->getpNext() : nullptr;
        return *this;
    }
    binary_tree_iterator& operator--() {
    if (Parent::m_pNode) {
        Parent::m_pNode = (Node*)Parent::m_pNode->getpPrev();
    } else {
        Parent::m_pNode = Parent::m_pContainer
            ? Parent::m_pContainer->getExtremeNode(Parent::m_pContainer->m_pRoot, 1)
            : nullptr;
    }
    return *this;
}
};
template <typename _T>
struct BinaryTreeAscTraits{
    using  T         = _T;
    using  Node      = CBinaryTreeNode<BinaryTreeAscTraits<_T>>;
    using  CompareFn = less<T>;
};
template <typename _T>
struct BinaryTreeDescTraits
{
    using  T         = _T;
    using  Node      = CBinaryTreeNode<BinaryTreeDescTraits<_T>>;
    using  CompareFn = greater<T>;
};
template <typename Traits>
class CBinaryTree{
public:
    using value_type    = typename Traits::T;
    using Node          = typename Traits::Node;
    
    using CompareFn     = typename Traits::CompareFn;
    using Container     = CBinaryTree<Traits>;
    using iterator      = binary_tree_iterator<Container>;
protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    void insert(value_type elem, Ref ref) {
        internal_insert(elem, ref, nullptr, m_pRoot);
    }
     Node* getExtremeNode(Node* startNode, int direction) const {
        if (!startNode) return nullptr;
        
        Node* pNode = startNode;
        while (pNode->getChild(direction)) {
            pNode = pNode->getChild(direction);
        }
        return pNode;
    }
protected:
    Node* CreateNode(Node* pParent, value_type elem, Ref ref) {
        return new Node(pParent, elem, ref);
    }
    virtual Node* internal_insert(value_type &elem, Ref ref,
                                  Node* pParent, Node*& rpOrigin)
    {
        if (!rpOrigin) {
            ++m_size;
            return (rpOrigin = CreateNode(pParent, elem, ref));
        }

        size_t branch = Compfn(elem, rpOrigin->getDataRef()) ? 0 : 1;
        Node *pNode = internal_insert(elem, ref, nullptr, rpOrigin->getChildRef(branch));
        return pNode;
    }
public:
    CBinaryTree(){} // Empty tree
    
    // TODO: Copy Constructor. We have duplicate each node
    CBinaryTree(CBinaryTree &other);
    
    // TODO: Done: Move Constructor
    CBinaryTree(CBinaryTree &&other)
        : m_pRoot(std::exchange(other.m_pRoot, nullptr)), 
          m_size (std::exchange(other.m_size, 0)), 
          Compfn (std::exchange(other.Compfn, nullptr))
    { }

    // TODO: Recursivo y seguro. Destruir Nodes recursivamente ✅
    // virtual ~CBinaryTree(){  } 
    void Destroy(Node* pNode);
    virtual ~CBinaryTree();

    // TODO: begin dede comenzar el el nodo mas a la izquierda (0) ✅
    iterator begin() { 
        if (!m_pRoot) return end();
        return iterator(this, getExtremeNode(m_pRoot, 0));
    }
    iterator end()   { return iterator(this, nullptr); }
    
    using reverse_iterator = std::reverse_iterator<iterator>;

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }

    // TODO: Generalizar estos recorridos para recibir cualquier funcion
    // con una cantidad flexible de parametros con variadic templates
    // Google: C++ parameter packs cplusplus

    // Variadic templates
    template <typename Function, typename... Args>
    void inorder_variadic(Function&& func, Args&&... args) {
        inorder_temp(m_pRoot, std::forward<Function>(func), std::forward<Args>(args)...);
    }

    template <typename Function, typename... Args>
    void inorder_temp(Node* pNode, Function&& func, Args&&... args) {
        if (pNode) {
            inorder_temp(pNode->getChild(0), std::forward<Function>(func), std::forward<Args>(args)...);
            std::invoke(std::forward<Function>(func), pNode, std::forward<Args>(args)...);
            inorder_temp(pNode->getChild(1), std::forward<Function>(func), std::forward<Args>(args)...);
        }
    }

    void inorder  (ostream &os)    {   inorder  (m_pRoot, 0, os);  }
    // TODO: 
    void inorder(Node  *pNode, size_t level, ostream &os){
        if( pNode ){
            //Node *pParent = pNode->getParent();
            inorder(pNode->getChild(0), level+1, os);
            os << " --> " << pNode->getDataRef();
            inorder(pNode->getChild(1), level+1, os);
        }
    }

    // TODO: Generalize this function by using iterators and apply any function
    void inorder(Node  *pNode, void (*visit) (value_type& item)){
        if( pNode ){   
            inorder(pNode->getChild(0), *visit);
            (*visit)(pNode->getDataRef());
            inorder(pNode->getChild(1), *visit);
        }
    }

    // Variadic templates (See foreach.h)
    template <typename Function, typename... Args>
    void postorder(Function func, Args const&... args)
    {    postorder(m_pRoot, 0, func, args...);}

    void postorder_variadic(Function&& func, Args&&... args) {
        postorder_temp(m_pRoot, std::forward<Function>(func), std::forward<Args>(args)...);
    }

    template <typename Function, typename... Args>
    void postorder_temp(Node* pNode, Function&& func, Args&&... args) {
        if (pNode) {
            postorder(pNode->getChild(0), level + 1, func, args...);
            postorder(pNode->getChild(1), level + 1, func, args...);
            func(pNode, level); 
            postorder_temp(pNode->getChild(0), std::forward<Function>(func), std::forward<Args>(args)...);
            postorder_temp(pNode->getChild(1), std::forward<Function>(func), std::forward<Args>(args)...);
            std::invoke(std::forward<Function>(func), pNode, std::forward<Args>(args)...);
        }
    }
    // TODO: generalize this function to apply any function
    void postorder(Node  *pNode, size_t level, ostream &os){
        if( pNode ){   
            postorder(pNode->getChild(0), level+1, os);
            postorder(pNode->getChild(1), level+1, os);
            os << " --> " << pNode->getDataRef();
        }
    }

    template <typename Function, typename... Args>
    void preorder_variadic(Function&& func, Args&&... args) {
        preorder_tmp(m_pRoot, std::forward<Function>(func), std::forward<Args>(args)...);
    }

    template <typename Function, typename... Args>
    void preorder_tmp(Node* pNode, Function&& func, Args&&... args) {
        if (pNode) {
            std::invoke(std::forward<Function>(func), pNode, std::forward<Args>(args)...);
            preorder_tmp(pNode->getChild(0), std::forward<Function>(func), std::forward<Args>(args)...);
            preorder_tmp(pNode->getChild(1), std::forward<Function>(func), std::forward<Args>(args)...);
        }
    }

    // TODO: Generalize this function to apply any function
    void preorder (ostream &os)    {   preorder (m_pRoot, 0, os);  }
    // TODO: Generalize this function to apply any function
    void preorder(Node  *pNode, size_t level, ostream &os){
        if( pNode ){   
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), level+1, os);
            preorder(pNode->getChild(1), level+1, os);            
        }
    }

    void print    (ostream &os)    {   print    (m_pRoot, 0, os);  }
    // TODO: generalize this function to apply any function
    // Google: C++ parameter packs cplusplus
    void print(Node  *pNode, size_t level, ostream &os){
        if( pNode ){
            Node *pParent = pNode->getParent();
            print(pNode->getChild(1), level+1, os);
            for(size_t i = 0; i < level; ++i){
				os << string(" | ");
			}
            os << pNode->getDataRef() << "(" << (pParent?to_string(pParent->getData()):"Root") << ")" <<endl;
            print(pNode->getChild(0), level+1, os);
        }
    }

    // TODO: Tip: recorrer el arbol en preorden
    void Write(ostream &os) { os << *this;  }

    // TODO: Leer en el arbol desde un stream asumiendo que esta en preorden
    void Read(istream &is)  { /* TODO */  }
};

// TODO: este operator << debe seguir estando fuera de la clase
template <typename Traits>
ostream & operator<<(std::ostream &os, CBinaryTree<Traits> &obj){
    os << "CBinaryTree with " << obj.size() << " elements.";
    obj.inorder(os);
    return os;
}

template <typename Traits>
void CBinaryTree<Traits>::Destroy(Node* pNode){
    if (!pNode) return;
    Destroy(pNode->m_pChild[0]); 
    // pNode->m_pChild[0] = nullptr;
    Destroy(pNode->m_pChild[1]); 
    // pNode->m_pChild[1] = nullptr;
    delete pNode;
}

template <typename Traits>
CBinaryTree<Traits>::~CBinaryTree(){
    Destroy(m_pRoot);
    m_pRoot = nullptr;
    m_size  = 0;
}

template <typename Traits>
istream & operator>>(istream &is, CBinaryTree<Traits> &obj){
    // Leer el arbol
    return is;
}

void DemoBinaryTree();

#endif // __BINARY_TREE_H__