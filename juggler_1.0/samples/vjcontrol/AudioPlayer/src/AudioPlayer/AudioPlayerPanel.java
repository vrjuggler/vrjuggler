/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package AudioPlayer;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.sound.sampled.*;
import java.io.*;
import javax.swing.event.*;

import VjComponents.UI.PlugPanel;
import VjComponents.UI.ControlUIModule;
import VjConfig.*;
import VjControl.Core;
import VjControl.VjComponent;
import VjControl.SuffixFilter;

/** AudioPlayerPanel extension component for VjControl.
 *  This sample extension is used to demonstrate simple features of a
 *  VjControl component, and also to show how JBuilder can be used to
 *  write VjControl extensions.
 *  <p>
 *  The first step is to create a JBuilder project, and to put an
 *  up-to-date copy of VjControl.jar in the project's "required libraries".
 *  This allows JBuilder to access all of the VjControl classes.
 *  <p>
 *  The next step, in this case, was to create a new JPanel subclass
 *  in JBuilder and lay out the GUI elements.
 *  <p>
 *  The new component implements the VjComponents.UI.PlugPanel interface,
 *  which means that it provides a graphical panel that can plug into
 *  VjControl's main UI frame.
 *
 * @author Christopher Just
 * @version $Revision$
 */

public class AudioPlayerPanel extends JPanel implements PlugPanel, Runnable {
    JTextField name_display_field = new JTextField();
    JButton load_button = new JButton();
    GridLayout gridLayout1 = new GridLayout();
    JPanel jPanel1 = new JPanel();
    JPanel jPanel2 = new JPanel();
    JButton play_button = new JButton();
    JButton stop_button = new JButton();
    JPanel jPanel3 = new JPanel();
    JLabel volume_label = new JLabel();
    JSlider volume_slider = new JSlider();

    /** VjControl's ControlUI Module is used to put up file dialogs. */
    ControlUIModule ui_module = null;
    /** Unique name used by the VjComponent interface. */
    String component_name;
    /** The ConfigChunk used to configure an instance of this component. */
    ConfigChunk component_chunk;
    /** The most-recently selected sound file. */
    File audiofile;
    /** A filter we pass to the file dialog. */
    SuffixFilter file_filter = null;
    /** Used to keep track of the thread we spin off to play sounds. */
    Thread thread = null;
    /** Used to tell the player thread to stop. */
    boolean cancel_play = true;
    /** A javax.sound interface used for accessing & playing the audio data. */
    SourceDataLine dataLine;

    /** Constructor. */
    public AudioPlayerPanel() {
        // the usual VjComponent convention is to provide a default name
        // for an instance that specifically tells us it hasn't been set up
        // correctly yet.
        component_name = "Unconfigured AudioPlayerPanel";
        audiofile = null;
        dataLine = null;
        // everything below here was autocreated by JBuilder.
        try {
            jbInit();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
    private void jbInit() throws Exception {
        name_display_field.setEditable(false);
        name_display_field.setText("<no file selected>");
        name_display_field.setColumns(25);
        load_button.setActionCommand("Load...");
        load_button.setText("Load...");
        load_button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                load_button_actionPerformed(e);
            }
        });
        this.setLayout(gridLayout1);
        gridLayout1.setRows(5);
        gridLayout1.setColumns(1);
        play_button.setActionCommand("play_button");
        play_button.setText("Play");
        play_button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                play_button_actionPerformed(e);
            }
        });
        stop_button.setText("Stop");
        stop_button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                stop_button_actionPerformed(e);
            }
        });
        volume_label.setText("Volume");
        volume_slider.setValue(100);
        volume_slider.setPreferredSize(new Dimension(275, 16));
        volume_slider.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                volume_slider_stateChanged(e);
            }
        });
        this.add(jPanel1, null);
        jPanel1.add(name_display_field, null);
        jPanel1.add(load_button, null);
        this.add(jPanel2, null);
        jPanel2.add(play_button, null);
        jPanel2.add(stop_button, null);
        this.add(jPanel3, null);
        jPanel3.add(volume_label, null);
        jPanel3.add(volume_slider, null);
    }

    /** Stop Button was pressed. */
    void stop_button_actionPerformed(ActionEvent e) {
        cancel_play = true;
    }

    /** Play button was pressed.
     *  If a file's been selected, and we're not already playing,
     *  we start a new player thread.
     */
    void play_button_actionPerformed(ActionEvent e) {
        if ((audiofile != null) && (thread == null)) {
            thread = new Thread (this);
            thread.start();
        }
    }

    /** Load button was pressed.
     *  We use the "EasyFileDialog" built into ControlUIModule to create a
     *  file dialog, which by default uses the filter we set up in configure().
     */
    void load_button_actionPerformed(ActionEvent e) {
        // put up file requester
        File file_dir = (audiofile == null)?null:audiofile.getParentFile();
        audiofile = ui_module.getEasyFileDialog().requestOpenFile (file_dir, ui_module, file_filter);
        if (audiofile != null)
            name_display_field.setText (audiofile.toString());
    }

    /** Someone's playing with the volume slider.
     *  We let a utility function called setVolume do the real work.
     */
    void volume_slider_stateChanged(ChangeEvent e) {
        setVolume();
    } // playAudioStream

    /** Configure this component using the given ConfigChunk.
     *  Until the component has been configured, we can't do anything with
     *  it.  In particular, other methods in AudioPlayerPanel can assume
     *  that the ControlUIModule has been resolved.
     *  <p>
     *  Every VjComponent uses a configure method.
     */
    public boolean configure (ConfigChunk ch) {
        // get the name & store our chunk.
        component_chunk = ch;
        component_name = ch.getName();

        // get pointers to the modules we need.  We grab this information
        // from the "Dependencies" property of the Chunk.  We check which
        // interfaces the components implement to match them up with what
        // we need.  In this case, all we need is a ControlUIModule.  But
        // this code snippet will generalize to getting multiple comps.
        // The reason we get the names of VjComponents from the ConfigChunk
        // is so that we can have, for example, multiple ControlUIModules
        // each with a separate set of panels.  This may prove helpful
        // later on.
        Property p = ch.getPropertyFromToken ("Dependencies");
        if (p != null) {
            int i;
            int n = p.getNum();
            String s;
            VjComponent c;
            for (i = 0; i < n; i++) {
                s = p.getValue(i).toString();
                c = Core.getComponentFromRegistry(s);
                if (c != null) {
                    if (c instanceof ControlUIModule)
                        ui_module = (ControlUIModule)c;
                }
            }
        }
        if (ui_module == null)
            ui_module = (ControlUIModule)Core.getComponentFromRegistry ("ControlUI Module");
        if (ui_module == null) {
            Core.consoleErrorMessage (component_name, "Instantiated with unmet VjComponent Dependencies. Fatal Configuration Error!");
            return false;
        }

        // We create a file filter which we can pass to the file dialog.  This
        // one will only show audio files.
        file_filter = new SuffixFilter ("Sound Files (*.mp3, *.wav, *.aiff)", ".mp3");
        file_filter.addSuffix(".wav");
        file_filter.addSuffix(".aiff");
        file_filter = (SuffixFilter)ui_module.getEasyFileDialog().addFilter (file_filter, "SoundFile");

        return true;
    }

    /* The next several functions are part of the VjComponent & PlugPanel
     * interfaces.
     */
    public ConfigChunk getConfiguration () {
        return component_chunk;
    }

    public ImageIcon getComponentIcon () {
        return null;
    }

    public String getComponentName () {
        return component_name;
    }


    public boolean addConfig (ConfigChunk ch) {
        return false;
    }


    public boolean removeConfig (String s) {
        return false;
    }


    public void destroy () {
        cancel_play = true;
    }


    public void rebuildDisplay () {

    }

    // Utility functions for sound playing.

    /** Checks the position of the volume slider & sets the line volume. */
    protected void setVolume () {
        // Adjust the volume on the output line.
        if( (dataLine != null) && dataLine.isControlSupported( FloatControl.Type.MASTER_GAIN ) ) {
            float volume_val = (float)volume_slider.getValue();
            FloatControl volume = (FloatControl) dataLine.getControl( FloatControl.Type.MASTER_GAIN );
            // We need to do some manipulation to get the volume value from
            // the 0-100 scale to whatever the control wants (-80 to +13 in
            // at least one case.
            float max_vol = volume.getMaximum();
            float min_vol = volume.getMinimum();
            volume_val = volume_val/100.0f;
            volume_val = min_vol + (volume_val*(max_vol - min_vol));
            volume.setValue( volume_val );
        }
    }

    /** This is the body for our player thread. */
    public void run () {
        cancel_play = false;
        playAudioFile (audiofile);
        thread = null;
    }

    // This code forms the actual meat of the audio player.  It is
    // based on the Play.java example by Dan Becker (beckerdo@io.com)

    /** Play audio from the given file name. */
    protected void playAudioFile( File soundFile ) {
        try {
            // Create a stream from the given file.
            // Throws IOException or UnsupportedAudioFileException
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream( soundFile );
            playAudioStream( audioInputStream );
        } catch ( Exception e ) {
            System.out.println( "Problem with file " + soundFile.toString() + ":" );
            e.printStackTrace();
        }
    } // playAudioFile


    /** Plays audio from the given audio input stream. */
    protected void playAudioStream( AudioInputStream audioInputStream ) {
        // Audio format provides information like sample rate, size, channels.
        AudioFormat audioFormat = audioInputStream.getFormat();
        System.out.println( "Play input audio format=" + audioFormat );

        // Convert compressed audio data to uncompressed PCM format.
        if ( audioFormat.getEncoding() != AudioFormat.Encoding.PCM_SIGNED ) {
            AudioFormat newFormat = new AudioFormat(
                AudioFormat.Encoding.PCM_SIGNED,
                audioFormat.getSampleRate(),
                16,
                audioFormat.getChannels(),
                audioFormat.getChannels() * 2,
                audioFormat.getSampleRate(),
                false );
            System.out.println( "Converting audio format to " + newFormat );
            AudioInputStream newStream = AudioSystem.getAudioInputStream( newFormat, audioInputStream );
                audioFormat = newFormat;
                audioInputStream = newStream;
        }

        // Open a data line to play our type of sampled audio.
        // Use SourceDataLine for play and TargetDataLine for record.
        DataLine.Info info = new DataLine.Info( SourceDataLine.class, audioFormat );
        if ( !AudioSystem.isLineSupported( info ) ) {
            System.out.println( "playAudioStream does not handle this type of audio on this system." );
            return;
        }

        try {
            // Create a SourceDataLine for play back (throws LineUnavailableException).
            dataLine = (SourceDataLine) AudioSystem.getLine( info );
            // System.out.println( "SourceDataLine class=" + dataLine.getClass() );

            // The line acquires system resources (throws LineAvailableException).
            dataLine.open( audioFormat );

            setVolume();

            // Allows the line to move data in and out to a port.
            dataLine.start();

            // Create a buffer for moving data from the audio stream to the line.
            int bufferSize = (int) audioFormat.getSampleRate() * audioFormat.getFrameSize();
            byte [] buffer = new byte[ bufferSize ];

            // Move the data until done or there is an error.
            try {
                int bytesRead = 0;
                while ( (bytesRead >= 0) && !cancel_play) {
                    bytesRead = audioInputStream.read( buffer, 0, buffer.length );
                    if ( bytesRead >= 0 ) {
                        // System.out.println( "Play.playAudioStream bytes read=" + bytesRead +
                        //    ", frame size=" + audioFormat.getFrameSize() + ", frames read=" + bytesRead / audioFormat.getFrameSize() );
                        // Odd sized sounds throw an exception if we don't write the same amount.
                        int framesWritten = dataLine.write( buffer, 0, bytesRead );
                    }
                } // while
            } catch ( IOException e ) {
                e.printStackTrace();
            }

            System.out.println( "Play.playAudioStream draining line." );
            // Continues data line I/O until its buffer is drained.
            dataLine.drain();

            System.out.println( "Play.playAudioStream closing line." );
            // Closes the data line, freeing any resources such as the audio device.
            dataLine.close();
        } catch ( LineUnavailableException e ) {
            e.printStackTrace();
        }

        dataLine = null;
    }

}
