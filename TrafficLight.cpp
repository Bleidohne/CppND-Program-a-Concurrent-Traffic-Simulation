#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <thread>                   // FP.2: Needed fr seperate thread
#include <chrono>                   // FP.2: Needed for time options. 

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // DONE
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    std::unique_lock<std::mutex> ulock(_mutex);                 // FP.5a: Lock the queue
    _condition.wait(ulock, [this] { return !_queue.empty();});  // FP.5a: Wait condition to wait for the queue
    T message_queued = std::move(_queue.back());                // FP.5a: Get the last message in the queue with move Semantics
    _queue.pop_back();                                          // FP.5a: Remove the last element in the queue. 

    return message_queued;                                      // FP.5a: Return the message.                                 

}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // DONE
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> ulock(_mutex);      // FP.4a: Lock the _queue
    _queue.push_back(std::move(msg));               // FP.4a: Add message to the queue
    _condition.notify_one();                        // FP.4a: Send the notificaiton

}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;                 // FP.1: Initializes every Traffic Light with RED. 

    // FP.1: Randomize the Traffic Light time between 4 and 6sec. 
    std::random_device rd;                                  // FP.2a: Define a random number (as learned in udacity examples)
    _generated = std::mt19937(rd());                        // FP.2a: Define a randum number
    _distribution = std::uniform_int_distribution<long>(minLightms,maxLightms); // FP.2a: Define the standard distribution
    _lastUpdate = std::chrono::system_clock::now();         // FP.2a: Log the current system time, to start the Light phases.
    randMiliSecToWait();                                    // FP.2a: Create the random wait time. 


}

void TrafficLight::waitForGreen()
{
    // DONE
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    while(1)                            // FP.5b: Infinite Loop
    {
        if(_messageQueue.receive() == TrafficLightPhase::green)         // FP.5b: Check the Traffic Light for green. 
            return;

    }


}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));  // FP.2a: Use Thread queue in the base class. 

}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // DONE
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    long time_diff;                                             // FP.2a: Variable for difference between two cycles
       

    while(1)            // FP.2a: Infinite Loop
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));      // FP.2a: sleep at every iteration to safe CPU load 

        time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-_lastUpdate).count();       // FP.2a: Calculate time difference

        if(time_diff < _milliseconds_to_wait)
        {
             continue;                   // FP.2a: Steop over the loop if CycleTime is not exceeded.
        }else                            // FP.2a: Now it is time to switch the light signal:   
        {
             
             if(_currentPhase == TrafficLightPhase::red)        // FP.2a: Toggle the traffic light. 
             {
                _currentPhase = TrafficLightPhase::green;       // FP.2a: Switch from red to green.
             }                
             else
             {
                _currentPhase = TrafficLightPhase::red;         // FP.2a: Switch from green to red.      
             }
   
            // std::cout << _milliseconds_to_wait << " msec "<<std::endl;
   
             _lastUpdate = std::chrono::system_clock::now();    // FP.2a: Reset the watch.        
             
             _messageQueue.send(std::move(_currentPhase));      // FP.4b: Sending the update by move sematics.       

            randMiliSecToWait();                                // FP.2a: Define the next random time to wait. 

        }

    }

}

void TrafficLight::randMiliSecToWait()                          // FP.2a: Random time to wait for the raffic Light Phases. 
{
    long s = _distribution(_generated);                         // FP.2a: Generate the random number
    _milliseconds_to_wait = s;                                  // FP.2a: Write the random number to the private member.
}