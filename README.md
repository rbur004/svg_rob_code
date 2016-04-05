# svg_rob_code
Implementation of Rings of Bytes Code generator in SVG, embedded in an HTML page, with an in page form to modify the RoBCode parameters. 

The RoBCodes are computer generated art. There is no known decoder. There is no key to orient the image for decoding, though it may be possible to deduce the bytes from the parity bits. Our original house design was a RoBCode, but for practical reasons, it got culled back to an octagon (not that an octagon is very practical :) (see: https://www.google.co.nz/maps/@-36.9906225,174.4870949,197m/data=!3m1!1e3). 

Want to play: http://www.burrowes.org/svg_arc.html

##History
Reimplements the earlier Python code, that produced a postscript file as output (ca the summer of 2013/14), and based on the target_library.ps and cardTemplate.ps from Diego Lopez de Iping TripCode generator (Diego's code was in turn, based on Jeremy Henty's code). 

The Python code, was in turn, a reimplementation of the C version, ca 1985, which leveraged even earlier code for drawing bytes (8 bit, plus 1 parity bit) on concentric circles, to help students visualise how data is stored on disks (B&W Quickdraw on Lisa and 128M Macs). Many of the disk visualisations where visually attractive, reminiscent of scifi art, and so was born the first RoBCode generator (Yes, a pun. We had lots of Robs in Computer Science at that time). 

#Calculating where the Bytes go
Bytes are drawn in concentric rings,  either increasing each ring linearly, or exponentially. 

###Linear Case
```
  For a step_size of 2, then there are 2 bytes in ring 1, 4 bytes in ring 2, 6 bytes in ring 3, 8 bytes in ring 4, ...
  The number of bytes in any ring r, is r * step_size
  The number of bytes that can be presented in r rings is step_size/2 * (r*r + r)
  To find the ring, from a byte's index i: Math.ceil((-step_size/2 + Math.sqrt(step_size/2*step_size/2 - 4*step_size/2*(-i)))/step_size);
```  
###Exponential case
```
  For a base of 2, then there are 2 bytes in ring 1, 4 bytes in ring 2, 8 bytes in ring 3, 16 in ring 4, ...
  The number of bytes in any ring r, is Math.pow(base, r)
  The number of bytes that can be presented in r rings is ((1-Math.pow(base,r))/(1-base))*base
  To find the ring r, from a byte's index i: Math.floor( Math.log( (1 - (i-1)/base*(1-base) )) / Math.log(base) ) + 1
```
##Reducing White Space
XOR and other encodings have been used to improved to visual appearance, by reducing large areas of white space, especially when there are partially filled rings. This further complicates decoding, unless the XOR byte(s), or other encoding is known. 

##Centre
A central circle, a half circle and ring, and a one byte pattern have all been used to enhance the visual appearance. The original also added an encoding marker, to mimic the hole in floppy disk to find the first sector. 

##Frame
Adding a thin ring, just beyond the last byte ring, helps frame the RoBCode.

##Linear version
The Unroll option draws the tracks as stacked rectangles, the inner track an top.


