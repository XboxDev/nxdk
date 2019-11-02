/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#ifndef __VCRUNTIME_EXCEPTION_H__
#define __VCRUNTIME_EXCEPTION_H__

struct __std_exception_data
{
    const char *_What;
    bool _DoFree;
};

extern "C"
{
    void __cdecl __std_exception_copy (const __std_exception_data *_From, __std_exception_data *_To);
    void __cdecl __std_exception_destroy (__std_exception_data *_Data);
}

namespace std
{
    class exception
    {
    public:
        exception () noexcept : _Data{NULL, false}
        {
        }

        explicit exception (const char *const _Message) noexcept
        {
            const __std_exception_data data = {_Message, true};
            __std_exception_copy(&data, &this->_Data);
        }

        // MS-specific extension to avoid string allocation
        explicit exception (const char *const _Message, int) noexcept : _Data{_Message, false}
        {
        }

        exception (const exception &rhs) noexcept
        {
            __std_exception_copy(&rhs._Data, &this->_Data);
        }

        exception &operator= (const exception &rhs) noexcept
        {
            if (this != &rhs) {
                __std_exception_destroy(&this->_Data);
                __std_exception_copy(&rhs._Data, &this->_Data);
            }

            return *this;
        }

        virtual ~exception () noexcept
        {
            __std_exception_destroy(&this->_Data);
        }

        virtual const char *what () const noexcept
        {
            return this->_Data._What ? this->_Data._What : "Unknown exception";
        }

    private:
        __std_exception_data _Data;
    };

    class bad_exception : public exception
    {
    public:
        bad_exception () noexcept : exception("bad exception", 0)
        {
        }
    };

    class bad_alloc : public exception
    {
    public:
        bad_alloc () noexcept : exception("bad allocation", 0)
        {
        }
    private:
        friend class bad_array_new_length;

        bad_alloc (const char *const _Message) noexcept : exception(_Message, 0)
        {
        }
    };

    class bad_array_new_length : public bad_alloc
    {
    public:
        bad_array_new_length () noexcept : bad_alloc("bad exception")
        {
        }
    };
}

#endif
