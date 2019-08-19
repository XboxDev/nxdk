
#include <xboxkrnl/xboxkrnl.h>

#if 0
/*
 * Convert a XBOX time format to Unix time.
 * If not NULL, 'remainder' contains the fractional part of the filetime,
 * in the range of [0..9999999] (even if time_t is negative).
 * Borrowed from synce/libwine/wine.c
 */
time_t XBOXFileTimeToUnixTime(LARGE_INTEGER xboxTime, DWORD *remainder)
{
    unsigned int a0;    /* 16 bit, low    bits */
    unsigned int a1;    /* 16 bit, medium bits */
    unsigned int a2;    /* 32 bit, high   bits */
    unsigned int r;     /* remainder of division */
    unsigned int carry; /* carry bit for subtraction */
    int negative;       /* whether a represents a negative value */
    
    /* Copy the time values to a2/a1/a0 */
    a2 =  (unsigned int)xboxTime.u.HighPart;
    a1 = ((unsigned int)xboxTime.u.LowPart) >> 16;
    a0 = ((unsigned int)xboxTime.u.LowPart) & 0xffff;

    /* Subtract the time difference */
    if (a0 >= 32768           ) a0 -=             32768        , carry = 0;
    else                        a0 += (1 << 16) - 32768        , carry = 1;
    
    if (a1 >= 54590    + carry) a1 -=             54590 + carry, carry = 0;
    else                        a1 += (1 << 16) - 54590 - carry, carry = 1;

    a2 -= 27111902 + carry;
    
    /* If a is negative, replace a by (-1-a) */
    negative = (a2 >= ((unsigned int)1) << 31);
    if (negative)
    {
        /* Set a to -a - 1 (a is a2/a1/a0) */
        a0 = 0xffff - a0;
        a1 = 0xffff - a1;
        a2 = ~a2;
    }

    /* Divide a by 10000000 (a = a2/a1/a0), put the rest into r.
       Split the divisor into 10000 * 1000 which are both less than 0xffff. */
    a1 += (a2 % 10000) << 16;
    a2 /=       10000;
    a0 += (a1 % 10000) << 16;
    a1 /=       10000;
    r   =  a0 % 10000;
    a0 /=       10000;
    
    a1 += (a2 % 1000) << 16;
    a2 /=       1000;
    a0 += (a1 % 1000) << 16;
    a1 /=       1000;
    r  += (a0 % 1000) * 10000;
    a0 /=       1000;
    
    /* If a was negative, replace a by (-1-a) and r by (9999999 - r) */
    if (negative)
    {
        /* Set a to -a - 1 (a is a2/a1/a0) */
        a0 = 0xffff - a0;
        a1 = 0xffff - a1;
        a2 = ~a2;

        r  = 9999999 - r;
    }

    if (remainder) *remainder = r;

    /* Do not replace this by << 32, it gives a compiler warning and it does
       not work. */
    return ((((time_t)a2) << 16) << 16) + (a1 << 16) + a0;
}

int fstat(int fd, struct stat *st)
{
    NTSTATUS                      status;
    IO_STATUS_BLOCK               ioStatusBlock;
    FILE_NETWORK_OPEN_INFORMATION networkInfo;
    
    status = NtQueryInformationFile(
        (void*)fd, 
        &ioStatusBlock,
        &networkInfo, 
        sizeof(networkInfo), 
        FileNetworkOpenInformation);
    if (!NT_SUCCESS(status))
    {
        errno = RtlNtStatusToDosError(status);
        return -1;
    }
        
    if (st)
    {
        st->st_dev = 0;   // default it to zero for now
        st->st_mode = 0;  // default it to zero for now
        st->st_ino = 0;   // surely we don't care, do we?
        st->st_nlink = 0; // default to zero
        st->st_uid = 0;   // surely we don't care, do we?
        st->st_gid = 0;   // surely we don't care, do we?
        st->st_rdev = 0;  // dunno what this is... 
        st->st_size = (unsigned int)networkInfo.EndOfFile.u.LowPart;
        st->st_atime = XBOXFileTimeToUnixTime(networkInfo.LastAccessTime, NULL);
        st->st_mtime = XBOXFileTimeToUnixTime(networkInfo.LastWriteTime, NULL);
        st->st_ctime = XBOXFileTimeToUnixTime(networkInfo.ChangeTime, NULL);
        
        // stdin, stdout and stderr are all character devices
        if (fd == 0 || fd == 1 || fd == 2)
            st->st_mode = S_IFCHR;
        else
        {
            if (networkInfo.FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                st->st_mode = S_IFDIR;
            else if (networkInfo.FileAttributes & FILE_ATTRIBUTE_DEVICE)
                st->st_mode = S_IFBLK;
            else
                st->st_mode = S_IFREG;
        }
    }
    return 0;
}

int stat(const char *filename, struct stat *st)
{
    int fd = open((char *)filename, O_RDONLY);
    if (fd != -1)
    {
        int rc = fstat(fd, st);
        close(fd);
        return rc;
    }
    else if (errno == ERROR_ACCESS_DENIED && st)
    {
        // TODO: use a different BIOS call so we can get proper date/time info
        // for the directory? -- th0mas, June 9th, 2005
        memset(st, 0, sizeof(struct stat));
        st->st_mode = S_IFDIR;
    } 
    else
    {
        return -1;
    }
}
#endif