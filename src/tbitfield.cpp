// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = (BitLen + 15) >> 5;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	if (pMem != 0)
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n >> 5);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n > 31) && (n < 0)) {
		throw std::runtime_error("Unimprobable index of bit!");
	}
	TELEM masska = 1;
	return masska << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int i = GetMemIndex(n);
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n - i*32);
	else
		throw std::runtime_error("Unimprobable index of bit!");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int i = GetMemIndex(n);
	if ((n > -1) && (n < BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n - i*32);
	else
		throw std::runtime_error("Unimprobable index of bit!");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	int i = GetMemIndex(n);
	if (pMem[GetMemIndex(n)] &= ~GetMemMask(n - i*32))
		return 1;
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) {
		MemLen = bf.MemLen;
		if (pMem != NULL)
			delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL) 
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen )
		res = 0;
		for(int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) {
				res = 0;
				break;
			}
		return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res = 0;
	if (BitLen != bf.BitLen)
		res = 1;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) {
			res = 1;
			break;
		}
	return res;
}


TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.BitLen; i++)
		temp.pMem[i] |= bf.pMem[i];
	return temp;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.BitLen; i++)
		temp.pMem[i] &= bf.pMem[i];
	return temp;

}

TBitField TBitField::operator~(void) // отрицание
{
	int len = BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = ~pMem[i];
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int a;
	for (int i = 0; i < bf.GetLength(); i++) {
		istr >> a;
		if (a == 1)
			bf.SetBit(i);
		else
			bf.ClrBit(i);
	}
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
		ostr << bf.GetBit(i);
	return ostr;
}
