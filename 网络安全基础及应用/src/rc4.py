def init(key, keyLength):
    s = [i for i in range(256)]
    j = 0
    for i in range(256):
        j = (j + s[i] + key[i % keyLength]) % 256
        temp = s[i]
        s[i] = s[j]
        s[j] = temp
    return s


def crypt(rawS, data, keyLength):
    i = 0
    j = 0
    s = rawS[:]
    ans = data[:]
    for k in range(keyLength):
        i = (i + 1) % 256
        j = (j + s[i]) % 256
        temp = s[i]
        s[i] = s[j]
        s[j] = temp
        t = (s[i] + s[j]) % 256
        ans[k] ^= s[t]
    for k in range(keyLength):
        ans[k] = (ans[k] % 256 + 256) % 256
    return ans


def main():
    # key = [5, 2, 3]
    # keyLength = 3
    # data = [1, 2, 3, 4]
    # s = init(key, keyLength)
    s = [i for i in range(256)]
    s[2] = 0x66
    s[0x66] = 2
    keyLength = 1
    data = [0, 0xFC]
    crypted = crypt(s, data, keyLength)
    print(crypted)

    uncrypted = crypt(s, crypted, keyLength)
    print(uncrypted)

    pass


if __name__ == '__main__':
    main()
