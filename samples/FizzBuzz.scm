(write
  (let loop ((n 30) (r '()))
    (if (eq? n 0) r
    (if (eq? (remainder n 15) 0)
	(loop (- n 1)
	      (cons 'FizzBuzz r))
    (if (eq? (remainder n 3) 0)
	(loop (- n 1) (cons 'Fizz r))
    (if (eq? (remainder n 5) 0)
	(loop (- n 1) (cons 'Buzz r))
	(loop (- n 1) (cons n r))))))))
