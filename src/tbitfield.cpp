// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) : BitLen(len)
{
	if (len <= 0) throw "Length is not correct";
	BitLen = len;
	MemLen = (BitLen - 1) / (8 * sizeof(TELEM)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n > BitLen) || (n < 0))
	{
		throw "Not correct";
	}
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n > BitLen) || (n < 0))
	{
		throw "Not correct";
	}
	return TELEM(1) << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int i = GetMemIndex(n);
	if ((n > BitLen) || (n < 0))
	{
		throw "Not correct";
	}
	pMem[i] = pMem[i] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n > BitLen) || (n < 0))
	{
		throw "Not correct";
	}
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n > BitLen) || (n < 0))
	{
		throw "Not correct";
	}
	int i = GetMemIndex(n);
	int Bit = 0;
	if (pMem[i] & GetMemMask(n))
		Bit = 1;
	return Bit;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (&bf != this)
	{
		if (BitLen == bf.BitLen)
			for (int i = 0; i < MemLen; i++)
				pMem[i] = bf.pMem[i];
		else
		{
			BitLen = bf.BitLen;
			MemLen = bf.MemLen;
			pMem = new TELEM[MemLen];
			for (int i = 0; i < MemLen; i++)
				pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen) res = 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) {
			res = 0;
			break;
		}

	return res;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	int res = 0;
	if (BitLen != bf.BitLen) res = 1;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) {
			res = 1;
			break;
		}
	return res;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > BitLen)
		len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (i = 0; i < MemLen; i++)
		temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int len = MemLen;
	int Max = BitLen;
	int Min = bf.BitLen;
	if (bf.BitLen >= BitLen)
	{
		len = bf.MemLen;
		Max = bf.BitLen;
		Min = BitLen;
	}
	TBitField temp(Max);
	for (int i = 0; i < len - 1; i++)
	{
		temp.pMem[i] = bf.pMem[i] & pMem[i];
	}
	for (int i = (len - 1) * sizeof(TELEM) * 8; i < Min; i++)
		if (bf.GetBit(i) && GetBit(i))
			temp.SetBit(i);
	return temp;

}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField Tmp(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			Tmp.SetBit(i);
		else Tmp.ClrBit(i);
	}
	return Tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	string Tmp;
	istr >> Tmp;
	bf = TBitField(Tmp.size());
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (Tmp[i] == 1)
			bf.SetBit(i);
		else if (Tmp[i] != 0)
			throw "Not a bitfield";
	}
	return istr;

}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{

	for (int i = 0; i < bf.BitLen; ++i)

	 ostr << bf.GetBit(i);
	return ostr;
}
