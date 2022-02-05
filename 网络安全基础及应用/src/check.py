import base64
import rc4


def genBase64(password):
    simpleEncoded = bytearray()
    for i in password:
        high = i // 16
        low = i % 16
        simpleEncoded.append(0x78 + high)
        simpleEncoded.append(0)
        simpleEncoded.append(0x7a + low)
        simpleEncoded.append(0)

    # print(simpleEncoded)

    base64ed = base64.b64encode(simpleEncoded)
    # print(base64ed)
    ansBase64ed = base64ed[:len(base64ed) // 2]
    return ansBase64ed


def genKey(ansBase64ed):
    key = []
    for i in ansBase64ed:
        key.append(i)
        key += [0]
    return key


raw = [
    316603392, 521699724, -1896982701, 23873730, -1217616604, -653821873, 147644523, 1788871112, -401501575, -670459317,
    -247200577, 691461817, 800007765, -1582862633, 779312504, -1253129845, -1463759142, -434474754, -729680901,
    254367706, -1086890754
]
data = []
for i in raw:
    data.append(i % 256)
    data.append(i // 256 % 256)
    data.append(i // 256 // 256 % 256)
    data.append(i // 256 // 256 // 256)

targetString = b'flag{'
targetCrypted = []
for i in targetString:
    targetCrypted.append(i)
    targetCrypted.append(0)


def taskCrypt(password):
    ansBase64ed = genBase64(password)
    key = genKey(ansBase64ed)
    keyLength = len(key)
    s = rc4.init(key, keyLength)
    keyLength = 84
    crypted = rc4.crypt(s, data, keyLength)
    # print(crypted)
    if bytearray(crypted[:4]).lower() == b'f\x00l\x00':
        print(password)
        print(crypted[4:10])
        if bytearray(crypted[:6]).lower() == b'f\x00l\x00a\x00':
            print(password)

    pass


password = b'VRRC4-123456'
taskCrypt(password)

# alphabet = list(range(32, 127))
# for i in alphabet:
#     for j in alphabet:
#         for k in alphabet:
#             password = bytearray([i, j, k])
