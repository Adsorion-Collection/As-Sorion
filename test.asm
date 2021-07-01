    no equ #x38
    STORE $x100 no 
    SHL REG2h #39
    JMPEQ REG1h
hello:
    ADD $x100 #3320