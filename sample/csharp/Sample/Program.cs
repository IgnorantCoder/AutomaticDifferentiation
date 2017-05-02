using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sample
{
    class Program
    {
        static Dictionary<String, Double> SampleData
        {
            get
            {
                return new Dictionary<String, Double> {
                    { @"x", 107 },
                    { @"t", 0.5 },
                    { @"k", 100 },
                    { @"s", 0.3 },
                    { @"r", 0.1 }};
            }
        }

        static void Analytic()
        {
            var result = CallOptionPricerDouble.Calculate(
                x: SampleData["x"],
                t: SampleData["t"],
                k: SampleData["k"],
                s: SampleData["s"],
                r: SampleData["r"]);
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"price", result.Price));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"delta", result.Delta));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"theta", result.Theta));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"vega", result.Vega));
            return;
        }

        static void TestBottomup()
        {
            var result = CallOptionPricerBottomup.Calculate(
               x: SampleData["x"],
               t: SampleData["t"],
               k: SampleData["k"],
               s: SampleData["s"],
               r: SampleData["r"]);
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"price", result.Price));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"delta", result.Delta));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"theta", result.Theta));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"vega", result.Vega));
            return;
        }

        static void TestTopdown()
        {
            var result = CallOptionPricerTopdown.Calculate(
               x: SampleData["x"],
               t: SampleData["t"],
               k: SampleData["k"],
               s: SampleData["s"],
               r: SampleData["r"]);
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"price", result.Price));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"delta", result.Delta));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"theta", result.Theta));
            System.Console.WriteLine(
                String.Format(@"{0}:{1}", @"vega", result.Vega));
            return;
        }

        static void Main(string[] args)
        {
            System.Console.WriteLine(@"start analytic calculation");
            Analytic();
            System.Console.WriteLine(@"end analytic calculation");

            System.Console.WriteLine(@"start bottom up test");
            TestBottomup();
            System.Console.WriteLine(@"end bottom up test");

            System.Console.WriteLine(@"start top down test");
            TestTopdown();
            System.Console.WriteLine(@"end top down test");
        }
    }
}
