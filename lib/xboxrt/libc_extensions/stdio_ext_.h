// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

// Define required MS-specific aliases
#define _vsnprintf vsnprintf
#define _snprintf snprintf

/**
 * @brief Associate an open descriptor with a stdio stream, preserving its offset.
 * @param fd Open descriptor; fclose takes ownership only when fdopen succeeds.
 * @param mode Compatible stdio mode; w does not truncate and a enables O_APPEND.
 * @return Stream, or NULL with errno set; failure leaves fd owned by the caller.
 */
FILE *fdopen (int fd, const char *mode);
/**
 * @brief Get a stream's descriptor without transferring ownership.
 * @param stream Open stdio stream.
 * @return Descriptor, or -1 with errno set.
 */
int fileno (FILE *stream);
