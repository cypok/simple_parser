Simple recursive descent parser for arithmetic expressions with following grammar:

    E -> T { [+−] T }*
    T -> F { [* /] F }*
    F -> NUM | ( E )
