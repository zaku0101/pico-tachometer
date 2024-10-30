#include "global.h"

int main(void){
    //sterowanie: jeden przycisk z interrupem, wywołanie powoduje zmiane stanu state na "menu" jeżeli wczesniej był inny stan
    //              dwa przyciski sterujące, 1. dalej, 2. OK/ENTER           
    //main loop
    while(1){
        switch (state){
        case init:
            /*Włączenie peryferi, inicjalizacja składowej stałej*/
            state = measure;
            break;
        case menu:
            /*Wybór operacji: pomiar, kalibracja, debug
            Czekaj do póki nie zostanie wciśnięty przycisk*/
            menu_handler();
            break;
        case measure:
            /*Pomiar i wyswietlanie wyniku na ekranie
            pomiar przez sekunde
            oblicz
            wyswielt
            W razie niepewnych pomiarow, komunikat np. out of range*/
            break;
        case calibration:
            /*Pomiar składowej stałej pochodzącej z oświetlenia w pomieszczeniu
            Wybranie czy obiekt to czarna tarcza z białym punktem, czy biała z czarnym*/
            break;
        case debug:
            /*
            Logi po USB
            */
            break;
        
        default:
            break;
        };

    }
    return 0;
}

void przerwanie_menu(void){
    if(state == measure) state = menu;
    menu_selection = measure;
}
void przerwanie_enter(void){
    button_enter_value = 1;
}
void przerwanie_next(void){
    button_next_value = 1;
}

void menu_handler(void){
    while (button_enter_value != 1)
    {
        if (button_next_value){
            (++menu_selection) % number_of_menu_options;
            button_next_value = 0;
        }
            
        switch (menu_selection)
        {
        case mmeasure:
            // wyświetl napis measure
            state = measure;
            break;
        case mcalibration:
            // wyświetl napis calibration
            state = calibration;
            break;
        case mdebug:
            // wyświetl napis debug
            state = debug;
            break;
        default:
            break;
        };
    };
}