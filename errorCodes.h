#include <stdio.h>

typedef enum errorCodes{
    ErrorCodeOk = 0, // preklad prebeho bez chyby
    ErrorCodeLex = 1, // chyba programu v ramci lexikalnej analyzy
    ErrorCodeSyn = 2, // chyba programu v ramci syntaktickej analyzy
    ErrorCodeSemUndf = 3, // chyba programu v ramci semantickej analyzy, nedefinovana funkcia/premenna
    ErrorCodeSemParam = 4, // chyba programu v ramci semantickej analyzy, nespravny pocet/typ parametru pri volani funkcie
    ErrorCodeSemRedef = 5, // chyba programu v ramci semantickej analyzy, redefinicia premennej/funkcie
    ErrorCodeSemRetExp = 6, // chyba programu v ramci semantickej analyzy, chybajuci/nadbytocny vyraz v navrate funkcie
    ErrorCodeSemComp = 7, // chyba programu v ramci semantickej analyzy, nekompatibilny typ vyrazu
    ErrorCodeSemNoType = 8, // chyba programu v ramci semantickej analyzy, nie je uvedeny/neda sa urcit typ premennej
    ErrorCodeSemUnuserVar = 9, // chyba programu v ramci semantickej analyzy, nevyuzita premenna
    ErrorCodeSemOther = 10, // chyba programu v ramci semantickej analyzy, ostatne chyby
    ErrorCodeInternal = 99, // chyba programu v ramci prekladaca, interne chyby
} ErrorCodes;