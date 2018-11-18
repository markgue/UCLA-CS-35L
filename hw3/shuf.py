#!/usr/bin/python

# Mark Guevara
# 704962920
# CS35L
# TA: Zhaowei Tan

import random, sys, argparse, string
from optparse import OptionParser

class shuf:
  
  def __init__(self):
    self.lines = []
  
  def getInputFromstdin(self):
    self.lines = sys.stdin.readlines()
  
  # Checks for '-' and otherwise reads from the file
  def getInputFromFile(self, filename):
    if filename is '-':
      self.lines = sys.stdin.readlines()
    else:
      f = open(filename, 'r')
      self.lines = f.readlines()
      f.close()
    
  # Fills lines with the data from the args of the parser
  def getInputFromParser(self, parserInput):
    for i in range(len(parserInput)):
      self.lines.append(parserInput[i] + '\n')
  
  # Outputs different shuffled lines based on whether -n was used
  def shuffleLines(self, useCount, count):
    random.shuffle(self.lines)
    if useCount:
      for i in range(min(count, len(self.lines))):
        sys.stdout.write(self.lines[i])
    else:
      for i in self.lines:
        sys.stdout.write(i)
    
  # For when -r is used. Outputs random lines based on whether -n was used 
  def repeatLines(self, useCount, count):
    if useCount:
      for i in range(count):
        sys.stdout.write(random.choice(self.lines))
    else:
      while True:
        sys.stdout.write(random.choice(self.lines))

def main():
  usage_msg = """Usage: %prog [OPTION]... [FILE]
   or: %prog -e [OPTION]... [ARG]..."""
  
  # OptionParser information
  parser = OptionParser(usage=usage_msg)
  parser.add_option("-e", "--echo",
                    action="store_true", dest="doEcho", default = False,
                    help="treat each ARG as an input line") 
  parser.add_option("-n", "--head-count",
                    action="append", dest="numlines", default = [],
                    help="output at most NUMLINES lines")     
  parser.add_option("-r", "--repeat",
                    action="store_true", dest="doRepeat", default = False,
                    help="output lines can be repeated")                      
                    
  options, args = parser.parse_args(sys.argv[1:])
  
  
  # Check that numlines input is correct
  if options.numlines: # Check that numlines list has something in it
    try:
      numlines = int(options.numlines[0])
    except:
      parser.error("invalid line count: {0}".format(options.numlines[0]))
    if numlines < 0:
      parser.error("invalid line count: {0}".format(numlines))
  else:
    numlines = 0
  
  generator = shuf()
  
  # No -e, so take input from file
  if options.doEcho is False:
    # Check that there is only one file
    if len(args) is 0:
      generator.getInputFromstdin();
    elif len(args) != 1:
      parser.error("extra operand {0}".format(args[1]))
    else:
      input_file = args[0]  
      # Check that the file exists
      try:
        generator.getInputFromFile(input_file)
      except:
        parser.error("{0}: No such file or directory".format(input_file))
    
    # Differentiate between repeats and shuffles, and use the size of the numlines list to
    # check if the funciton needs to use the given numlines or not
    if options.doRepeat:
      generator.repeatLines(options.numlines, numlines)
    else: 
      generator.shuffleLines(options.numlines, numlines)

  # there is a -e, so take input from args
  else: 
    generator = shuf()
    generator.getInputFromParser(args)
    
    if options.doRepeat:
      generator.repeatLines(options.numlines, numlines)
    else: 
      generator.shuffleLines(options.numlines, numlines)


if __name__ == "__main__":
  main()