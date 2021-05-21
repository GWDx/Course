from brainfuck import brainfuck
import pyautogui
import time

def bit_to_list(t):
    S = [0 for i in range(7)]    
    i = -1
    while t != 0:
        S[i] = t % 2
        t = t >> 1
        i -= 1
    return S[::-1]

def clicksw(i):
    sw7x=300
    sw0x=720
    swy=500
    swix=(i*sw7x+(7-i)*sw0x)/7
    pyautogui.moveTo(swix,swy,.01)
    pyautogui.click()

def clickbu():
    pyautogui.moveTo(1450,700,.01)
    pyautogui.click()


code= input()
inp = input()
ans = brainfuck(code,inp)

sw=[0 for i in range(7)]
encode="><+-.,[]"
time.sleep(3)
pyautogui.scroll(-50)

for c in code:
    i=encode.find(c)
    target=bit_to_list(i)
    
    for j in range(7):
        if(target[j]!=sw[j]):
            clicksw(j)
            sw[j]=1-sw[j]
    clickbu()

clicksw(7)

for c in inp:
    i=ord(c)
    target=bit_to_list(i)
    
    for j in range(7):
        if(target[j]!=sw[j]):
            clicksw(j)
            sw[j]=1-sw[j]
    clickbu()