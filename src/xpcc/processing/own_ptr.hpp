#pragma once

/* Poor's man std::unique_ptr
 *
 * OwnPtr is a container for a pointer declaring ownership.
 *
 * The goal is to allow to own object, pass it around and automatically delete
 * the pointer when the container goes out of scope.
 *
 * In order to pass it around the move constructor or move assignment operator
 * must be used. operator*, operator-> and the get() method can be used to get
 * the pointer contained in the OwnPtr container.
 *
 * The OwnPtr name comes from similar class in WebKit, before they switched to
 * std::unique_ptr. The implementation is different/simpler. We need our own
 * implementation since the <memory> header (and thus std::unique_ptr) is not
 * compatible with the PX4 toolchain and/or nuttx headers.
 */
namespace xpcc {

template<typename T>
class OwnPtr {
public:
    OwnPtr() : _ptr(nullptr) { }
    OwnPtr(std::nullptr_t) : _ptr(nullptr) { }

    /* non-copyable */
    OwnPtr(const OwnPtr<T> &other) = delete;

    /* Allow construction from a derived class U */
    template<typename U>
    OwnPtr(OwnPtr<U>&& other) : _ptr(other.leak()) { }

    OwnPtr(T *ptr) : _ptr(ptr) { }

    OwnPtr<T>& operator=(std::nullptr_t) { clear(); return *this; }

    template<typename U>
    OwnPtr<T>& operator=(OwnPtr<U>&& other)
    {
        T *old = _ptr;
        _ptr = other.leak();
        delete old;
        return *this;
    }

    template<typename U>
    OwnPtr<T>& operator=(U *other)
    {
        T *old = _ptr;
        _ptr = other;
        delete old;
        return *this;
    }

    ~OwnPtr() { delete _ptr; }

    void clear()
    {
        delete leak();
    }

    T *leak()
    {
        T *old = _ptr;
        _ptr = nullptr;
        return old;
    }

    T *get() const
    {
        return _ptr;
    }

    T& operator*() const { return *_ptr; }
    T *operator->() const { return _ptr; }
    bool operator !() const { return !_ptr; }
    explicit operator bool() const { return _ptr != nullptr; }

private:
    T *_ptr;
};

template<typename T>
inline bool operator==(T* a, const OwnPtr<T>& b)
{
    return a == b.get();
}

template<typename T>
inline bool operator==(const OwnPtr<T>& a, T* b)
{
    return a.get() == b;
}

template<typename T>
inline bool operator!=(T* a, const OwnPtr<T>& b)
{
    return a != b.get();
}

template<typename T>
inline bool operator!=(const OwnPtr<T>& a, T* b)
{
    return a.get() != b;
}

}
