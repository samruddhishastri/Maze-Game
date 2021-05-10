from random import shuffle, randrange

def make_maze(w = 16, h = 8):
    vis = [[0] * w + [1] for _ in range(h)] + [[1] * (w + 1)]
    ver = [["|  "] * w + ['|'] for _ in range(h)] + [[]]
    hor = [["+--"] * w + ['+'] for _ in range(h + 1)]
    p = 0
    q = 0

    def walk(x, y):
        vis[y][x] = 1

        d = [(x - 1, y), (x, y + 1), (x + 1, y), (x, y - 1)]
        shuffle(d)
        for (xx, yy) in d:
            if vis[yy][xx]: continue
            if xx == x: 
                hor[max(y, yy)][x] = "+  "
                p = xx
                q = max(y, yy)
            if yy == y: 
                ver[y][max(x, xx)] = "   "
                p = max(x, xx)
                q = y
            walk(xx, yy)

    walk(2,2)
    for (a, b) in zip(hor, ver):
        print(''.join(a + ['\n'] + b))

    print(p)
    print(q)
    
make_maze()