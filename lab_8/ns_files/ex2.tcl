set ns [new Simulator]

set nf [open out.nam w]
$ns namtrace-all $nf

set tf [open outall.tr w]
$ns trace-all $tf

$ns rtproto DV 
#Agent/rtProto/DV set advertInterval 4

proc finish {} {
	global nf tf 
	#Close the trace and output files
    close $nf
	close $tf
	
	#Call xgraph to display the results
	# exec xgraph out0.tr out1.tr out2.tr -geometry 800x400 &
	#Execute nam on the trace file
       # exec nam out.nam &
        exit 0
}

proc rtdump {} {
	global ns

	set now [$ns now]
	puts "Routing table at time $now"
	#$ns dump-routelogic-nh
	$ns dump-routelogic-distance
}




# creating seven nodes
for {set i 0} {$i < 7} {incr i} {
	set n($i) [$ns node]
}

for {set i 0} {$i < 7} {incr i} {
	$ns duplex-link $n($i) $n([expr ($i+1)%7]) 1Mb 10ms DropTail
}


set udp0 [new Agent/UDP]
$ns attach-agent $n(0) $udp0

set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $udp0

set sink0 [new Agent/LossMonitor]
$ns attach-agent $n(3) $sink0

$ns connect $udp0 $sink0

#$ns rtmodel-at 1.0 down $n(1) $n(2)
#$ns rtmodel-at 2.0 up $n(1) $n(2)

$ns at 0.0 "$cbr0 start"
$ns at 5.0 "$cbr0 start"

$ns at 0.5 "rtdump"
$ns at 1.5 "rtdump"
$ns at 2.5 "rtdump"

$ns at 6.0 "finish"

$ns run


