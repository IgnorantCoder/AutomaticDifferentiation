import math
from adpy import bottomup, topdown

def create_normal_distribution_cdf_factory(erf) :
    def generate(mu, sigma) :
        def calc(x) :
            return 0.5 * (1.0 + erf((x - mu) / math.sqrt(2.0 * sigma * sigma)))
        return calc
    return generate

def create_normal_distribution_sdf_factory(exp) :
    def generate(mu, sigma) :
        def calc(x) :
            return exp(- (x - mu) * (x - mu) / (2.0 * sigma * sigma)) / math.sqrt(2.0 * math.pi * sigma * sigma)
        return calc
    return generate

def create_d1_calculator(log, sqrt) :
    def calc(x, t, k, s, r) :
        return (log(x / k) + (r + s * s * 0.5) * t) / (s * sqrt(t))
    return calc

def create_d2_calculator(log, sqrt) :
    def calc(x, t, k, s, r) :
        return (log(x / k) + (r - s * s * 0.5) * t) / (s * sqrt(t))
    return calc

def create_call_option_calculator(exp, log, sqrt, erf) :
    calc_d1 = create_d1_calculator(log, sqrt)
    calc_d2 = create_d2_calculator(log, sqrt)
    n = create_normal_distribution_cdf_factory(erf)(0.0, 1.0)
    def calc(x, t, k, s, r) :
        d = exp(- r * t)
        return x * n(calc_d1(x, t, k, s, r)) - d * k * n(calc_d2(x, t, k, s, r))
    return calc

def calc_delta(x, t, k, s, r) :
    calc_d1 = create_d1_calculator(math.log, math.sqrt)
    n = create_normal_distribution_cdf_factory(math.erf)(0.0, 1.0)
    return n(calc_d1(x, t, k, s, r))

def calc_theta(x, t, k, s, r) :
    calc_d1 = create_d1_calculator(math.log, math.sqrt)
    calc_d2 = create_d2_calculator(math.log, math.sqrt)
    n = create_normal_distribution_cdf_factory(math.erf)(0.0, 1.0)
    ndash = create_normal_distribution_sdf_factory(math.exp)(0.0, 1.0)
    d = math.exp(- r * t)
    return (x * ndash(calc_d1(x, t, k, s, r)) * s / (2.0 * math.sqrt(t))) + (r * k * d * n(calc_d2(x, t, k, s, r)))

def calc_vega(x, t, k, s, r) :
    calc_d1 = create_d1_calculator(math.log, math.sqrt)
    ndash = create_normal_distribution_sdf_factory(math.exp)(0.0, 1.0)
    return x * ndash(calc_d1(x, t, k, s, r)) * math.sqrt(t)

def sample_data() :
    return {"x" : 107, "k" : 100, "t" : 0.5, "s" : 0.3, "r" : 0.1 }

def analytic(test_data) :
    '''
    calculate without automatic differentiation
    '''
    calc_calloption = create_call_option_calculator(
        math.exp,
        math.log,
        math.sqrt,
        math.erf)
    v_x = test_data["x"]
    v_t = test_data["t"]
    v_k = test_data["k"]
    v_s = test_data["s"]
    v_r = test_data["r"]
    p = calc_calloption(x = v_x, t = v_t, k = v_k, s = v_s, r = v_r)
    print('{}:{}'.format('price', p))
    print('{}:{}'.format('delta', calc_delta(x = v_x, t = v_t, k = v_k, s = v_s, r = v_r)))
    print('{}:{}'.format('theta', calc_theta(x = v_x, t = v_t, k = v_k, s = v_s, r = v_r)))
    print('{}:{}'.format('vega', calc_vega(x = v_x, t = v_t, k = v_k, s = v_s, r = v_r)))

    return

def test_bottomup(test_data) :
    mgr = bottomup.create_variable_manager()
    calc_calloption = create_call_option_calculator(
        bottomup.exp,
        bottomup.log,
        bottomup.sqrt,
        bottomup.erf)
    v_x = mgr.to_variable(test_data["x"])
    v_t = mgr.to_variable(test_data["t"])
    v_k = mgr.to_variable(test_data["k"])
    v_s = mgr.to_variable(test_data["s"])
    v_r = mgr.to_variable(test_data["r"])

    p = calc_calloption(x = v_x, t = v_t, k = v_k, s = v_s, r = v_r)
    print('{}:{}'.format('price', bottomup.to_double(p)))
    print('{}:{}'.format('delta', bottomup.d(p).d(v_x)))
    print('{}:{}'.format('theta', bottomup.d(p).d(v_t)))
    print('{}:{}'.format('vega', bottomup.d(p).d(v_s)))

    return 

def test_topdown(test_data) :
    mgr = topdown.create_variable_manager()
    calc_calloption = create_call_option_calculator(
        topdown.exp,
        topdown.log,
        topdown.sqrt,
        topdown.erf)
    v_x = mgr.to_variable(test_data["x"])
    v_t = mgr.to_variable(test_data["t"])
    v_k = mgr.to_variable(test_data["k"])
    v_s = mgr.to_variable(test_data["s"])
    v_r = mgr.to_variable(test_data["r"])

    p = calc_calloption(x = v_x, t = v_t, k = v_k, s = v_s, r = v_r)
    grad = topdown.gradient(p)
    print('{}:{}'.format('price', topdown.to_double(p)))
    print('{}:{}'.format('delta', grad.in_direction_of(v_x)))
    print('{}:{}'.format('theta', grad.in_direction_of(v_t)))
    print('{}:{}'.format('vega', grad.in_direction_of(v_s)))

    return 

def main() :
    print('start analytic calculation')
    analytic(sample_data())
    print('end analytic calculation')
    
    print()    

    print('start bottom up test')
    test_bottomup(sample_data())
    print('end bottom up test')
    
    print()    

    print('start top down test')
    test_topdown(sample_data())
    print('end top down test')
    return

if __name__ == '__main__' :
    main()