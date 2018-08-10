/*
 * scheduling.h
 *
 *  Created on: 09/08/2018
 *      Author: Sara Vega Martinez
 */

#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include <stdint.h>

struct task{
  //int id;
    char name;
    int priority;
  //int deadline;
    int cpuTime;
  //char state;
    int period;
    char function;
    char resource[3];
    int  AvgPower;
};

struct task_array{
    struct task;
    int    time;
    char event_type; // "T - terminar execucao da tarefa" ou "I - incluir na lista de prontas para serem executadas"
    struct task_array *next_task;
    };

#endif /* SCHEDULING_H_ */
