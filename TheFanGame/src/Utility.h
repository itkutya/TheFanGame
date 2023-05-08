#pragma once

class NonCopyable
{
protected:
    NonCopyable() noexcept = default;
    virtual ~NonCopyable() noexcept = default;
public:
    NonCopyable(const NonCopyable&) = delete;
    const NonCopyable& operator=(const NonCopyable&) = delete;
};

template<typename T>
class Singleton : public NonCopyable
{
protected:
    Singleton() noexcept = default;
    virtual ~Singleton() noexcept = default;
public:
    [[nodiscard]] static T& getInstance() noexcept;
};

template<typename T>
inline T& Singleton<T>::getInstance() noexcept
{
    static T instance;
    return instance;
}