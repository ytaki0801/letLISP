//
// letLISP.c:
// A Pure LISP Interpreter in C
// with dynamic-scope named-let
//
// (C) 2022 TAKIZAWA Yozo
// This code is licensed under CC0.
// https://creativecommons.org/publicdomain/zero/1.0/
//

#include <stdio.h>
#include <stdlib.h>

// u_int_32_t and -DNMAX=16553000
// for tarai(10,5,0)
typedef u_int16_t node_t;
#define CB 127
#ifndef NMAX
#define NMAX 32768
#endif
#define SMAX 32
node_t node[NMAX], nnum = 1;
node_t cons(node_t a, node_t d) {
  node_t r = nnum;
  node[nnum++] = a; node[nnum++] = d;
  return r + CB;
}
#define NIL 0
node_t ssym(const char *s) {
  node_t r = nnum;
  for (int i = 0; s[i] != '\0'; i++)
    node[nnum++] = (node_t)s[i];
  node[nnum++] = NIL;
  return r + CB;
}
void wsym(node_t s) {
  node_t i;
  for (i = s; node[i - CB] != '\0'; i++)
    putchar(node[i - CB]);
}

node_t null(node_t s) {
  return s == NIL;
}
node_t atom(node_t s) {
  return (node[s - CB] <= CB) ||
	  null(s);
}
node_t pair(node_t s) {
  return !(null(s))
	  && node[s - CB] > CB;
}
node_t car(node_t s) {
  return node[s - CB];
}
node_t cdr(node_t s) {
  return node[s - CB + 1];
}
node_t eq(node_t a, node_t b) {
  if (null(a) && null(b))
    return !(NIL);
  else if (!null(a) && !null(b) &&
           atom(a) && atom(b)) {
    node_t i = 0;
    while(node[a - CB + i] != 0 &&
          node[b - CB + i] != 0 &&
          node[a - CB + i] ==
	  node[b - CB + i])
      i++;
    return node[a - CB + i] == 0 &&
           node[b - CB + i] == 0;
  } else return NIL;
}
node_t eqst(node_t a, char b[]) {
  node_t i = 0;
  while(node[a - CB + i] != 0 &&
        b[i] != 0 &&
        node[a - CB + i] == b[i]) i++;
  return node[a - CB + i] == 0 &&
	 b[i] == 0;
}
node_t intp(node_t n) {
  node_t i = 0; char c;
  while ((c = node[n - CB + i])
         != '\0' &&
         (('0' <= c && c <= '9') ||
          c == '-')) i++;
  return c == '\0';
}
int stoi(node_t s) {
  char str[SMAX]; int i;
  for (i = 0;
       node[s - CB + i] != '\0';
       i++)
    str[i] = node[s - CB  + i];
  str[i] = '\0';
  return atoi(str);
}
node_t itos(int n) {
  char str[SMAX];
  sprintf(str, "%d", n);
  return ssym(str);
}

char LH = 0;
char TOKEN[SMAX]; node_t tnum = 0;
void put_c1(node_t x) { LH = x; }
char get_c1(FILE *f) {
  if (LH) {
    char lh = LH; put_c1(0);
    return lh;
  } else return getc(f);
}
node_t tstring() {
  TOKEN[tnum] = NIL;
  node_t r = ssym(TOKEN); tnum = 0;
  return r;
}
node_t cstring(char c) {
  char s[2]; s[0] = c; s[1] = NIL;
  node_t r = ssym(s); tnum = 0;
  return r;
}
void skip_spaces(FILE *f) {
  char c = get_c1(f);
  while (c == ' ' || c <= 13)
    c = get_c1(f);
  put_c1(c);
}
node_t get_token(FILE *f) {
  char c = get_c1(f); tnum = 0;
  while (1) {
    if (c == ' ' || c <= 13) {
      if (tnum == 0) {
        skip_spaces(f); c = get_c1(f);
      } else return tstring();
    } else if (c == '(' || c == '\'') {
      skip_spaces(f); return cstring(c);
    } else if (c == ')') {
      if (tnum == 0) return cstring(c);
      else {
        put_c1(c); return tstring();
      }
    } else {
      TOKEN[tnum++] = c; c = get_c1(f);
    }
  }
}

node_t sread(FILE *f);
node_t srlist(FILE *f) {
  node_t t = get_token(f);
  if      (eqst(t, ")"))
    return NIL;
  else if (eqst(t, "(")) {
    node_t h = srlist(f);
    return cons(h, srlist(f));
  } else if (eqst(t, "'")) {
    node_t h = ssym("quote");
    node_t r = cons(sread(f), NIL);
    return cons(cons(h, r), srlist(f));
  } else return cons(t, srlist(f));
}
node_t sread(FILE *f) {
  node_t t = get_token(f);
  if (eqst(t, "("))
    return srlist(f);
  else if (eqst(t, "'")) {
    node_t h = ssym("quote");
    node_t r = cons(sread(f), NIL);
    return cons(h, r);
  } else return t;
}

void swrite(node_t s) {
  if (atom(s)) {
    if (null(s)) printf("()");
    else wsym(s);
  }
  else {
    putchar('('); swrite(car(s));
    while (!null(cdr(s))) {
      putchar(' ');
      s = cdr(s); swrite(car(s));
    }
    putchar(')');
  }
}

node_t append(node_t a, node_t b) {
  if (null(a)) return b;
  else {
    node_t r = append(cdr(a), b);
    return cons(car(a), r);
  }
}
node_t plis(node_t a, node_t b) {
  if (null(a) || null(b)) return NIL;
  else {
    node_t r = plis(cdr(a), cdr(b));
    r = cons(car(a), cons(car(b), r));
    return r;
  }
}
node_t prop(node_t k, node_t p) {
  if (null(p)) return NIL;
  else if (eq(car(p), k))
    return car(cdr(p));
  else
    return prop(k, cdr(cdr(p)));
}
node_t cadr(node_t s) {
  return car(cdr(s));
}
node_t caddr(node_t s) {
  return car(cdr(cdr(s)));
}
node_t cadddr(node_t s) {
  return car(cdr(cdr(cdr(s))));
}

node_t ev(node_t s, node_t e);

node_t quo_syntax(node_t s, node_t e) {
  return cadr(s);
}
node_t if_syntax(node_t s, node_t e) {
  if (ev(cadr(s), e))
    return ev(caddr(s), e);
  else
    return ev(cadddr(s), e);
}
node_t let_syntax(node_t s, node_t e) {
  node_t vs = NIL, vs1;
  node_t rs = NIL, rs1;
  node_t v = caddr(s);
  while (!null(v)) {
    vs1 = cons(car(car(v)), NIL);
    vs = append(vs, vs1);
    rs1 = cons(cadr(car(v)), NIL);
    rs = append(rs, rs1);
    v = cdr(v);
  }

  node_t fn = ssym("fn");
  node_t s1 = cons(cadr(s), NIL);
  node_t s3 = cons(cadddr(s), NIL);
  node_t r1 = cons(fn, cons(s1, NIL));
  node_t r2 = cons(append(s1, rs), NIL);
  node_t r3 = cons(fn, cons(vs, s3));
  node_t r4 = cons(r3, NIL);
  node_t r = cons(append(r1, r2), r4);
  return ev(r, e);
}

node_t apply_func(node_t s, node_t e) {
  node_t f = ev(car(s), e);
  node_t a = NIL, a1;
  s = cdr(s);
  while (!null(s)) {
    a1 = ev(car(s), e);
    a = append(a, cons(a1, NIL));
    s = cdr(s);
  }

  if (atom(f)) {
    if      (eqst(f, "cons"))
      return cons(car(a), cadr(a));
    else if (eqst(f, "car"))
      return car(car(a));
    else if (eqst(f, "cdr"))
      return cdr(car(a));
    else if (eqst(f, "pair?"))
      return pair(car(a));
    else if (eqst(f, "eq?"))
      return eq(car(a), cadr(a));
    else if (eqst(f, "read"))
      return sread(stdin);
    else if (eqst(f, "write"))
      swrite(car(a));
    else {
      int a1 = stoi(car(a));
      int a2 = stoi(cadr(a));
      if      (eqst(f, "+"))
        return itos(a1 + a2);
      else if (eqst(f, "-"))
        return itos(a1 - a2);
      else if (eqst(f, "*"))
        return itos(a1 * a2);
      else if (eqst(f, "quotient"))
        return itos(a1 / a2);
      else if (eqst(f, "remainder"))
        return itos(a1 % a2);
      else if (eqst(f, "<"))
        return a1 < a2;
      else if (eqst(f, "="))
        return a1 == a2;
      else return NIL;
    }
  } else {
    node_t va = plis(cadr(f), a);
    node_t er = append(va, e);
    return ev(caddr(f), er);
  }
}

node_t ev(node_t s, node_t e) {
  if (atom(s)) {
    if (eqst(s, "cons")      ||
        eqst(s, "car")       ||
        eqst(s, "cdr")       ||
        eqst(s, "pair?")     ||
        eqst(s, "eq?")       ||
        eqst(s, "read")      ||
        eqst(s, "write")     ||
        eqst(s, "+")         ||
        eqst(s, "-")         ||
        eqst(s, "*")         ||
        eqst(s, "quotient")  ||
        eqst(s, "remainder") ||
        eqst(s, "<")         ||
        eqst(s, "=")         ||
        intp(s)) return s;
    else {
      return prop(s, e);
    }
  } else {
    if (eqst(car(s), "quote"))
      return quo_syntax(s, e);
    else if (eqst(car(s), "fn"))
      return s;
    else if (eqst(car(s), "let"))
      return let_syntax(s, e);
    else if (eqst(car(s), "if"))
      return if_syntax(s, e);
    else if (eqst(car(s), "exit"))
      exit(0);
    else
      return apply_func(s, e);
  }
}

int main(int argc, char* argv[]) {
  node_t s;
  if (argc < 2)
    while (1) {
      nnum = 1;
      printf("S> ");
      swrite(ev(sread(stdin), NIL));
      printf("\n");
    }
  else {
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
      printf("%s is not found.\n",
             argv[1]);
      return 1;
    }
    nnum = 1;
    s = sread(fp); fclose(fp);
    ev(s, NIL);
  }

  return 0;
}

