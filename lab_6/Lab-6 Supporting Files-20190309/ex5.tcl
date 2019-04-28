set ns [new Simulator]
set f0 [open out0.tr w]
set f1 [open out1.tr w]
set f2 [open out2.tr w]
set f3 [open out3.tr w]
set nf [open out.nam w]
$ns namtrace-all $nf
set tf [open outall.tr w]
$ns trace-all $tf



proc finish {} {
	
	global ns nf tf f0 f1 f2 f3 tp
	$ns flush-trace

	close $nf
	close $tf
	close $f0
	close $f1
	close $f2
	close $f3

	puts "[expr $tp*16/10000000.0]"
	#exec nam out.nam &
	exit 0

}

set tp 0

proc record {} {
        global sink0 sink1 f0 f1 f2 tcp0 f3 tp
	#Get an instance of the simulator
	set ns [Simulator instance]
	#Set the time after which the procedure should be called again
        set time .1
	#How many bytes have been received by the traffic sinks?
        set bw0 [$sink0 set bytes_]
        set bw1 [$sink1 set bytes_]
		set cwnd [$tcp0 set cwnd_]
	#Get the current time
        set now [$ns now]
		#puts "[expr $tp + $bw1/$time*8/10000000]"
		set tp [expr $tp+$bw0]
	#Calculate the bandwidth (in MBit/s) and write it to the files
        puts $f0 "$now [expr ($bw0/$time)*(8/1000000.0)]"
        puts $f1 "$now [expr $bw1/$time*8/1000000]"
		puts $f2 "$now $cwnd"
		puts $f3 "$now [expr 20-$bw0/500.0]"
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

#establishing links
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 2Mb 10ms DropTail




# creating a UDP agent
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0

#creating cbr traffic source
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $udp0



# creating a TCP agent
set tcp0 [new Agent/TCP]
$ns attach-agent $n1 $tcp0

# creating a FTP traffic source
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
$tcp0 set packet_size_ 512




#Create three traffic sinks and attach them to the node n3
set sink0 [new Agent/LossMonitor]
set sink1 [new Agent/TCPSink]
$ns attach-agent $n3 $sink0
$ns attach-agent $n3 $sink1

$ns connect $udp0 $sink0
$ns connect $tcp0 $sink1




$ns at 0.0 "record"
$ns at 0.0 "$ftp0 start"
$ns at 5.0 "$cbr0 start"

$ns at 10.0 "$ftp0 stop"
$ns at 10.0 "$cbr0 stop"

$ns at 11.0 "finish"

$ns run


