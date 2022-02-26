(write
  (let loop ((x 10) (y 5) (z 0))
    (if (< y x)
        (loop (loop (- x 1) y z)
              (loop (- y 1) z x)
              (loop (- z 1) x y))
        y)))
