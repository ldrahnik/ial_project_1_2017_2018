	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    // dokud neni prvni nulovy
    while (L->First != NULL) {

        tDLElemPtr elem;
        elem = L->First;

        // uvolneni pameti prvniho elementu
        free(elem);

        // posunuti na dalsi prvek
        L->First = L->First->rptr;
    }

    // vynulovani ukazatelu (L->First uz je NULL)
    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr newElem = NULL;   // novy prvek + alokovani pameti
    if ((newElem = malloc(sizeof(struct tDLElem))) == NULL) {
        DLError();
    } else {
        // vlozeni na zacatek
        newElem->lptr = NULL;
        newElem->rptr = L->First;

        // nastaveni dat
        newElem->data = val;

        // napojeni na zbytek seznamu
        if (L->First != NULL)
            L->First->lptr = newElem; // propojeni
        else
            L->Last = newElem;

        // novou polozku na zacatek
        L->First = newElem;
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr newElem = NULL;  // novy prvek + alokovani pameti
    if ((newElem = malloc(sizeof(struct tDLElem))) == NULL) {
        DLError();
    } else {
        // vlozeni na konec
        newElem->rptr = NULL;
        newElem->lptr = L->Last;

        // nastaveni dat
        newElem->data = val;

        if (L->First != NULL)
            L->Last->rptr = newElem;
        else
            L->First = newElem;

        // novou polozku na konec
        L->Last = newElem;
  }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    // seznam musi byt neprazdny jinak error
    if (L->First != NULL)
        *val = L->First->data;
    else
        DLError();
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    // seznam musi byt neprazdny jinak error
    if (L->First != NULL)
        *val = L->Last->data;
    else
        DLError();
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    // seznam musi byt neprazdny
    if (L->First != NULL) {

        // pomocny prvek
        tDLElemPtr elem;
        elem = L->First;

        // aktivni prvek - zrusime aktivitu
        if (L->First == L->Act)
            L->Act = NULL;

        // jediny prvek seznamu - nulujeme ukazatele
        if (L->First == L->Last) {
             L->First = NULL;
             L->Last = NULL;
        }
        // vice prvku seznamu
        else {
             L->First = L->First->rptr;
             L->First->lptr = NULL;
        }

        // uvolneni pameti
        free(elem);
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
    // seznam musi byt neprazdny
    if (L->First != NULL) {

        // pomocny prvek
        tDLElemPtr elem;
        elem = L->Last;

        // aktivni prvek - zrusime aktivitu
        if (L->Last == L->Act)
            L->Act = NULL;

        // jediny prvek seznamu - nulujeme ukazatele
        if (L->First == L->Last) {
            L->First = NULL;
            L->Last = NULL;
        }
        // vice prvku seznamu
        else {
            L->Last = L->Last->lptr;
            L->Last->rptr = NULL;
        }

        // uvolneni pameti
        free(elem);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    // pomocny prvek
    tDLElemPtr elem;

    // pokud je seznam L neaktivni nebo pokud je aktivni prvek posledni prvek -> nic se nedeje
    if (L->Act != NULL && L->First != NULL && L->Act != L->Last) {

        // prvek za aktivnim
        elem =  L->Act->rptr;

        // ukazatel na prvek, ktery rusime
        L->Act->rptr =  elem->rptr;

        // pokud za rusenou polozkou uz neni dalsi, aktivni je posledni
        if (elem == L->Last)
            L->Last =  L->Act;
            // rusena polozka neni posledni, polozku za provazeme s aktivni
        else
            elem->rptr->lptr =  L->Act;

        // uvolneni pameti
        free(elem);
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    // pomocny prvek
    tDLElemPtr elem;

    // seznam je aktivni, prvni prvek seznamu neni aktivni
    if (L->Act != NULL && L->First != NULL && L->Act != L->First) {

        // prvek pred aktivnim
        elem =  L->Act->lptr;

        // ukazatel na prvek, ktery chceme rusit
        L->Act->lptr =  elem->lptr;

        // pred rusenou polozkou uz nic neni, aktivni je prvni
        if (elem == L->First)
            L->First =  L->Act;
        // rusena polozka neni prvni, polozku pred provazeme s aktivni
        else
            elem->lptr->rptr =  L->Act;

        // uvolneni pameti
        free(elem);
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	// pouze pokud je seznam aktivni
    if (L->Act != NULL) {

        // novy prvek + alokace pameti
        tDLElemPtr newElem = NULL;
        if ((newElem = malloc(sizeof(struct tDLElem))) == NULL) {
            DLError();
        } else {
            // ukazatele na predchozi a dalsi
            newElem->lptr = L->Act;
            newElem->rptr = L->Act->rptr;

            // nastavime data
            newElem->data = val;

            // navazeme polozku do seznamu
            L->Act->rptr = newElem;

            // navazeme zprava novou polozku
            if (L->Act != L->Last)
	            newElem->rptr->lptr = newElem;
            // nastavime novou posledni polozku
            else
	            L->Last = newElem;
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	// seznam musi byt aktivni
    if (L->Act != NULL) {

        // novy prvek + alokace pameti
        tDLElemPtr newElem = NULL;
        if ((newElem = malloc(sizeof(struct tDLElem))) == NULL) {
            DLError();
        } else {
          // ukazatele na predchozi a dalsi
          newElem->rptr = L->Act;
          newElem->lptr =  L->Act->lptr;

          // priradime data
          newElem->data =  val;

          // navazeme polozku do seznamu
          L->Act->lptr = newElem;

          // navazeme zleva novou polozku
          if (L->Act != L->First)
    	       newElem->lptr->rptr = newElem;
          // nastavime novou prvni polozku
          else
    	       L->First = newElem;
        }
      }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    // seznam musi byt aktivni jinak error
    if (L->Act != NULL)
        *val = L->Act->data;
    else
        DLError();
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    // seznam musi byt aktivni
    if (L->Act != NULL)
        L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    // seznam musi byt aktivni
    if (L->Act != NULL) {

        // aktivni prvek neni posledni -> posun
        if (L->Act != L->Last)
            L->Act = L->Act->rptr;
        else // zrusena aktivita
            L->Act = NULL;
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    // seznam musi byt aktivni
    if (L->Act != NULL) {

        // aktivni prvek neni prvni -> posun
        if (L->Act != L->First)
            L->Act = L->Act->lptr;
        else
            L->Act = NULL;
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return (L->Act != NULL);
}

/* Konec c206.c*/
