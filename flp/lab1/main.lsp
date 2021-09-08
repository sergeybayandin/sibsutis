(defun get_star()
	(cadr(cadadr'((1 2) (3 (4 *) 5))))
)

(defun what()
	(append nil '(a b c))
)

#((1 (2 (3))))

(defun with_cons()
	(cons (cons 1 (cons (cons 2 (cons (cons 3 nil) nil)) nil)) nil)
)

(defun with_list()
	(list (list 1 (list 2 (list 3))))
)

(defun replace_first_third(l)
	(append (list (caddr l) (cadr l) (car l)) (cdddr l))
)
