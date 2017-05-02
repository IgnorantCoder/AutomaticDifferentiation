using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sample
{
    class CallOptionPricerDouble 
    {
        private Dictionary<String, Double> Result { get; }
        public Double Price
        {
            get
            {
                return this.Result[@"Price"];
            }
        }

        public double Delta
        {
            get
            {
                return this.Result[@"Delta"];
            }
        }

        public double Theta
        {
            get
            {
                return this.Result[@"Theta"];
            }
        }

        public double Vega
        {
            get
            {
                return this.Result[@"Vega"];
            }
        }

        private static Double N(Double x)
        {
            return 0.5 * (1.0 + CMath.Math.Erf(x / Math.Sqrt(2.0)));
        }

        private static Double Ndash(Double x)
        {
            return Math.Exp(-(x * x) / 2.0) / Math.Sqrt(2.0 * Math.PI);
        }

        private static Double CalcD1(Double x, Double t, Double k, Double s, Double r)
        {
            return (Math.Log(x / k) + (r + s * s * 0.5) * t) / (s * Math.Sqrt(t));
        }

        private static Double CalcD2(Double x, Double t, Double k, Double s, Double r)
        {
            return (Math.Log(x / k) + (r - s * s * 0.5) * t) / (s * Math.Sqrt(t));
        }

        public static Double CalcPrice(Double x, Double t, Double k, Double s, Double r)
        {
            var d1 = CalcD1(x, t, k, s, r);
            var d2 = CalcD2(x, t, k, s, r);
            var df = Math.Exp(-r * t);
            return x * N(d1) - df * k * N(d2);
        }

        public static Double CalcDelta(Double x, Double t, Double k, Double s, Double r)
        {
            var d1 = CalcD1(x, t, k, s, r);
            return N(d1);
        }

        public static Double CalcTheta(Double x, Double t, Double k, Double s, Double r)
        {
            var d1 = CalcD1(x, t, k, s, r);
            var d2 = CalcD2(x, t, k, s, r);
            var df = Math.Exp(-r * t);

            return (x * Ndash(d1) * s / (2.0 * Math.Sqrt(t))) + (r * k * df * N(d2));
        }

        public static Double CalcVega(Double x, Double t, Double k, Double s, Double r)
        {
            var d1 = CalcD1(x, t, k, s, r);
            return x * Ndash(d1) * Math.Sqrt(t);
        }

        static public CallOptionPricerDouble Calculate(
            Double x,
            Double t,
            Double k,
            Double s,
            Double r)
        {
            var data = new Dictionary<String, Double>
            {
                { @"Price", CalcPrice(x, t, k, s, r) },
                { @"Delta", CalcDelta(x, t, k, s, r) },
                { @"Vega", CalcVega(x, t, k, s, r) },
                { @"Theta", CalcTheta(x, t, k, s, r) }
            };

            return new CallOptionPricerDouble(data);
        }

        private CallOptionPricerDouble(Dictionary<String, Double> result)
        {
            this.Result = result;
        }
    }
}
