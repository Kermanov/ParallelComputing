using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace task3
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
