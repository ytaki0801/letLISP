####
#### letLISP.py:
#### A Pure LISP Interpreter
#### with dynamic-scope named-let
#### in pseudocode-style Python 3
####
#### (C) 2022 TAKIZAWA Yozo
#### This code is licensed under CC0.
#### https://creativecommons.org/publicdomain/zero/1.0/
####

from sys import argv, stdin, exit

def cons(x, y):
    return [x] + y
def car(x):
    return x[0]
def cdr(x):
    return x[1:]
def null(x):
    return x == []
def pair(x):
    r = isinstance(x, list)
    return r and not null(x)
def atom(x):
    r = isinstance(x, str)
    return r or null(x)
def eq(x, y):
    if atom(x) and atom(y):
        return x == y
    else:
        return False
def intp(x):
    try:
        r = int(x)
        return True
    except ValueError:
        return False

LH = False
def put_c1(x):
    global LH
    LH = x
def get_c1(f):
    if not LH:
        try:
            return f.read(1)
        except EOFError:
            pass
    else:
        lh = LH
        put_c1(False)
        return lh
def tstring(t):
    return ''.join(t)
def skip_spaces(f):
    c = get_c1(f);
    while c in ' \t\n\r':
        c = get_c1(f)
    put_c1(c)
def get_token(f):
    c = get_c1(f)
    t = []
    while True:
        if c in ' \t\n\r':
            if not t:
                skip_spaces(f);
                c = get_c1(f)
            else:
                return tstring(t)
        elif c == '(' or c == "'":
            skip_spaces(f)
            return c
        elif c == ')':
            if not t:
                return c
            else:
                put_c1(c)
                return tstring(t)
        else:
            t += [c]
            c = get_c1(f)

def srlist(f):
    t = get_token(f)
    if   t == ')':
        return []
    elif t == '(':
        h = srlist(f)
        return cons(h, srlist(f))
    elif t == "'":
        h = cons('quote', [sread(f)])
        return cons(h, srlist(f))
    else:
        return cons(t, srlist(f))
def sread(f):
    t = get_token(f)
    if t == '(':
        return srlist(f)
    if t == "'":
        return cons('quote', [sread(f)])
    else:
        return t

def output(x):
    print(x, end='', flush=True)
def swlist(x):
    swrite(car(x))
    if not null(cdr(x)):
        output(' ')
        swlist(cdr(x))
def swrite(s):
    if pair(s):
        output('(')
        swlist(s)
        output(')')
    elif null(s):
        output('()')
    else:
        output(s)

def add(x, y):
    return str(int(x) + int(y))
def sub(x, y):
    return str(int(x) - int(y))
def mul(x, y):
    return str(int(x) * int(y))
def quo(x, y):
    return str(int(x) // int(y))
def rem(x, y):
    return str(int(x) % int(y))
def eqn(x, y):
    return int(x) == int(y)
def lt(x, y):
    return int(x) < int(y)
def sread_std():
    return sread(stdin)

G = {'cons'     : cons,
     'car'      : car,
     'cdr'      : cdr,
     'pair?'    : pair,
     'eq?'      : eq,
     '+'        : add,
     '-'        : sub,
     '*'        : mul,
     'quotient' : quo,
     'remainder': rem,
     '<'        : lt,
     '='        : eqn,
     'read'     : sread_std,
     'write'    : swrite}

def quote_syntax(s, e):
    return s[1]

def if_syntax(s, e):
    if ev(s[1], e):
        return ev(s[2], e)
    else:
        return ev(s[3], e)

def let_syntax(s, e):
    vs, rs, v = [], [], s[2]
    while v:
        vs += [car(v)[0]]
        rs += [car(v)[1]]
        v = cdr(v)
    r = [['fn', [s[1]], [s[1]] + rs],
         ['fn', vs, s[3]]]
    return ev(r, e)

def apply_function(s, e):
    f = ev(car(s), e)
    a = []
    s = cdr(s)
    while s:
        a += [ev(car(s), e)]
        s = cdr(s)
    if callable(f):
        return f(*a)
    else:
        va = dict(zip(f[1], a))
        e1 = {**e, **va}
        return ev(f[2], e1)

def ev(s, e):
    if atom(s):
        if intp(s):
            return s
        elif s in G:
            return G[s]
        else:
            return e[s]
    elif car(s) == 'quote':
        return quote_syntax(s, e)
    elif car(s) == 'if':
        return if_syntax(s, e)
    elif car(s) == 'fn':
        return s
    elif car(s) == 'let':
        return let_syntax(s, e)
    elif car(s) == 'exit':
        exit()
    else:
        return apply_function(s, e)

if len(argv) < 2:
    while True:
        output('S> ')
        s = sread(stdin)
        swrite(ev(s, {}))
        print()
else:
    with open(argv[1]) as f:
        s = sread(f)
        ev(s, {})

