	/*
	Tytuł: calculation.h
	Przeznaczenie kodu: Składowa kodu obliczającego prędkość obrotową
	Wersja: 1.0 stabilna
	Autor: Michał Boguski
	Data: 18.01.2025
	Wymagania sprzętowe: Windows 10
	*/


#ifndef _CALCULATION_H
#define _CALCULATION_H

#include <inttypes.h>
#include <stdio.h>
#include "pico/fft.h"

#pragma once
float calculate_frequency(uint16_t *capture_buf);
void precalculate_window();

#endif