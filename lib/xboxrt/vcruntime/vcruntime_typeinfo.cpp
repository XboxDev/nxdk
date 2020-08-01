/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include <string.h>
#include <vcruntime_typeinfo.h>

extern "C"
{
    int __cdecl __std_type_info_compare (const __std_type_info_data *const lhs, const __std_type_info_data *const rhs)
    {
        if (lhs == rhs) {
            return 0;
        }

        return strcmp(lhs->_DecoratedName + 1, rhs->_DecoratedName + 1);
    }

    size_t __cdecl __std_type_info_hash (const __std_type_info_data *const data)
    {
        // Computes the FNV-1a hash of the mangled name
        constexpr size_t fnv_offset_basis = 0x811C9DC5;
        constexpr size_t fnv_prime = 0x1000193;

        size_t hash = fnv_offset_basis;
        for (auto *it = reinterpret_cast<const unsigned char *>(data->_DecoratedName + 1); *it != '\0'; ++it) {
            hash ^= static_cast<size_t>(*it);
            hash *= fnv_prime;
        }

        return hash;
    }
}

type_info::~type_info() noexcept
{
}

type_info::type_info () noexcept
{
}
