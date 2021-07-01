    no equ #x38
    STORE $x100 no 
    SHL REG2 #39
    JMPEQ REG1
hello:
    ADD $x100 #3320