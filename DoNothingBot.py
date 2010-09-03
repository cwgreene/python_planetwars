import sys

def main():
  logger = open("logger.txt","w")
  data = ""
  while True: #Calling process will terminate us
    line = raw_input()+"\n"
    data += line
    logger.write(line)
    logger.flush()
    if len(data)>3 and data[-3:] == "go\n":
      print "go"
      sys.stdout.flush() #critical
      logger.flush()

main()
