def brainfuck(code,inp):
    n = len(code)
    mem = [0 for i in range(100)]
    ptr = 0
    i = -1
    inpn = 0
    ans=""
    while True:
        i+=1
        if i>=n:
            break
        e=code[i]
        if e=='>':
            ptr+=1
        elif e=='<':
            ptr-=1
        elif e=='+':
            mem[ptr]+=1
        elif e=='-':
            mem[ptr]-=1
        elif e=='.':
            ch=chr(mem[ptr])
            print('%s %#x'%(ch,mem[ptr]))
            ans += ch
        elif e==',':
            mem[ptr]=ord(inp[inpn])
            inpn+=1
        elif e=='[':
            if mem[ptr]==0:
                pas=1
                while pas>0:
                    i+=1
                    if code[i]=='[':
                        pas+=1
                    elif code[i]==']':
                        pas-=1
        elif e==']':
            if mem[ptr]!=0:
                ret=1
                while ret>0:
                    i-=1
                    if code[i]==']':
                        ret+=1
                    elif code[i]=='[':
                        ret-=1
    return ans


if __name__ == '__main__':
    code= input()
    inp = input()
    ans = brainfuck(code,inp)
    print(ans)