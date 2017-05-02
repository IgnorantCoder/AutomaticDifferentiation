using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sample
{
    class CallOptionPricerBottomup
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

        private static Adcs.Bu.Variable N(Adcs.Bu.Variable x)
        {
            return 0.5 * (1.0 + Adcs.Bu.Math.Erf(x / Math.Sqrt(2.0)));
        }

        private static Adcs.Bu.Variable Ndash(Adcs.Bu.Variable x)
        {
            return Adcs.Bu.Math.Exp(-(x * x) / 2.0) / Math.Sqrt(2.0 * Math.PI);
        }

        private static Adcs.Bu.Variable CalcD1(
            Adcs.Bu.Variable x,
            Adcs.Bu.Variable t,
            Adcs.Bu.Variable k,
            Adcs.Bu.Variable s,
            Adcs.Bu.Variable r)
        {
            return (Adcs.Bu.Math.Log(x / k) + (r + s * s * 0.5) * t) / (s * Adcs.Bu.Math.Sqrt(t));
        }

        private static Adcs.Bu.Variable CalcD2(
            Adcs.Bu.Variable x,
            Adcs.Bu.Variable t,
            Adcs.Bu.Variable k,
            Adcs.Bu.Variable s,
            Adcs.Bu.Variable r)
        {
            return (Adcs.Bu.Math.Log(x / k) + (r - s * s * 0.5) * t) / (s * Adcs.Bu.Math.Sqrt(t));
        }

        public static Adcs.Bu.Variable CalcPrice(
            Adcs.Bu.Variable x, 
            Adcs.Bu.Variable t,
            Adcs.Bu.Variable k,
            Adcs.Bu.Variable s,
            Adcs.Bu.Variable r)
        {
            var d1 = CalcD1(x, t, k, s, r);
            var d2 = CalcD2(x, t, k, s, r);
            var df = Adcs.Bu.Math.Exp(-r * t);
            return x * N(d1) - df * k * N(d2);
        }

        static public CallOptionPricerBottomup Calculate(
            Double x,
            Double t,
            Double k,
            Double s,
            Double r)
        {
            var mgr = Adcs.Bu.VariableManager.Create();
            var v_x = mgr.ToVariable(x);
            var v_t = mgr.ToVariable(t);
            var v_k = mgr.ToVariable(k);
            var v_s = mgr.ToVariable(s);
            var v_r = mgr.ToVariable(r);
            var ret = CalcPrice(
                x: v_x,
                t: v_t,
                k: v_k,
                s: v_s,
                r: v_r);
            var dy = Adcs.Bu.Derivative.Create(ret);

            var data = new Dictionary<String, Double>
            {
                { @"Price", ret.ToDouble() },
                { @"Delta", dy.D(v_x) },
                { @"Vega", dy.D(v_s) },
                { @"Theta", dy.D(v_t) }
            };

            return new CallOptionPricerBottomup(data);
        }

        private CallOptionPricerBottomup(Dictionary<String, Double> result)
        {
            this.Result = result;
        }
    }
}
