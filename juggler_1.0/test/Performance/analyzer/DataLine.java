

public class DataLine {
    public double vals[];
    public double diffs[]; // diffs[i] is val[i]-val[i-1]
    //public boolean valid[]; /* used to mark missing datapoints */
    public int  numlost; // # points missing after last point 
    public double linetotal; // time from end of last dataline toend
                            // of this one...
// i'll assume for now there's a nan value that i can use.

    public DataLine(int num) {
        vals = new double[num];
        diffs = new double[num];
        //valid = new boolean[num];
	numlost = 0;
    }
};
