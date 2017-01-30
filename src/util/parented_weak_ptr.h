#ifndef UTIL_PARENTED_WEAK_POINTER_H
#define UTIL_PARENTED_WEAK_POINTER_H

#include <QPointer>
#include "util/assert.h"


/**
 * Use this wrapper class to represent a pointer that is owned by the QT object tree, and needs to be shared
 * freely like a shared_ptr. The underlying object could become null at any time, so always check before using it.
 * The QT object tree takes care of deleting the underlying heap data when the parent goes out of scope, so no manual
 * "delete" statement is required inside this class.
**/
template <typename T>
class parented_weak_ptr {
  public:
    explicit parented_weak_ptr(T* t) : m_pObject(t) {
        DEBUG_ASSERT(t->parent() != nullptr);
    }

    /* If U* is convertible to T* then we also want parented_weak_ptr<U> convertible to parented_weak_ptr<T> */
    template <typename U>
    parented_weak_ptr(const parented_weak_ptr<U>& u, typename std::enable_if<std::is_convertible<U*, T*>::value, void>::type * = 0)
            : m_pObject(u.get()) {
        DEBUG_ASSERT(u->parent() != nullptr);
    }

    parented_weak_ptr() : m_pObject(nullptr) {}

    ~parented_weak_ptr() = default;

    T* get() const {
        return m_pObject;
    }

    T& operator* () const {
        return *m_pObject;
    }

    T* operator-> () const {
        return m_pObject;
    }

    bool operator== (const parented_weak_ptr& other) const {
        return m_pObject == other.m_ptr;
    }

    bool operator== (const T* other) const {
        return m_pObject == other;
    }

    bool operator!= (const parented_weak_ptr& other) const {
        return m_pObject != other.m_pObject;
    }

    bool operator!= (const T* other) const {
        return m_pObject != other;
    }

    operator bool() const {
        return m_pObject != nullptr;
    }

    /*
     * If U* is convertible to T* then we also want parented_weak_ptr<U> assignable to parented_weak_ptr<T>
     * E.g. parented_weak_ptr<Base> base = make_parented<Derived>(); should work as expected.
     */
    template <typename U>
    typename std::enable_if<std::is_convertible<U*, T*>::value, parented_weak_ptr<T>&>::type
            operator=(const parented_weak_ptr<U>& p) {
        m_pObject = p.get();
        return *this;
    }

  private:
    // Wrap T* in QPointer to ensure it is set to nullptr when the underlying data is deleted
    QPointer<T> m_pObject;
};

namespace {

template<typename T, typename... Args>
inline parented_weak_ptr<T> make_parented_weak(Args&&... args) {
    return parented_weak_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace

#endif /* UTIL_PARENTED_WEAK_POINTER_H */
