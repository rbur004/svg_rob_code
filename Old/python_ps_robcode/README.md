# Python RoBCode Generator

The RoBCodes are computer generated art. There is no known decoder. There is no key to orient the image for decoding, though it may be possible to deduce the bytes from the parity bits (even parity). 

## History
Uses TripCode postscript library files (target_library.ps and cardTemplate7.ps by Diego Lopez de Ipina and Jeremy Henty) to draw circles and arcs. This generates a postscript file, with an anti-clockwise RoBCode, with 0 point on the right, at 90 degrees. 

The Python code, was a reimplementation (Summer 2013/14), from memory, of the C version, ca 1985, which leveraged even earlier code for drawing bytes (8 bit, plus 1 parity bit) on concentric circles, to help students visualise how data is stored on disks (B&W Quickdraw on Lisa and 128M Macs). Many of the disk visualisations where visually attractive, reminiscent of scifi art, and so was born the first RoBCode generator. 

# Calculating where the Bytes go
Bytes are drawn in concentric rings,  either increasing each ring linearly, or exponentially. 

### Linear Case
```
  For a step_size of 2, then there are 2 bytes in ring 1, 4 bytes in ring 2, 6 bytes in ring 3, 8 bytes in ring 4, ...
  The number of bytes in any ring r, is r * step_size
  The number of bytes that can be presented in r rings is step_size/2 * (r*r + r)
  To find the ring, from a byte's index i: Math.ceil((-step_size/2 + Math.sqrt(step_size/2*step_size/2 - 4*step_size/2*(-i)))/step_size);
```  
### Exponential case
```
  For a base of 2, then there are 2 bytes in ring 1, 4 bytes in ring 2, 8 bytes in ring 3, 16 in ring 4, ...
  The number of bytes in any ring r, is Math.pow(base, r)
  The number of bytes that can be presented in r rings is ((1-Math.pow(base,r))/(1-base))*base
  To find the ring r, from a byte's index i: Math.floor( Math.log( (1 - (i-1)/base*(1-base) )) / Math.log(base) ) + 1
```
## Reducing White Space
XOR and other encodings have been used to improved to visual appearance, by reducing large areas of white space, especially when there are partially filled rings. This further complicates decoding, unless the XOR byte(s), or other encoding is known. 

## Centre
A central circle is added to enhance the visual appearance. The original also added an encoding marker, to mimic the hole in floppy disk to find the first sector. 

## Frame
Adding a thin ring, just beyond the last byte ring, helps frame the RoBCode.
