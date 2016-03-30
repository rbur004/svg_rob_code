# svg_rob_code
Implementation of Rings of Bytes Code generator in SVG, embedded in an HTML page, with an in page form to modify the RoBCode parameters. 

Reimplements the earlier Python code, that produced a postscript file as output (ca the summer of 2013/14), and based on the target_library.ps and cardTemplate.ps from Diego Lopez de Iping TripCode generator (Diego's code was in turn, based on Jeremy Henty's code). 

The Python code, was in turn, a reimplementation of the C version, ca 1985, which leveraged even earlier code for drawing bytes (8 bit, plus 1 parity bit) on concentric circles, to help students visualise how data is stored on disks. Many of the disk visualisations where visually attractive, reminiscent of scifi art, and so was born the first RoB Code generator.

The RoB codes are computer generated art. There is no known decoder. There is no key to orient the image for decoding, though it may be possible to deduce the bytes from the parity bits (odd parity).

Bytes are drawn in concentric rings, increasing the number of bytes in a ring, either linearly (ring * n), or exponentially ( Math.pow(ring,n); ). 

XOR and other encodings have been used to improved to visual appearance, by reducing large areas of white space, especially when there are partially filled rings. This further complicates decoding, unless the XOR byte(s), or other encoding is known. 

A central circle, half circle and ring, and a one byte pattern have all been used to enhance the visual appearance. Adding a thin ring, just beyond the last byte ring, helps frame the RoB code.








