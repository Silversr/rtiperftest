/*
 * (c) 2005-2016  Copyright, Real-Time Innovations, Inc. All rights reserved.
 * Subject to Eclipse Public License v1.0; see LICENSE.md for details.
 */

package com.rti.perftest;


// ===========================================================================

/**
 * Runs the performance test.
 */
public interface IMessaging {
    public boolean initialize(int argc, String[] argv);

    public void printCmdLineHelp();

    public void shutdown();

    /**
     * If the implementation supports batching and the test scenario is
     * using batching, this function should return the size of the batch
     * in bytes.
     */
    public int getBatchSize();

    /* Used only for scan mode.
     * The maximum size of a message's binary payload. If the size
     * exceeds this during a scan, the test will stop.
     */
    public int getMaxBinDataSize();

    public IMessagingWriter createWriter(String topicName);
    
    /**
     * Pass null for callback if using IMessagingReader.ReceiveMessage()
     * to get data.
     */
    public IMessagingReader createReader(
            String topicName, IMessagingCB callback);
    
    public void dispose();

}

// ===========================================================================
