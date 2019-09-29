using System;

namespace task4
{
    class Calc
    {
        readonly double eps;
        public double Result { get; private set; }
        public Calc(double eps)
        {
            this.eps = eps;
            Result = 0;
        }
        public void CalcPi()
        { 
            double term;
            uint i = 0;
            do
            {
                term = 4 * Math.Pow(-1, i) / (2 * i + 1);
                Result += term;
                ++i;
            }
            while (Math.Abs(term) > eps);
        }
    }
}
