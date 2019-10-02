#ifdef __cplusplus
extern "C" {
#endif

char *strdup (const char *s);

static char *_strdup (const char *s)
{
    return strdup(s);
}

#ifdef __cplusplus
}
#endif
