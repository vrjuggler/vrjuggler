DVG Cluster How To

Written by: Infiscape Corporation<br>
Aron Bierbaum and Allen Bierbaum<br>
<br>
<br>
<br>
<h1>Introduction</h1>
The purpose of this document is to explain how to run a VR Juggler cluster application on a ORAD DVG cluster. At the time of this writing, the ORAD system has only been tested with two compositing modes: Split Image and Multi-Sampling/Anti-Alias. This document will briefly explain the needed DvgWrapper and VR Juggler configuration files along with providing tips to help you along the way.<br>
<br>
<h1>DVG Configuration Files</h1>
In order for the DvgWrapper to know how to set up the compositing chain, you must provide a configuration file that specifies the compositing methods to be used. As the DVG documentation explains, there are multiple ways to specify a DVG wrapper configuration file. We recommend that you specify the configuration file by setting the <code>DVG_GL_CONFIG</code> environment variable. For example to use the attached [/DvgGL-four.cfg example configuration file] for a 4&times;4 tiled screen:<br>
<br>
<pre><code>	setenv DVG_GL_CONFIG /home/aronb/dvgconfig/DvgGL-four.cfg<br>
</code></pre>

Note: This must be set to your local configuration files.<br>
<br>
Before discussing the specific compositing methods and how you configure them, it is worth noting a few key sections of the attached example configuration files that remain the same for all compositing methods. These sections will be explained briefly below.<br>
<br>
Please see the DVG wrapper documentation for more detailed descriptions of all the following options.  We will only discuss them as they pertain to VR Juggler operation.<br>
<br>
<h2>Basic DVG Settings</h2>
The first two lines shown below specify the video output format to be used.<br>
<br>
| Video Mode | You must have the <code>VideoMode</code> attribute set to <code>VR</code> for the DvgWrapper to function properly. For more information about the <code>VROutputFormat</code> attribute consult your DVG wrapper documentation. |<br>
| Genlocking | The next attribute, <code>GenlockSource</code> specifies the signal coming into the chain to use for Genlock. This attribute should be set to <code>None</code> for proper operation.  |<br>
| Pass Through | The <code>PassThrough</code> attribute disables all functionality of the wrapper and defaults to 0. |<br>
| Barrier | The final attribute, <code>SwapBarrier</code> informs the wrapper if it should use its internal TCP/IP swap lock method. Since VR Juggler already provides a method to SwapLock the nodes, this should be disabled. |<br>
<br>
<pre><code>  &lt;DvgConfig<br>
	 VideoMode		  = "VR"<br>
	 VROutputFormat	= "60.02,1280,1328,1440,1688,1024,1025,1028,1066"<br>
	 GenlockSource	 = "None"<br>
	 Multisample		= "0"	<br>
	 PassThrough		= "0"<br>
	 SwapBarrier		= "0"<br>
  /&gt;<br>
</code></pre>

<h2>X Server Configuration</h2>
There are a few X Server configuration settings that are recommended. The first <code>MaximizeWindow</code> forces any window opened to be full screen. This feature was tested, but seemed to only occasionally function. The last setting shown below, <code>HideMouseCursor</code> causes the X mouse curser to disappear.<br>
<br>
<pre><code>  &lt;XConfig<br>
	 MaximizeWindow	= "1"<br>
	 HideMouseCursor  = "1"<br>
  /&gt;<br>
</code></pre>

<h2>Synchronization Configuration</h2>
To correctly configure the syncronization of the DVG cluster, you must specify three things: <code>SyncronizeDelay</code>, <code>SynchronizeOn</code>, and a list of render nodes in your chain. The <code>SyncronizeDelay</code> attribute specifies the number of synchronization points that are initially skipped in order to ensure proper start-up of your application. The default value is 50, but smaller values will also work. The <code>SynchronizeOn</code> specifies which !OpenGL command the DvgWrapper will wait for to initiate each synchronization barrier.<br>
<br>
<pre><code>  &lt;Synchronization&gt;<br>
	 &lt;Config SynchronizeDelay = "2" SynchronizeOn = "glXSwapBuffers" /&gt;<br>
	 &lt;Renderer HostName = "dvg1-a" /&gt;<br>
	 &lt;Renderer HostName = "dvg1-b" /&gt;<br>
	 &lt;Renderer HostName = "dvg2-a" /&gt;<br>
	 &lt;Renderer HostName = "dvg2-b" /&gt;<br>
  &lt;/Synchronization&gt;<br>
</code></pre>

<h1>VR Juggler Configuration Files</h1>
Configuring your application to work properly on a DVG cluster is identical to any other configuration. There are a few things that you should keep in mind while creating the configuration files though.<br>
<br>
DisplayWindows will only be visible at the end of the DVG composite chain if they are opened on the proper section of the X Server and are of the correct size. For example, assume that you have a single monitor attached to the last node in your DVG composite chain of four render nodes. Let's also assume that your final resolution is 1280&times;1024 and you want to split the image into four quadrants:<br>
+------------------=  =|			 |			 |<br>
|  Node 1  |  Node 2  |<br>
|			 |			 |<br>
+-------= ----------+ =|			 |			 |<br>
|  Node 3  |  Node 4  |<br>
|			 |			 |<br>
+------------------=  =}}}<br>
<br>
Since all nodes are drawing an equal sized section of the final image, on each node you must open a window at the origin with the proper size.<br>
<br>
	* Origin: (0,0)<br>
	* Size: (640, 512)<br>
<br>
One method we have found usefull for debugging the imaging compositing is to hook into the VGA outputs of the chain in order testing for the correct outputs on each node.<br>
<br>
=  Important Environment Variables =<br>
In order for the !DvgWrapper to know which X server to run your application you '''must''' set the `DISPLAY` environment variable to `:0` on each machine before running your application as shown below.<br>
<br>
{{{<br>
	setenv DISPLAY :0<br>
	./runDvgGL myApp cluster.jconf<br>
}}}<br>
<br>
In order to see debug information from the !DvgWrapper you can set the `EB_MSG_LEVEL` environment variable to one of the following settings.<br>
	* warning - Only display warnings and errors.<br>
	* info - Show a more verbose level of debug output.<br>
<br>
The last environment variable discussed, `DVG_NO_NOISE`, disables the PC speaker that makes noises while initializing the DVG composite chain.<br>
<br>
=  Miscellaneous Issues =<br>
There are several issues that we have run into that can be good to watch for.<br>
<br>
	* Machine naming and resolving: In their default configurations, system utilities may not all return the same hostname information for a given DVG node due to the way networking, `/etc/hosts`, and various other settings of configured.  This can cause some problems with VR Juggler clustering and with user-developed scripts.<br>
<br>
	* The wrapper settings can override and/or extend other graphics board specific settings.  For example with nvidia Linux boards the antialiasing mode can be overridden by the wrapper configuration.<br>
<br>
	* The wrapper does not like to work with multi-threaded apps that Juggler creates.  So, in a bind you can run the RunDvgGL script with glxgears for a minute to get the state set for the boards and then simply run your app without the wrapper.<br>
<br>
<br>
<br>
</code></pre>