/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#ifndef __VCRUNTIME_TYPEINFO_H__
#define __VCRUNTIME_TYPEINFO_H__

#include <assert.h>
#include <vcruntime_exception.h>

struct __std_type_info_data
{
    char *_UndecoratedName;
    char _DecoratedName[1];
};

extern "C"
{
    int __cdecl __std_type_info_compare (const __std_type_info_data *const lhs, const __std_type_info_data *const rhs);
    size_t __cdecl __std_type_info_hash (const __std_type_info_data *const data);
}

class type_info
{
public:
    virtual ~type_info () noexcept;

    const char *name () const noexcept
    {
        // FIXME: return human-readable name
        assert(false);
        return NULL;
    }

    const char *raw_name () const noexcept
    {
        return this->_Data._DecoratedName;
    }

    bool before (const type_info &rhs) const noexcept
    {
        return __std_type_info_compare(&this->_Data, &rhs._Data) < 0;
    }

    size_t hash_code () const noexcept
    {
        return __std_type_info_hash(&this->_Data);
    }

    bool operator== (const type_info &rhs) const noexcept
    {
        return __std_type_info_compare(&this->_Data, &rhs._Data) == 0;
    }

    bool operator!= (const type_info &rhs) const noexcept
    {
        return !operator==(rhs);
    }

private:
    // According to the standard, this constructor should not exist. However,
    // not having any constructors causes LLVM to not emit a vftable.
    type_info () noexcept;

    type_info (type_info const &) noexcept = delete;
    type_info &operator= (type_info const &) = delete;

    mutable __std_type_info_data _Data;
};

namespace std
{
    using ::type_info;

    class bad_cast : public exception
    {
    public:
        bad_cast () noexcept : exception("bad cast", 0)
        {
        }
    };

    class bad_typeid : public exception
    {
    public:
        bad_typeid () noexcept : exception("bad typeid", 0)
        {
        }
    };
}

#endif
