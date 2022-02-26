(write
  (let loop ((n (read)) (a 0) (b 1))
    (if (eq? n 0) (cons a '())
	(cons a
	      (loop (- n 1)
		    b (+ a b))))))
