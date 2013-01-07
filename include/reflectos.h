/*=============================================================================

Copyright (c) 2011 Ville Ruusutie

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

=============================================================================*/
#pragma once
#ifndef REFLECTOS2_H
#define REFLECTOS2_H

#define REFLECTOS_USE_FASTDELEGATES

#include <new>
#include <type_traits>
#ifdef REFLECTOS_USE_FASTDELEGATES
#   define REFLECTOS_DELEGATE  FastDelegate
#   include "FastDelegate.h"
#else
#   define REFLECTOS_DELEGATE  std::function
#   include <functional>
#endif

namespace reflectos {

struct FieldInfo;
struct FunctionInfo;

namespace internal {
    template<typename T,typename Base,bool IsClass> struct TypeStorage;
	template<typename T> struct type_inspect_base;
}
typedef unsigned int uint32;
//-----------------------------------------------------------------------------

#define REFLECT_CLASS(NAME)				    REFLECT_CLASS_(NAME)
#define REFLECT_VIRTUAL_CLASS(NAME,BASE)    REFLECT_VIRTUAL_CLASS_(NAME,BASE)
#define REFLECT_FIELD(NAME)                 REFLECT_FIELD_(NAME)
#define REFLECT_FUNCTION(NAME)              REFLECT_FUNCTION_(NAME)
#define REFLECT_END()                       REFLECT_END_()
#define REGISTER_CLASS(NAME)                REGISTER_CLASS_(NAME)
#define REGISTER_TYPE(NAME)                 REGISTER_TYPE_(NAME)
//-----------------------------------------------------------------------------

struct TypeInfo
{
    const char*     name() const;
    uint32          id() const;
    size_t          size() const;
    bool            isClass() const;
    bool            isPOD() const;
    bool            isAbstract() const;
    bool            isPolymorphic() const;
    bool            hasSimpleConstructor() const;
    TypeInfo const* base() const;
    TypeInfo const* next() const;

    virtual void*   construct() const = 0;
    virtual void*   constructArray(size_t count) const = 0;
    virtual void    constructInPlace(void* ptr) const = 0;
    virtual void    destroy(void* ptr) const = 0;
    virtual void    destroyArray(void* ptr) const = 0;
    virtual void    destroyInPlace(void* ptr) const = 0;
    virtual void    copy(void* dst, void const* src) const = 0;

    virtual FieldInfo const*    getField(uint32 id) const = 0;
    virtual FieldInfo const*    getField(const char* name) const = 0;
    virtual FieldInfo const*    getFirstField() const = 0;

    virtual FunctionInfo const* getFunction(uint32 id) const = 0;
    virtual FunctionInfo const* getFunction(const char* name) const = 0;
    virtual FunctionInfo const* getFirstFunction() const = 0;

protected:
    uint32 m_id;
    uint32 m_flags;
    size_t m_size;
    TypeInfo* m_next;
    TypeInfo* m_base;
    char const* m_name;
};
//-----------------------------------------------------------------------------

struct FieldInfo
{
    uint32                      id() const;
    uint32                      offset() const;
    char const*                 name() const;
    FieldInfo const*            next() const;

public:
    uint32 m_id;
    uint32 m_offset;
    char const* m_name;
    FieldInfo* m_next;
};
//-----------------------------------------------------------------------------

using fastdelegate::FastDelegate;

struct FunctionInfo
{
    typedef void (*PtrType)();

    uint32                      id() const;
    char const*                 name() const;
    FunctionInfo const*         next() const;

    template<typename Signature>
    REFLECTOS_DELEGATE<Signature> asDelegate(void* obj) const;

    template<typename R>
    R                           call0(void* obj) const;
    template<typename R,typename A>
    R                           call1(void* obj, A) const;
    template<typename R,typename A,typename B>
    R                           call2(void* obj, A, B) const;
    template<typename R,typename A,typename B,typename C>
    R                           call3(void* obj, A, B, C) const;
    template<typename R,typename A,typename B,typename C,typename D>
    R                           call4(void* obj, A, B, C, D) const;
    template<typename R,typename A,typename B,typename C,typename D,typename E>
    R                           call5(void* obj, A, B, C, D, E) const;
    template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
    R                           call6(void* obj, A, B, C, D, E, F) const;

protected:
    PtrType m_ptr;
    char const* m_name;
    FunctionInfo* m_next;

    uint32 m_id;
};
//-----------------------------------------------------------------------------

template<typename T>
struct type_inspect : internal::TypeStorage<T,typename internal::type_inspect_base<T>::type,std::is_class<T>::value>
{
    static char const*      name();
    static uint32           id();
    static size_t           size();
    static bool             isClass();
    static bool             isPOD();
    static bool             isAbstract();
    static bool             isPolymorphic();
    static bool             hasSimpleConstructor();

    static void*            construct();
    static void*            constructArray(size_t count);
    static void             constructInPlace(void* ptr);
    static void             destroy(void* ptr);
    static void             destroyArray(void* ptr);
    static void             destroyInPlace(void* ptr);
    static void             copy(void* dst, void const* src);

    static FieldInfo const*     getField(uint32 id);
    static FieldInfo const*     getField(char const* name);
    static FieldInfo const*     getFirstField();

    static FunctionInfo const*  getFunction(uint32 id);
    static FunctionInfo const*  getFunction(char const* name);
    static FunctionInfo const*  getFirstFunction();
};
//-----------------------------------------------------------------------------

template<typename Function>
struct function_inspect;
// Type, returns, count, R, A, B, C,..
//-----------------------------------------------------------------------------

template<typename T,typename Function>
struct member_function_inspect;
// Type, count, R, A, B, C,..
//-----------------------------------------------------------------------------

TypeInfo const* inspect(uint32 id);
TypeInfo const* inspect(char const* name);

template<typename T>
TypeInfo const* inspect(T const&);

//=============================================================================
// Implementation starts here
//=============================================================================

namespace internal {

inline uint32 fnv1a_hash(const char* str)
{
    uint32 value = 16777619U; // basis
    while (*str != 0) {
        value ^= str[0];
        value *= 2166136261U; // prime
        ++str;
    }
    return value;
}
//-----------------------------------------------------------------------------

template<typename T>
struct has_simple_constructor
{
    template<typename C> static std::true_type test(int, decltype(C())* a = nullptr);
    template<typename> static std::false_type test(...);
    typedef decltype(test<T>(0)) type;
    static const bool value = type::value & !std::is_abstract<T>::value;
};
//----------------------------------------------------------------------------

struct empty {};

template<typename T>
struct type_inspect_base
{
	template<typename I> struct has { typedef type_inspect<typename I::base_t> base; };
	template<typename I> static has<I> test(typename I::base_t* a = nullptr);
    template<typename> static empty test(...);
    typedef decltype(test<T>()) type;
};
//-----------------------------------------------------------------------------

template<typename T>
struct RegistryImpl
{
    static TypeInfo* s_typeList;

    static TypeInfo const* getType(uint32 id)
    {
        TypeInfo const* c = s_typeList;
        while(c != nullptr) {
            if(c->id() == id)
                return c;
            c = c->next();
        }
        return nullptr;
    }
};

template<typename T>
TypeInfo* RegistryImpl<T>::s_typeList = nullptr;

typedef RegistryImpl<void> Registry;
//-----------------------------------------------------------------------------

#ifdef REFLECTOS_USE_FASTDELEGATES
#   define REFLECTOS_BIND0(P,FP) FastDelegate<R()>(P,FP)
#   define REFLECTOS_BIND1(P,FP) FastDelegate<R(A)>(P,FP)
#   define REFLECTOS_BIND2(P,FP) FastDelegate<R(A,B)>(P,FP)
#   define REFLECTOS_BIND3(P,FP) FastDelegate<R(A,B,C)>(P,FP)
#   define REFLECTOS_BIND4(P,FP) FastDelegate<R(A,B,C,D)>(P,FP)
#   define REFLECTOS_BIND5(P,FP) FastDelegate<R(A,B,C,D,E)>(P,FP)
#   define REFLECTOS_BIND6(P,FP) FastDelegate<R(A,B,C,D,E,F)>(P,FP)
#else
#   define REFLECTOS_BIND0(P,FP) std::bind(FP,P)
#   define REFLECTOS_BIND1(P,FP) std::bind(FP,P,_1)
#   define REFLECTOS_BIND2(P,FP) std::bind(FP,P,_1,_2)
#   define REFLECTOS_BIND3(P,FP) std::bind(FP,P,_1,_2,_3)
#   define REFLECTOS_BIND4(P,FP) std::bind(FP,P,_1,_2,_3,_4)
#   define REFLECTOS_BIND5(P,FP) std::bind(FP,P,_1,_2,_3,_4,_5)
#   define REFLECTOS_BIND6(P,FP) std::bind(FP,P,_1,_2,_3,_4,_5,_6)
#endif
//-----------------------------------------------------------------------------

struct FunctionInfoImpl : public FunctionInfo
{
    FunctionInfoImpl(char const* name, FunctionInfo::PtrType ptr, FunctionInfo** prev) {
        m_ptr = ptr;
        m_name = name;
        m_next = *prev;
        m_id = fnv1a_hash(name);
        (*prev) = this;
    }
};
//-----------------------------------------------------------------------------

template<typename Signature>
struct FunctionPtr;

template<typename R>
struct FunctionPtr<R(*)()> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R()> ptr(void* ptr)            { return REFLECTOS_BIND0(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A>
struct FunctionPtr<R(*)(A)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R(A)> ptr(void* ptr)           { return REFLECTOS_BIND1(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B>
struct FunctionPtr<R(*)(A,B)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R(A,B)> ptr(void* ptr)         { return REFLECTOS_BIND2(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C>
struct FunctionPtr<R(*)(A,B,C)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R(A,B,C)> ptr(void* ptr)       { return REFLECTOS_BIND3(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C,typename D>
struct FunctionPtr<R(*)(A,B,C,D)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R(A,B,C,D)> ptr(void* ptr)     { return REFLECTOS_BIND4(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C,typename D,typename E>
struct FunctionPtr<R(*)(A,B,C,D,E)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R(A,B,C,D,E)> ptr(void* ptr)   { return REFLECTOS_BIND5(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct FunctionPtr<R(*)(A,B,C,D,E,F)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static REFLECTOS_DELEGATE<R(A,B,C,D,E,F)> ptr(void* ptr) { return REFLECTOS_BIND6(static_cast<T*>(ptr), Func); }
};
//-----------------------------------------------------------------------------

enum Flags {
    flag_pod                = (1<<0),
    flag_class              = (1<<1),
    flag_abstract           = (1<<2),
    flag_polymorphic        = (1<<3),
    flag_simple_constructor = (1<<4)
};

template<typename T>
struct TypeInfoImpl : public TypeInfo
{
    static_assert(std::is_class<T>::value == false, "Type is a class. You need to use REGISTER_CLASS for this");
    TypeInfoImpl(char const* name) {
        m_id = fnv1a_hash(name);
        m_size = sizeof(T);
        m_flags = flag_pod | flag_simple_constructor;
        m_name = name;
        m_base = nullptr;
        m_next = Registry::s_typeList;
        Registry::s_typeList = this;
    }

    virtual void* construct() const                     { return new T(); }
    virtual void* constructArray(size_t count) const    { return new T[count]; }
    virtual void constructInPlace(void* ptr) const      { new (ptr) T(); }
    virtual void destroy(void* ptr) const               { delete static_cast<T*>(ptr); }
    virtual void destroyArray(void* ptr) const          { delete [] static_cast<T*>(ptr); }
    virtual void destroyInPlace(void* ptr) const        { static_cast<T*>(ptr)->~T(); }
    virtual void copy(void* dst, void const* src) const { *static_cast<T*>(dst) = *static_cast<T const*>(src); }

    virtual FieldInfo const* getField(uint32 id) const              { return nullptr; }
    virtual FieldInfo const* getField(const char* name) const       { return nullptr; }
    virtual FieldInfo const* getFirstField() const                  { return nullptr; }

    virtual FunctionInfo const* getFirstFunction() const            { return nullptr; }
    virtual FunctionInfo const* getFunction(uint32 id) const        { return nullptr; }
    virtual FunctionInfo const* getFunction(const char* name) const { return nullptr; }
};
//-----------------------------------------------------------------------------

template<typename T,bool Can> struct constructClass {
    static void* one()               { return new T(); }
    static void* many(size_t count)  { return new T[count]; }
    static void inplace(void* ptr)   { new (ptr) T(); }
};

template<typename T> struct constructClass<T,false> {
    static void* one()               { return nullptr; }
    static void* many(size_t)        { return nullptr; }
    static void inplace(void*)       {}
};
//-----------------------------------------------------------------------------

template<typename T>
struct ClassInfoImpl : public TypeInfo
{
    static_assert(std::is_class<T>::value == true, "Type isn't a class. You need to use REGISTER_TYPE for this");
    ClassInfoImpl(const char* name) {
        m_id = fnv1a_hash(name);
        m_size = sizeof(T);
        m_flags = flag_class;
        m_flags |= std::is_pod<T>::value ? flag_pod : 0;
        m_flags |= std::is_abstract<T>::value ? flag_abstract : 0;
        m_flags |= std::is_polymorphic<T>::value ? flag_polymorphic : 0;
        m_flags |= has_simple_constructor<T>::value ? flag_simple_constructor : 0;
        m_name = name;
        m_firstFunction = nullptr;
        m_base = nullptr;
        m_next = Registry::s_typeList;
        Registry::s_typeList = this;
    }

    virtual void* construct() const                     { return constructClass<T,has_simple_constructor<T>::value>::one(); }
    virtual void* constructArray(size_t count) const    { return constructClass<T,has_simple_constructor<T>::value>::many(count); }
    virtual void constructInPlace(void* ptr) const      { constructClass<T,has_simple_constructor<T>::value>::inplace(ptr); }
    virtual void destroy(void* ptr) const               { delete static_cast<T*>(ptr); }
    virtual void destroyArray(void* ptr) const          { delete [] static_cast<T*>(ptr); }
    virtual void destroyInPlace(void* ptr) const        { static_cast<T*>(ptr)->~T(); }
    virtual void copy(void* dst, void const* src) const { *static_cast<T*>(dst) = *static_cast<T const*>(src); }

    virtual FieldInfo const* getFirstField() const                  { return m_firstField; }
    virtual FieldInfo const* getField(const char* name) const       { return getField(fnv1a_hash(name)); }
    virtual FieldInfo const* getField(uint32 id) const
    {
        FieldInfo const* ite = m_firstField;
        while(ite != nullptr) {
            if(ite->id() == id)
                return ite;
            ite = ite->next();
        }
        return nullptr;
    }

    virtual FunctionInfo const* getFirstFunction() const            { return m_firstFunction; }
    virtual FunctionInfo const* getFunction(const char* name) const { return getFunction(fnv1a_hash(name)); }
    virtual FunctionInfo const* getFunction(uint32 id) const
    {
        FunctionInfo const* ite = m_firstFunction;
        while(ite != nullptr) {
            if(ite->id() == id)
                return ite;
            ite = ite->next();
        }
        return nullptr;
    }

    template<typename T,typename F>
    void visitField(const char* name, T const* object, F const* address)
    {
        FieldInfo info;
        info.m_id = fnv1a_hash(name);
        info.m_name = name;
        info.m_offset = (uint32)((ptrdiff_t)address - (ptrdiff_t)object);
        info.m_next = m_firstField;
        //m_firstField = &field;
    }

    template<typename T,typename funcptr_t,funcptr_t Function>
    void visitFunction(const char* name, T const*) {
        typedef reflectos::member_function_inspect<T,funcptr_t> Inspection;
        auto funcPtr = &(FunctionPtr<Inspection::Type>::ptr<T,funcptr_t,Function>);
        static FunctionInfoImpl info(name, FunctionInfo::PtrType(funcPtr), &m_firstFunction);
    }

private:
    FieldInfo* m_firstField;
    FunctionInfo* m_firstFunction;
};
//-----------------------------------------------------------------------------

template<typename T,typename Base>
struct TypeStorage<T,Base,false>
{
    static TypeInfoImpl<T> info;
};

template<typename T,typename Base>
struct TypeStorage<T,Base,true> : public Base
{
    static ClassInfoImpl<T> info;
};

} // end of internal

//-----------------------------------------------------------------------------

#define REFLECT_CLASS_(NAME)\
    public:\
        template<typename Visitor>\
        static void reflect(Visitor* visitor, NAME const* c)\
        {\
            typedef NAME ThisType;
#define REFLECT_VIRTUAL_CLASS_(NAME,BASE)\
    public:\
		typedef BASE base_t;\
        template<typename Visitor>\
        static void reflect(Visitor* visitor, NAME const* c)\
        {\
            typedef NAME ThisType;
#define REFLECT_FIELD_(NAME)\
            visitor->visitField(#NAME, c, &c->NAME);
#define REFLECT_FUNCTION_(NAME)\
            visitor->visitFunction<ThisType,decltype(&ThisType::NAME),&ThisType::NAME>(#NAME, c);
#define REFLECT_END_()\
        }

#define REGISTER_CLASS_(NAME)\
    reflectos::internal::ClassInfoImpl<NAME> reflectos::internal::TypeStorage<NAME,typename reflectos::internal::type_inspect_base<NAME>::type,true>::info(#NAME);
//-----------------------------------------------------------------------------

#define REGISTER_TYPE_(NAME)\
	reflectos::internal::TypeInfoImpl<NAME> reflectos::internal::TypeStorage<NAME,reflectos::internal::empty,false>::info(#NAME);
//-----------------------------------------------------------------------------

inline const char* TypeInfo::name() const           { return m_name; }
inline uint32 TypeInfo::id() const                  { return m_id; }
inline size_t TypeInfo::size() const                { return m_size; }
inline bool TypeInfo::isClass() const               { return (m_flags & internal::flag_class) != 0; }
inline bool TypeInfo::isPOD() const                 { return (m_flags & internal::flag_pod) != 0; }
inline bool TypeInfo::isAbstract() const            { return (m_flags & internal::flag_abstract) != 0; }
inline bool TypeInfo::isPolymorphic() const         { return (m_flags & internal::flag_polymorphic) != 0; }
inline bool TypeInfo::hasSimpleConstructor() const  { return (m_flags & internal::flag_simple_constructor) != 0; }
inline TypeInfo const* TypeInfo::base() const       { return m_base; }
inline TypeInfo const* TypeInfo::next() const       { return m_next; }
//-----------------------------------------------------------------------------

inline uint32 FieldInfo::id() const                                             { return m_id; }
inline char const* FieldInfo::name() const                                      { return m_name; }
inline FieldInfo const* FieldInfo::next() const                                 { return m_next; }
//-----------------------------------------------------------------------------

inline uint32 FunctionInfo::id() const                                          { return m_id; }
inline char const* FunctionInfo::name() const                                   { return m_name; }
inline FunctionInfo const* FunctionInfo::next() const                           { return m_next; }

template<typename Signature>
inline REFLECTOS_DELEGATE<Signature> FunctionInfo::asDelegate(void* obj) const  { return reinterpret_cast<REFLECTOS_DELEGATE<Signature>(*)(void*)>(m_ptr)(obj); }

template<typename R>
inline R FunctionInfo::call0(void* obj) const                                   { return asDelegate<R()>(obj)(); }

template<typename R,typename A>
inline R FunctionInfo::call1(void* obj,A a) const                               { return asDelegate<R(A)>(obj)(a); }

template<typename R,typename A,typename B>
inline R FunctionInfo::call2(void* obj,A a,B b) const                           { return asDelegate<R(A,B)>(obj)(a,b); }

template<typename R,typename A,typename B,typename C>
inline R FunctionInfo::call3(void* obj,A a,B b,C c) const                       { return asDelegate<R(A,B,C)>(obj)(a,b,c); }

template<typename R,typename A,typename B,typename C,typename D>
inline R FunctionInfo::call4(void* obj,A a,B b,C c,D d) const                   { return asDelegate<R(A,B,C,D)>(obj)(a,b,c,d); }

template<typename R,typename A,typename B,typename C,typename D,typename E>
inline R FunctionInfo::call5(void* obj,A a,B b,C c,D d,E e) const               { return asDelegate<R(A,B,C,D,E)>(obj)(a,b,c,d,e); }

template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
inline R FunctionInfo::call6(void* obj,A a,B b,C c,D d,E e,F f) const           { return asDelegate<R(A,B,C,D,E,F)>(obj)(a,b,c,d,e,f); }

//-----------------------------------------------------------------------------

template<typename T> struct type_inspect<const T> : public type_inspect<T> {};
template<typename T> struct type_inspect<T&> : public type_inspect<T> {};
template<typename T> struct type_inspect<const T&> : public type_inspect<T> {};
template<typename T> struct type_inspect<T&&> : public type_inspect<T> {};
template<typename T> struct type_inspect<T*> : public type_inspect<T> {};
template<typename T> struct type_inspect<const T*> : public type_inspect<T> {};

template<typename T> inline char const* type_inspect<T>::name()                   { return info.name(); }
template<typename T> inline uint32 type_inspect<T>::id()                          { return info.id(); }
template<typename T> inline size_t type_inspect<T>::size()                        { return sizeof(T); }
template<typename T> inline bool type_inspect<T>::isClass()                       { return std::is_class<T>::value; }
template<typename T> inline bool type_inspect<T>::isPOD()                         { return std::is_pod<T>::value; }
template<typename T> inline bool type_inspect<T>::isAbstract()                    { return std::is_abstract<T>::value; }
template<typename T> inline bool type_inspect<T>::isPolymorphic()                 { return std::is_polymorphic<T>::value; }
template<typename T> inline bool type_inspect<T>::hasSimpleConstructor()          { return internal::has_simple_constructor<T>::value; }
template<typename T> inline void* type_inspect<T>::construct()                    { return info.construct(); }
template<typename T> inline void* type_inspect<T>::constructArray(size_t count)   { return info.constructArray(count); }
template<typename T> inline void type_inspect<T>::constructInPlace(void* ptr)     { info.constructInPlace(ptr); }
template<typename T> inline void type_inspect<T>::destroy(void* ptr)              { info.destroy(ptr); }
template<typename T> inline void type_inspect<T>::destroyArray(void* ptr)         { info.destroyArray(ptr); }
template<typename T> inline void type_inspect<T>::destroyInPlace(void* ptr)       { info.destroyInPlace(ptr); }
template<typename T> inline void type_inspect<T>::copy(void* dst, void const* src) { info.copy(dst, src); }
//-----------------------------------------------------------------------------

template<typename R>
struct function_inspect<R()> {
    typedef R(*Type)();
    typedef R R;
    static const bool returns = !std::is_same<R,void>::value;
    static const int count = 0;
    static const bool is_global = false;
    static const bool is_static = false;
    static const bool is_const = false;
};

template<typename R,typename A>
struct function_inspect<R(A)> : public function_inspect<R()> {
    typedef R(*Type)(A);
    typedef A A;
    static const int count = 1;
};

template<typename R,typename A,typename B>
struct function_inspect<R(A,B)> : public function_inspect<R(A)> {
    typedef R(*Type)(A,B);
    typedef B B;
    static const int count = 2;
};

template<typename R,typename A,typename B,typename C>
struct function_inspect<R(A,B,C)> : public function_inspect<R(A,B)> {
    typedef R(*Type)(A,B,C);
    typedef C C;
    static const int count = 3;
};

template<typename R,typename A,typename B,typename C,typename D>
struct function_inspect<R(A,B,C,D)> : public function_inspect<R(A,B,C)> {
    typedef R(*Type)(A,B,C,D);
    typedef D D;
    static const int count = 4;
};

template<typename R,typename A,typename B,typename C,typename D,typename E>
struct function_inspect<R(A,B,C,D,E)> : public function_inspect<R(A,B,C,D)> {
    typedef R(*Type)(A,B,C,D,E);
    typedef E E;
    static const int count = 5;
};

template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct function_inspect<R(A,B,C,D,E,F)> : public function_inspect<R(A,B,C,D,E)> {
    typedef R(*Type)(A,B,C,D,E,F);
    typedef F F;
    static const int count = 6;
};
//----------------------------------------------------------------------------

template<typename R>
struct function_inspect<R(*)()> : public function_inspect<R()> { static const bool is_global = true; };

template<typename R,typename A>
struct function_inspect<R(*)(A)> : public function_inspect<R(A)> { static const bool is_global = true; };

template<typename R,typename A,typename B>
struct function_inspect<R(*)(A,B)> : public function_inspect<R(A,B)> { static const bool is_global = true; };

template<typename R,typename A,typename B,typename C>
struct function_inspect<R(*)(A,B,C)> : public function_inspect<R(A,B,C)> { static const bool is_global = true; };

template<typename R,typename A,typename B,typename C,typename D>
struct function_inspect<R(*)(A,B,C,D)> : public function_inspect<R(A,B,C,D)> { static const bool is_global = true; };

template<typename R,typename A,typename B,typename C,typename D,typename E>
struct function_inspect<R(*)(A,B,C,D,E)> : public function_inspect<R(A,B,C,D,E)> { static const bool is_global = true; };

template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct function_inspect<R(*)(A,B,C,D,E,F)> : public function_inspect<R(A,B,C,D,E,F)> { static const bool is_global = true; };
//----------------------------------------------------------------------------

template<typename T,typename R>
struct member_function_inspect<T,R(T::*)()> : public function_inspect<R()> {};
template<typename T,typename R>
struct member_function_inspect<T,R(T::*)()const> : public function_inspect<R()> { static const bool is_const = true; };
template<typename T,typename R>
struct member_function_inspect<T,R(*)()> : public function_inspect<R()> { static const bool is_static = true; };

template<typename T,typename R,typename A>
struct member_function_inspect<T,R(T::*)(A)> : public function_inspect<R(A)> {};
template<typename T,typename R,typename A>
struct member_function_inspect<T,R(T::*)(A)const> : public function_inspect<R(A)> { static const bool is_const = true; };
template<typename T,typename R,typename A>
struct member_function_inspect<T,R(*)(A)> : public function_inspect<R(A)> { static const bool is_static = true; };

template<typename T,typename R,typename A,typename B>
struct member_function_inspect<T,R(T::*)(A,B)> : public function_inspect<R(A,B)> {};
template<typename T,typename R,typename A,typename B>
struct member_function_inspect<T,R(T::*)(A,B)const> : public function_inspect<R(A,B)> { static const bool is_const = true; };
template<typename T,typename R,typename A,typename B>
struct member_function_inspect<T,R(*)(A,B)> : public function_inspect<R(A,B)> { static const bool is_static = true; };

template<typename T,typename R,typename A,typename B,typename C>
struct member_function_inspect<T,R(T::*)(A,B,C)> : public function_inspect<R(A,B,C)> {};
template<typename T,typename R,typename A,typename B,typename C>
struct member_function_inspect<T,R(T::*)(A,B,C)const> : public function_inspect<R(A,B,C)> { static const bool is_const = true; };
template<typename T,typename R,typename A,typename B,typename C>
struct member_function_inspect<T,R(*)(A,B,C)> : public function_inspect<R(A,B,C)> { static const bool is_static = true; };

template<typename T,typename R,typename A,typename B,typename C,typename D>
struct member_function_inspect<T,R(T::*)(A,B,C,D)> : public function_inspect<R(A,B,C,D)> {};
template<typename T,typename R,typename A,typename B,typename C,typename D>
struct member_function_inspect<T,R(T::*)(A,B,C,D)const> : public function_inspect<R(A,B,C,D)> { static const bool is_const = true; };
template<typename T,typename R,typename A,typename B,typename C,typename D>
struct member_function_inspect<T,R(*)(A,B,C,D)> : public function_inspect<R(A,B,C,D)> { static const bool is_static = true; };

template<typename T,typename R,typename A,typename B,typename C,typename D,typename E>
struct member_function_inspect<T,R(T::*)(A,B,C,D,E)> : public function_inspect<R(A,B,C,D,E)> {};
template<typename T,typename R,typename A,typename B,typename C,typename D,typename E>
struct member_function_inspect<T,R(T::*)(A,B,C,D,E)const> : public function_inspect<R(A,B,C,D,E)> { static const bool is_const = true; };
template<typename T,typename R,typename A,typename B,typename C,typename D,typename E>
struct member_function_inspect<T,R(*)(A,B,C,D,E)> : public function_inspect<R(A,B,C,D,E)> { static const bool is_static = true; };

template<typename T,typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct member_function_inspect<T,R(T::*)(A,B,C,D,E,F)> : public function_inspect<R(A,B,C,D,E,F)> {};
template<typename T,typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct member_function_inspect<T,R(T::*)(A,B,C,D,E,F)const> : public function_inspect<R(A,B,C,D,E,F)> { static const bool is_const = true; };
template<typename T,typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct member_function_inspect<T,R(*)(A,B,C,D,E,F)> : public function_inspect<R(A,B,C,D,E,F)> { static const bool is_static = true; };
//----------------------------------------------------------------------------

template<typename T> inline TypeInfo const* inspect(T const&)   { return &type_inspect<T>::info; }
template<> inline TypeInfo const* inspect(void* const&)         { return nullptr; }
inline TypeInfo const* inspect(uint32 id)                       { return internal::Registry::getType(id); }
inline TypeInfo const* inspect(char const* name)                { return internal::Registry::getType(internal::fnv1a_hash(name)); }
//----------------------------------------------------------------------------

} // end of reflectos

#endif
