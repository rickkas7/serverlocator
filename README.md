# serverlocator

*Sample code for locating Particle Photons on your local network for direct client-server communication*

Often, publish and subscribe is the most convenient way for two Particle Photons to communicate. But sometimes when you have two or more Photons on a local network, sometimes you may want to use direct client-server communications using TCP or UDP. Some common reasons:

- Your data is too large
- You need to send data very frequently
- You have limited or no connectivity to the Internet

One problem is that to put the server on a Photon, the IP address is often dynamic. Having to code this address into all of the clients when it might change can be a problem.

This sample code implements two ways of solving this problem:

- Using publish and subcribe (pubsubserverlocator)
- Using UDP multicast (mcastserverlocator)

The first method is easier; the second can be used even if you have no Internet access. In fact, the mcastserverlocator examples run in WiFi-only mode, with no cloud connection at all.

In each directory there is a "c" directory (cpubsubserverlocator, for example) that's the client Photon code and an "s" directory that's the server Photon code.

I also have another example that uses a [local server written in node.js] (https://github.com/rickkas7/localserver) that uses publish and subscribe to not only locate the server IP address, but also for authentication.

There's another technology, [mdns] (https://en.wikipedia.org/wiki/Multicast_DNS), that could also be used. There is an [mdns server for the Photon] (https://github.com/mrhornsby/spark-core-mdns). It's really useful if you're running a HTTP server on your Photon and you want to connect to it from a web browser Mac or Linux. The problem is that there isn't an mdns client, and it adds a lot of complexity that you don't need for Photon to Photon communication, so I didn't use it here.

 

