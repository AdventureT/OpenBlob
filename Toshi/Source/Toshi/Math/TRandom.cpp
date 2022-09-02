#include "pch.h"
#include "TRandom.h"

void Toshi::TRandom::Isaac()
{
    register unsigned int a, b, x, y, * m, * mm, * m2, * r, * mend;
    mm = m_pRandmem; 
    r = m_pRandrsl;
    a = m_uiRandA; 
    b = cut(m_uiRandB + (++m_uiRandC));
    for (m = mm, mend = m2 = m + (RANDSIZ / 2); m < mend; m+=4)
    {
        shuffle(a, b, mm, m, m2, r, x);
    }
    for (m2 = mm; m2 < m + (RANDSIZ / 2); m2+=4)
    {
        shuffle(a, b, mm, m, m2, r, x);
    }
    m_uiRandA = a;
    m_uiRandB = b;
}

void Toshi::TRandom::RandInit()
{
    unsigned int a, b, c, d, e, f, g, h;
    int i;

    m_uiRandA = m_uiRandB = m_uiRandC = 0;

    /* Initialize a to h with the golden ratio and seed */
    a = b = c = d = e = f = g = h = 0x9e3779b9 + m_uiSeed;

    /* scramble it */
    for (i = 0; i < 4; i++) {
        mix(a, b, c, d, e, f, g, h);
    }

    unsigned int *m = m_pRandmem;
    unsigned int* r = m_pRandrsl;

    /* initialize using the contents of r[] as the seed */
    for (i = 0; i < RANDSIZ; i += 8) {
        a += r[i];
        b += r[i + 1];
        c += r[i + 2];
        d += r[i + 3];
        e += r[i + 4];
        f += r[i + 5];
        g += r[i + 6];
        h += r[i + 7];

        mix(a, b, c, d, e, f, g, h);

        m[i] = a;
        m[i + 1] = b;
        m[i + 2] = c;
        m[i + 3] = d;
        m[i + 4] = e;
        m[i + 5] = f;
        m[i + 6] = g;
        m[i + 7] = h;
    }

    /* do a second pass to make all of the seed affect all of m */
    for (i = 0; i < RANDSIZ; i += 8) {
        a += m[i];
        b += m[i + 1];
        c += m[i + 2];
        d += m[i + 3];
        e += m[i + 4];
        f += m[i + 5];
        g += m[i + 6];
        h += m[i + 7];

        mix(a, b, c, d, e, f, g, h);

        m[i] = a;
        m[i + 1] = b;
        m[i + 2] = c;
        m[i + 3] = d;
        m[i + 4] = e;
        m[i + 5] = f;
        m[i + 6] = g;
        m[i + 7] = h;
    }

    Isaac();
    m_uiRndCnt = RANDSIZ;

}

int Toshi::TRandom::GetInt(int a_iLower, int a_iUpper)
{
    TASSERT(a_iUpper > a_iLower, "");
    return a_iLower + (a_iUpper - a_iLower) * (RandRaw() >> 0x20);
}

int Toshi::TRandom::GetInt(int a_iUpper)
{
    return a_iUpper * RandRaw() >> 0x20;
}

int Toshi::TRandom::GetInt()
{
    return RandRaw();
}

float Toshi::TRandom::GetFloat()
{
    return (RandRaw() >> 1) * (float)0x30000000;
}

float Toshi::TRandom::GetFloat2()
{
    return (RandRaw() >> 1) * (float)0x30800000 * -1.0f;
}

void Toshi::TRandom::SetSeed(unsigned int a_uiSeed)
{
    m_uiSeed = a_uiSeed;
    RandInit();
}

unsigned int Toshi::TRandom::RandRaw()
{
    if (m_uiRndCnt-- == 0)
    {
        Isaac();
        m_uiRndCnt = RANDSIZ - 1;
        //return *(uint *)((int)this + 0x44);
    }
    return m_pRandrsl[m_uiRndCnt];
}

Toshi::TRandom::TRandom()
{
    THPTimer timer = THPTimer();
    SetSeed(timer.GetRaw32());
}
