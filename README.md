bigly

Library for arithmetic using arbitrarily large numbers.

The numbers are stored as a series of binary-coded digits, and processed using
"long" arithmetic - with simulated signs (and hopefully soon) decimal points.

  mantissa fraction
+876543210 12345678-
 102345670.98238742

Decimal point is assumed before the 0th digit (i.e., base^0). The decimal is a
count of the number of fractional digits, hence in the example above d is 8.

So, if d = m then there is no mantissa(!) - except for the assumed leading 0.

Zero is a very interesting number to represent, it turns out. If I have a
maginitude as zero, and decimal point at zero, is this represented as
integer zero (0) or decimal zero (0.0). Perhaps a presentation issue?

