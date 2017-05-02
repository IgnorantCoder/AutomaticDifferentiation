#pragma once

namespace Adcs { namespace Bu {
    ref class Variable;

    public ref class Math {
    public:
        static Variable^ Sqrt(Variable^ x);
        static Variable^ Sin(Variable^ x);
        static Variable^ Cos(Variable^ x);
        static Variable^ Tan(Variable^ x);
        static Variable^ Log(Variable^ x);
        static Variable^ Exp(Variable^ x);
        static Variable^ Erf(Variable^ x);
    };
}}