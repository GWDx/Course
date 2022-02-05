#include <iostream>

using std::cout;
using std::endl;

#define maxLength 256

#define FOR(i, l, r) for (i = l; i < r; i++)

bool inline compare(int ans) {
    return ans == 0x12B2FC66 || ans == 0x12B2FC46 || ans == 0x1292FC66 || ans == 0x1292FC46;
    // return ans == 0xC43957FE;
    // '0x'+''.join(reversed(['{:02X}'.format(crypted[i] ^ data[i]) for i in range(4)]))
}

void prepare_key(unsigned char* key_data_ptr, int key_data_len, unsigned char* state) {
    unsigned char swapByte;
    unsigned char index1, index2;
    int counter;

    index1 = index2 = 0;
    for (counter = 0; counter < 256; counter++) {
        index2 = key_data_ptr[index1] + state[counter] + index2;

        swapByte = state[counter];
        state[counter] = state[index2];
        state[index2] = swapByte;

        index1++;
        if (index1 == key_data_len)
            index1 -= key_data_len;
    }
}

bool GetKeyStream(unsigned char* buffer_ptr, int buffer_len, unsigned char* state) {
    unsigned char swapByte;
    unsigned char x;
    unsigned char y;
    unsigned char xorIndex;
    int counter;

    x = y = 0;
    for (counter = 0; counter < buffer_len; counter++) {
        x++;               // x = (x + 1) % 256;
        y = state[x] + y;  // y = (state[x] + y) % 256;

        swapByte = state[x];
        state[x] = state[y];
        state[y] = swapByte;

        xorIndex = state[x] + state[y];  // xorIndex = (state[x] + state[y]) % 256;
        buffer_ptr[counter] = state[xorIndex];
    }

    return compare(*(int*)buffer_ptr);
}

void printArray(const unsigned char* array, int arrayLength) {
    int i;
    int target[] = {0x66, 0xFC, 0x92, 0x12, 0xED, 0x81, 0x7F, 0x1F};
    // 66 FC 92 12 ED 81 7F 1F

    for (i = 0; i < arrayLength; i++)
        printf("%02X ", array[i]);

    cout << '\t';
    for (i = 0; i < arrayLength; i++)
        if (array[i] == target[i] or (array[i] ^ target[i]) == 0x20)
            cout << i;
    cout << endl;
}
