set ns [new Simulator]

$ns color 1 Red
$ns color 2 Blue

set nf [open out.nam w]
$ns namtrace-all $nf

set cw0 [open cwnd0.tr w]
set b0 [open bw0.tr w]
set cw1 [open cwnd1.tr w]
set b1 [open bw1.tr w]

set tf [open outall.tr w]
$ns trace-all $tf



proc finish {} {
	global b0 cw0 nf tf b1 cw1
	#Close the trace and output files
    close $nf
	close $tf
	close $b0
	close $cw0
	close $b1
	close $cw1
	#Call xgraph to display the results
	# exec xgraph out0.tr out1.tr out2.tr -geometry 800x400 &
	#Execute nam on the trace file
       # exec nam out.nam &
        exit 0
}



proc record {} {
        global sink0 b0 tcp0 cw0 b1 cw1 sink1 tcp1
	#Get an instance of the simulator
	set ns [Simulator instance]
	#Set the time after which the procedure should be called again
        set time 1
	#How many bytes have been received by the traffic sinks?
        set bw0 [$sink0 set bytes_]
        set cwnd0 [$tcp0 set cwnd_]
		set bw1 [$sink1 set bytes_]
        set cwnd1 [$tcp1 set cwnd_]
	#Get the current time
        set now [$ns now]
	#Calculate the bandwidth (in MBit/s) and write it to the files
        puts $b0 "$now [expr ($bw0/$time)*(8/1000000.0)]"
        puts $cw0 "$now $cwnd0"
		puts $b1 "$now [expr ($bw1/$time)*(8/1000000.0)]"
        puts $cw1 "$now $cwnd1"

	#Reset the bytes_ values on the traffic sinks
        $sink0 set bytes_ 0
		$sink1 set bytes_ 0
	#Re-schedule the procedure
        $ns at [expr $now+$time] "record"
}



#creating nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]

$ns duplex-link $n4 $n1 2Mb 100ms DropTail
$ns duplex-link $n1 $n2 2Mb 100ms DropTail
$ns duplex-link $n1 $n3 2Mb 100ms DropTail
$ns duplex-link $n0 $n4 2Mb 100ms DropTail
$ns queue-limit $n1 $n2 2

# creating a TCP agent
set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0
$tcp0 set fid_ 1

# creating a FTP traffic source
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
$tcp0 set packet_size_ 512

# creating a TCP agent
set tcp1 [new Agent/TCP]
$ns attach-agent $n3 $tcp1
$tcp1 set fid_ 2

# creating a FTP traffic source
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1
$tcp1 set packet_size_ 512



set sink0 [new Agent/TCPSink]
$ns attach-agent $n2 $sink0
set sink1 [new Agent/TCPSink]
$ns attach-agent $n2 $sink1

$ns connect $tcp0 $sink0
$ns connect $tcp1 $sink1

$ns at 0.0 "record"
$ns at 1.0 "$ftp0 start"
$ns at 10.0 "$ftp1 start"

$ns at 31.0 "$ftp0 stop"
$ns at 31.0 "$ftp1 stop"

$ns at 35.0 "finish"

$ns run






