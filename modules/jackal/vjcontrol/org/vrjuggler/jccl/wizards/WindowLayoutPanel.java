

package VjWizards;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

/** Draws a graphical depiction of multiple windows on a single screen.
 *
 *  You know what really sucks about this?  Java graphics uses an origin
 *  at the upper-left, and juggler uses lower-left.  We massage these
 *  values when we call addWindow or moveWindow...
 */
public class WindowLayoutPanel extends JPanel {

    protected class WindowListEl {
        public String name;
        public int x, y, width, height;
        public Color color, border_color;

        public WindowListEl () {
            ;
        }
    }

    protected ArrayList window_list;
    protected Dimension screen_size;

    public WindowLayoutPanel () {
        window_list = new ArrayList();
        screen_size = null;
    }

    public void setWindowScreenSize (Dimension d) {
        screen_size = d;
    }

    public void clear () {
        boolean was_empty = window_list.isEmpty();
        window_list.clear();
        if (!was_empty)
            repaint();
    }

    protected WindowListEl getWindowListEl (String name) {
        WindowListEl e;
        int i, n = window_list.size();
        for (i = 0; i < n; i++) {
            e = (WindowListEl)window_list.get(i);
            if (e.name.equals(name))
                return e;
        }
        return null;
    }


    public void addWindow (String name, int x, int y, int width, int height,
                           Color color) {
        WindowListEl e;
        boolean do_insert = false;
        e = getWindowListEl (name);
        if (e == null) {
            e = new WindowListEl();
            do_insert = true;
        }
        e.name = name;
        e.x = x;
        e.y = screen_size.height - y - height;
        e.width = width;
        e.height = height;
        e.color = color;
        e.border_color = color.brighter().brighter().brighter();
        if (do_insert)
            window_list.add (e);
        repaint();
    }

    public void moveWindow (String name, int x, int y, int width, int height) {
        WindowListEl e = getWindowListEl (name);
        if (e != null) {
            e.x = x;
            e.y = screen_size.height - y - height;
            e.width = width;
            e.height = height;
            repaint();
            return;
        }
    }

    public void removeWindow (String name) {
        WindowListEl e;
        int i, n = window_list.size();
        for (i = 0; i < n; i++) {
            e = (WindowListEl)window_list.get(i);
            if (e.name.equals(name)) {
                window_list.remove(i);
                repaint();
                return;
            }
        }
    }

    public void paintComponent (Graphics g) {
        int xborder = 5;
        int yborder = 5;
        int cwidth = getWidth();
        int cheight = getHeight();
        int useable_width = cwidth - xborder*2;
        int useable_height = cheight - yborder*2;
        g.clearRect (0,0,cwidth,cheight);
        g.setColor (getBackground().darker());
        g.fillRect (xborder, yborder, useable_width, useable_height);
        g.setColor (Color.black);
        g.drawRect (xborder, yborder, useable_width, useable_height);

        if (screen_size != null) {
            float x_scale = useable_width / (float) screen_size.width;
            float y_scale = useable_height / (float) screen_size.height;
            WindowListEl e;
            int x, y, width, height;
            int i, n = window_list.size();
            //System.out.println ("painting " + n + " windows.");
            int font_ascent = g.getFontMetrics().getAscent();
            for (i = 0; i < n; i++) {
                e = (WindowListEl)window_list.get(i);
                x = (int)(x_scale * e.x) + xborder;
                y = (int)(y_scale * e.y) + yborder;
                width = (int)(x_scale * e.width);
                height = (int)(y_scale * e.height);
                g.setColor (e.color);
                g.fillRect (x,y,width,height);
                g.setColor (e.border_color);
                g.drawRect (x,y,width,height);
                g.setColor (Color.white);
                g.setClip (x,y,width,height);
                g.drawString (e.name, x, y+font_ascent);
                g.setClip (0,0,cwidth, cheight);
            }
        }
    }

}
