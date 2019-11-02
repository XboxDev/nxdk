/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <vcruntime_exception.h>

extern "C"
{
    void __cdecl __std_exception_copy (const __std_exception_data *_From, __std_exception_data *_To)
    {
        if (_From->_DoFree) {
            char *str_copy = strdup(_From->_What);
            assert(str_copy != NULL);
            if (!str_copy) {
                return;
            }
            _To->_What = str_copy;
        } else {
            _To->_What = _From->_What;
        }

        _To->_DoFree = _From->_DoFree;
    }

    void __cdecl __std_exception_destroy (__std_exception_data *_Data)
    {
        if (_Data->_DoFree) {
            free(const_cast<char *>(_Data->_What));
        }

        _Data->_DoFree = false;
        _Data->_What = NULL;
    }
}
