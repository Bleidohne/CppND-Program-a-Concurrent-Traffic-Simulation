#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"
#include <chrono>                               // FP.2x: Needed.
#include <random>                               // FP.2x: Needed.    

// forward declarations to avoid include cycle
class Vehicle;

enum class TrafficLightPhase                    // FP.1 Define the enumeration class TrafficLightPhase
{
    red, green                                  // FP.1 Options to be set for the type TrafficLightPhase
};

// DONE
// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

template <class T>
class MessageQueue
{
public:

    void send(T &&msg);             // FP.3: Send function taking a rvalue reference
    T receive();                    // FP.3: Receive function 

private:
    std::deque<T> _queue;                // FP.3: Queue definition
    std::condition_variable _condition;  // FP.3: Condition definition
    std::mutex _mutex;                   // FP.3: Mutex tefinition.
    
};

// DONE
// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

class TrafficLight : public TrafficObject               // FP.1 Class becomes child of TrafficObject.
{
public:
    // constructor / desctructor
    TrafficLight();                                     // FP.1 Constructor for the classe
    

    // getters / setters
    TrafficLightPhase getCurrentPhase();                // FP.1 Public function which returns the current Traffic Light Phase (getter-method)

    // typical behaviour methods
    void waitForGreen();                                // FP.1 Public function that waits for green
    void simulate();                                    // FP.1 Public function to do simulations
    

private:
    // typical behaviour methods
    void cycleThroughPhases();                          // FP.1 Method to cycle throuth the phases

    // FP.x: Private Members
    TrafficLightPhase _currentPhase;                    // FP.1 Private variable for the current Traffic Light Phase (value acc. TrafficLightPhase class)
    
    long _milliseconds_to_wait;                         // FP.2a: Milliseconds to wait.
    static constexpr long minLightms = 4000;            // FP.2a: Minimum Traffic Light time 4sec. 
    static constexpr long maxLightms = 6000;            // FP.2a: Maximum Traffic Lgiht time 5sec. 
    std::mt19937 _generated;                            // FP.2a: Random Number Generation
    std::uniform_int_distribution<long> _distribution;  // FP.2a: Random Number Generation
    std::chrono::time_point<std::chrono::system_clock> _lastUpdate;  // FP.2a: Time of Last update of the traffic light. 

    void randMiliSecToWait();                           // FP.2a: Randomize the Wait time for the Traffic Light Phases. 


    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.
    MessageQueue<TrafficLightPhase> _messageQueue;      // FP.4b: Defintion of message queue. 

    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif