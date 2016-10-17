# Test Parameters
Several parameters are available; you can enter them on the command line.
All parameters are optional and case-insensitive; partial matches are allowed (such as `-h` instead of `-help`).

Some parameters only make sense in the publishing or subscribing application. The parameters are presented in the following tables, based on whether they may be used in a publishing application, a subscribing application, or both:

- See [Test Parameters for Publishing and Subscribing Applications](#params-pub-sub)
- See [Test Parameters Only for Publishing Applications](#params-pub)
- See [Test Parameters Only for Subscribing Applications](#params-sub)
- See [Test Parameters to Control RTI Secure DDS Options](#params-pub-sub-secure)

As you will see in the tables, the `-pub` parameter specifies a publishing application and the `-sub` specifies a subscribing application. If you do not specify See `-pub` then `-sub` is assumed.

For additional information on setting the parameters, see sections:

- [Spinning vs. Sleeping](#sleepVsSpin)
- [Send-Queue Size and Queue-Full Behavior](#queueSize)
- [Number of Iterations vs. Latency Count](#iterationsVsLatencyCount)
- [Warming Up](#warmUp)
- [WaitSet Event Count and Delay](#WaitSetEventCount)
- [How to Measure Latency for a Given Throughput](#lat)
- [Auto Tuning and Turbo Mode](#AutoTuningTurboMode)

## Test Parameters for Publishing and Subscribing Applications <a id="params-pub-sub"></a>

-   `-bestEffort`

    Use best-effort communication.

    **Default:** `false` (use reliable communication).

    For an introduction to the RTI reliability model, see the Strict Reliability design pattern in the RTI Connext DDS Core Libraries Getting Started Guide. See also: Reliable Communications in the RTI Connext DDS Core Libraries User’s Manual.

-   `-dataLen <bytes>`

    Length of payload in bytes for each send.
    
    **Default:** `100 bytes.`<br>
    **Range:** `28 - 63000 bytes`

    The lower limit is the number of "overhead" bytes in the message (i.e., the timestamp, sequence number, and other meta-data used by the test); the upper limit ensures that, when the overhead of the wire protocol is added, it doesn't overflow the UDP maximum datagram size of 64KB.

    If See `-scan` is specified, this value is ignored.

-   `-debug`

    Run in debug mode (generates more verbose logging messages, which are useful to RTI support personnel).

    **Default:** false
    
-   `-dynamicData`

    Run using the Dynamic Data API functions instead of the *rtiddsgen* generated calls.

    **Default:** false

-   `-durability <0|1|2|3>`

    Sets the Durability kind:

    `0` - `VOLATILE` (default)  
    `1` - `TRANSIENT LOCAL`  
    `2` - `TRANSIENT`  
    `3` - `PERSISTENT`

    For an introduction to the RTI durability model, see the Historical Data design pattern in the RTI Connext DDS Core Libraries Getting Started Guide. See also: Mechanisms for Achieving Information Durability and Persistence, Chapter 12, in the RTI Connext DDS Core Libraries User’s Manual.

-   `-domain <ID>`

    Domain ID.

    The publisher and subscriber applications must use the same domain ID in order to communicate.

    **Default:** `1`  
    **Range:** `0 - 200`
    
    See Choosing a Domain ID and Creating Multiple Domains, Section 8.3.4, in the RTI Connext DDS Core Libraries User’s Manual.

-   `-enableSharedMemory`

    Enable the shared memory transport.

    **Default:** shared memory transport is disabled

-   `-enableTcpOnly`

    Disable all the other transports and use only TCP transport for communication.

    **Default:** TCP transport not enabled

-   `-help`
    
    Print an informative message with all the available command-line parameters and exit.
    
-   `-instanceHashBuckets <n>`

    Number of hash buckets for instances.

    **Default:** `-1` (means same as the number of instances)  
    **Range:** `> 0`

-   `-instances <int>`

    Set the number of instances to use in the test. The publishing and subscribing applications must specify the same number of instances.

    This option only makes sense when testing a keyed data type; to do so, use See `-keyed`.

    **Default:** `1`  
    **Range:** `> 0`

-   `-keepDurationUsec <usec>`

    Minimum duration that a sample is queued for ACK-disabled readers. Only used if See `-noPositiveAcks` is specified on the publisher side.

    See Disabling Positive Acknowledgements, Section 6.5.3.3 in the RTI Connext DDS Core Libraries User’s Manual.

    **Default:** `1000 µsec` (1 millisec).  
    **Range:** `>= 0`.

-   `-keyed`

    Specify the use of a keyed type.

    **Default:** `Unkeyed` type.

-   `-multicast`

    Use multicast to receive data.

    **Default:** do not use multicast.

-   `-multicastAddress <address>`

    Specify the multicast receive address for receiving user data.

    If unspecified, the following default values will be used according to the topic:

    **latency:** `239.255.1.2`  
    **throughput:** `239.255.1.1`  
    **announcement:** `239.255.1.100`

-   `-nic <ipaddr>`

    Restrict RTI Connext DDS to sending output through this interface. This can be the IP address of any available network interface on the machine.

    By default, RTI Connext DDS will attempt to contact all possible subscribing nodes on all available network interfaces. Even on a multi-NIC machine, the performance over one NIC vs. another may be different (e.g., Gbit vs. 100 Mbit), so choosing the correct NIC is critical for a proper test.
    
-   `-noDirectCommunication`

    Indicates if the subscribing application will receive samples from the publishing application when RTI Persistence Service is used.

    Only applies when `-durability <0|1|2|3>` is `TRANSIENT (2)` or `PERSISTENT (3)`.

    If set to `true` (the default), the subscribing application gets samples from the publishing application and *RTI Persistence Service*. This mode provides low latency between endpoints.

    If set to `false`, the subscribing application only gets samples from *RTI Persistence Service*. This brokered communication pattern provides a way to guarantee eventual consistency.
    
    **Default:** `true` (direct communication)

-   `-nomulticast`

    Do not use multicast.

    **Note:** Starting in 5.1.0, this option is no longer needed since multicast is disabled by default. It exists only to maintain backward compatibility.

    **Default:** Do not use multicast

-   `-noPositiveAcks`

    Disable use of positive ACKs in the reliable protocol.

    **Default:** `true` (use positive ACKs)

    See `-qosprofile <filename>` option for more information.

-   `-noPrintIntervals`

    Prevent printing of statistics at intervals during the test.

    By default, statistics are printed every second in the subscribing application, and after receiving every latency echo in the publishing application.
    
-   `-qosprofile <filename>`

    Path to the XML file containing DDS QoS profiles.

    **Default:** `perftest_qos_profiles.xml`
    
    The default file contains these QoS profiles:  
    The `ThroughputQos`, `LatencyQos`, and `AnnouncementQos` profiles are used by default.  
    The `NoAckThroughputQos` and `NoAckLatencyQos` profiles are used if you specify `-noPositiveAcks`.

    **Note:** some QoS values are ‘hard-coded’ in the application, therefore setting them in the XML file has no effect; see the See Note:.

    See comments in `perftest_qos_profiles.xml`, as well as **Configuring QoS with XML, Chapter 17** in the *RTI Connext DDS Core Libraries* User’s Manual.

-   `-useReadThread`

    Use a separate thread (instead of a callback) to read data.

    See See WaitSet Event Count and Delay
    
    **Default:** use callback for subscriber
-   `-waitsetDelayUsec <usec>`

    Process incoming data in groups, based on time, rather than individually.

    Only used if the See `-useReadThread` option is specified on the subscriber side.

    See WaitSet Event Count and Delay.

    **Default:** `100`  
    **Range:** `>= 0`

-   `-waitsetEventCount <count>`

    Process incoming data in groups, based on the number of samples, rather than individually.

    Only used if the See `-useReadThread` option is specified on the subscriber side.

    See See WaitSet Event Count and Delay.

    **Default:** `5`
    **Range:** `>= 1`

## Test Parameters Only for Publishing Applications <a id="params-pub"></a>

- `-batchSize <bytes>`

    Enable batching and set the maximum batched message size.

    **Default:** `0` (batching disabled)  
    **Range:** `1 to 63000`

    For more information on batching data for high throughput, see the **High Throughput design pattern** in the *RTI Connext DDS Core Libraries Getting Started Guide*. See also: **How to Measure Latency for a Given Throughput and the BATCH QosPolicy, Section 6.5.2** in the *RTI Connext DDS Core Libraries Getting User’s Manual*.

-   `-enableAutoThrottle`

    Enable the Auto Throttling feature. See Auto Tuning and Turbo Mode.

    **Default:** feature is disabled.

-   `-enableTurboMode`

    Enables the Turbo Mode feature. See See Auto Tuning and Turbo Mode. When turbo mode is enabled, See `-batchSize <bytes>` is ignored.

    **Default:** feature is disabled.

-   `-executionTime <sec>`

    Allows you to limit the test duration by specifying the number of seconds to run the test.
    
    **Default:** feature is not set.

-   `-heartbeatPeriod <sec>:<nanosec>`

    The period at which the publishing application will send heartbeats.

    See **Reliable Communications, Chapter 10**, in the *RTI Connext DDS Core Libraries Getting User’s Manual*.

    **Default:** `heartbeat period sec = 0`, `heartbeat period nanosec = 0` (meaning use the value as specified in the XML QoS Profile, which is set to (10 millisec = 10000000 nanosec)). 
    
    See `-qosprofile <filename>`.
    
    **Range:** 1 nanosec to 1 year (31,536,000 sec.)

-   `-fastHeartbeatPeriod <sec>:<nanosec>`

    An alternative heartbeat period used when the publishing application needs to flush unacknowledged samples more quickly.

    See **Reliable Communications, Chapter 10**, in the *RTI Connext DDS Core Libraries Getting User’s Manual*.

    **Default:** `heartbeat period sec = 0`, `heartbeat period nanosec = 0` (meaning use the value as specified in the XML QoS Profile, which is set to (1 millisec = 1000000 nanosec)). See 

    See `-qosprofile <filename>`.

    **Range:** (actual value) `1 nanosec` to `1 year (31,536,000 sec)`. Must not be slower than See `-heartbeatPeriod <sec>:<nanosec>`.

-   `-latencyCount <count>`

    Number samples to send before a latency ping packet is sent.

    See Number of Iterations vs. Latency Count.

    **Default:** `-1` (if `-latencyTest` is not specified, automatically adjust to 10000; if -latency Test is specified, automatically adjust to 1).

    **Range:** must be `<= -numIter`

-   `-latencyTest`

    Run a latency test consisting of a ping-pong.

    The publisher sends a ping, then blocks until it receives a pong from the subscriber.

    Can only be used on a publisher whose `pidMultiPubTest = 0` (see See `-pidMultiPubTest <id>`).

    **Default:** `false`
-   `-numIter <count>`

    Number of samples to send.
    
    See Number of Iterations vs. Latency Count and See Warming Up.

    If you set `scan` = `true`, you cannot set this option (See `-scan`).

    **Default:** `0` (infinite)  
    **Range:** `latencyCount` (adjusted value) or higher (see `-latencyCount <count>`).

-   `-numSubscribers <count>`

    Have the publishing application wait for this number of subscribing applications to start.

    **Default:** `1`

-   `-pidMultiPubTest <id>`

    Set the ID of the publisher in a multi-publisher test.

    Use a unique value for each publisher running on the same host that uses the same domain ID.

    **Default:** `0`  
    **Range:** `0 to n-1`, inclusive, where n is the number of publishers in a multi-publisher test.

-   `-pub`

    Set test to be a publisher.

    **Default:** `-sub`

-   `-pubRate`

    Limit the throughput to the specified number of samples per second.

    **Default:** `0` (no limit)  
    **Range:** `1 to 10000000`

-   `-scan`

    Run test in scan mode, traversing a range of sample data sizes from 32 to 63,000 bytes.

    If you set `scan = true`, you cannot set `-numIter <count>`.

    **Default:** `false` (no scan)

-   `-sendQueueSize <number>`

    Size of the send queue.

    When `-batchSize <bytes>` is used, the size is the number of batches.

    See Send-Queue Size and Queue-Full Behavior.

    **Default:** `50`  
    **Range:** `[1-100 million]` or `-1` (indicating an unlimited length).

-   `-sleep <millisec>`

    Time to sleep between each send.

    See Spinning vs. Sleeping.

    **Default:** `0`  
    **Range:** `0` or higher

-   `-spin <count>`

    Number of times to run in a spin loop between each send.

    See Spinning vs. Sleeping.

    **Default:** `0`  
    **Range:** `0` or higher

## Test Parameters Only for Subscribing Applications <a id="params-sub"></a>

-   `-numPublishers <count>`

    The subscribing application will wait for this number of publishing applications to start.

    **Default:** `1`

-   `-sidMultiSubTest <id>`

    ID of the subscriber in a multi-subscriber test.

    Use a unique value for each subscriber running on the same host that uses the same domain ID.

    **Default:** `0`  
    **Range:** `0 to n-1`, inclusive, where n is the number of subscribers in a multi-subscriber test.

-   `-sub`

    Set test to be a subscriber.

    **Default:** `-sub`

## Test Parameters to Control RTI Secure DDS Options (Publishing and Subscribing Applications) <a id="params-pub-sub-secure"></a>

-   `-secureEncryptDiscovery`

    Encrypt discovery traffic.

    **Default:** Not set.

-   `-secureSign`

    Sign discovery and user data packages.

    **Default:** Not set.

-   `-secureEncryptData`

    Encrypt at the user data level.

    **Default:** Not set.

-   `-secureEncryptSM`

    Encrypt at the RTPS sub-message level.

    **Default:** Not set.

-   `-secureGovernanceFile <file>`

    Governance file. If specified, the authentication, signing, and encryption arguments are ignored. The governance document configuration will be used instead.

    **Default:** Not set.

-   `-securePermissionsFile <file>`

    Permissions file to be used.

    **Default for Publisher:** `./resource/secure/signed_PerftestPermissionsPub.xml`  
    **Default for Subscriber:** `./resource/secure/signed_PerftestPermissionsSub.xml`

-   `-secureCertAuthority <file>`

    Certificate authority file to be used.

    **Default for Publisher:** `./resource/secure/pub.pem`  
    **Default for Subscriber:** `./resource/secure/sub.pem`

-   `-secureCertFile <file>`

    Certificate file to be used.

    **Default:** `./resource/secure/cacert.pem`

-   `-securePrivateKey <file>`

    Private key file to be used.

    **Default for Publisher:** `./resource/secure/pubkey.pem`
    **Default for Subscriber:** `./resource/secure/subkey.pem`

## Additional information about the parameters

### Spinning vs. Sleeping <a id="sleepVsSpin"></a>

When the publisher is writing as fast as it can, sooner or later, it is likely to get ahead of the subscriber. There are 4 things you can do in this case:

1.  Nothing -- for reliable communication, `write()` will block until the subscriber(s) catch up.

2.  Slow the writing down by sleeping (See `-sleep <millisec>`). This approach is friendlier to the other processes on the host because it does not monopolize the CPU. However, context switching is expensive enough that you can't actually "sleep" for amounts of time on the order of microseconds, so you could end up sleeping too long and hurting performance. (Operating systems (including Linux and Windows) have a minimum resolution for sleeping; i.e., you can only sleep for a period of 1 or 10 ms. If you specify a sleep period that is less than that minimum, the OS may sleep for its minimum resolution.)

3.  Spin in a tight loop between writes (See `-spin <count>`). This approach will add a pause without giving up the CPU, making it easier to "sleep" for very short periods of time. In the test implementation, there is a very short loop that just performs some simple math to take up CPU time. The argument to `-spin <count>` (any number >= 0) is the number of times to go through that loop. The default is 0. If you specify something else, it should be a fairly large number (100’s or 1000’s), since spinning the loop just a few times will take negligible time. Avoid spinning on a single-core machine, as the code that would break you out of the spin may not be able to execute in a timely manner.

4.  Let the publisher automatically adjust the writing rate (See `-enableAutoThrottle`). This option enables the Auto Throttle feature introduced in RTI Connext DDS 5.1.0 and its usage is preferred over See `-spin <count>` because the amount of spin is automatically determined by the publisher based on the number of unacknowledged samples in the send queue.

See also: Send-Queue Size and Queue-Full Behavior.


### Send-Queue Size and Queue-Full Behavior <a id="queueSize"></a>

In many distributed systems, a data producer will often outperform data consumers. That means that, if the communications are to be reliable, the producer must be throttled in some way to allow the consumers to keep up. In some situations, this may not be a problem, because data may simply not be ready for publication at a rate sufficient to overwhelm the subscribers. If you're not so lucky, your publisher's queue of unacknowledged data will eventually fill up. When that happens, if data is not to be lost, the publication will have to block until space becomes available. Blocking can cost you in terms of latency.

To avoid the cost of blocking, consider the following:

- Enlarge your publisher's queue (See `-sendQueueSize <number>`). Doing so will mean your publisher has to block less often. However, it may also let the publisher get even further ahead of slower subscribers, increasing the number of dropped and resent packets, hurting throughput. Experimenting with the send queue size is one of the easy things you can do to squeeze a little more throughput from your system.

- Enable Auto Throttling (See `-enableAutoThrottle`). This option enables the Auto Throttle feature introduced in *RTI Connext DDS 5.1.0*. When this option is used, the publisher automatically adjusts the writing rate based on the number of unacknowledged samples in the send queue to avoid blocking.

**Note:**

The following values in the `DataWriterProtocolQosPolicy` are ‘hard-coded’ in the application, therefore setting these values in the XML QoS profile will have no effect:

- `rtps_reliable_writer.heartbeats_per_max_samples` is set to (`sendQueueSize/10`)
- `rtps_reliable_writer.low_watermark` is set to (`sendQueueSize * 0.10`)
- `rtps_reliable_writer.high_watermark` is set to (`sendQueueSize * 0.90`)

For more information on the send queue size, see the `RESOURCE_LIMITS` QosPolicy, **Section 6.5.20** in the *RTI Connext DDS Core Libraries User’s Manual* (specifically, the `max_samples` field).


### Number of Iterations vs. Latency Count <a id="iterationsVsLatencyCount"></a>

When configuring the total number of samples to send during the test (See `-numIter <count>`) and the number of samples to send between latency pings (See `-latencyCount <count>`), keep these things in mind:

- Don't send latency pings too often. One of the purposes of the test is to measure the throughput that the middleware is able to achieve. Although the total throughput is technically the total data sent on both the throughput and latency topics, for the sake of simplicity, the test measures only the former. The implicit assumption is that the latter is negligible by comparison. If you violate this assumption, your throughput test results will not be meaningful.

- Keep the number of iterations large enough to send many latency pings over the course of the test run. Your latency measurements, and the spread between them, will be of higher quality if you are able to measure more data points.

- When selecting See `-numIter <count>`, choose a value that allows the test to run for at least a minute to get accurate results. Set See `-numIter <count>` to be millions for small message sizes (<1k); reduce as needed for larger sizes (otherwise the tests will take longer and longer to complete).

### Warming Up <a id="warmUp"></a>

When running the performance test in *Java*, and to a lesser extent, *C#*, you may observe that throughput slowly increases through the first few incremental measurements and then levels off. This improvement reflects the background activity of the just-in-time (JIT) compiler and optimizer on these platforms. For the best indication of steady-state performance, be sure to run the test for a number of samples (See `-numIter <count>`) sufficient to smooth out this start-up artifact.

### WaitSet Event Count and Delay <a id="WaitSetEventCount"></a>

*RTI Connext DDS*, and by extension, this performance test, gives you the option to either process received data in the middleware's receive thread, via a listener callback, or in a separate thread (See `-useReadThread`) via an object called a WaitSet. The latter approach can be beneficial in that it decouples the operation of your application from the middleware, so that your processing will not interfere with *Connext DDS*'s internal activities. However, it does introduce additional context switches into your data receive path. When data is arriving at a high rate, these context switches can adversely impact performance when they occur with each data sample.

To improve efficiency, the command line parameters `-waitsetDelayUsec <usec>` and `-waitsetEventCount <count>` allow you to process incoming data in groups, based on the number of samples and/or time, rather than individually, reducing the number of context switches. Experiment with these values to optimize performance for your system.

For more information, see these sections in the *RTI Connext DDS Core Libraries User’s Manual*: **Receive Threads (Section 19.3)** and **Conditions and WaitSets (Section 4.6)**.


### How to Measure Latency for a Given Throughput <a id="lat"></a>

If you want to measure the minimum latency for a given throughput, you have to use the command-line parameters `-sleep <millisec>`, `-spin <count>` and `-batchSize <bytes>` to experimentally set the throughput level for a given test run.

For example, suppose you want to generate a graph of latency vs. throughput for a packet size of `200 bytes` and throughput rates of `1000`, `10K`, `20K`, `50K`, `100K`, `500K`, and `Max messages` per second.

For throughput rates under 1000 messages per second, use `-sleep <ms>` to throttle the publishing application. For example, `-sleep 1` will produce a throughput of approximately 1000 messages/second; `-sleep 2` will produce a throughput of approximately 500 messages/second.

For throughput rates higher than 1000 messages per second, use `-spin <spin count>` to cause the publishing application to busy wait between sends. The `<spin count>` value needed to produce a given throughput must be experimentally determined and is highly dependent on processor performance. For example `-spin 19000` may produce a message rate of 10000 messages/second with a slow processor but a rate of 14000 messages/second with a faster processor.

Use batching when you want to measure latency for throughput rates higher than the maximum rates of sending individual messages. First, determine the maximum throughput rate for the data size under test without batching (omit See `-batchSize <bytes>`). For example, on a 1-Gigabyte network, for a data size of `200 bytes`, the maximum throughput will be about 70,000 messages/sec. We will refer to this value as `max_no_batch`.

For all throughput rates less than `max_no_batch` (e.g., 70,000 messages/sec.), do not use batching, as this will increase the latency.

Use batching to test for throughput rates higher than `max_no_batch`: start by setting `-batchSize` to a multiple of the data size. For example, if the data size is `200 bytes`, use `-batchSize 400` (this will put 2 messages in each batch), `-batchSize 800` (4 per batch), etc. This will allow you to get throughput/latency results for throughputs higher than the `max_no_batch` throughput rate.

**Note:** For larger data sizes (`8000 bytes` and higher), batching often does not improve throughput, at least for 1-Gigabyte networks.

### Auto Tuning and Turbo Mode <a id="AutoTuningTurboMode"></a>

*RTI Connext DDS 5.1.0* includes two features that allow the middleware to auto-tune the communications to achieve better performance. These features are **Auto Throttling** and **Turbo Mode**. For more information about both features, refer to **Sections 10.4, Auto Throttling for DataWriter Performance -- Experimental Feature** and **6.5.2.4 Turbo Mode: Automatically Adjusting the Number of Bytes in a Batch -- Experimental** Feature in the *RTI Connext DDS Core Libraries User's Manual*. The performance test application includes two command-line options to enable these features: `-enableAutoThrottle` and `-enableTurboMode`.

With Auto Throttling, the publisher automatically adjusts the writing rate based on the number of unacknowledged samples in the send queue to avoid blocking and provide the best latency/throughput tradeoff.

With Turbo Mode, the size of a batch is automatically adjusted to provide the best latency for a given write rate. For slow write rates, the batch size will be smaller to minimize the latency penalty. For high write rates, the batch size will be bigger to increase throughput. When turbo mode is used, the command line option See `-batchSize <bytes>` is ignored.

To achieve the best latency under maximum throughput conditions, use See `-enableAutoThrottle` and See `-enableTurboMode` in combination.
