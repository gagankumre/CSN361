#-------Event scheduler object creation--------#

set ns [new Simulator]

#----------creating nam objects----------------#

set nf [open prog1.nam w]
$ns namtrace-all $nf

#----------creating trace objects----------------#

set nd [open prog1.tr w]
$ns trace-all $nd

#---------finish procedure--------#
proc finish { } {
global ns nf nd
$ns flush-trace
close $nf
close $nd
exec nam prog1.nam &
exec awk -f 1.awk prog1.tr > output.tr &
exit 0
}

#----------Setting color ID----------------#

$ns color 1 darkmagenta
$ns color 2 yellow
$ns color 3 blue
$ns color 4 green
$ns color 5 black

#---------- Creating Network----------------#

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

set server 0
set router 1
set client1 2




#---------- Creating Duplex Link----------------#

$ns duplex-link $n0 $n1 2.2Mb 10ms DropTail
$ns duplex-link $n1 $n2 1.1Mb 10ms DropTail

$ns queue-limit $n0 $n1 8
$ns queue-limit $n1 $n2 10

$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right-down

#------------Labelling----------------#

$ns at 0.0 "$n0 label Node0"
$ns at 0.0 "$n1 label Node1"
$ns at 0.0 "$n2 label Node2"



$n0 shape hexagon
$n1 shape hexagon
$n2 shape hexagon

#------------Data Transfer between Nodes----------------#

set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0


set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $udp0


set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1


set cbr1 [new Application/Traffic/CBR]
$cbr1 set packetSize_ 500
$cbr1 set interval_ 0.005
$cbr1 attach-agent $udp1


set sink [new Agent/Null]
$ns attach-agent $n2 $sink
$ns connect $udp0 $sink
$ns connect $udp1 $sink


# Setting flow color

$udp0 set fid_ 4
$udp1 set fid_ 4

# data packet generation starting time
$ns at 0.2 "$cbr0 start"
$ns at 0.5 "$cbr1 start"

# data packet generation ending time
$ns at 4.5 "$cbr0 stop"
$ns at 4.7 "$cbr1 stop"

#Calling finish procedure
$ns at 5.0 "finish"
$ns run

#Command to know the number of packets dropped : gawk -f 1.awk prog1.tr 
