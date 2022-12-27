#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:

  int prevuser = 0;
  int prevnice = 0;
  int prevsystem = 0;
  int previdle = 0;
  int previowait = 0;
  int previrq = 0;
  int prevsoftirq = 0;
  int prevsteal = 0;
  int prevguest = 0;
  int prevguest_nice = 0;
};

#endif