(write
  (let app ((a '(a b c)) (b '(x y z)))
    (if (eq? a '()) b
        (cons (car a) (app (cdr a) b)))))
