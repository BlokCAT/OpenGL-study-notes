#pragma once

//Ԥ�����
#ifdef DEBUG
#define CK(func) func;checkError();
#else
#define CK(func) func;
#endif
void checkError();