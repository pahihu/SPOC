--  This program implements a processor farm solution to calculate integrals
--  using the trapezoidal method. The system consists of seven process types

--  1. Controller Process
--     Handles user input, and then squirts out all the work packets to the
--     valve process, at the same time collecting results returned from the
--     farm, summing the results to calculate the final integral, which is
--     output to the console together with statistics about which workers
--     performed the most work.
--     Prompts for Y/N if repeat execution required.

--  2. Valve Process (Subprocess from Control Process)
--     Accepts work packets from the control process and outputs them to the
--     farm, acting as a valve to ensure that the farm only contains enough
--     packets for the number of processors.

--  3. Worker Process
--     The worker process, which consists of three subprocess invocations, is
--     the process which actually performs the calculation and communication
--     of results. Consists of two buffer processes running at high priority
--     performing communication and routing, and one calculation process.
--     The buffer processes execute in parallel with the calculation process
--     and act to isolate the calculation from the communication.

--  4. Forward Buffer Process
--     This buffer process passes work packets either to the calculation
--     process of this worker, or if the worker is busy, passes the work
--     packet forwards to the successive worker. Attempts to keep one work
--     packet in storage, ready for the calculation process to ensure the
--     calculation process is never idle.

--  5. Return Buffer Process
--     This process multiplexes results from the calculation process onto the
--     result stream from the  following process, passing the combined stream
--     back to the preceding worker, the final destination being the control
--     process, which sums the individual results

--  6. Calculator Process
--     Accepts work and initialisation packets from the forward buffer,
--     calculates the integral and returns results by passing them to
--     the Return Buffer Process

--  7. Loopback Process
--     The loopback process sits at the 'end' of the process line, and
--     performs two functions, it acts to swallow up the initialisation
--     packet and also loops the 'STOP' packet from the last worker process
--     back onto the return path. This allows the 'STOP' packet
--     to terminate all active processes.

--  Process Model
--  *********     *******     ********     ********   //   ********   ******
--  *       *-out>*Valve*-out>*Buffer*-out>*Buffer*---\\-->*Buffer*-->*    *
--  *       *     *******     ********     ********   //   ********   *    *
--  *       *                  ||  /\       ||  /\    \\    ||  /\    *    *
--  *       *                  \/  ||       \/  ||    //    \/  ||    *    *
--  *       *                 ********     ********   \\   ********   *    *
--  *Control*                 *Calc 1*     *Calc 2*   //   *Calc N*   *Loop*
--  *       *                 ********     ********   \\   ********   *    *
--  *       *                    ||           ||      //      ||      *    *
--  *       *                    \/           \/      \\      \/      *    *
--  *       *                 ********     ********   //   ********   *    *
--  *       *<in--------------*Buffer*<in--*Buffer*<--\\---*Buffer*<--*    *
--  *********                 ********     ********   //   ********   ******


--  The integral is calculated using the fact that an integral
--  I = Integral of F(a,b)
--  is equal to the sum of I1 + I2 where
--  I1 = Integral of F(a,a+(b-a)/2)
--  I2 = Integral of F(a+(b-a)/2,b)
--  i.e. you can subdivide the integral range and sum the resulting
--  partial integrals to give the final answer

--  The integral range (Lower,Upper) is subdivided on two levels

--  NumberOfPackets - How many work packets to be sent
--     gives each work packet a range
--             PacketSize  =  (Upper-Lower)/NumberOfPackets
--     where each packet signifies a range from a start value 'StartX'
--             range is     (StartX)  to  (StartX + PacketSize)
--     and returns a result equal to the integral over that range

--  NumberOfSamples  - How many 'Samples' to calculate per work packet
--     Each packet integral is calculated with this number of Samples
--     and the value of 'x' can be accumulated by the addition of a value
--     'DeltaX' at each sample

--  The packets are passed using a variant protocol with five types
--  1. initial.packet
--    Contains initialisation data for each worker, such as the equation to
--    be calculated, the Power of 'x' in the eqn, the deltax to be added for
--    each sample, and the number of Samples to be calculated per work packet

--  2. work.packet
--    Contains one value, the StartX for a particular integral packet

--  3. result.packet
--    Contains one value, the integral result from a work packet

--  4. stop.packet
--    Causes all processes to stop, after passing the message on

--  5. more.packet
--    Sent from the calculator to the buffer process, to request more work
--    and also to the valve process, to allow further output

--  The STOP mechanism ensures that ALL processes terminate correctly
--  The controlling process sends a STOP packet to the farm.
--  Each worker input buffer process forwards it to both the calculator and
--  the next worker, then terminates. Each calculator process passes it to
--  the return buffer process and terminates. Each result process waits for
--  a stop packet to be received from BOTH the calculator and the following
--  worker process, passes it to the preceding worker and terminates.
--  This ensures that all nodes to the right have finished calculating,
--  returned results and terminated. The loopback process acts to receive
--  the STOP packet from the last worker, and return it back to the same
--  worker result process, after which it terminates.
--  Thus all worker processes terminate, returning this stop packet to the
--  control process which can then terminate correctly.

--  May be expanded for any number of processors by altering the variable
--  'MaxWorkers'. The actual number of working processes is entered by the
--  user and ranges from 1 to MaxWorkers. Each Worker is assigned a number
--  at the time of passing the initialisation packet, incrementing the Node
--  Number before passing it to the next node. The results passed back contain
--  this Node number so that statistics can be gathered about which nodes do
--  the most work.

--  As far as mapping on the network is concerned, the control and valve
--  processes are mapped onto one processer, the other processors in the
--  chain each execute a 'worker' process, except for the last process which
--  also executes to loopback process.


VAL MaxWorkers IS 16 :

PROTOCOL farming
  CASE
    initial.packet; INT; INT; REAL64; REAL64; INT
    -- Initialisation packet.
    -- INT EqnType     0 = x^Power,  1 = exp(-x^Power)
    -- INT Samples     Number of 'Samples' to calculate per work packet
    -- REAL64 deltax   Offset added to x, at each sample
    -- REAL64 Power    Power of x in equation
    -- INT Node        Node Number
    work.packet; REAL64    -- initx    Starting value of x for work packet
    result.packet; REAL64; INT  -- result,Node   Result from integration
    more.packet
    stop.packet
    debug.packet; INT
:
