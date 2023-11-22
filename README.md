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

Buffer with an insertion point.

To keep the mantissa and fraction separate but still part of the whole number,
I need a buffer that has an insertion point. This point - d - marks the
position in the buffer where digits for the mantisa are inserted to the "left"
of d, and fraction digits are inserted "right" of d.

123456654321 ==> 123456.654321
     d
    
insert 7:                insert .7
1234567654321            12345677654321
      d (unch)                 d (+1)




