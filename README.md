# letLISP: A Pure LISP interpreter with dynamic-scope named-let

This project is aimed to define and implement a minimum LISP interpreter for beginners to learn LISP programming or implement small LISP interpreters in various programming languages.

## Language Specification

It has Scheme-like reserved words but just several Pure-oriented LISP syntax and built-in functions, including dynamic-scope named-let. Source codes are mostly running in Scheme implementations too, although the named-let must be supposed in dynamic-scope variable binding system.

* S-expressions are accepted with parenthesis enclosing and space separating, NOT dot notation for cdr of conscells because of implementation in some languages by using recursive array structures.
* Special forms
	* named-`let` with dynamic scope and Lisp-1.
	* `if` as conditional operator. The false-clause must be provided.
	* `quote` same as single-quotation.
* Built-in functions for list and integer processing
	* `cons`, `car`, `cdr` and `pair?` for lists and atoms, `eq?` for atoms
	* `+`, `-`, `*`, `quotient` and `remainders` for integers
	* `<` and `=` for integers. boolean values are NOT supported.
        * `read` and `write` for S-expressions

## Sample codes

letLISP has no global environment so dynamic-scope named-let is only the method to define recursion or loop. See `samples` directory for more sample codes.

```
$ python3 letLISP.py 
S> (let loop ((a '(x y z)) (b '(h i j)))
     (if (eq? a '()) b
         (cons (car a) (loop (cdr a) b))))
(x y z h i j)
S> (exit)
$ cat samples/fib-read.scm 
(write
  (let loop ((n (read)) (a 0) (b 1))
    (if (eq? n 0) (cons a '())
        (cons a
              (loop (- n 1)
                    b (+ a b))))))
$ python3 letLISP.py samples/fib-read.scm 
21
(0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597 2584 4181 6765 10946)
```

## Implementations

* `letLISP.py`: A reference-oriented implementation written in pseudocode-style Python 3 with basic syntax only, without list comprehensions nor lambda expressions.
* `letLISP.c`: A simple implementation with just one u_int16_t or u_int32_t array as memory for conscells and symbols, without GC nor storage area of integers.
* `letLISP.html`(`letLISP.js`): An implementation in JavaScript with HTML forms to input LISP code and read code.

## License

(C) 2021 TAKIZAWA Yozo

The codes in this repository are licensed under [CC0, Creative Commons Zero v1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)

