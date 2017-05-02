using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sample
{
    class CallOptionPricerTopdown
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

        private static Adcs.Td.Variable N(Adcs.Td.Variable x)
        {
            return 0.5 * (1.0 + Adcs.Td.Math.Erf(x / Math.Sqrt(2.0)));
        }

        private static Adcs.Td.Variable Ndash(Adcs.Td.Variable x)
        {
            return Adcs.Td.Math.Exp(-(x * x) / 2.0) / Math.Sqrt(2.0 * Math.PI);
        }

        private static Adcs.Td.Variable CalcD1(
            Adcs.Td.Variable x,
            Adcs.Td.Variable t,
            Adcs.Td.Variable k,
            Adcs.Td.Variable s,
            Adcs.Td.Variable r)
        {
            return (Adcs.Td.Math.Log(x / k) + (r + s * s * 0.5) * t) / (s * Adcs.Td.Math.Sqrt(t));
        }

        private static Adcs.Td.Variable CalcD2(
            Adcs.Td.Variable x,
            Adcs.Td.Variable t,
            Adcs.Td.Variable k,
            Adcs.Td.Variable s,
            Adcs.Td.Variable r)
        {
            return (Adcs.Td.Math.Log(x / k) + (r - s * s * 0.5) * t) / (s * Adcs.Td.Math.Sqrt(t));
        }

        public static Adcs.Td.Variable CalcPrice(
            Adcs.Td.Variable x, 
            Adcs.Td.Variable t,
            Adcs.Td.Variable k,
            Adcs.Td.Variable s,
            Adcs.Td.Variable r)
        {
            var d1 = CalcD1(x, t, k, s, r);
            var d2 = CalcD2(x, t, k, s, r);
            var df = Adcs.Td.Math.Exp(-r * t);
            return x * N(d1) - df * k * N(d2);
        }

        static public CallOptionPricerTopdown Calculate(
            Double x,
            Double t,
            Double k,
            Double s,
            Double r)
        {
            var mgr = Adcs.Td.VariableManager.Create();
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
            var dy = Adcs.Td.Gradient.Generate(ret);

            var data = new Dictionary<String, Double>
            {
                { @"Price", ret.ToDouble() },
                { @"Delta", dy.InDirectionOf(v_x) },
                { @"Vega", dy.InDirectionOf(v_s) },
                { @"Theta", dy.InDirectionOf(v_t) }
            };

            return new CallOptionPricerTopdown(data);
        }

        private CallOptionPricerTopdown(Dictionary<String, Double> result)
        {
            this.Result = result;
        }
    }
}
