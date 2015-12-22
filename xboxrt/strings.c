int ffs(int mask) {
    if (mask == 0)
        return 0;

    int bit;
    for (bit = 1; !(mask & 1); bit++)
        mask >>= 1;
    return bit;
}