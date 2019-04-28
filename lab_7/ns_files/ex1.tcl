set ns [new Simulator]

set nf [open out.nam w]
$ns namtrace-all $nf
set cw [open cwnd0.tr w]
set f0 [open bw0.tr w]
set tf [open outall.tr w]
$ns trace-all $tf

proc finish {} {
	global f0 cw nf tf
	#Close the trace and output files
    close $nf
	close $tf
	close $f0
	close $cw
	#Call xgraph to display the results
	# exec xgraph out0.tr out1.tr out2.tr -geometry 800x400 &
	#Execute nam on the trace file
       # exec nam out.nam &
        exit 0
}

proc record {} {
        global sink0 f0 tcp0 cw
	#Get an instance of the simulator
	set ns [Simulator instance]
	#Set the time after which the procedure should be called again
        set time 1
	#How many bytes have been received by the traffic sinks?
        set bw0 [$sink0 set bytes_]
        set cwnd [$tcp0 set cwnd_]
	#Get the current time
        set now [$ns now]
	#Calculate the bandwidth (in MBit/s) and write it to the files
        puts $f0 "$now [expr ($bw0/$time)*(8/1000000.0)]"
        puts $cw "$now $cwnd"

	#Reset the bytes_ values on the traffic sinks
        $sink0 set bytes_ 0
	#Re-schedule the procedure
        $ns at [expr $now+$time] "record"
}

#creating nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 2Mb 100ms DropTail
$ns duplex-link $n1 $n2 2Mb 100ms DropTail
$ns queue-limit $n1 $n2 2

# creating a TCP agent
set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0

# creating a FTP traffic source
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
$tcp0 set packet_size_ 512

set sink0 [new Agent/TCPSink]
$ns attach-agent $n2 $sink0

$ns connect $tcp0 $sink0

$ns at 0.0 "record"
$ns at 1.0 "$ftp0 start"

$ns at 31.0 "$ftp0 stop"

$ns at 35.0 "finish"

$ns run






