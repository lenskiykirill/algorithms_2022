import random

def get_rand_s(size):
    s = ""
    for i in range(size):
        s += chr(random.randint(ord('a'), ord('c')))
    return s

if __name__ == "__main__":
    L = 10
    s = get_rand_s(10)

