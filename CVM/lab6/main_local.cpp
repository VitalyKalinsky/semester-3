
int main() {
    int iMas[10];
    int *pI;
    unsigned int i;

    iMas[3] = 12;
    iMas[4] = 12345;
    iMas[5] = 12345678;
    iMas[6] = 0x123456789ABCDE;

    pI = &iMas[2];
    *pI = 0xAB12;

    for (i = 0; i < 9; i++) {
        if (i >= 4)
            iMas[i] = 13 * i + 7;
        else
            iMas[i] = (i * 8) ^ 0xC;
    }
    return 0;
}
