
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, říjen 2017                                        */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;


/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
    // pomocny znak
    char topChar;

    // nacteni znaku z vrcholu zasobniku
    stackTop(s, &topChar);

    // dokud neni zasobnik prazdny hledame levou zavorku `(`, kterou take odstranime -> do te doby odstranujeme znaky
    while (!stackEmpty(s)) {
        if (topChar != '(') {

    	    // vlozeni znaku do vystupniho pole
    		postExpr[*postLen] = topChar;

    		// posuneme ukazatel na prvni volne misto ve vystupnim poli
    		*postLen += 1;

            // znak odebereme ze zasobniku
    		stackPop(s);

    		// nacteme dalsi znak ze zasobniku
    		stackTop(s, &topChar);
    	} else {

    	    // leva zavorka bude take odstranena (pouze)
    		stackPop(s);

    		return;
    	}
    }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
	char topChar;

	// nacteni znaku z vrcholu zasobniku pokud neni zasobnik prazdny
	if (!stackEmpty(s)) {
		stackTop(s, &topChar);
	}

	// Operátor vkladam na vrchol zasobniku v pripade, ze:
	// - zásobník je prázdný
	// - na vrcholu zásobníku je levá závorka
	// - na vrcholu zásobníku je operátor s nižší prioritou
	if (
	    (stackEmpty(s))  ||
	    (topChar == '(') ||
	    (topChar == '+' && c != '-' && c != '+') ||
	    (topChar == '-' && c != '-' && c != '+')
	   ) {
		stackPush(s, c);
	} else {
		postExpr[*postLen] = topChar;
		*postLen += 1;
		stackPop(s);
		doOperation(s, c, postExpr, postLen);
	}
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
    // vystupni retezec + alokace vystupniho retezce
	char *postExpr;
    if ((postExpr = (char *) malloc(MAX_LEN)) == NULL)
    	return NULL;

    // alokace a init zasobniku
	tStack *s;
    if ((s = (tStack*) malloc(sizeof(tStack))) == NULL)
    	return NULL;
    stackInit(s);

    // zjisteni delky retezce
    int infExprLength = 0;

    // Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
    // char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
    while (infExpr[infExprLength] != '\0') infExprLength++;

	// zpracovani retezce
	unsigned int i, postLen = 0;
	for (i = 0; i <= infExprLength; i++) {

	    // Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
        // a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
		if ((infExpr[i] >= '0' && infExpr[i] <= '9') ||
		    (infExpr[i] >= 'A' && infExpr[i] <= 'Z') ||
		    (infExpr[i] >= 'a' && infExpr[i] <= 'z')
		   ) {
		    // pridame do vysledneho vyrazu a inkrementujeme pozici
			postExpr[postLen] = infExpr[i];
			postLen++;
		} else if (infExpr[i] == ')') {
			untilLeftPar(s, postExpr, &postLen);
		} else if (infExpr[i] == '(') {
		    stackPush(s, infExpr[i]);
		// Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
        // ukončovacím znakem '='.
		} else if (infExpr[i] == '=') {
			while (!stackEmpty(s)) {
				stackTop(s, &postExpr[postLen]);
				stackPop(s);
				postLen++;
			}
			postExpr[postLen] = '=';
			postLen++;

			// Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
            // char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
			postExpr[postLen] = '\0';
		// Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
        // násobení a dělení.
		} else if ((infExpr[i] == '+' || infExpr[i] == '-') ||
		            infExpr[i] == '*' || infExpr[i] == '/') {
			doOperation(s, infExpr[i], postExpr, &postLen);
		} else {
		    // Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy.
		}
	}

    // uvolneni pameti zasobniku
	free(s);

    // vracim postExpr - k uvolneni pameti nedochazi (NUTNO UVOLNIT PAMET!)
    return postExpr;
}

/* Konec c204.c */
