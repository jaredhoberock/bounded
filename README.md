# bounded

`bounded<bound>` is a type representing a dynamic value with a compile-time upper bound.

It is intended for use cases where it is important to know statically the largest value a variable can take.

For example, `bounded<16>` could bound a `for` loop:

    // the value of n may not be greater than 16
    bounded<16> n = 10;

    #pragma unroll
    for(int i = 0; i != n.bound; ++i)
    {
      if(i < n)
      {
        f(i);
      }
    }

One might write a loop this way to encourage unrolling and predication in an effort to minimize warp divergence in CUDA code.

Arithmetic between `bounded` and `constant` can track upper bounds in some cases:

    bounded<10> a = 7;
    bounded<50> b = 42;

    // the sum of a + b may be no larger than 60
    auto c = a + b; // the type of c is bounded<60>

    bounded<20> d = 13;
    auto e = 42_c;

    // the product of c * d may be no larger than 840
    auto f = d * e; // the type of f is bounded<840>

