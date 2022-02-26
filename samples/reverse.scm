(write
  (let rev ((a '(x y z w v)))
    (let loop ((a a) (b '()))
      (if (eq? a '()) b
          (loop (cdr a)
                (cons (car a) b))))))

