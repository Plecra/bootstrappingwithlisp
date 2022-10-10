

; === Forms ===
;   (quote x)
;   (#def x y)
;   (#= x y)
;   (#let x y z)
;   (#lambda (args) body)
;   (#if cond true false)
;   (#do . body)
;   ()
;   #true
;   #false

; === Magic ===
;   (defmacro (name . args) . body)

; === Builtins ===
;   (car x)
;   (cdr x)
;   (cons x y)
;   (pair? x)
;   (nil? x)
;   (+ x y)
;   (- x y)


; === Bootstrap ===

; List function (defined using primitives)
(#define list (#lambda xs xs))

; Standardised `def` form that can do function and value definitions
(defmacro (def name . rest)
    (#if (pair? name)
         (list 'define
               (car name)
               (list (string->symbol "lambda")
                     (cdr name)
                     (cons (string->symbol "do") rest)))
         (list (string->symbol "define")
               name
               (cons (string->symbol "do") rest))))

; Truthiness function
; TODO: more builtin types need to be checked here!
(def (bool x)

    ; Can *only* work on booleans, as we dont have a safe `if` form yet.
    (def (unsafe-not y) (#if y #false #true))

    (#if (pair? x) #true
    (#if (int? x) (unsafe-not (int-eq? x 0))
    (#if (float? x) (unsafe-not (float-eq? x 0.0)))
    (#if (string? x) (unsafe-not (int-eq? (string-length x) 0))

    #false))))

; Standardised `if` form - `#if` can *only* accept boolean arguments.
(defmacro (if a b c) (list (list `bool a) b c))

; Logical operations
(defmacro (and a b)
    (list `def `a-evald a)
    (list `if `a-evald `a-evald b))

; Cond macro
(defmacro (cond . cases)

    (def (rec cases)
        (if (pair? cases)
            (do
                (def case (car cases))
                (def rest (cdr cases))

                ; Dont have general equality yet!
                (if (and (nil? rest) (and (symbol? (car rest)) (symbol-eq? (car rest) `else)))
                    (cdr case)
                    (list `if (car case) (cdr case) (rec rest))))

            #nil))
    
    (rec cases))

; Equality function that can compare any types together
(defmacro (

; Recursive quasiquote (no unquote-splicing yet)
(defmacro (quasiquote value)

    (def (index i l)
        (if (eq? i 0)
            (car l)
            (index (- i 1)
                (cdr l))))

    (def (qq-value value)
        (if (pair? value)
            (if (eq? (car value) `unquote)
                (index 1 value)
                (list `pair (qq-value (car value)) (qq-value (cdr value))))
            (list `quote value)))

    (qq-value value))

; Standardised `let` form that can bind multiple values
(defmacro (let args . body)
    (if (nil? args)
        in
        (let a (head args)
            '(let (head a) (tail a) (let* (tail args) in)))))







(def (list . xs) xs)

(def (eq? x y) ...)

(def (index i l)
    (if (eq? i 0)
        (head l)
        (index (- i 1)
               (tail l))))

(defmacro (quasiquote value)
    (def (qq-value value)
        (if (pair? value)
            (if (eq? (head value) `unquote)
                (index 1 value)
                (list `pair (qq-value (head value)) (qq-value (tail value))))
            (list `quote value)))
    (qq-value value))

(defmacro (let* args in)
    (if (nil? args)
        in
        (let a (head args)
            '(let (head a) (tail a) (let* (tail args) in)))))

; Parser

; Macro expand

; Compiler


This is one char!
﷽
hello world!
