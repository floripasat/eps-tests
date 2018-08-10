#include <msp430.h>
#include "scheduling.h"

/*****************************************************************************/
/* - name: task_scheduler
 * - description: this function is used as a simple task scheduler. It controls the battery voltage by changing the current drained by the load.
 *  The action on the load current is determined by a comparison between the current measured power and a previous measured power. If the previous power is higher, than the load current is decreased.
 *  Else, the load current is increased.
 */

void task_scheduler(void){



    if(curr>0.02){

        if(previous_power<mean_power)           // determine if previous load current action increased or decreased the input power
            LCA = 0;        // set action to decrease load current (increase battery voltage)
        else{
                if(previous_power>mean_power)
                    LCA = 1;        // set action to increase load current (decrease battery voltage)
                else
                    LCA = 4;
        }
    }else{
        if(accumulated_current>=0.55 && LCA == 2)   //determine if battery voltage is less than 2,6 to turn off the load
            LCA = 2;
        if(accumulated_current<=0.56 && LCA == 3)
            LCA = 3;
        if(accumulated_current<0.55)
            LCA = 3;
        if(accumulated_current>0.56)
            LCA = 2;
    }

    previous_power = mean_power;

}

