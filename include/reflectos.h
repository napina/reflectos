/*=============================================================================

Copyright (c) 2013 Ville Ruusutie

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
#ifndef REFLECTOS_H
#define REFLECTOS_H

#include <new>
#pragma warning(disable: 4100)
#include "FastDelegate.h"
#pragma warning(default: 4100)

namespace reflectos {

using fastdelegate::FastDelegate;
struct FieldInfo;
struct FunctionInfo;
template<typename T1,typename T2> struct is_same;

namespace internal {
    template<typename T,bool IsClass> struct TypeStorage;
    template<typename T> struct type_inspect_base;
    template<typename T> struct RegistryImpl;
}
typedef internal::RegistryImpl<void> Registry;
typedef unsigned int uint32_t;
//-----------------------------------------------------------------------------

#define REFLECT_CLASS(NAME)                 XXX_REFLECT_CLASS_(NAME)
#define REFLECT_BASE_CLASS(NAME)            XXX_REFLECT_BASE_CLASS_(NAME)
#define REFLECT_ABSTRACT_CLASS(NAME)        XXX_REFLECT_ABSTRACT_CLASS_(NAME)
#define REFLECT_DERIVED_CLASS(NAME,BASE)    XXX_REFLECT_DERIVED_CLASS_(NAME,BASE)
#define REFLECT_FIELD(NAME)                 XXX_REFLECT_FIELD_(NAME)
#define REFLECT_FUNCTION(NAME)              XXX_REFLECT_FUNCTION_(NAME)
#define REFLECT_END()                       XXX_REFLECT_END_()
#define REGISTER_CLASS(NAME,VERSION)        XXX_REGISTER_CLASS_(NAME,VERSION)
#define REGISTER_TYPE(NAME)                 XXX_REGISTER_TYPE_(NAME)
//-----------------------------------------------------------------------------

struct TypeInfo
{
    const char*     name() const;
    uint32_t        id() const;
    uint32_t        version() const;
    size_t          size() const;
    bool            isClass() const;
    bool            isPOD() const;
    bool            isAbstract() const;
    bool            isPolymorphic() const;
    bool            isReflected() const;
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

    virtual FieldInfo const*    getField(uint32_t id) const = 0;
    virtual FieldInfo const*    getField(const char* name) const = 0;
    virtual FieldInfo const*    getFirstField() const = 0;

    virtual FunctionInfo const* getFunction(uint32_t id) const = 0;
    virtual FunctionInfo const* getFunction(const char* name) const = 0;
    virtual FunctionInfo const* getFirstFunction() const = 0;

protected:
    friend Registry;
    virtual void init() = 0;

    uint32_t m_id;
    uint32_t m_size : 16;
    uint32_t m_flags : 8;
    uint32_t m_version : 8;
    TypeInfo* m_next;
    TypeInfo* m_base;
    char const* m_name;
    // TODO add padding and sort if needed
};
//-----------------------------------------------------------------------------

struct FieldInfo
{
    uint32_t                    id() const;
    uint32_t                    offset() const;
    char const*                 name() const;
    TypeInfo const*             type() const;
    FieldInfo const*            next() const;

public:
    uint32_t m_id;
    uint32_t m_offset;
    TypeInfo* m_type;
    FieldInfo* m_next;
    char const* m_name;
};
//-----------------------------------------------------------------------------

#ifdef REFLECTOS_USE_FASTDELEGATES
using fastdelegate::FastDelegate;
#endif

struct FunctionInfo
{
    typedef void (*PtrType)();

    uint32_t                    id() const;
    char const*                 name() const;
    FunctionInfo const*         next() const;

    template<typename Signature>
    FastDelegate<Signature> asDelegate(void* obj) const;

    template<typename R>
    R                           call(void* obj) const;
    template<typename R,typename A>
    R                           call(void* obj, A) const;
    template<typename R,typename A,typename B>
    R                           call(void* obj, A, B) const;
    template<typename R,typename A,typename B,typename C>
    R                           call(void* obj, A, B, C) const;
    template<typename R,typename A,typename B,typename C,typename D>
    R                           call(void* obj, A, B, C, D) const;
    template<typename R,typename A,typename B,typename C,typename D,typename E>
    R                           call(void* obj, A, B, C, D, E) const;
    template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
    R                           call(void* obj, A, B, C, D, E, F) const;

protected:
    PtrType m_ptr;
    char const* m_name;
    FunctionInfo* m_next;
    uint32_t m_id;
    // TODO padding for 64bit architecture?
};
//-----------------------------------------------------------------------------

template<typename T>
struct type_inspect : internal::TypeStorage<T,__is_class(T) & !__is_pod(T)>
{
    static char const*      name();
    static uint32_t         id();
    static size_t           size();
    static bool             isClass();
    static bool             isPOD();
    static bool             isAbstract();
    static bool             isPolymorphic();
    static bool             hasSimpleConstructor();
    static TypeInfo const*  type();

    static void*            construct();
    static void*            constructArray(size_t count);
    static void             constructInPlace(void* ptr);
    static void             destroy(void* ptr);
    static void             destroyArray(void* ptr);
    static void             destroyInPlace(void* ptr);
    static void             copy(void* dst, void const* src);

    static FieldInfo const*     getField(uint32_t id);
    static FieldInfo const*     getField(char const* name);
    static FieldInfo const*     getFirstField();

    static FunctionInfo const*  getFunction(uint32_t id);
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
// Type, returns, count, R, A, B, C,..
//-----------------------------------------------------------------------------

TypeInfo const* inspect(uint32_t id);
TypeInfo const* inspect(char const* name);

template<typename T>
TypeInfo const* inspect(T const* ptr);

template<typename Visitor,typename T>
void reflect(Visitor* visitor, T const* obj);

//=============================================================================
// Implementation starts here
//=============================================================================

namespace internal {

inline uint32_t fnv1a_hash(const char* str)
{
    uint32_t value = 2166136261U; // basis
    do {
        value ^= str[0];
        value *= 16777619u; // prime
    } while((*str++) != 0);
    return value;
}
//-----------------------------------------------------------------------------

struct true_type  { enum { value = true  }; };
struct false_type { enum { value = false }; };

template<typename T>
struct has_simple_constructor
{
    template<typename C> static true_type test(decltype(C())* a);
    template<typename> static false_type test(...);
    typedef decltype(test<T>(nullptr)) type;
    static const bool value = type::value & !__is_abstract(T);
};

template<typename T>
struct has_trivial_destructor
{
    static const bool value = __has_trivial_destructor(T) | __is_abstract(T);
};
//----------------------------------------------------------------------------

template<typename T>
struct has_base
{
    template<typename I> static true_type test(typename I::base_t* a);
    template<typename> static false_type test(...);
    typedef decltype(test<T>(nullptr)) type;
    static const bool value = type::value;
};

template<typename T,bool Has> struct define_base    {};
template<typename T> struct define_base<T,true>     { typedef type_inspect<typename T::base_t> base; };

template<typename T,bool Has> struct get_base       { static TypeInfo* get() { return nullptr; } };
template<typename T> struct get_base<T,true>        { static TypeInfo* get() { return (TypeInfo*)Registry::getType(type_inspect<typename T::base_t>::id()); } };
//-----------------------------------------------------------------------------

template<typename T>
struct has_typeinfo
{
    template<typename I> static true_type test(decltype(&I::getTypeInfo)* a);
    template<typename> static false_type test(...);
    typedef decltype(test<T>(nullptr)) type;
    static const bool value = type::value;
};

template<typename T,bool Has> struct get_typeinfo   { static TypeInfo const* get(T const* /*obj*/)    { return &type_inspect<T>::info; } };
template<typename T> struct get_typeinfo<T,true>    { static TypeInfo const* get(T const* obj)        { return obj->getTypeInfo(); } };
//----------------------------------------------------------------------------

template<typename T>
struct RegistryImpl
{
    static TypeInfo* s_typeList;

    static TypeInfo const* getType(uint32_t id)
    {
        TypeInfo const* c = s_typeList;
        while(c != nullptr) {
            if(c->id() == id)
                return c;
            c = c->next();
        }
        return nullptr;
    }

    static void init()
    {
        TypeInfo* c = s_typeList;
        while(c != nullptr) {
            c->init();
            c = const_cast<TypeInfo*>(c->next());
        }
    }

    static void init(TypeInfo* type)
    {
        type->init();
    }

    static void merge(TypeInfo* list)
    {
        TypeInfo* c = list;
        while(c != nullptr) {
            TypeInfo* next = const_cast<TypeInfo*>(c->next());
            if(getType(c->id()) == nullptr) {
                c->m_next = s_typeList;
                s_typeList = c;
            }
            c = next;
        }
    }
};

template<typename T>
TypeInfo* RegistryImpl<T>::s_typeList = nullptr;
//-----------------------------------------------------------------------------

#define REFLECTOS_BIND0(P,FP) FastDelegate<R()>(P,FP)
#define REFLECTOS_BIND1(P,FP) FastDelegate<R(A)>(P,FP)
#define REFLECTOS_BIND2(P,FP) FastDelegate<R(A,B)>(P,FP)
#define REFLECTOS_BIND3(P,FP) FastDelegate<R(A,B,C)>(P,FP)
#define REFLECTOS_BIND4(P,FP) FastDelegate<R(A,B,C,D)>(P,FP)
#define REFLECTOS_BIND5(P,FP) FastDelegate<R(A,B,C,D,E)>(P,FP)
#define REFLECTOS_BIND6(P,FP) FastDelegate<R(A,B,C,D,E,F)>(P,FP)
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
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R()> ptr(void* ptr)            { return REFLECTOS_BIND0(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A>
struct FunctionPtr<R(*)(A)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R(A)> ptr(void* ptr)           { return REFLECTOS_BIND1(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B>
struct FunctionPtr<R(*)(A,B)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R(A,B)> ptr(void* ptr)         { return REFLECTOS_BIND2(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C>
struct FunctionPtr<R(*)(A,B,C)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R(A,B,C)> ptr(void* ptr)       { return REFLECTOS_BIND3(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C,typename D>
struct FunctionPtr<R(*)(A,B,C,D)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R(A,B,C,D)> ptr(void* ptr)     { return REFLECTOS_BIND4(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C,typename D,typename E>
struct FunctionPtr<R(*)(A,B,C,D,E)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R(A,B,C,D,E)> ptr(void* ptr)   { return REFLECTOS_BIND5(static_cast<T*>(ptr), Func); }
};

template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
struct FunctionPtr<R(*)(A,B,C,D,E,F)> {
    template<typename T,typename funcptr_t,funcptr_t Func> static FastDelegate<R(A,B,C,D,E,F)> ptr(void* ptr) { return REFLECTOS_BIND6(static_cast<T*>(ptr), Func); }
};
//-----------------------------------------------------------------------------

enum Flags {
    flag_pod                = (1<<1),
    flag_class              = (1<<2),
    flag_abstract           = (1<<3),
    flag_polymorphic        = (1<<4),
    flag_simple_constructor = (1<<5),
    flag_reflected          = (1<<6),
};

template<typename T>
struct TypeInfoImpl : public TypeInfo
{
    static_assert(!__is_class(T) | __is_pod(T), "Type is a class. You need to use REGISTER_CLASS for this");
    TypeInfoImpl(char const* name) {
        m_id = fnv1a_hash(name);
        m_version = 0;
        m_flags = flag_pod | flag_simple_constructor | flag_reflected;
        m_size = sizeof(T);
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
    virtual void destroyInPlace(void* /*ptr*/) const    {}
    virtual void copy(void* dst, void const* src) const { *static_cast<T*>(dst) = *static_cast<T const*>(src); }

    virtual FieldInfo const* getField(uint32_t /*id*/) const        { return nullptr; }
    virtual FieldInfo const* getField(const char* /*name*/) const   { return nullptr; }
    virtual FieldInfo const* getFirstField() const                  { return nullptr; }

    virtual FunctionInfo const* getFirstFunction() const                { return nullptr; }
    virtual FunctionInfo const* getFunction(uint32_t /*id*/) const      { return nullptr; }
    virtual FunctionInfo const* getFunction(const char* /*name*/) const { return nullptr; }

protected:
    virtual void init() {}
};
//-----------------------------------------------------------------------------

template<typename T,bool Simple> struct constructClass {
    static void* one()               { return new T(); }
    static void* many(size_t count)  { return new T[count]; }
    static void inplace(void* ptr)   { new (ptr) T(); }
};

template<typename T> struct constructClass<T,false> {
    static void* one()               { return nullptr; }
    static void* many(size_t)        { return nullptr; }
    static void inplace(void*)       {}
};

template<typename T,bool Trivial> struct destructClass {
    static void destroy(void*)       {}
};

template<typename T> struct destructClass<T,false> {
    static void destroy(void* ptr)   { static_cast<T*>(ptr)->~T(); }
};
//-----------------------------------------------------------------------------

template<typename T>
struct ClassInfoImpl : public TypeInfo
{
    static_assert(__is_class(T) == true, "Type isn't a class. You need to use REGISTER_TYPE for this");
    ClassInfoImpl(const char* name, uint32_t version) {
        m_id = fnv1a_hash(name);
        m_version = version;
        m_size = sizeof(T);
        m_flags = flag_class;
        m_flags |= __is_pod(T) ? flag_pod : 0;
        m_flags |= __is_abstract(T) ? flag_abstract : 0;
        m_flags |= __is_polymorphic(T) ? flag_polymorphic : 0;
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
    virtual void destroyInPlace(void* ptr) const        { destructClass<T,has_trivial_destructor<T>::value>::destroy(ptr); }
    virtual void copy(void* dst, void const* src) const { *static_cast<T*>(dst) = *static_cast<T const*>(src); }

    virtual FieldInfo const* getFirstField() const                  { return m_firstField; }
    virtual FieldInfo const* getField(const char* name) const       { return getField(fnv1a_hash(name)); }
    virtual FieldInfo const* getField(uint32_t id) const
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
    virtual FunctionInfo const* getFunction(uint32_t id) const
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
    void visitField(const char* name, T const* object, F const* field)
    {
        static FieldInfo info;
        info.m_id = fnv1a_hash(name);
        info.m_name = name;
        info.m_type = const_cast<TypeInfo*>(type_inspect<F>::type());
        info.m_offset = uint32_t((ptrdiff_t)field - (ptrdiff_t)object);
        info.m_next = m_firstField;
        m_firstField = &info;
    }

    template<typename T,typename funcptr_t,funcptr_t Function>
    void visitFunction(const char* name, T const*)
    {
        typedef member_function_inspect<T,funcptr_t> Inspection;
        auto funcPtr = &(FunctionPtr<Inspection::Type>::ptr<T,funcptr_t,Function>);
        static FunctionInfoImpl info(name, FunctionInfo::PtrType(funcPtr), &m_firstFunction);
    }

protected:
    virtual void init()
    {
        // TODO get rid of this nasty code
        if(isReflected())
            return;

        m_base = get_base<T,has_base<T>::value>::get();
        if(m_base) {
            Registry::init(m_base);
        }

        T::reflect(this, nullptr);
        m_flags |= internal::flag_reflected;
    }

private:
    FieldInfo* m_firstField;
    FunctionInfo* m_firstFunction;
};
//-----------------------------------------------------------------------------

template<typename T>
struct TypeStorage<T,false>
{
    static TypeInfoImpl<T> info;

    template<typename Visitor,typename T>
    inline static void reflect(Visitor*,T const*) {};
};

template<typename T>
struct TypeStorage<T,true> : public internal::define_base<T,internal::has_base<T>::value>
{
    static ClassInfoImpl<T> info;

    template<typename Visitor,typename T>
    inline static void reflect(Visitor* visitor,T const* obj) { T::reflect(visitor, obj); };
};

} // end of internal

//-----------------------------------------------------------------------------

#define XXX_REFLECT_CLASS_(NAME)\
    public:\
        reflectos::TypeInfo const* getTypeInfo() const { return &reflectos::type_inspect<NAME>::info; }\
        template<typename Visitor>\
        static void reflect(Visitor* visitor, NAME const* c)\
        {\
            typedef NAME ThisType;
#define XXX_REFLECT_BASE_CLASS_(NAME)\
    public:\
        virtual reflectos::TypeInfo const* getTypeInfo() const { return &reflectos::type_inspect<NAME>::info; }\
        template<typename Visitor>\
        static void reflect(Visitor* visitor, NAME const* c)\
        {\
            typedef NAME ThisType;
#define XXX_REFLECT_ABSTRACT_CLASS_(NAME)\
    public:\
        virtual reflectos::TypeInfo const* getTypeInfo() const = 0;\
        template<typename Visitor>\
        static void reflect(Visitor* visitor, NAME const* c)\
        {\
            typedef NAME ThisType;
#define XXX_REFLECT_DERIVED_CLASS_(NAME,BASE)\
    public:\
        virtual reflectos::TypeInfo const* getTypeInfo() const { return &reflectos::type_inspect<NAME>::info; }\
        typedef BASE base_t;\
        template<typename Visitor>\
        static void reflect(Visitor* visitor, NAME const* c)\
        {\
            typedef NAME ThisType;\
            BASE::reflect(visitor, c);
#define XXX_REFLECT_FIELD_(NAME)\
            visitor->visitField(#NAME, c, &c->NAME);
#define XXX_REFLECT_FUNCTION_(NAME)\
            visitor->visitFunction<ThisType,decltype(&ThisType::NAME),&ThisType::NAME>(#NAME, c);
#define XXX_REFLECT_END_()\
        }

#define XXX_REGISTER_CLASS_(NAME,VERSION)\
    reflectos::internal::ClassInfoImpl<NAME> reflectos::internal::TypeStorage<NAME,true>::info(#NAME,VERSION);
//-----------------------------------------------------------------------------

#define XXX_REGISTER_TYPE_(NAME)\
    reflectos::internal::TypeInfoImpl<NAME> reflectos::internal::TypeStorage<NAME,false>::info(#NAME);
//-----------------------------------------------------------------------------

inline const char* TypeInfo::name() const           { return m_name; }
inline uint32_t TypeInfo::id() const                { return m_id; }
inline uint32_t TypeInfo::version() const           { return m_version; }
inline size_t TypeInfo::size() const                { return m_size; }
inline bool TypeInfo::isClass() const               { return (m_flags & internal::flag_class) != 0; }
inline bool TypeInfo::isPOD() const                 { return (m_flags & internal::flag_pod) != 0; }
inline bool TypeInfo::isAbstract() const            { return (m_flags & internal::flag_abstract) != 0; }
inline bool TypeInfo::isPolymorphic() const         { return (m_flags & internal::flag_polymorphic) != 0; }
inline bool TypeInfo::hasSimpleConstructor() const  { return (m_flags & internal::flag_simple_constructor) != 0; }
inline bool TypeInfo::isReflected() const           { return (m_flags & internal::flag_reflected) != 0; }
inline TypeInfo const* TypeInfo::base() const       { return m_base; }
inline TypeInfo const* TypeInfo::next() const       { return m_next; }
//-----------------------------------------------------------------------------

inline uint32_t FieldInfo::id() const                                           { return m_id; }
inline char const* FieldInfo::name() const                                      { return m_name; }
inline TypeInfo const* FieldInfo::type() const                                  { return m_type; }
inline FieldInfo const* FieldInfo::next() const                                 { return m_next; }
//-----------------------------------------------------------------------------

inline uint32_t FunctionInfo::id() const                                        { return m_id; }
inline char const* FunctionInfo::name() const                                   { return m_name; }
inline FunctionInfo const* FunctionInfo::next() const                           { return m_next; }

template<typename Signature>
inline FastDelegate<Signature> FunctionInfo::asDelegate(void* obj) const        { return reinterpret_cast<FastDelegate<Signature>(*)(void*)>(m_ptr)(obj); }

template<typename R>
inline R FunctionInfo::call(void* obj) const                                    { return asDelegate<R()>(obj)(); }

template<typename R,typename A>
inline R FunctionInfo::call(void* obj,A a) const                                { return asDelegate<R(A)>(obj)(a); }

template<typename R,typename A,typename B>
inline R FunctionInfo::call(void* obj,A a,B b) const                            { return asDelegate<R(A,B)>(obj)(a,b); }

template<typename R,typename A,typename B,typename C>
inline R FunctionInfo::call(void* obj,A a,B b,C c) const                        { return asDelegate<R(A,B,C)>(obj)(a,b,c); }

template<typename R,typename A,typename B,typename C,typename D>
inline R FunctionInfo::call(void* obj,A a,B b,C c,D d) const                    { return asDelegate<R(A,B,C,D)>(obj)(a,b,c,d); }

template<typename R,typename A,typename B,typename C,typename D,typename E>
inline R FunctionInfo::call(void* obj,A a,B b,C c,D d,E e) const                { return asDelegate<R(A,B,C,D,E)>(obj)(a,b,c,d,e); }

template<typename R,typename A,typename B,typename C,typename D,typename E,typename F>
inline R FunctionInfo::call(void* obj,A a,B b,C c,D d,E e,F f) const            { return asDelegate<R(A,B,C,D,E,F)>(obj)(a,b,c,d,e,f); }

//-----------------------------------------------------------------------------

template<typename T> struct type_inspect<const T> : public type_inspect<T> {};
template<typename T> struct type_inspect<T&> : public type_inspect<T> {};
template<typename T> struct type_inspect<const T&> : public type_inspect<T> {};
template<typename T> struct type_inspect<T&&> : public type_inspect<T> {};
template<typename T> struct type_inspect<T*> : public type_inspect<T> {};
template<typename T> struct type_inspect<const T*> : public type_inspect<T> {};

template<typename T> inline char const* type_inspect<T>::name()                   { return info.name(); }
template<typename T> inline uint32_t type_inspect<T>::id()                        { return info.id(); }
template<typename T> inline size_t type_inspect<T>::size()                        { return sizeof(T); }
template<typename T> inline bool type_inspect<T>::isClass()                       { return __is_class(T); }
template<typename T> inline bool type_inspect<T>::isPOD()                         { return __is_pod(T); }
template<typename T> inline bool type_inspect<T>::isAbstract()                    { return __is_abstract(T); }
template<typename T> inline bool type_inspect<T>::isPolymorphic()                 { return __is_polymorphic(T); }
template<typename T> inline bool type_inspect<T>::hasSimpleConstructor()          { return internal::has_simple_constructor<T>::value; }
template<typename T> inline TypeInfo const* type_inspect<T>::type()               { return &info; }
template<typename T> inline void* type_inspect<T>::construct()                    { return info.construct(); }
template<typename T> inline void* type_inspect<T>::constructArray(size_t count)   { return info.constructArray(count); }
template<typename T> inline void type_inspect<T>::constructInPlace(void* ptr)     { info.constructInPlace(ptr); }
template<typename T> inline void type_inspect<T>::destroy(void* ptr)              { info.destroy(ptr); }
template<typename T> inline void type_inspect<T>::destroyArray(void* ptr)         { info.destroyArray(ptr); }
template<typename T> inline void type_inspect<T>::destroyInPlace(void* ptr)       { info.destroyInPlace(ptr); }
template<typename T> inline void type_inspect<T>::copy(void* dst, void const* src)             { info.copy(dst, src); }
template<typename T> inline FieldInfo const* type_inspect<T>::getField(uint32_t id)            { return info.getField(id); }
template<typename T> inline FieldInfo const* type_inspect<T>::getField(char const* name)       { return info.getField(name); }
template<typename T> inline FieldInfo const* type_inspect<T>::getFirstField()                  { return info.getFirstField(); }
template<typename T> inline FunctionInfo const* type_inspect<T>::getFunction(uint32_t id)      { return info.getFunction(id); }
template<typename T> inline FunctionInfo const* type_inspect<T>::getFunction(char const* name) { return info.getFunction(name); }
template<typename T> inline FunctionInfo const* type_inspect<T>::getFirstFunction()            { return info.getFirstFunction(); }
//-----------------------------------------------------------------------------

template<typename T1,typename T2> struct is_same    { enum { value = false }; };
template<typename T> struct is_same<T,T>            { enum { value = true }; };

template<typename R>
struct function_inspect<R()> {
    typedef R(*Type)();
    typedef R R;
    static const bool returns = !is_same<R,void>::value;
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

template<typename T> inline TypeInfo const* inspect(T const* p) { return internal::get_typeinfo<T,internal::has_typeinfo<T>::value>::get(p); }
template<> inline TypeInfo const* inspect(void const*)          { return nullptr; }
inline TypeInfo const* inspect(uint32_t id)                     { return Registry::getType(id); }
inline TypeInfo const* inspect(char const* name)                { return Registry::getType(internal::fnv1a_hash(name)); }

template<typename Visitor,typename T>
inline void reflect(Visitor* visitor, T const* obj)             { type_inspect<T>::reflect(visitor, obj); }
//----------------------------------------------------------------------------

} // end of reflectos

#endif
