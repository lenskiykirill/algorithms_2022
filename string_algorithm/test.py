import random

def get_rand_s(size):
    s = ""
    for i in range(size):
        s += chr(random.randint(ord('a'), ord('c')))
    return s

if __name__ == "__main__":

    log = open("test.log", "w")

    N = 10
    L = 5
    l = 3

    s = get_rand_s(L)
    log.write(s+'\n')
    log.write(str(N) + '\n')
    print(s)
    print(N)
    for i in range(N):
        s = get_rand_s(l)
        log.write(s+'\n')
        print (s)
