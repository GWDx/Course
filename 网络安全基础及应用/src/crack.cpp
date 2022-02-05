#include <string.h>
#include "function.h"

int passwordLength = 12;
int simplePasswordLength = passwordLength / 2;
unsigned char password[maxLength];

static unsigned char stateinit[256];
static unsigned char state[256];

unsigned char alphabet[96];

void base64Encode(unsigned char* s, int length, unsigned char* ans) {
    static unsigned char base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    while (1) {
        if (length == 0) {
            ans[0] = 0;
            return;
        }
        if (length == 1) {
            ans[0] = base64Alphabet[s[0] >> 2];
            ans[1] = base64Alphabet[(s[0] << 4) & 63];
            ans[2] = ans[3] = '=';
            ans[4] = 0;
            return;
        }
        if (length == 2) {
            ans[0] = base64Alphabet[s[0] >> 2];
            ans[1] = base64Alphabet[(s[0] << 4 | s[1] >> 4) & 63];
            ans[2] = base64Alphabet[(s[1] << 2) & 63];
            ans[3] = '=';
            ans[4] = 0;
            return;
        }
        ans[0] = base64Alphabet[s[0] >> 2];
        ans[1] = base64Alphabet[(s[0] << 4 | s[1] >> 4) & 63];
        ans[2] = base64Alphabet[(s[1] << 2 | s[2] >> 6) & 63];
        ans[3] = base64Alphabet[s[2] & 63];
        s += 3;
        length -= 3;
        ans += 4;
    }
}

void genBase64(unsigned char* password, int passwordLength, unsigned char* ansBase64ed, int& ansBase64edLength) {
    int high, low, length, i;
    unsigned char simpleEncoded[maxLength];
    unsigned char* simpleEncodedEnd = simpleEncoded;

    FOR (i, 0, simplePasswordLength) {
        high = password[i] / 16;
        low = password[i] % 16;

        simpleEncodedEnd[0] = 0x78 + high;
        simpleEncodedEnd[1] = 0;
        simpleEncodedEnd[2] = 0x7a + low;
        simpleEncodedEnd[3] = 0;
        simpleEncodedEnd += 4;
    }

    length = simpleEncodedEnd - simpleEncoded;
    base64Encode(simpleEncoded, length, ansBase64ed);
    ansBase64edLength = 2 * ((4 * passwordLength + 2) / 3);
    ansBase64ed[ansBase64edLength] = 0;
}

void genKey(unsigned char* ansBase64ed, int ansBase64edLength, unsigned char* key, int& keyLength) {
    int i;
    keyLength = 2 * ansBase64edLength;
    FOR (i, 0, ansBase64edLength) {
        key[2 * i] = ansBase64ed[i];
        key[2 * i + 1] = 0;
    }
}

void Recursion(int idx) {
    register int i;
    unsigned char key[maxLength], ansBase64ed[maxLength], prevAnsBase64ed[maxLength] = "", keyStream[8];
    int keyLength, ansBase64edLength;

    for (i = 0; i < sizeof(alphabet) - 1; i++) {
        password[idx] = alphabet[i];
        if (idx + 1 < simplePasswordLength)
            Recursion(idx + 1);
        else {
            if (password[1] == '1')
                password[1] = '1';

            genBase64(password, passwordLength, ansBase64ed, ansBase64edLength);

            if (memcmp(prevAnsBase64ed, ansBase64ed, ansBase64edLength) == 0)
                continue;
            memcpy(prevAnsBase64ed, ansBase64ed, ansBase64edLength);

            genKey(ansBase64ed, ansBase64edLength, key, keyLength);

            memcpy(state, stateinit, sizeof(stateinit));
            prepare_key(key, keyLength, state);

            if (GetKeyStream(keyStream, 8, state)) {
                password[simplePasswordLength] = 0;
                printf("%d\t%s \t", passwordLength, password);

                printArray(keyStream, 8);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int i, alphabetLength, from, to, allProcess, number;
    for (i = 0; i < 256; i++)
        stateinit[i] = i;
    alphabetLength = sizeof(alphabet) - 1;
    for (i = 0; i < alphabetLength; i++)
        alphabet[i] = 32 + i;
    cout << alphabet << '\n';

    allProcess = argc > 1 ? argv[1][0] - '0' : 1;
    number = argc > 2 ? argv[2][0] - '0' : 1;

    from = (number - 1) * alphabetLength / (double)allProcess;
    to = number * alphabetLength / (double)allProcess;

    cout << allProcess << ' ' << number << ' ' << from << ' ' << to << endl;
    for (i = from; i < to; i++) {
        password[0] = alphabet[i];
        Recursion(1);
    }

    return 0;
}
