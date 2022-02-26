(write
  (let propsearch
       ((p '(a 1 b 2 c 3))
        (k 'b))
    (if (eq? p '()) '()
    (if (eq? k (car p)) (car (cdr p))
    (propsearch (cdr (cdr p)) k)))))
