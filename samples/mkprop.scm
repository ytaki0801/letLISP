(write
  (let mkprop ((k '(a b c)) (v '(1 2 3)))
    (if (eq? k '()) '()
        (cons (car k) (cons (car v)
              (mkprop (cdr k)
		      (cdr v)))))))
