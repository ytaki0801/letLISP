(write
  (let rev
       ((a (let app ((a '(a b c))
                     (b '(x y z)))
             (if (eq? a '()) b
                 (cons (car a)
                       (app (cdr a)
                            b)))))
        (b '()))
    (if (eq? a '()) b
        (rev (cdr a)
             (cons (car a) b)))))
