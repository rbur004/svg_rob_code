#!/usr/bin/env python
import sys, os, types
import argparse

#Turn char into bits, represented by chars of 1's and 0's in a string.
#xor helps remove large runs of white space (0's) in the diagram.
def encodeChar(letter, xor, msb):
  byte_code = ord(letter)
  byte_code ^= xor
  parity = 0
  ringCode = ""
  if msb:
      for i in range(8):
          if (byte_code & 1) == 1:
              ringCode = "1 " + ringCode 
              parity ^= 1
          else:
              ringCode =  "0 " + ringCode 
          byte_code >>= 1
      
      if (parity & 1) == 1:
          ringCode = "1 " + ringCode 
      else:
          ringCode =  "0 " + ringCode 
      return (ringCode)
  else: #lsb
      for i in range(8):
          if (byte_code & 1) == 1:
              ringCode = ringCode + " 1" 
              parity ^= 1
          else:
              ringCode =  ringCode + " 0" 
          byte_code >>= 1
      
      if (parity & 1) == 1:
          ringCode = ringCode + " 1" 
      else:
          ringCode =  ringCode + " 0" 
      return (ringCode)

#Determine how many bytes this ring has.
#order is either the linear step size between rings
#      or the base, when the difference between rings is exponential
#exponential is a boolean, to indicate an exponential increase in the bytes between each ring
def bytes_in_ring(ring, order, exponential):
    if exponential:
        return order ** ring
    else:
        return order * ring

#Takes the string RoBCode, 
#the RoBCode order (step size or base) 
#exponential is a boolean, to indicate an exponential increase in the bytes between each ring
#xor byte, to help reduce large areas of white space
#Produces an array, with a string of 1's and 0's for each rings bytes
def encodeRoBCode(RoBCode, order, exponential, xor):
    # transform number into ternary taking into consideration that the first 3 bits are used for 
    # parity and the first is 1.
    ring = 0
    byteIndex = 0
    ring_capacity = bytes_in_ring(ring + 1, order, exponential)
    ringCode = []
    
    for letter in RoBCode :
        if len(ringCode) <= ring:
            ringCode.append( encodeChar(letter, xor, False) )
        else:
            ringCode[ring] += encodeChar(letter, xor, False)
        byteIndex += 1
        if byteIndex == ring_capacity:
            ring += 1;
            byteIndex = 0;
            ring_capacity = bytes_in_ring(ring + 1, order, exponential)
    
    return ringCode

#Output the postscript for printing a label under the RoBCode
def emitLabel(fd, fontSize, label):   
    if not label:
        line = "/labelCard () def\n"
    else: ### Label & ID
        line = "/labelCard (" + label + ") def\n"                    
    fd.write(line)

    line = "/fontSize " + `fontSize` + " def\n"
    fd.write(line)
    
#Output the RoBCode as postscript.
#ring_width is the width of each ring 
#the RoBCode order (step size or base) 
#label is an optional string that gets printed under the RoBCode
#exponential is a boolean, to indicate an exponential increase in the bytes between each ring
#xor byte, to help reduce large areas of white space
def generatePostScript(RoBCode, ring_width, label, order, exponential, xor):  
    ringCode = encodeRoBCode(RoBCode, order, exponential, xor)
    psLibraryFile = open('target_library.ps') #Emit postscipt routines to draw circles and arcs
    psLibraryCode = psLibraryFile.read()
    psLibraryFile.close()
    
    fileName = 'RoBCode_' + RoBCode + '.ps'
    fd = open(fileName, 'w')
    fd.write(psLibraryCode)
    
    emitLabel(fd, ring_width, label)
        
    line = "/radius " + `ring_width` + " mm def\n"
    fd.write(line)

    #Emit array to drow the bounding circle.
    line = "/keyring0 [ 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ] def\n" #Solid ring
    fd.write(line)

    #Emit postscript arrays for each of the data rings
    rings_used = len(ringCode)
    for i in range(rings_used):
        line = "/code" + str(i) + " [ " + ringCode[i] + "] def\n"
        fd.write(line)

    #Emit postscript to draw the central circle
    circle_unit = int(((ring_width/(rings_used + 3)) / 25.4) * 72 )
    spacer = circle_unit * 0.1
    fd.write('a4centre\n')
    fd.write(`circle_unit` + " % x y r\n") #centre circle
    fd.write("2 index 2 index circle % x y r x y -> x y\n") 
    
    #Emit postscript to draw each ring, based on the arrays emitted earlier
    for i in range(rings_used):
        fd.write( `circle_unit * (i+1)` + " " + `circle_unit * (i+2)` + " code" + `i` + " draw_code\n")
    
    #Emit postscript to draw the bounding circle
    fd.write(`circle_unit * (rings_used + 1.1)` + " " + `circle_unit * (rings_used + 1.3)` + " keyring0 draw_code\n") 
    
    cardCodeFile = open('cardTemplate7.ps') #Emit postscript trailer.
    fd.write(cardCodeFile.read())

    cardCodeFile.close()
    fd.close()

    print str('PostScript code for tag with ID: '+`RoBCode`+' has been generated in: '+fileName+"!")
    
   
######################################################################
if __name__ == '__main__':
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", type=int, help="The ring width", default=20, dest="ring_width")
    parser.add_argument("-l", type=str, help="Print with label", default=None, dest="label")
    parser.add_argument("-o", type=int, help="The RoBCode Order", default=2, dest="order" )
    parser.add_argument("-x", type=int, help="Xor Byte", default=0, dest="xor" )
    parser.add_argument("-e", action="store_true", help="Exponential RoBCode", default=False, dest="exponential" )
    parser.add_argument("encode_me", help="String to encode")
    args = parser.parse_args()
    
    generatePostScript(args.encode_me, args.ring_width, args.label, args.order, args.exponential, args.xor)
    












